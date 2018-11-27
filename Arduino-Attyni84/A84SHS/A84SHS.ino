/*
  ATTiny84
  HS
  ADD 0x0D
*/

#include <TinyWireS.h>


// Define endereço do slave
const uint8_t SLAVE_ADDRESS = 0x0D;
volatile uint8_t comand;
volatile uint8_t c = 0;
volatile uint8_t contador = 0;
int state=0;
const int hallPin = 7;

// Answer a read request from the master...
void requestEvent() {

  TinyWireS.send(c);
  c = 0;
}

// Answer a write request from the master...
void receiveEvent(uint8_t bytes) {


  comand = TinyWireS.receive();
  if (comand == 0X33)
    c = digitalRead(hallPin);


}

void setup() {

  /*pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
  */
  pinMode(hallPin, INPUT);
 // attachInterrupt(0, pin_ISR, FALLING);
  TinyWireS.begin(SLAVE_ADDRESS);
  TinyWireS.onRequest(requestEvent);
  TinyWireS.onReceive(receiveEvent);
}

void loop() {

  // Without this, the "i2c_incoming()" handler is never called...
  TinyWireS_stop_check();

  // The "delay()" function cannot be used together with TinyWireS...
  tws_delay(10);
}
