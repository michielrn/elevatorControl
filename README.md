# Developing a state machine to control a model elevator
This code programs an arduino Uno (rev 3) to move an elevator based on commands from inside the elevator car and from the platforms on all the floors in the building. Software and hardware development are happening simultaneously. A model will be constructed later with a suitable platform.
# Notepad
- distance = abs(destination - position)
- direction = int(distance - (destination - position))