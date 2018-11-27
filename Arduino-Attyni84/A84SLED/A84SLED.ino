/*
  ATTiny84
  LED
  ADD 0x0E
*/

#include <TinyWireS.h>



// Define endereço do slave
const uint8_t SLAVE_ADDRESS = 0x0E;
volatile char comand;
volatile uint8_t c = 0;
int ledPinY = A0;//13
int ledPinG = A1;//12
int ledPinR = A2;//11

// Answer a read request from the master...
void requestEvent() {

  TinyWireS.send(c);
  c = 0;
}

// Answer a write request from the master...
void receiveEvent(uint8_t bytes) {

  comand = TinyWireS.receive();

  switch (comand) {
    case 'G':
      digitalWrite(ledPinG, HIGH);

      break;

    case 'Y':
      digitalWrite(ledPinY, HIGH);

    case 'R':
      digitalWrite(ledPinR, HIGH);

    case 'S':
      digitalWrite(ledPinG, LOW);
      digitalWrite(ledPinY, LOW);
      digitalWrite(ledPin, LOW);
      break;
    default:
      c = 0;
      //TinyWireS.send(c);
      break;
  }


}

void setup() {
  pinMode(ledPinY, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinR, OUTPUT);

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
