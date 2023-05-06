# Line-Following-Sensor
Created a Line following robot that uses three optocoupler sensors to follow a black line

The robot is programmed to run along a black line on the floor autonomously.
The robot’s motor is controlled by the Dual Full-Bridge Driver (U2) which receives
signals from the MCU. The MCU will output two types of signals to the driver, PWM
signal and rotational direction signal. The PWM duty cycle controls the speed of the
robot and the rotational direction controls the robot going forward or backward. The
robot can also make a turn when one motor spins faster than another. The robot has two
types of inputs, three optocoupler sensors for following the line and two front bumper
switches for avoiding an obstacle.
