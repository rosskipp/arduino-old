#define echoPin 2
#define triggerPin 3

void setup(){
  Serial.begin (9600); // start the serial stream
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop(){
  int duration, distance;
  
  // Turn on the trigger for 1 millisecond
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(triggerPin, LOW);
  
  // Find the duration it took to get the signal using the pulseIn function
  duration = pulseIn(echoPin, HIGH);
  // Calc the distance and convert to cm
  distance = (duration / 2) / 29.1;
  
  // Print the results
  Serial.print(distance);
  Serial.print(" in");
  Serial.println();
  delay(500);
  
}

