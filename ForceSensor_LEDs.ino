/*
Project to mimic the LittleBites force transducer
 Which lights up LEDs based on the pressure sensed by a little force pad
 Ross Kippenbrock - 9/2/13
 */

float lowlow = 1 * (1023 / 5);
float lowmed = 2 * (1023 / 5);
float lowhigh = 3 * (1023 / 5);
float med = 4 * (1023 / 5);
float high = 4.5 * (1023 / 5);
float sensor = 0;

void setup()
{
  // Start serial at 9600 baud
  Serial.begin(9600); 
  pinMode(2, OUTPUT);  // Set LEDs at outputs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop()
{
  //
  // Read the voltage off the force transducer
  sensor = analogRead(0);

  // Act on force reading
  // No touching
  if ( sensor < lowlow )
  {
    // Turn all LEDs off
    for ( int a = 2; a <7; a++ )
    {
      digitalWrite(a, LOW);
    }
  }
  // LED 1
  else if ( sensor < lowmed && sensor > lowlow )
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  } 
  // LED 2
  else if ( sensor < lowhigh && sensor > lowmed )
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  } 
  // LED 3
  else if ( sensor < med && sensor > lowhigh )
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  } 
  // LED 4
  else if ( sensor < high && sensor > med )
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
  }
  // LED5
  else if ( sensor > high )
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
  }

}



