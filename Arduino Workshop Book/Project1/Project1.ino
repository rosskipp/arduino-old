/*
Arduino Workshop
Project 1 - Creating a Blinking LED Wave
Ross Kippenbrock - 7/31/13
*/

int d = 100;

void setup()
{
  pinMode(2, OUTPUT);  // Set LEDs at outputs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop()
{
  digitalWrite(2,HIGH);  // Turn LED 1 on
  delay(d);            // wait a half second
  digitalWrite(2,LOW);   // Turn LED 1 off
  digitalWrite(3,HIGH);  // Turn LED 2 on and repeat
  delay(d);
  digitalWrite(3,LOW);
  digitalWrite(4,HIGH);
  delay(d);
  digitalWrite(4,LOW);
  digitalWrite(5,HIGH);
  delay(d);
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  delay(d);
  digitalWrite(6,LOW);
  digitalWrite(5,HIGH);
  delay(d);
  digitalWrite(5,LOW);
  digitalWrite(4,HIGH);
  delay(d);
  digitalWrite(4,LOW);
  digitalWrite(3,HIGH);
  delay(d);
  digitalWrite(3,LOW);
  // the loop repeats from here
}


