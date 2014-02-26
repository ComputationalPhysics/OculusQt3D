#!/usr/bin/python
import subprocess
import os
import os.path
import Tkinter, tkFileDialog
import sys

if len(sys.argv) < 4:
    print "Note, you may also input paths as ./setup.py <qmake_path> <oculussdk_path> <install_path>"
    qmake_path = raw_input("Path to qmake: ")
    oculus_path = raw_input("Path to OculusSDK: ")
    libs_path = raw_input("Installation directory (oculusqt3d will be appended): ")
else:
    qmake_path = sys.argv[1]
    oculus_path = sys.argv[2]
    libs_path = sys.argv[3]

if qmake_path[-5:] != "qmake":
    qmake_path = os.path.join(qmake_path, "qmake")
if not os.path.isfile(qmake_path):
    print "Cannot find qmake in " + qmake_path
    sys.exit(0)
if oculus_path[-9:] != "OculusSDK" and oculus_path[-10:] != "OculusSDK/":
    oculus_path = os.path.join(oculus_path, "OculusSDK")
if not os.path.exists(oculus_path):
    print "Cannot find Oculus SDK in " + oculust_path
libs_path = os.path.join(libs_path, "oculusqt3d")
if not os.path.exists(libs_path):
    os.makedirs(libs_path)

# ************
#     Qt3D
# ************

print "Looking for Qt3D..."
tmppro = open("tmp.pro", "w")
tmppro.write("QT += 3dquick")
tmppro.close()
subprocess.call(qmake_path + " tmp.pro 2> errors.tmp", shell=True)
errorstmp = open("errors.tmp", "r")
errors = errorstmp.read()
if errors == "Project MESSAGE: Warning: unknown QT: 3dquick\n":
    print "Qt3D missing. Installing now..."
    qt3d_src_path = os.path.join(libs_path, "qt3d")
    if not os.path.exists(qt3d_src_path):
        subprocess.call("git clone https://git.gitorious.org/qt/qt3d.git " + qt3d_src_path, shell=True, cwd=libs_path)
    else:
        subprocess.call("git pull origin master", shell=True, cwd=qt3d_src_path)
    qt3d_build_path = os.path.join(libs_path, "build-qt3d")
    if not os.path.exists(qt3d_build_path):
        os.makedirs(qt3d_build_path)
    subprocess.call(qmake_path + " " + os.path.join(qt3d_src_path, "qt3d.pro"), shell=True, cwd=qt3d_build_path)
    subprocess.call("make", shell=True, cwd=qt3d_build_path)
    subprocess.call("make install", shell=True, cwd=qt3d_build_path)
else:
    print "Qt3D found as already installed."

# ****************
#  MultiBillboard
# ****************

print "Cloning and installing MultiBillboard"
multibillboard_src_path = os.path.join(libs_path, "multibillboard")
multibillboard_build_path = os.path.join(libs_path, "build-multibillboard")
if not os.path.exists(multibillboard_src_path):
    subprocess.call("git clone https://github.com/dragly/multibillboard " + multibillboard_src_path, shell=True, cwd=libs_path)
else:
    subprocess.call("git pull origin master", shell=True, cwd=multibillboard_src_path)
if not os.path.exists(multibillboard_build_path):
    os.makedirs(multibillboard_build_path)
subprocess.call(qmake_path + " " + os.path.join(multibillboard_src_path, "multibillboard.pro"), shell=True, cwd=multibillboard_build_path)
subprocess.call("make", shell=True, cwd=multibillboard_build_path)
subprocess.call("make install", shell=True, cwd=multibillboard_build_path)


# ****************
# FlyModeNavigator
# ***************

print "Cloning and installing FlyModeNavigator"
flymode_src_path = os.path.join(libs_path, "flymodenavigator-qt3d")
flymode_build_path = os.path.join(libs_path, "build-flymodenavigator-qt3d")
if not os.path.exists(flymode_src_path):
    subprocess.call("git clone https://github.com/ComputationalPhysics/flymodenavigator-qt3d.git " + flymode_src_path, shell=True, cwd=libs_path)
else:
    subprocess.call("git pull origin master", shell=True, cwd=flymode_src_path)
if not os.path.exists(flymode_build_path):
    os.makedirs(flymode_build_path)
subprocess.call(qmake_path + " " + os.path.join(flymode_src_path, "flymodenavigator.pro"), shell=True, cwd=flymode_build_path)
subprocess.call("make", shell=True, cwd=flymode_build_path)
subprocess.call("make install", shell=True, cwd=flymode_build_path)

# **************
# Oclus Rift SDK
# **************

print "Setting up Oculus SDK includes..."
oculusincludes = open("oculus.pri", "w")
oculusincludes.write("# Created by setup.py. Do not modify manually!\n")
oculusincludes.write("INCLUDEPATH += " + oculus_path + "/LibOVR/Include\n")
oculusincludes.write("LIBS += -L"+ oculus_path + "/LibOVR/Lib/Linux/Release/x86_64/ -lovr\n")
oculusincludes.close()
print "All done! You may now open oculusqt3d.pro and build the project"
