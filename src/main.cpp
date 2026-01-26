//  Libraries
#include <Arduino.h>
#include <LCD_I2C.h>
#include <I2CKeyPad.h>

//  Types
enum ElevatorStates {STOPPED, GET_INPUT, ACCELERATING, DECELERATING, MOVING_FULL_SPEED};

//  Constants
const int A1A = 11;         // Motor Control pins
const int A1B = 12;
const int posSensorPin = 2; //  Input pin position sensor
const int upward = -1;      //  Direction mapping
const int downward = 1;
const byte kbInput = 2;     //  LCD status bytes
const byte displacement = 3;
const byte showDestination = 5;

//  initialize I2C LCD and Keypad objects
LCD_I2C lcd(0x27, 16, 2); // I2C address, columns, rows
I2CKeyPad keyPad(0x20); // I2C address

//  Global vars
unsigned long currentMillis;  // for timers
unsigned long previousMillis = 0;
byte speed = 0;           
volatile int position = 100;        
int direction = upward; // upwards is -1
int destination = 100;
int distance;       // Necessary? Maybe better in loop conditions?
int value = 0;          // for Keypad
int counter = 0;        // for the loops
byte lcdFlag = 0;
byte count = 0;
ElevatorStates elevatorState = STOPPED;

// Function definitions
void elevatorMachine(){
  switch (elevatorState)  {
    case STOPPED: {
      delay(2000);
      lcd.clear();
      elevatorState = GET_INPUT;
      break;
    }
    case GET_INPUT: {
      if (destination == position) {break;}
      elevatorState = ACCELERATING;
      break;
    }
    case ACCELERATING:  {
      break;
    }
    case DECELERATING:  {
      break;
    }
    case MOVING_FULL_SPEED: {
      break;
    }
  }
}

void motorA(int s, int d)  {
  if (d == downward) {
    analogWrite (A1A, 255-s);
    digitalWrite(A1B, HIGH);
  }
  if (d == upward) {
    analogWrite (A1A, s);
    digitalWrite (A1B, LOW);
  }
}

void changePosition () {  // Attached to interrupt (see setup())
  position -= direction; // if direction = -1, increase position by 1
}

void toggleDirection()  {
 direction = (-1 * direction);
}

void lcdPrint() {
  if (lcdFlag == 0) return;
  if (lcdFlag == kbInput) {
    lcd.clear();
    lcd.print(value);
    lcdFlag = 0;
    Serial.println(value);
    return;
  }
  if (lcdFlag == showDestination)   {
    lcd.clear();
    lcd.print(F("Moving to: "));
    lcd.print(destination);
    Serial.print(F("Moving to "));
    Serial.print(destination);
    lcdFlag = 0;
    return;
  }

  count++;
  lcd.clear();
  lcd.print(count);
  lcd.print(position);

  lcdFlag = 0;
}

void doLCD()  {
  lcd.clear();
  lcd.print(position);
  lcd.print(F(" "));
  lcd.print(direction);

}

void handleKeyPadValue(int &value)
{
  char v[19] = "123A456B789C*0#DNF";  //  N = NoKey, F = Fail
  static uint8_t lastKey = 0;

  uint8_t idx = keyPad.getKey();
  char c = v[idx];

  if (lastKey != c)
  {
    lastKey = c;
    switch (c)
    {
      case '0' ... '9':
        value *= 10;
        value += c - '0';
        lcdFlag = kbInput;
        break;
      case '*':
        if (value > 0) value /= 10;
        lcdFlag = kbInput;
        break;
      case '#':
        value = 0;
        lcdFlag = kbInput;
        break;
      case 'A':
        destination = position + value;
        value = 0;
        lcdFlag = showDestination;
        break;
      case 'B':
        destination = position - value;
        lcdFlag = showDestination;
        value = 0;
        break;
      case 'C':
        break;
      case 'D':
        destination = value;
        lcdFlag = showDestination;
        value = 0;
        break;
      case 'F':
        Serial.println("FAIL");
        break;
      case 'N':
        // Serial.println("NOKEY");
        break;
      default:
        break;
    }
  }
  // return c;
}

void doSerial() {
    Serial.print(position);
    Serial.print(" ");
    Serial.print(direction);
    Serial.print(" ");
    Serial.print(distance);
    Serial.print(" ");
    Serial.println(speed);
}

void updateDistance() {   //distance always positive integer
  distance = (abs(position - destination)); 
}

void setDirection() {
  direction =  ((position-destination) / abs (position - destination));
}

void setup() {
  pinMode(posSensorPin, INPUT_PULLUP);  // position sensor
  pinMode(A1A, OUTPUT);                 // Motor pin A
  pinMode(A1B, OUTPUT);                 // Motor pin b

  attachInterrupt(digitalPinToInterrupt(posSensorPin),
                  changePosition,
                  CHANGE);


  Serial.begin(9600);
  Serial.println("go!");

  // Initialise the LCD display
  lcd.begin();
  lcd.backlight();  //backlight ON.. off with /noBacklight
  lcd.setCursor(0, 0);
  lcd.print(F("Go!"));
  lcd.setCursor(0, 1);

  //  Initialise the I2C channel for Keypad and test connection
  Wire.begin();
  Wire.setClock(400000);
  if (keyPad.begin() == true) {lcd.print(F("KEYPAD SUCCESS"));}
  else  {lcd.print(F("KEYPAD FAIL"));}

  // Motor heen en weer, voor testen alle onderdelen

  delay(1000);

  // maak precies 1 rondje
  destination = 180;
  setDirection(); // wordt dus 1 voor naar beneden of -1 voor naar boven
  Serial.println(direction);

  
  while (abs(position - destination) >= 1)  {
    motorA(155, direction);
  }
  doLCD();
  doSerial();
  
  motorA(0, direction);
  doLCD();
  doSerial();
  
  delay(1000);

  // terug naar 100
  destination = 100;
  setDirection(); // wordt dus 1 voor naar beneden of -1 voor naar boven
  Serial.println(direction);
  
  while (abs(position - destination) >= 1)  {
    motorA(155, direction);
  }
  // doLCD();
  // doSerial();
  // while (abs(position - destination) > 2)  {
  //   motorA(155, direction);
  // }
  motorA(0, direction);
  doLCD();
  doSerial();
}

void loop() {

  //  elevatorMachine();
  handleKeyPadValue(value);
  lcdPrint();
  // Serial.println(value);



  // while(1);


}
