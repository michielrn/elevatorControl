# Developing a state machine to control a model elevator
This code programs an arduino Uno (rev 3) to move an elevator based on commands from inside the elevator car and from the platforms on all the floors in the building. Software and hardware development are happening simultaneously. A model will be constructed later with a suitable platform.

# Notepad
- In the current setup, direction -1 is upward and 1 is downward.
Therefore, when position = 30 and destination = 15 (moving downward to get to destination):
- distance = abs(position - destination) = abs(30 - 15) = 15
- direction = int(distance / (position - destination)) = 15/15 = 1.

# New problem: counter during timed operation >> counter during state machine operation
Found in: ```setup()```:
Turning the motor on and off (full speed) for a predetermined time using ```delay``` has a certain result for position in both directions, roughly the same. Moving the motor whilst updating ```distance``` and accelerating/decelerating according to this value results in FAR SHORTER movement. What is going on?