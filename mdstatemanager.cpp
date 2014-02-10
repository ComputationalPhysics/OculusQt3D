#include "mdstatemanager.h"
#include "mdstate.h"
#include <qvector3d.h>

MDStateManager::MDStateManager():
    m_currentTimestep(0),
    m_currentState(NULL),
    m_numberOfProcessors(QVector3D(1,1,1)),
    m_systemSize(QVector3D(1,1,1))
{

}

const QArray<QVector3D> &MDStateManager::getPositions() {
    if(m_currentState!=NULL) return m_currentState->getPositions();
    else return QArray<QVector3D>();
}

const QArray<QColor4ub> &MDStateManager::getColors() {
    if(m_currentState!=NULL) return m_currentState->getColors();
    else return QArray<QColor4ub>();
}

const QArray<QSizeF> &MDStateManager::getSizes() {
    if(m_currentState!=NULL) return m_currentState->getSizes();
    else return QArray<QSizeF>();
}

//void MDStateManager::read_data(ifstream *file, void *value) {
//    int N;
//    file->read (reinterpret_cast<char*>(&N), sizeof(int));
//    file->read (reinterpret_cast<char*>(value), N);
//    file->read (reinterpret_cast<char*>(&N), sizeof(int));
//}

//void MDStateManager::read_mts(char *filename, QArray<int> &atomTypesThisCPU, QArray<int> &atomIdsThisCPU, QArray<QVector3D > &positionsThisCPU, QVector3D &systemSize) {
//    ifstream *file = new ifstream();
//    file->open(filename, ios::in | ios::binary);
//    if (!*file) {
//        cout << "Error in Mts0_io::read_mts(): Failed to open file " << filename << endl;
//        exit(1);
//    }
//    int numAtomsThisCPU;
//    read_data(file, &numAtomsThisCPU);

//    double *phase_space = new double[6*numAtomsThisCPU];
//    double *tmp_atom_data = new double[numAtomsThisCPU];

//    atomTypesThisCPU.resize(numAtomsThisCPU);
//    atomIdsThisCPU.resize(numAtomsThisCPU);
//    positionsThisCPU.resize(numAtomsThisCPU);

//    read_data(file, tmp_atom_data);
//    read_data(file, phase_space);

//    for(int i=0;i<num_atoms_local;i++) {
//        atomTypesThisCPU[i] = int(tmp_atom_data[i]);
//        // Handle roundoff errors from 2 -> 1.99999999 -> 1
//        atomIdsThisCPU[i] = (tmp_atom_data[i]-atomTypesThisCPU[i])*1e11 + 1e-5;
//        positionsThisCPU [i] = QVector3D(phase_space[3*i+0], phase_space[3*i+1], phase_space[3*i+2])
//    }

//    QArray<QArray<QVector3D> > h_matrix;

//    h_matrix.resize(2);
//    for(int i=0;i<2;i++) {
//        h_matrix.at(i).resize(3);
//        for(int j=0;j<3;j++) {
//            h_matrix(i)(j).resize(3);
//        }
//    }

//    double *tmp_h_matrix = new double[18];
//    read_data(file,tmp_h_matrix);
//    int count = 0;
//    for(int k=0;k<2;k++) {
//        for(int j=0;j<3;j++) {
//            for(int i=0;i<3;i++) {
//                h_matrix[k][i][j] = float(tmp_h_matrix[count++]);
//            }
//        }
//    }

//    systemSize.setX(h_matrix[0][0][0]);
//    systemSize.setY(h_matrix[0][1][1]);
//    systemSize.setZ(h_matrix[0][2][2]);

//    file->close();

//    h_matrix.clear();
//    delete tmp_h_matrix;
//    delete phase_space;
//    delete tmp_atom_data;
//    delete file;
//}

//    MDState *MDStateManager::load_atoms(string mts0_directory) {
//    MDState *state = new MDState();

//    QArray<QVector3D> positionsThisCPU;
//    QArray<int> atomTypesThisCPU;
//    QArray<int> atomIdsThisCPU;

//    int nx = m_numberOfProcessors.x();
//    int ny = m_numberOfProcessors.y();
//    int nz = m_numberOfProcessors.z();

//    int numCPUs = nx*ny*nz;
//    QVector3D nodeVectorIndex;
//    QVector3D nodeOrigin;
//    QVector3D nodeOffset;

//    char filename[1000];

//    node_offset[0] = 1.0/nx;
//    node_offset[1] = 1.0/ny;
//    node_offset[2] = 1.0/nz;

//    for(int cpuID=0; cpuID<numCPUs; cpuID++) {
//        nodeVectorIndex[0] = cpuID/(ny*nz);   // Node id in x-direction
//        nodeVectorIndex[1] = (cpuID/nz) % ny; // Node id in y-direction
//        nodeVectorIndex[2] = cpuID % nz; 	  // Node id in z-direction

//        nodeOrigin.x() = nodeOffset.x()*nodeVectorIndex.x(); // Displacement in x-direction
//        nodeOrigin.y() = nodeOffset.y()*nodeVectorIndex.y(); // Displacement in y-direction
//        nodeOrigin.z() = nodeOffset.z()*nodeVectorIndex.z(); // Displacement in z-direction

//        sprintf(filename,"%s/mt%04d",mts0_directory.c_str(), cpuID);

//        read_mts(filename, atomTypesThisCPU, atomIdsThisCPU, positionsThisCPU, m_systemSize);
//        int numAtomsThisCPU = positionsThisCPU.size();

//        for(int j=0;j<numAtomsThisCPU;j++) {
//            for(int a=0; a<3; a++) {
//                positionsThisCPU[j][a] += nodeOrigin[a];
//                positionsThisCPU[j][a] *= m_systemSize[a]*bohr;

//            }
//        }

//        positions.insert ( positions.end() , positions_local.begin() , positions_local.end()  );
//        atom_types.insert ( atom_types.end() , atom_types_local.begin() , atom_types_local.end()  );
//        atom_ids.insert ( atom_ids.end() , atom_ids_local.begin() , atom_ids_local.end()  );
//    }

//    delete filename;
//    atom_types_local.clear();
//    atom_ids_local.clear();
//    positions_local.clear();

//    node_vector_index.clear();
//    node_origin.clear();
//    node_offset.clear();
//    }

void MDStateManager::loadMts0(string foldername) {

}


bool MDStateManager::loadXyz(string filename) {
    FILE *filePointer;
    filePointer = fopen(filename.c_str(), "rb");
    if (!filePointer) {
        cerr << "Could not open file " << filename << endl;
        return true;
    }

    char atomName[1024], fileBuffer[1024], *k;
    bool hasNewTimestep = true;

    int numberOfAtoms, j;
    float x,y,z;

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
        state->reserveMemory(numberOfAtoms);

        for(int i=0; i<numberOfAtoms; i++) {
            k = fgets(fileBuffer, 1024, filePointer);
            j = sscanf(fileBuffer, "%s %f %f %f", atomName, &x, &y, &z);

            if (k == NULL) {
                cerr << "Error reading file " << filename << endl;
                return false;
            } else if (j < 4) {
                fprintf(stderr, "xyz timestep) missing type or coordinate(s) in file '%s' for atom '%d'\n",filename.c_str(),i+1);
                return false;
            } else if (j >= 4) {
                state->addAtom(QVector3D(x,y,z), atomName);
            } else {
              break;
            }
        }
    }

    return true;
}
