//  Libraries
#include <Arduino.h>
#include <LCD_I2C.h>
#include <I2CKeyPad.h>

//  Global Const
const byte A1A = 11;
const byte A1B = 12;
const byte posSensorPin = 2;

//  Initialize I2C LCD object
LCD_I2C lcd(0x27, 16, 2); // I2C address, columns, rows

//  Initialize I2C Keypad object
I2CKeyPad keyPad(0x20);


//  Global Var
int speed = 0;
int count = 0;
int position = 100;
byte direction = false;
bool lcdFlag = false;

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

// Count number of flanks passing before break beam sensor
// triggered by hardware interrupt on posSensorPin
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

  Serial.begin(9600);
  Serial.println("go!");

  // LCD start message
  lcd.begin();
  lcd.backlight();  //backlight ON.. off with /noBacklight
  lcd.setCursor(0, 0);
  lcd.print(F("Go!"));
  delay(1000);

  // Initialize Keypad
  Wire.begin();
  Wire.setClock(400000);
  if (keyPad.begin() == true)
  {
    Serial.println("\nSUCCESS: keypad begin");
  //  while(1);
  }


}

void loop() {

  // De ene kant op
  // op gang komen
  // Serial.print(position);
  // Serial.print (' ');

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
  toggleDirection();
  delay(1000);


}
