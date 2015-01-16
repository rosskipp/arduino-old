#include <NewPing.h>
 
#define TRIGGER_PIN  3
#define ECHO_PIN     2
//#define MAX_DISTANCE 200
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN);
 
void setup() {
  Serial.begin(115200);
}
 
void loop() {
  delay(50);
  int uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM);
  Serial.println("cm");
}

