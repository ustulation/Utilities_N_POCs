Utilities_N_POCs
================

Proof of concepts that I make for the organizations I contribute to or work in, to demonstrate, substantiate, measure etc.

To use (tested on Ubuntu 14.04):

`<1>` Install git, cmake and g++:
```
sudo apt-get install cmake git git-gui g++
```
`<2>` Set up the project:
```
mkdir -p any_name/build
cd any_name
git clone git@github.com:ustulation/Utilities_N_POCs.git
```
`<3>` build and run (Eg., project `CompileTimeMap`):
```
cd build
cmake ../Utilities_N_POCs/
make CompileTimeMap
./CompileTimeMap/CompileTimeMap
 ```
