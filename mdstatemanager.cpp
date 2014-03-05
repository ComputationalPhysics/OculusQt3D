#include "mdstatemanager.h"
#include <qvector3d.h>
#include <fstream>
using std::cout;
#define BOHR 0.5291772

MDStateManager::MDStateManager():
    m_currentTimestep(0),
    m_timeDirection(1),
    m_playBackSpeed(1),
    m_showWater(1),
    m_numberOfAtoms(0),
    m_systemSize(QVector3D(1,1,1))
{
    timer.setInterval(16);
    connect(&timer, SIGNAL(timeout()),this,SLOT(updateNextTimestep()));
    timer.start();
}

int MDStateManager::getNumberOfTimesteps() {
    return m_states.size();
}

QArray<DataBundle*> *MDStateManager::dataBundles()
{
    if(m_states.size() == 0) return 0;
    return m_states.at(m_currentTimestep)->dataBundles();
}

void MDStateManager::updateNextTimestep() {
    if(getNumberOfTimesteps() == 0) return;

    m_currentTimestep += m_timeDirection*m_playBackSpeed;
    if(m_currentTimestep < 0) {
        // We have reached the first timestep, reversing time direction
        m_currentTimestep = 0;
        m_timeDirection = 1;
    } else if(m_currentTimestep >= getNumberOfTimesteps()) {
        // We have reached the last timestep, reversing time direction
        m_currentTimestep = getNumberOfTimesteps()-1;
        m_timeDirection = -1;
    }

    if(m_currentTimestep >= 0 && getNumberOfTimesteps()-1) {
        setNumberOfAtoms(m_states.at(m_currentTimestep)->numberOfAtoms());
    } else {
        setNumberOfAtoms(0);
    }
    emit currentTimestepChanged(m_currentTimestep);
}

void MDStateManager::readData(ifstream *file, void *value) {
    int N;
    file->read (reinterpret_cast<char*>(&N), sizeof(int));
    file->read (reinterpret_cast<char*>(value), N);
    file->read (reinterpret_cast<char*>(&N), sizeof(int));
}

char *type[] = {(char*)"Not in use", (char*)"Si",(char*)"A",(char*)"H",(char*)"O",(char*)"Na",(char*)"Cl",(char*)"X"};

void MDStateManager::readMts(char *filename, QArray<char*> &atomTypesThisCPU, QArray<int> &atomIdsThisCPU, QArray<QVector3D > &positionsThisCPU, QVector3D &systemSize) {
    ifstream *file = new ifstream();
    file->open(filename, std::ios::in | std::ios::binary);
    if (!*file) {
        cerr << "Error in MDStateManager::read_mts(): Failed to open file " << filename << endl;
        exit(1);
    }
    int numAtomsThisCPU;
    readData(file, &numAtomsThisCPU);

    double *phase_space = new double[6*numAtomsThisCPU];
    double *tmp_atom_data = new double[numAtomsThisCPU];

    atomTypesThisCPU.resize(numAtomsThisCPU);
    atomIdsThisCPU.resize(numAtomsThisCPU);
    positionsThisCPU.resize(numAtomsThisCPU);

    readData(file, tmp_atom_data);
    readData(file, phase_space);

    for(int i=0;i<numAtomsThisCPU;i++) {
        int atom_type = int(tmp_atom_data[i]);
        atomTypesThisCPU[i] = type[atom_type];
        // Handle roundoff errors from 2 -> 1.99999999 -> 1
        atomIdsThisCPU[i] = (tmp_atom_data[i]-atom_type)*1e11 + 1e-5;
        positionsThisCPU [i] = QVector3D(phase_space[3*i+0], phase_space[3*i+1], phase_space[3*i+2]);
    }

    QArray<QArray<QVector3D> > h_matrix;

    h_matrix.resize(2);
    for(int i=0;i<2;i++) {
        h_matrix[i].resize(3);
    }

    double *tmp_h_matrix = new double[18];
    readData(file,tmp_h_matrix);
    int count = 0;
    for(int k=0;k<2;k++) {
        for(int j=0;j<3;j++) {
            for(int i=0;i<3;i++) {
                h_matrix[k][i][j] = float(tmp_h_matrix[count++]);
            }
        }
    }

    systemSize.setX(h_matrix[0][0][0]);
    systemSize.setY(h_matrix[0][1][1]);
    systemSize.setZ(h_matrix[0][2][2]);

    file->close();

    h_matrix.clear();
    delete tmp_h_matrix;
    delete phase_space;
    delete tmp_atom_data;
    delete file;
}

MDState *MDStateManager::loadTimestepMts0(string mts0_directory, QVector3D numberOfCPUs) {
    MDState *state = new MDState();

    QArray<QVector3D> positionsThisCPU;
    QArray<char*> atomTypesThisCPU;
    QArray<int> atomIdsThisCPU;

    int nx = numberOfCPUs.x();
    int ny = numberOfCPUs.y();
    int nz = numberOfCPUs.z();

    int numCPUs = nx*ny*nz;
    QVector3D nodeVectorIndex;
    QVector3D nodeOrigin;
    QVector3D nodeOffset;

    char filename[1000];

    nodeOffset[0] = 1.0/nx;
    nodeOffset[1] = 1.0/ny;
    nodeOffset[2] = 1.0/nz;

    for(int cpuID=0; cpuID<numCPUs; cpuID++) {
        nodeVectorIndex[0] = cpuID/(ny*nz);   // Node id in x-direction
        nodeVectorIndex[1] = (cpuID/nz) % ny; // Node id in y-direction
        nodeVectorIndex[2] = cpuID % nz; 	  // Node id in z-direction

        nodeOrigin[0] = nodeOffset.x()*nodeVectorIndex.x(); // Displacement in x-direction
        nodeOrigin[1] = nodeOffset.y()*nodeVectorIndex.y(); // Displacement in y-direction
        nodeOrigin[2] = nodeOffset.z()*nodeVectorIndex.z(); // Displacement in z-direction

        sprintf(filename,"%s/mt%04d",mts0_directory.c_str(), cpuID);

        readMts(filename, atomTypesThisCPU, atomIdsThisCPU, positionsThisCPU, m_systemSize);
        int numAtomsThisCPU = positionsThisCPU.size();
        m_systemSize *= BOHR;
        for(int j=0;j<numAtomsThisCPU;j++) {
            for(int a=0; a<3; a++) {
                positionsThisCPU[j][a] += nodeOrigin[a];
                positionsThisCPU[j][a] *= m_systemSize[a];
            }
        }

        state->addAtoms(positionsThisCPU,atomTypesThisCPU, m_periodic, m_systemSize); // Switch these two to have cpu based periodic boundary conditions
    }

    positionsThisCPU.clear();
    atomTypesThisCPU.clear();
    atomIdsThisCPU.clear();
    emit systemSizeChanged(m_systemSize);
    return state;
}

void MDStateManager::loadMts0(QString foldername, int numberOfTimesteps, QVector3D numberOfCPUs) {
    reset();
    cout << "Will read " << numberOfTimesteps << " timesteps mts0 from " << foldername.toStdString().c_str() << endl;
    char mts0Directory[1024];
    for(int timestep=0; timestep<numberOfTimesteps; timestep++) {
        sprintf(mts0Directory,"%s/%06d/mts0/",foldername.toStdString().c_str(),timestep);
        MDState *state = this->loadTimestepMts0(mts0Directory, numberOfCPUs);
        state->buildVertexBundle();
        cout << "Loaded " << state->numberOfAtoms() << " in timestep " << timestep+1 << " / " << numberOfTimesteps << endl;
        m_states.push_back(state);
    }
}

void MDStateManager::reset() {
    for(int i=0; i<m_states.size(); i++) {
        // Free memory
        delete m_states[i];
    }
    setNumberOfAtoms(0);

    m_states.clear();
    m_currentTimestep = 0;
    m_showWater = true;
}

bool MDStateManager::loadXyz(QString filename) {
    reset();

    FILE *filePointer;
    filePointer = fopen(filename.toStdString().c_str(), "rb");
    if (!filePointer) {
        cerr << "Could not open file " << filename.toStdString() << endl;
        return true;
    }

    char atomName[1024], fileBuffer[1024], *k;
    bool hasNewTimestep = true;

    int numberOfAtoms, j;
    float x = 1337;
    float y = 1337;
    float z = 1337;

    while(hasNewTimestep) {
        k = fgets(fileBuffer, 1024, filePointer);
        j = sscanf(fileBuffer, "%d", &numberOfAtoms);
        if(k==NULL) {
            hasNewTimestep = false;
            continue;
        }

        std::cout << "Found " << numberOfAtoms << " atoms." << std::endl;

        k = fgets(fileBuffer, 1024, filePointer); // Skip the next line
        MDState *state = new MDState();
//        state->reserveMemory(numberOfAtoms);

        for(int i=0; i<numberOfAtoms; i++) {
            k = fgets(fileBuffer, 1024, filePointer);
            j = sscanf(fileBuffer, "%s %f %f %f", atomName, &x, &y, &z);
            std::cout << atomName << " " << x << " " << y  << " " << z << std::endl;
            std::cout << fileBuffer << std::endl;
            if (k == NULL) {
                cerr << "Error reading file " << filename.toStdString() << endl;
                return false;
            } else if (j < 4) {
                fprintf(stderr, "xyz timestep) missing type or coordinate(s) in file '%s' for atom '%d'. Found %d params.\n",filename.toStdString().c_str(),i+1,j);
                return false;
            } else if (j >= 4) {
                state->addAtom(QVector3D(x,y,z), atomName);
            } else {
                break;
            }
        }

        state->buildVertexBundle();
        m_states.append(state);
    }
    return true;
}
