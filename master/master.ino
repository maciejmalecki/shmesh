/*
   SmartHome Master Unit implementation.
*/
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh.h>

RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

void setup() {
  Serial.begin(9600);
  mesh.setNodeID(0);
  mesh.begin();
  Serial.print("Master Unit initialized");
}

void loop()
{
  mesh.update();
  mesh.DHCP();
  while(network.available()) {
    RF24NetworkHeader header;
    char message[100];
    network.read(header, &message, sizeof(message));
    handleMessage(header, message);
  }
}

void handleMessage(RF24NetworkHeader header, char *message) {
    Serial.print("from:");
    Serial.print(header.from_node);
    Serial.print(",id:");
    Serial.print(header.id);
    Serial.print(",type:");
    Serial.print(header.type);
    Serial.print(",message:{");
    Serial.print(message);
    Serial.print("}");
}

