/*
   SmartHome Node Unit implementation
*/
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh.h>
// #include <LowPower.h>

const int sensorPin = 0;
const int ledPin = 2;
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);
uint8_t nodeAddress = 4;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Node Unit...");
  pinMode(ledPin, OUTPUT);
  mesh.setNodeID(nodeAddress);
  Serial.print("Node Unit configured with ");
  Serial.print(mesh.getNodeID());
  Serial.println(" address.");
  mesh.begin();
  Serial.println("Node Unit initialized");
}

void loop()
{
  Serial.println("wakeup");
  mesh.update();
  float temp = readTemp();
  bool error = false;
  String message = String("tm0=");
  message.concat(temp);
  uint16_t address = mesh.renewAddress();
  Serial.print("renewing mesh address:");
  Serial.println(address);
  
  if (mesh.write(message.c_str(), 0x01, sizeof(message))) {
    Serial.println("Data sent: " + message);
  } else {
    Serial.println("Connection lost");
    error = true;
  }
  mesh.releaseAddress();

  if (!error) {
    digitalWrite(ledPin, HIGH);
    delay(100);
  }
  digitalWrite(ledPin, LOW);
  delay(4000);
  // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

float readTemp()
{
  return analogRead(sensorPin) * 0.48828125;
}

