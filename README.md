# Developing a state machine to control a model elevator
This code programs an arduino Uno R3 to move an elevator based on commands from inside the elevator car and from the platforms on all the floors in the building. Software and hardware development are happening simultaneously. A model will be constructed later with a suitable platform.

## Notepad
- In the current setup, direction -1 is upward and 1 is downward. (Red lead on inner terminal of motor driver board)
Therefore, when position = 30 and destination = 15 (moving downward to get to destination):
- distance = abs(position - destination) = abs(30 - 15) = 15
- direction = int(distance / (position - destination)) = 15/15 = 1.

## Hardware setup


## Unsolved problem: using PWM on motor output pin interferes with counts of interrupt events
The goal is to determine the position of a gear mounted on an axle driven by a DC motor by counting the passing teeth before an IR receiver (Break Beam Sensor) connected to pin 2 (interrupt pin). The change of the output of the IR reveiver (the "event") triggers the interrupt routine which adds or subtracts 1 from the position of the gear, depending on the direction of rotation. For a gear with N teeth, a full revolution is therefore complete when 2*N events have been detected as both the obstruction of the light and the passing of the obstruction count as events.

Motor is controlled with a L9110 chip over pins 6 (PWM) to A1A and 8 to A1B.

Two I2C devices are connected to SDA and SDC: a port expander for a keypad and a 1602 LCD screen. They are for now considered to be of no effect on the problem at hand.

### The problem
Using pwm duty cycle of 155 or 180 (out of 255), counting of interrupt events (which determine the position of gear on the axle) is unreliable: the counting of events happens MUCH more than the actual occurrence. At full speed, this does not occur. 

Hypothesis: PWM signal from the Arduino Uno R3 interferes with the interrupt pin. 

Performed diagnostics: scope reading of output pin shows _some_ bounce but it seems not enough to mess up the input signal. Also, input signal happens over RC network of 1 kOhm, 100nF for debounce, bounce amplitude is <1v. An additional 1 uF capacitor in parrallel with the 100nF has no effect.

Scope reading pf PWM pins is normal.
Scope reading of Infrared gate output is normal.
