/*
   SmartHome Node Unit implementation
*/

#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh.h>
#include <LowPower.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include "Msg.h"

#define TMP_SENSOR_OFF 0
#define TMP_SENSOR_LM35 1
#define TMP_SENSOR_DHT11 2

#define LM35_PIN 0
#define DHT11_PIN 4

#define LED_PIN 2

const int tmpSensor = TMP_SENSOR_DHT11;

RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);
uint8_t nodeAddress = 6;

DHT_Unified dht(DHT11_PIN, DHT11);

sensors_event_t sensort;
sensors_event_t sensorh;
float readout;
Msg message(100);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Node Unit...");
  pinMode(LED_PIN, OUTPUT);
  mesh.setNodeID(nodeAddress);
  Serial.print("Node Unit configured with ");
  Serial.print(mesh.getNodeID());
  Serial.println(" address.");
  mesh.begin();
  Serial.println("Node Unit initialized");
}

void loop() {
  Serial.println("wakeup");
  mesh.update();
  bool error = false;

  if (tmpSensor == TMP_SENSOR_DHT11) {
    dht.begin();
  }

  readTemp();
  writeMessage();
  uint16_t address = mesh.renewAddress();
  Serial.print("renewing mesh address:");
  Serial.println(address);

  if (mesh.write(message.content(), 0x01, message.length())) {
    Serial.println("Data sent.");
  } else {
    Serial.println("Connection lost");
    error = true;
  }
  message.clear();
  mesh.releaseAddress();

  if (!error) {
    digitalWrite(LED_PIN, HIGH);
    delay(10);
  }
  digitalWrite(LED_PIN, LOW);
  for (int i = 0; i < 7; ++i) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}

void readTemp() {
  if (tmpSensor == TMP_SENSOR_OFF) {
    return;
  }
  if (tmpSensor == TMP_SENSOR_LM35) {
    readout = analogRead(LM35_PIN) * 0.48828125;
  } else if (tmpSensor == TMP_SENSOR_DHT11) {
    dht.temperature().getEvent(&sensort);
    dht.humidity().getEvent(&sensorh);
  }
}

void writeMessage() {
  message.append("nodeId=");
  message.append(nodeAddress);
  if (tmpSensor == TMP_SENSOR_DHT11) {
    message.append(",tmp=");
    message.append(sensort.temperature);
    sensort.temperature = 0;
    message.append(",hum=");
    message.append(sensorh.relative_humidity);
    sensorh.relative_humidity = 0;
  } else if (tmpSensor == TMP_SENSOR_LM35) {
    message.append(",tmp=");
    message.append(readout);
  }
}

