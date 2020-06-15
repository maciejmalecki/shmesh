
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
#define SLEEP_TIME 0

const int tmpSensor = TMP_SENSOR_DHT11;

RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);
uint8_t nodeAddress = 2;

DHT_Unified dht(DHT11_PIN, DHT11);

sensors_event_t sensort;
sensors_event_t sensorh;
float readout;
Msg message(100);
boolean addressReleased = true;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  mesh.setNodeID(nodeAddress);
  mesh.begin();
  radio.printDetails();

  blink(nodeAddress, 15);
}

void loop() {
  Serial.println("wakeup");
  mesh.update();

//  if (addressReleased) {

    if (tmpSensor == TMP_SENSOR_DHT11) {
      dht.begin();
    }

    readTemp();
    writeMessage();

    if (mesh.write(message.content(), 0x01, message.length())) {
      blink(1, 10);
    } else {
      if (!mesh.checkConnection()) {
        uint16_t address = mesh.renewAddress();
        Serial.print("renewing mesh address:");
        Serial.println(address);
      }
      blink(3, 10);
    }
    message.clear();
//  }

/*
  for (int i = 0; i < 5; ++i) {
    addressReleased = mesh.releaseAddress();
    if (addressReleased) {
      break;
    }
    delay(100);
  }

  if (!addressReleased) {
    blink(5, 20);
  }
  */


  for (int i = 0; i < SLEEP_TIME; ++i) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
  delay(10000);
}

void blink(int times, int delayTime) {
  for (int i = 0; i < times; ++i) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayTime);
    digitalWrite(LED_PIN, LOW);
    delay(250);
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
    Serial.print("tmp=");
    Serial.println(sensort.temperature);
    sensort.temperature = 0;
    message.append(",hum=");
    message.append(sensorh.relative_humidity);
    Serial.print("hum=");
    Serial.println(sensorh.relative_humidity);
    sensorh.relative_humidity = 0;
  } else if (tmpSensor == TMP_SENSOR_LM35) {
    message.append(",tmp=");
    message.append(readout);
  }
}
