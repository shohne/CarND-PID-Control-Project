# **Implementation Details of PID Controller

The core of controller is located in file **main.cpp* line 287:

```
steer_value = -pidTest.tau_p * cte - (step > 0 ? pidTest.tau_d : 0) * (cte - cte_prev) - pidTest.tau_i * cte_sum;
```

Where **pidTest.tau_p**, **pidTest.tau_d** and **pidTest.tau_i** are the controller parameters and must be choose by trial and error.

Finding the Parameters Values
---
We have decided to not implement [Twiddle](https://martin-thoma.com/twiddle/) and follow a semi-manual strategie. In fact, we could realize that find viables parameters becomes much more difficult as speed increase. We have started setting **throtle** to a low level (0.20), then we have tried values of parameters **tau_p**, **tau_d** and **tau_i** in wide range (choose randonly). We have picked the best parameters values that could drive the vehicle safely (absolute value of **cte** less than 3.0). 
In the next round, We have increased the throtle a little and searched the best parameters again. We have repeated this procedure until throtle reach 0.65. We could note that it is very hard to find viable valus if throtle is greater than 0.65.
In the final round, We have done a final tunning in throtle considering if current **cte** is low We can keep accelareting, if not, We must car stabilize to throtle again.
In the end, We have got:
```
tau_p = 0.137615
```
tau_d = 2.20015
```
```
tau_i = 0.0000325
```
Using this strategie, We could drive at low speed safety and reasonable at fast speed (best lat in 39 seconds).
