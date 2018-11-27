/*
  ATTiny84
  Buzz
  ADD 0x0F
*/

#include <TinyWireS.h>


// Define endereço do slave
const uint8_t SLAVE_ADDRESS = 0x0F;
volatile uint8_t comand;
volatile uint8_t c = 0;
const int Speaker = 7;

// Answer a read request from the master...
void requestEvent() {

  TinyWireS.send(c);
  c = 0;
}
void playBeep(void)
{
  for (int i = 0; i < 500; i++)
  {
    digitalWrite(Speaker, HIGH);
    delay(1);
    digitalWrite(Speaker, LOW);
    delay(1);
  }
}
// Answer a write request from the master...
void receiveEvent(uint8_t bytes) {


  comand = TinyWireS.receive();
  if (comand == 'O')
    playBeep();
  else if (comand == 'S')
    digitalWrite(Speaker, LOW);
    


}

void setup() {

  /*pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
  */
  pinMode(Speaker, OUTPUT);
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
