# Implementation Details of PID Controller

The core of the controller is located in the file **main.cpp** line *287*:

```
steer_value = -pidTest.tau_p * cte - (step > 0 ? pidTest.tau_d : 0) * (cte - cte_prev) - pidTest.tau_i * cte_sum;
```

Where **tau_p**, **tau_d** and **tau_i** are the controller parameters and must be choose by trial and error.

Finding the Parameters Values
---
It was decided not implement [Twiddle](https://martin-thoma.com/twiddle/) but to follow a semi-manual strategy. In fact, it is possible to realize that to find viable parameters becomes much more difficult as speed increases. We have started setting **throttle** to a low level (0.20), then we have tried values of parameters **tau_p**, **tau_d** and **tau_i** in a wide range (choosen randomly). 

The best parameters values that could drive the vehicle safely were selected (absolute values of **cte** less than 3.0). 

In the following round, the throttle was increased a little and the best parameters were found again. This procedure was repeated until throttle reached 0.65. It is noticed that it is very hard to find viable values if throttle is greater than 0.65.

To finish, final tunning in throtle was set considering that, if current **cte** is low, we can keep accelerating, if not, it is necessary to wait car to stabilize on lane to throtle again.

In the end, We have got:

```
tau_p = 0.137615
```
```
tau_d = 2.20015
```
```
tau_i = 0.0000325
```

Using this strategy, it is possible to at low speed safely and reasonably at fast speed (best lap in 39 seconds).

