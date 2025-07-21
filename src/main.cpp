//  Let op richting, teller en verschil met eerdere versie.


//  Libraries
#include <Arduino.h>
#include <LCD_I2C.h>
#include <I2CKeyPad.h>

//  Proprietary types
enum ElevatorStates {STOPPED, ACCELERATING, DECELERATING, MOVING};

//  Global Const
const int A1A = 11;
const int A1B = 12;
const int posSensorPin = 2;

//  Initialize I2C LCD and Keypad objects
LCD_I2C lcd(0x27, 16, 2); // I2C address, columns, rows
I2CKeyPad keyPad(0x20); // I2C address


int speed = 0;           
int position = 100;        
bool direction = false;   
//  int destination = 0;     

//  interface, output
bool lcdFlag = false;     //  for tracking if LCD needs updating
int count = 0;            //  test variable for showing increasing value on display

ElevatorStates elevatorState = STOPPED;

//  Functions

void motorA(int s, bool d)  {
  if (d == true) {
    analogWrite (A1A, 255-s);
    digitalWrite(A1B, HIGH);
  }
  if (d == false) {
    analogWrite (A1A, s);
    digitalWrite (A1B, LOW);
  }
}

void lcdPrint() {
  if (lcdFlag == false) return;

  count++;
  lcd.clear();
  lcd.print(count);
  lcd.print(position);

  lcdFlag = false;
}

void changePosition () {
  if (direction == true)  {
    position--;
  }
  else  {
    position++;
  }
}

void toggleDirection()  {
 direction = !direction;
}

void doLCD()  {
  lcd.clear();
  lcd.print(position);
  lcd.print(F(" "));
  lcd.print(direction);

}

void setup() {
  // put your setup code here, to run once:

  pinMode(posSensorPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(posSensorPin),
                  changePosition,
                  CHANGE);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);

  // Serial.begin(9600);
  // Serial.println("go!");

  // LCD start message
  lcd.begin();
  lcd.backlight();  //backlight ON.. off with /noBacklight
  lcd.setCursor(0, 0);
  lcd.print(F("Go!"));
  delay(1000);

  // Initialize Keypad
  Wire.begin();
  Wire.setClock(400000);
  lcd.setCursor(0, 1);

  if (keyPad.begin() == true)
  {
    lcd.print(F("KEYPAD SUCCESS"));
  }
  else  {
    lcd.print(F("KEYPAD FAIL"));
  }

  direction = false;


}

void loop() {

  // Op gang komen
  motorA(155, direction);
  delay(2500);
  doLCD();

  // Volle snelheid
  motorA(255, direction);
  delay(2500);
  doLCD();

  // Afremmen
  motorA(155, direction);
  delay(2500);
  doLCD();

  // Stoppen en Omkeren
   motorA(0, direction);
   toggleDirection();
   delay(1000);
  
  // De andere kant op
  motorA(155, direction);
  delay(2500);
  doLCD();

  motorA(255, direction);
  delay(2500);
  doLCD();

  motorA(155, direction);
  delay(1000);
  doLCD();

  motorA(0, direction);
  delay(1000);


}
