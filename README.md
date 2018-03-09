# Extended-Kalman-Filter-SDC-Term2-P1-Udacity

Self-Driving Car Engineer Nanodegree Program

In this project I utilized a kalman filter to estimate the state of a moving object of interest with noisy lidar and radar measurements. Passing the project requires obtaining RMSE values that are lower than the tolerance outlined in the project rubric (.11, .11, .52, .52).

This project involves the Term 2 Simulator which can be downloaded [here](https://github.com/udacity/self-driving-car-sim/releases)

## Prerequisites
The project has the following dependencies (from Udacity's seed project):

* cmake >= 3.5
* make >= 4.1
* gcc/g++ >= 5.4
* Udacity's simulator.
For instructions on how to install these components on different operating systems, please, visit [Udacity's seed project](https://github.com/udacity/CarND-Extended-Kalman-Filter-Project). As this particular implementation was done on Windows 10 using Bash, the rest of this documentation will be focused on Bash.

In order to install the necessary libraries, use the (./install-ubuntu.sh).

Compiling and executing the project
These are the suggested steps:

Clone the repo and cd to it on a Terminal.
Create the build directory: mkdir build
cd build
cmake ..
make: This will create two executables
ExtendedKF : EKF implementation.
