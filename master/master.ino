
/*
   SmartHome Master Unit implementation.
*/
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh.h>

#define MESH_DEBUG_SERIAL 1

RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);
uint32_t displayTimer = 0;
bool networkDebug = false;

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
  if(network.available()) {
    RF24NetworkHeader header;
    network.peek(header);
    char message[100];
    uint16_t len = network.read(header, &message, sizeof(message));
    Serial.print("Got network payload of size ");
    Serial.println(len);
    handleMessage(header, message, len);
  }

  
  if(networkDebug && millis() - displayTimer > 5000){
    displayTimer = millis();
    Serial.println(" ");
    Serial.println(F("********Assigned Addresses********"));
     for(int i=0; i<mesh.addrListTop; i++){
       Serial.print("NodeID: ");
       Serial.print(mesh.addrList[i].nodeID);
       Serial.print(" RF24Network Address: 0");
       Serial.println(mesh.addrList[i].address,OCT);
     }
    Serial.println(F("**********************************"));
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
