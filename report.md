# **Implementation Details of PID Controller

The core of controller is located in file **main.cpp* line 287:

```
steer_value = -pidTest.tau_p * cte - (step > 0 ? pidTest.tau_d : 0) * (cte - cte_prev) - pidTest.tau_i * cte_sum;
```

Where **pidTest.tau_p**, **pidTest.tau_d** and **pidTest.tau_i** are the controller parameters and must be choose by trial and error.

Finding the Parameters Values
---


### 

There are 3 main steps in this code:
1. Initialization;
2. Prediction;
3. Update state;

All these 3 steps are in the file **src/FusionEKF.cpp**, the *initialization* is on class *FusionEKF* constructor and beginning of FusionEKF::ProcessMeasurement method. The *Prediction** and *Update* are in **FusionEKF::ProcessMeasurement**.

There is a auxiliary class *Tools* that computes:
1. RMSE [root mean square error](https://en.wikipedia.org/wiki/Root-mean-square_deviation), method **Tools::CalculateRMSE**;
2. Jacobian Matrix for Radar, method **Tools::CalculateJacobian**;
3. Tools::AdjustAngle that, from a given angle, returns the correspondent between -&#960; and &#960;.

