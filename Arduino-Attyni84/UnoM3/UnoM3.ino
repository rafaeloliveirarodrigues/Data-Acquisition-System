/*
  Arduino uno Slave
  M3
  ADD 0X0C
*/

#include <Wire.h>



int M3 = 9;
//int M2 = 10;
int MotorEnable = 8;


void setup()
{
  Wire.begin(0x0C);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600); //
  pinMode(M3, OUTPUT);
  pinMode(MotorEnable, OUTPUT);
  Serial.println("setup");
}

int command ;
int answer;
int state = 0;

void loop()
{
  //if (state == 0) {
    digitalWrite(MotorEnable, HIGH);

    if (command == 0x11) {
      answer = 0x11;
      state = 1;
      Serial.println("Recebeu comando 0x11");
      Serial.print("Liga Motor 1 ");
      Serial.println(answer);
      digitalWrite(M3, HIGH);

    } else if (command == 0x31) {
      answer = 0x31;
      Serial.println("Recebeu comando 0x31");
      Serial.print("Para Motor 3");
      Serial.println(answer);
      state = 1;
      digitalWrite(M3, LOW);


    } else if (command != 0x00) {
      answer = 0x00;
      Serial.println("Comando desconhecido");
      Serial.println("A enviar codigo de erro 0x00");
      state = 1;
    }
  //}
}

void receiveEvent(int howMany)
{
  while (Wire.available())
  {
    command = Wire.read();
  }
}

void requestEvent(int howMany)
{
  state = 0;
  Serial.println("A enviar resposta");
  Wire.write(answer);
  command = 0;
  answer = 0;
}
