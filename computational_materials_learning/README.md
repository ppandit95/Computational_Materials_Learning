//////////////////////////////////////////////////////////////////////////////////////
A simple Repository to Determine Chemical Diffusivity using Arrhenius Equation for a Range of Temperatures
///////////////////////////////////////////////////////////////////////////////////// 

This repository requires - 
git version 2.43.0
cmake version 3.28.3
Python 3.12.3
g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
Python 3.12.3
NumPy 2.5.2
JAX 0.11.0
marimo 0.23.16

It contains a C++ code which calculates the chemical Diffusivities in cpp_foundations/ folder which can be built via commands -

cmake -S . -B build
cmake --build build

The executables can be accessed in build folder generated.Moreoover, any configuration of build can be altered through CMakeLists.txt .

To run the C++ executable ,
./build/...

To activate the python environment , one has to install dependencies in a virtual environment as illustrated requirements above in .venv virtual environment through

python3 -m venv .venv
source .venv/bin/activate

In this environment, a marimo arrhenius book can be launched via 
marimo edit marimo/arrhenius_diffusivity.py

While computing diffusivity if there is some sign change in foormula, then the code prompts a relevant output which can help one to debug whether the formula is being used correctly to compute diffusivities or not.Physically, one expects
dD/dT > 0 for provided D0,Q,R,T[].
			 



