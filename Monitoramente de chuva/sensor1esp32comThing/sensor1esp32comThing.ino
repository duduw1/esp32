#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "Robson";   // Write your SSID
const char* password = "98834198";   // Write your WIFI password

WiFiClient  client;

unsigned long Channel_ID = 1569208;  //replace with your Channel ID
const char * API_key = "76N1P3X8RJYPWTWW";


unsigned long last_time = 0;
unsigned long Delay = 30000;

const int trigPin = 5;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

    WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  if ((millis() - last_time) > Delay) {
    

    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Connecting...");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

        
    int Data = ThingSpeak.writeField(Channel_ID, 1, distanceCm,API_key);
 
    if(Data == 200){
      Serial.println("Channel updated successfully!");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(Data));
    }
    last_time = millis();
  }

  
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  
  delay(1000);
}
