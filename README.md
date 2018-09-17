# CarND-PID-Control
[![Udacity - Self-Driving Car NanoDegree](https://s3.amazonaws.com/udacity-sdc/github/shield-carnd.svg)](http://www.udacity.com/drive)

This repo contains the written code to complete the project **CarND-PID-Control** on Udacity Self-Driving Car Nanodegree. The goal is to control steer in order to keep vehicle on track. In fact, the [Proportional-Integral-Derivative Controller](https://en.wikipedia.org/wiki/PID_controller) tries to minimize the error between the actual trajectory and the center of lane. 

Installation
---
First, clone the repository:
```
git clone https://github.com/shohne/CarND-PID-Control-Project.git
```
Change current directory:
```
cd CarND-PID-Control-Project
```
Run build script:
```
./build.sh
```
Running the Application
---
To run the application:
```
./build/pid
```
Now, run the Simulator and select **Project 4: PID Controller**.

The implemented controller can drive the car as shown in this movie:

[normal.mp4](normal.mp4)

It is possible to run the controller to drive fast as in:

[fast.mp4](fast.mp4)

Implementation Details
---
Please visit the [report.md](report.md) for more information about implementation and the process to find the best parameters of the PID Controller.


