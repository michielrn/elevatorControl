# Developing a state machine to control a model elevator
This code programs an arduino Uno (rev 3) to move an elevator based on commands from inside the elevator car and from the platforms on all the floors in the building. Software and hardware development are happening simultaneously. A model will be constructed later with a suitable platform.

## Notepad
- In the current setup, direction -1 is upward and 1 is downward. (Red lead on inner terminal of motor driver board)
Therefore, when position = 30 and destination = 15 (moving downward to get to destination):
- distance = abs(position - destination) = abs(30 - 15) = 15
- direction = int(distance / (position - destination)) = 15/15 = 1.

## New problem: using PWM on motor output pin interferes with counts of interrupt events
Found in: ```setup()```:
Using pin 11 or pin 6 for motor speed control, and using pwm duty cycle of 155, ounting of interrupt events (which determine the position of the elevator main axle) is moving upward (but not Downward). At full speed, this does not occur. Hypothesis: PWM signal from the Arduino Uno R3 interferes with the interrupt pin. 

Performed diagnostics: scope reading of output pin shows _some_ bounce but it seems not enough to mess up the input signal. Also, input signal happens over RC network of 1 kOhm, 100nF for debounce, bounce amplitude is <1v.

Next diagnostics: scope reading pf PWM pins

Attempted solutions: using PWM pin 6 instead of 11: no result.
Next attempt: set different duty cycle of 180, 200.
