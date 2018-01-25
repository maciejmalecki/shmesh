/*
   SmartHome Node Unit implementation
*/
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh.h>
#include <LowPower.h>

const int sensorPin = 0;
const int ledPin = 2;
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);
uint8_t nodeAddress = 1;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  mesh.setNodeID(nodeAddress);
  mesh.begin();
  Serial.print("Node Unit initialized");
}

void loop()
{
  mesh.update();
  float temp = readTemp();
  bool error = false;
  String message = String("tm1=");
  message.concat(temp);  
  if(!mesh.write(message.c_str(), 0x01, sizeof(message))) {
    if(!mesh.checkConnection()) {
      Serial.print("renewing mesh address");
      mesh.renewAddress();
    } else {
      Serial.print("connection lost");
      error = true;
    }
  } else {
    Serial.print("Data sent: " + message);  
  }
  
  if (error) {
  } else {
    digitalWrite(ledPin, HIGH);
    delay(50);
  }
  digitalWrite(ledPin, LOW);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

float readTemp()
{
  return analogRead(sensorPin) * 0.48828125;
}

