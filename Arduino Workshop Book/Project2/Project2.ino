/*
Arduino Workshop
Project 2 - Repeating Project 1 with For Loops
Ross Kippenbrock - 7/31/13
*/

int d = 250;

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
  for ( int a = 2; a < 7; a++ )
  {
    digitalWrite(a, HIGH);
    delay(d);
    digitalWrite(a, LOW);
    delay(d);
  }
  for ( int a = 5; a > 2; a-- )
  {
    digitalWrite(a, HIGH);
    delay(d);
    digitalWrite(a, LOW);
    delay(d);
  }
}


