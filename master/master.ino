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
  Serial.println("Starting Master Unit...");
  mesh.setNodeID(0);
    mesh.begin();
  Serial.println("Master Unit initialized");
}

void loop()
{
  mesh.update();
  mesh.DHCP();
  while(network.available()) {
    RF24NetworkHeader header;
    char message[100];
    uint16_t len = network.read(header, &message, sizeof(message));
    handleMessage(header, message, len);
  }
}

void handleMessage(RF24NetworkHeader header, char *message, uint16_t len) {
    Serial.print("from:");
    Serial.print(header.from_node);
    Serial.print(",id:");
    Serial.print(header.id);
    Serial.print(",type:");
    Serial.print(header.type);
    Serial.print(",message:{");
    Serial.print(String(message).substring(0, len));
    Serial.println("}");
}

