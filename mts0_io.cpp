#include <mts0_io.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>

using namespace std;

string get_file_extension(string& filename){
    if(filename.find_last_of(".") != std::string::npos){
        return filename.substr(filename.find_last_of(".")+1);
    }
    return "folder";
}


string get_file_basename(string& filename){
    if(filename.find_last_of(".") != std::string::npos){
        return filename.substr(0, filename.rfind("."));
    }
    return "folder";
}

char *type[] = {(char*)"Not in use", (char*)"Si",(char*)"A ",(char*)"H ",(char*)"O ",(char*)"Na",(char*)"Cl",(char*)"X "};

vector<float> Timestep::get_lx_ly_lz() {
    vector<float> system_size(3);
    for(int i=0;i<3;i++) {
        system_size[i] = h_matrix[0][i][i]*bohr;
    }

    return system_size;
}

void Timestep::load_atoms_xyz(string xyz_file) {
    map<string,int> atom_type_list;
    atom_type_list.insert(pair<string,int>("Si",1));
    atom_type_list.insert(pair<string,int>("A",2));
    atom_type_list.insert(pair<string,int>("H",3));
    atom_type_list.insert(pair<string,int>("O",4));
    atom_type_list.insert(pair<string,int>("Na",5));
    atom_type_list.insert(pair<string,int>("Cl",6));
    atom_type_list.insert(pair<string,int>("N",4));

    ifstream file;
    file.open(xyz_file.c_str());
    int num_atoms;
    file >> num_atoms;
    string tmp;
    file >> tmp;

    string atom_type;
    double x,y,z;
    double max_x, max_y, max_z;
    max_x = 0; max_y = 0; max_z = 0;
    int atom_id;

    positions.resize(num_atoms);
    atom_ids.resize(num_atoms);
    atom_types.resize(num_atoms);
    for(int i=0; i<num_atoms; i++) {
        file >> atom_type >> x >> y >> z >> atom_id;
        int atom_type_int = atom_type_list.find(atom_type)->second;
        positions[i].resize(3);
        positions[i][0] = x; positions[i][1] = y; positions[i][2] = z;
        atom_ids[i] = atom_id;
        atom_types[i] = atom_type_int;
        max_x = max(max_x, x);
        max_y = max(max_y, y);
        max_z = max(max_z, z);
    }

    h_matrix.resize(2);
    for(int i=0;i<2;i++) {
        h_matrix[i].resize(3);
        for(int j=0;j<3;j++) {
            h_matrix[i][j].resize(3);
        }
    }

    h_matrix[0][0][0] = max_x;
    h_matrix[1][0][0] = max_x;
    h_matrix[0][1][1] = max_y;
    h_matrix[1][1][1] = max_y;
    h_matrix[0][2][2] = max_z;
    h_matrix[1][2][2] = max_z;
}

Timestep::Timestep(string mts0_directory,int nx_, int ny_, int nz_) {
    nx = nx_;
    ny = ny_;
    nz = nz_;

    if(get_file_extension(mts0_directory).compare("xyz") == 0) {
        load_atoms_xyz(mts0_directory);
    }
    else load_atoms(mts0_directory);
}

Timestep::~Timestep() {
    positions.clear();
    atom_ids.clear();
    atom_types.clear();
    h_matrix.clear();
}

// /projects/andershaf_nanoporous_sio2_compressed_pore/medium_silica_water/dump2

Mts0_io::Mts0_io(QObject *parent) {
    step = 1;
    time_direction = 1;
    nx = 2;
    ny = 2;
    nz = 2;
    preload = true;

    // foldername_base = "/projects/andershaf_nanoporous_sio2_compressed_pore/small_silica_water/dump/";
    // foldername_base = "/projects/andershaf_nanoporous_sio2_compressed_pore/medium_silica_water/dump2";
//    foldername_base = "/projects/andershaf_nanoporous_sio2_compressed_pore/pure_silica/dump/";
    foldername_base = "/projects/andershaf_nanoporous_sio2_compressed_pore/silica_nacl_1_permille/dump/";
    // foldername_base = "/home/svenni/scratch/mddata/dump";

    max_timestep = 0;
    current_timestep = -1; // Next will be 0
    currentTimestepObject = NULL;
    if(preload) load_timesteps();

    timer.setInterval(16);
    connect(&timer, SIGNAL(timeout()),this,SLOT(update_next_timestep()));
    timer.start();
}

//Mts0_io::Mts0_io(int nx_, int ny_, int nz_, int max_timestep_, string foldername_base_, bool preload_, int step_) {
//    step = step_;
//    time_direction = 1;
//    nx = nx_;
//    ny = ny_;
//    nz = nz_;
//    preload = preload_;
//    foldername_base = foldername_base_;
//    max_timestep = max_timestep_;
//    current_timestep = -1; // Next will be 0
//    if(preload) load_timesteps();

//    timer.setInterval(16);
//    connect(&timer, SIGNAL(timeout()),this,SLOT(update_next_timestep()));
//    timer.start();
//}

void Timestep::read_data(ifstream *file, void *value) {
    int N;
    file->read (reinterpret_cast<char*>(&N), sizeof(int));
    file->read (reinterpret_cast<char*>(value), N);
    file->read (reinterpret_cast<char*>(&N), sizeof(int));
}

void Timestep::read_mts(char *filename, vector<int> &atom_types_local, vector<int> &atom_ids_local, vector<vector<float> > &positions_local) {
    ifstream *file = new ifstream();
    file->open(filename, ios::in | ios::binary);
    if (!*file) {
        cout << "Error in Mts0_io::read_mts(): Failed to open file " << filename << endl;
        exit(1);
    }
    int num_atoms_local;
    read_data(file, &num_atoms_local);

    double *phase_space = new double[6*num_atoms_local];
    double *tmp_atom_data = new double[num_atoms_local];

    atom_types_local.resize(num_atoms_local);
    atom_ids_local.resize(num_atoms_local);
    positions_local.resize(num_atoms_local);
    for(int i=0;i<num_atoms_local;i++) {
        positions_local[i].resize(3);
    }

    read_data(file, tmp_atom_data);
    read_data(file, phase_space);

    for(int i=0;i<num_atoms_local;i++) {
        atom_types_local[i] = int(tmp_atom_data[i]);
        // Handle roundoff errors from 2 -> 1.99999999 -> 1
        atom_ids_local[i] = (tmp_atom_data[i]-atom_types_local[i])*1e11 + 1e-5;

        positions_local [i][0] = float(phase_space[3*i+0]);
        positions_local [i][1] = float(phase_space[3*i+1]);
        positions_local [i][2] = float(phase_space[3*i+2]);
    }

    double *tmp_h_matrix = new double[18];
    read_data(file,tmp_h_matrix);
    int count = 0;
    for(int k=0;k<2;k++) {
        for(int j=0;j<3;j++) {
            for(int i=0;i<3;i++) {
                h_matrix[k][i][j] = float(tmp_h_matrix[count++]);
            }
        }
    }

    file->close();

    delete tmp_h_matrix;
    delete phase_space;
    delete tmp_atom_data;
    delete file;
}

void Timestep::load_atoms(string mts0_directory) {
    positions.clear();
    atom_types.clear();
    atom_ids.clear();
    h_matrix.clear();

    h_matrix.resize(2);
    for(int i=0;i<2;i++) {
        h_matrix[i].resize(3);
        for(int j=0;j<3;j++) {
            h_matrix[i][j].resize(3);
        }
    }
    vector<vector<float> > positions_local;
    vector<int> atom_types_local;
    vector<int> atom_ids_local;

    int num_nodes = nx*ny*nz;
    vector<int> node_vector_index(3,0);
    vector<float> node_origin(3,0);
    vector<float> node_offset(3,0);

    char *filename = new char[1000];

    node_offset[0] = 1.0/nx;
    node_offset[1] = 1.0/ny;
    node_offset[2] = 1.0/nz;

    for(int node_id=0; node_id<num_nodes; node_id++) {
        node_vector_index[0] = node_id/(ny*nz);   // Node id in x-direction
        node_vector_index[1] = (node_id/nz) % ny; // Node id in y-direction
        node_vector_index[2] = node_id % nz; 	  // Node id in z-direction

        node_origin[0] = node_offset[0]*node_vector_index[0]; // Displacement in x-direction
        node_origin[1] = node_offset[1]*node_vector_index[1]; // Displacement in y-direction
        node_origin[2] = node_offset[2]*node_vector_index[2]; // Displacement in z-direction

        sprintf(filename,"%s/mt%04d",mts0_directory.c_str(), node_id);
        read_mts(filename, atom_types_local, atom_ids_local, positions_local);
        int num_atoms_local = positions_local.size();

        for(int j=0;j<num_atoms_local;j++) {
            for(int k=0;k<3;k++) {
                positions_local[j][k] += node_origin[k];
                positions_local[j][k] *= h_matrix[0][k][k]*bohr; // Possibly broken, we don't know how h_matrix really works
            }
        }

        positions.insert ( positions.end() , positions_local.begin() , positions_local.end()  );
        atom_types.insert ( atom_types.end() , atom_types_local.begin() , atom_types_local.end()  );
        atom_ids.insert ( atom_ids.end() , atom_ids_local.begin() , atom_ids_local.end()  );
    }

    delete filename;
    atom_types_local.clear();
    atom_ids_local.clear();
    positions_local.clear();

    node_vector_index.clear();
    node_origin.clear();
    node_offset.clear();
}

void Mts0_io::load_timesteps() {
    current_timestep = 0;

    timesteps.reserve(max_timestep+1);

    for(int timestep=0;timestep<=max_timestep;timestep++) {
        sprintf(mts0_directory, "%s/%06d/mts0/",foldername_base.c_str(), timestep);

        Timestep *new_timestep = new Timestep(string(mts0_directory),nx, ny, nz);
        timesteps.push_back(new_timestep);
        cout << "Loaded timestep " << timestep << endl;
    }
}

void Mts0_io::update_next_timestep() {
    current_timestep += step*time_direction;

    if(current_timestep>max_timestep || current_timestep < 0) {
        time_direction *= -1;
        current_timestep += step*time_direction;
    }

    if(preload) {
        Timestep *timestep = timesteps[current_timestep];
        currentTimestepObject = timestep;
    } else {
        if(get_file_extension(foldername_base).compare("xyz") == 0) {
            sprintf(mts0_directory, "%s",foldername_base.c_str());
        } else {
            sprintf(mts0_directory, "%s/%06d/mts0/",foldername_base.c_str(), current_timestep);
        }

        Timestep *timestep = new Timestep(string(mts0_directory),nx, ny, nz);
        currentTimestepObject = timestep;
    }

    emit currentTimestepObjectChanged();
}
