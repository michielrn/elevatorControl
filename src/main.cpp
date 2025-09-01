// TODO
// in void loop()
// refactor 

//  Libraries
#include <Arduino.h>
#include <LCD_I2C.h>
#include <I2CKeyPad.h>

//  Custom types
enum ElevatorStates {STOPPED, GET_INPUT, ACCELERATING, DECELERATING, MOVING_FULL_SPEED};

//  Global Const
const int A1A = 11;
const int A1B = 12;
const int posSensorPin = 2;
const int upward = -1;
const int downward = 1;
// const byte upward = false; // upward was forward
// const byte downward = true; // downward was backward
const byte kbInput = 2;
const byte displacement = 3;
const byte showDestination = 5;


//  initialize I2C LCD and Keypad objects
LCD_I2C lcd(0x27, 16, 2); // I2C address, columns, rows
I2CKeyPad keyPad(0x20); // I2C address

//  Global vars
int speed = 0;           
int position = 100;        
int direction = upward;   
int destination = 100;
int distance = 0;
int value = 0;     
byte lcdFlag = 0;
byte count = 0;
ElevatorStates elevatorState = STOPPED;


// Function declarations
// void motorA(int s, int d);
// void changePosition();
// void toggleDirection();
// // char handleKeyPadValue(value);
// void lcdPrint();
// void doLCD();
// void elevatorMachine();

// Main program

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
      while (destination == position) {break;}
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

void changePosition () {
  if (direction == downward)  {
    position--;
  }
  else  {
    position++;
  }
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

char handleKeyPadValue(int &value)
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
  return c;
}


void setup() {
  pinMode(posSensorPin, INPUT_PULLUP);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(posSensorPin),
                  changePosition,
                  CHANGE);


  Serial.begin(9600);
  Serial.println("go!");

  lcd.begin();
  lcd.backlight();  //backlight ON.. off with /noBacklight
  lcd.setCursor(0, 0);
  lcd.print(F("Go!"));
  lcd.setCursor(0, 1);

  Wire.begin();
  Wire.setClock(400000);
 
  if (keyPad.begin() == true) {lcd.print(F("KEYPAD SUCCESS"));}
  else  {lcd.print(F("KEYPAD FAIL"));}

  elevatorState = GET_INPUT;

    // op gang komen
  Serial.print(position);
  Serial.print (' ');
  delay(1000);
  motorA(155, direction);
  delay(1000);

  // Volle snelheid

  motorA(255, direction);
  delay(2500);

  // afremmen
  motorA(155, direction);
  delay(1000);
  
  // stoppen
  motorA(0, direction);
  doLCD();

  // Omkeren
  toggleDirection();
  
  // De andere kant op
  Serial.println(position);
  delay(2500);
  motorA(155, direction);

  // Volle snelheid

  delay(1000);
  motorA(255, direction);

  // Afremmen

  delay(500);
  motorA(155, direction);

  // Stoppen
  
  delay(500);
  motorA(0, direction);
  doLCD();
  Serial.println(position);
  
  toggleDirection();

  lcd.clear();
  lcd.setCursor(0,0);

}

void loop() {

  //  elevatorMachine();
  char c = handleKeyPadValue(value);
  lcdPrint();
  // Serial.println(value);



  // while(1);


}
