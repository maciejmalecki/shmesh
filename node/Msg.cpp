#include "Msg.h"
#include <stdlib.h>
#include <string.h>
#include "Arduino.h"

Msg::Msg(int _maxLength) {
  maxLength = _maxLength;
  ptr = 0;
  buffer = malloc(maxLength);
}

Msg::~Msg() {
  free(buffer);
}

int Msg::length() {
  return ptr;
}

void Msg::append(char* data) {
  int sizeOfData = strlen(data);
  if (ptr + sizeOfData > maxLength) {
    return;
  }
  memcpy(buffer + ptr, data, sizeOfData);
  ptr += sizeOfData;
}

void Msg::append(int data) {
  int sizeOfData = sizeof data;
  if (ptr + sizeOfData > maxLength) {
    return;
  }
  ptr += i2str(data, buffer + ptr);
}


void Msg::append(float data, int width, int precision) {
  // TODO buffer overflow checking!!!
  char* converted = dtostrf(data, width, precision, buffer + ptr);
  ptr += strlen(converted);
}

void Msg::clear() {
  ptr = 0;
}

void* Msg::content() {
  return buffer;
}

byte Msg::i2str(int i, char *buf){
  byte l=0;
  if(i<0) buf[l++]='-';
  boolean leadingZ=true;
  for(int div=10000, mod=0; div>0; div/=10){
    mod=i%div;
    i/=div;
    if(!leadingZ || i!=0){
       leadingZ=false;
       buf[l++]=i+'0';
    }
    i=mod;
  }
  return l;
}

