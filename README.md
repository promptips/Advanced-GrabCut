
# Advanced-GrabCut

This is an enhanced implementation of the image segmentation using GMM EM and Graph Cuts, based on the original work documented here: http://cvg.ethz.ch/teaching/cvl/2012/grabcut-siggraph04.pdf


## Getting Started

You can run
GrabCutExample data/soldier.png data/soldier_selection.fbmask result.png
to run an example segmentation for yourself.

## Build notes
This code depends on c++0x/11 additions to the c++ language. For Linux, this means it must be built with the flag
gnu++0x (or gnu++11 for gcc >= 4.7).

## Dependencies
- ITK >= 4
- Boost >= 1.51
You can tell this project's CMake to use a local boost build with: cmake . -DBOOST_ROOT=/path_to/boost_1_51
- Eigen >= 3.2
You can tell this project's CMake to use a local Eigen build with: cmake . -DEIGEN3_INCLUDE_DIR=/path_to/eigen-3.2.1/