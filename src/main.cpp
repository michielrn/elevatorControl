// Libraries
#include <Arduino.h>

// Global Const
const int A1A = 11;
const int A1B = 12;
const int posSensorPin = 2;


// Global Var
int speed = 0;
int position = 100;
bool direction = false;

// Functions

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

}

void loop() {

  // De ene kant op
  // op gang komen
  Serial.print(position);
  Serial.print (' ');
  delay(2500);
  motorA(155, direction);

  // Volle snelheid

  delay(2500);
  motorA(255, direction);

  // Afremmen

  delay(2500);
  motorA(155, direction);

  // Stoppen
  
  delay(2500);
  motorA(0, direction);
 
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
  toggleDirection();


}
