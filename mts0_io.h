/*
mts0_io.cpp mts0_io.h
Created by Jørgen Trømborg and Anders Hafreager on 15.04.13.
Copyright (c) 2013 Universitetet i Oslo. All rights reserved.

load_atoms(nx, ny, nz, velocities, positions, atom_type, h_matrix, mts0_directory)
save_atoms(nx, ny, nz, velocities, positions, atom_type, h_matrix, mts0_directory)
nx, ny, nz     - number of cpus [int].
velocities     - atom velocities [vector<vector<double> >], dimension num_atoms x 3, units unknown
positions      - atom positions [vector<vector<double> >], dimension num_atoms x 3, units Ångström
atom_types     - atom types [vector<int>], dimension num_atoms, {1-Si,2-A,3-H,4-O,5-Na,6-Cl,7-X}
atom_ids       - atom ids [vector<int>], dimension num_atoms
h_matrix       - h-matrix [vector<vector<vector<double> > >], dimension 2 x (3 x 3)
mts0_directory - mts0-directory [string]
*/

#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <QTimer>
#include <QObject>

using namespace std;

class Timestep {
public:
  int nx, ny, nz;
  double bohr = 0.5291772;
  vector<vector<float> > positions;
  vector<int> atom_ids;
  vector<int> atom_types;
  vector<vector<vector<float> > > h_matrix;
  int get_number_of_atoms();
  vector<float> get_lx_ly_lz();

  Timestep(string filename, int nx_, int ny_, int nz_);
  ~Timestep();
  void load_atoms(string filename);
  void load_atoms_xyz(string xyz_file);
  void read_data(ifstream *file, void *value);
  void read_mts(char *filename, vector<int> &atom_types_local, vector<int> &atom_ids_local, vector<vector<float> > &positions_local);
};

class Mts0_io : public QObject {
    Q_OBJECT
private:
  char mts0_directory[5000];
  int max_timestep;
  bool preload;
  vector<Timestep*> timesteps;
  string foldername_base;
  QTimer timer;

public:
  int step;
  int current_timestep;
  Timestep *currentTimestepObject;
  int nx, ny, nz;
  int time_direction;
  Mts0_io(QObject *parent = NULL);
  ~Mts0_io() {}

  // Mts0_io(int nx_, int ny_, int nz_, int max_timestep_, string foldername_base_, bool preload_, int step_);

  void load_timesteps();
public slots:
  void update_next_timestep();

signals:
  void currentTimestepObjectChanged();
};
