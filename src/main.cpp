//  Libraries
#include <Arduino.h>
#include <LCD_I2C.h>
#include <I2CKeyPad.h>

//  Custom types
enum ElevatorStates {STOPPED, ACCELERATING, DECELERATING, MOVING};

//  Global Const
const int A1A = 11;
const int A1B = 12;
const int posSensorPin = 2;
const byte upward = false; // upward was forward
const byte downward = true; // downward was backward

//  I2C LCD and Keypad objects
LCD_I2C lcd(0x27, 16, 2); // I2C address, columns, rows
I2CKeyPad keyPad(0x20); // I2C address

//  Global vars
int speed = 0;           
int position = 100;        
bool direction = upward;   
int destination = 0;     
bool lcdFlag = false;
int count = 0;
ElevatorStates elevatorState = STOPPED;
int keyPadValue = 0;

//  Function declarations
void motorA(int s, bool d);
void changePosition();
void toggleDirection();
char handleKeypadInput (int &keyPadValue);
void lcdPrint();
void doLCD();

// Main program

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
}

void loop() {

  // op gang komen
  Serial.print(position);
  Serial.print (' ');
  delay(2500);
  motorA(155, direction);
  delay(2500);

  // Volle snelheid

  motorA(255, direction);
  delay(2500);

  // afremmen
  motorA(155, direction);
  delay(2500);
  
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

  delay(2500);
  motorA(255, direction);

  // Afremmen

  delay(2500);
  motorA(155, direction);

  // Stoppen
  
  delay(1000);
  motorA(0, direction);
  doLCD();
  
  toggleDirection();


}

// Function definitions
void motorA(int s, bool d)  {
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
 direction = !direction;
}

void lcdPrint() {
  if (lcdFlag == false) return;

  count++;
  lcd.clear();
  lcd.print(count);
  lcd.print(position);

  lcdFlag = false;
}

void doLCD()  {
  lcd.clear();
  lcd.print(position);
  lcd.print(F(" "));
  lcd.print(direction);
}

char handleKeypadInput(int &keyPadValue) {
  return;
}