#include "UART.h"
#include "Arduino.h"

void UART::sendInt(int x){
  uint8_t lower = (x & 0xFF);
  uint8_t upper = (x >> 8) & 0xFF; 
  Serial3.write(upper);
  Serial3.write(lower);
}

void UART::sendFloat(float f){
  Serial3.write((uint8_t*)&f, sizeof(f));
}

