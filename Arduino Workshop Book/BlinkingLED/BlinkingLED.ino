/* 
Blinking LED sketch 
Created by Ross Kippenbrock Jul 29, 2013
*/


void setup()
{
  pinMode(13, OUTPUT);  // set digital pin 13 to an output
}

void loop()
{
  digitalWrite(13, HIGH);  // turn on LED
  delay(250);              // pause for 0.25 second
  digitalWrite(13, LOW);   // turn off LED  
  delay(250);              // pause for 0.25 second
}
