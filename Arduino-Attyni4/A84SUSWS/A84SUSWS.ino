/*
  ATTiny84
  US
  WS
  ADD 0x0A
*/

#include <TinyWireS.h>
#include <HCSR04.h>


// Define endereço do slave
const uint8_t SLAVE_ADDRESS = 0x0A;
volatile uint8_t comand;
volatile uint8_t c = 0;
int buttonPin = 10;

// confirmar estes pinos
UltraSonicDistanceSensor distanceSensor(8, 9);

int range = 0;
int buttonState = 0;   

// Answer a read request from the master...
void requestEvent() {

  TinyWireS.send(c);
  c = 0;
}

// Answer a write request from the master...
void receiveEvent(uint8_t bytes) {

  range = distanceSensor.measureDistanceCm();
  
  comand = TinyWireS.receive();

  switch (comand) {
    case 0x33:
      if (range < 5) {
        c = 0X01; // got piece
      } else if (range > 10) {
        c = 0X01; // no piece
      }
      break;

    case 0x66:
     if (buttonState == HIGH) {     
    c = 0X03; // A piece
  } 
  else {
    c = 0X04;// B piece
  }   break;

    default: c = 0x00; break;
  }

  tws_delay(100);
}

void setup() {

  /*pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
  */
  pinMode(10, INPUT);
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
