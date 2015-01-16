// Using Plot.ly's Arduino API to visualize Temperature and Humidity Readings from A DHT22 Sensor

#include <SPI.h>
#include <Ethernet.h>
#include "plotly_ethernet.h"
#include "DHT.h"


// Sensor Setup
#define DHTPIN 2 // We have connected the DHT to Digital Pin 2
#define DHTTYPE DHT22 // DHT 22 (RHT03) Sensor
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT object

//initialize plotly global variables
plotly plotly; // initialize a plotly object, named plotly
char layout[]="{}";
char filename[] = "Apartment_Temp_Humid"; // name of the plot that will be saved in your plotly account -- resaving to the same filename will simply extend the existing traces with new data

// Ethernet Setup
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // doesn't really matter
byte my_ip[] = { 172, 96, 161, 232 }; // google will tell you: "public ip address"

// Start Ethernet
void startEthernet(){
    Serial.println("Initializing ethernet");
    if(Ethernet.begin(mac) == 0){
        Serial.println("Failed to configure Ethernet using DHCP");
        Ethernet.begin(mac, my_ip);
    }
    Serial.println("Done initializing ethernet");
    delay(1000);
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
    
  dht.begin(); // initialize dht sensor reading
  startEthernet();    // initialize ethernet
  
  // Initialize plotly settings
  plotly.VERBOSE = true; // turn to false to suppress printing over serial
  plotly.DRY_RUN = false; // turn to false when you want to connect to plotly's servers 
  plotly.username = "USERNAME"; // plotly username
  plotly.api_key = "API_KEY"; // "public_arduino"'s api_key -- char api_key[10]
  plotly.timestamp = true; // tell plotly that you're stamping your data with a millisecond counter and that you want plotly to convert it into a date-formatted graph
  plotly.timezone = "America/New_York"; // full list of timezones is here:
}

void loop() {
  
      float h = dht.readHumidity(); // read humitidy from DHT pin
      float t = ((dht.readTemperature() - 1.5) * 9/5 ) + 32; // read temperature from DHT pin (ours calibrated accuractly to our home thermostat with -5 to the temperature)
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.println("");
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println("");
      Serial.println("");

      // Open the Stream
      plotly.open_stream(1, 2, filename, layout); // plotlystream(number_of_points, number_of_traces, filename, layout)

      plotly.post(millis(),t); // post temperature to plotly (trace 1)
      delay(150);
      plotly.post(millis(),h); // post humidity to plotly (trace 2)
      
      for(int i=0; i<300; i++) //number of seconds to wait before posting again (300 = 5 minutes)
      { 
        delay(1000);
      }
  }
