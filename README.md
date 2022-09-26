Implemenation of a line following robot using a PIC32 Kit board.
Hardware: One  IR sensor pmod, 4 IR sensors, 2 R/C servos, Robot Kit Frame, Microphone

Description: 
This is a line following robot that uses microphone to start on two claps within a second .
In addition, stop the robot at endline without going over. Uses 4 IR sensors to sense the 
line on the ground and follow. Use servo motors to move a battery powered and autonomous robot.
PWM period should be 16~23ms, which means the frequency should be 44Hz~62Hz.
The pulse width range is from 0.9ms to 2.1ms. Uses the Output Compare modules in PWM mode to control the two servos. Additional file groups the declarations of the functions that include libraries such as lcd/utils/servo/swt/Timer3/led were provided by Diligent