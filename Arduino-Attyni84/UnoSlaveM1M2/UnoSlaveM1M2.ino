/*
  Arduino uno Slave
  M1, M2
  ADD 0x0B
*/


#include <Wire.h>



int M1 = 9;
int M2 = 10;
int MotorEnable = 8;


int command ;
int answer;
int state = 0;
void receiveEvent(int howMany);
void requestEvent(int howMany);

void setup()
{
  Wire.begin(0x0B);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600); //
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(MotorEnable, OUTPUT);
  Serial.println("setup");
}


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
      digitalWrite(M1, HIGH);

    } else if (command == 0x12) {
      answer = 0x12;
      Serial.println("Recebeu comando 0x12");
      Serial.print("Liga Motor 2");
      Serial.println(answer);
      state = 1;
      digitalWrite(M2, HIGH);



    } else if (command == 0x31) {
      answer = 0x31;
      Serial.println("Recebeu comando 0x31");
      Serial.print("Para Motor 1");
      Serial.println(answer);
      state = 1;
      digitalWrite(M1, LOW);



    } else if (command == 0x32) {
      answer = 0x32;
      Serial.println("Recebeu comando 0x32");
      Serial.print("Para Motor 2");
      Serial.println(answer);
      state = 1;
      digitalWrite(M2, LOW);
    }

    else if (command != 0x00) {
      answer = 0x00;
      Serial.println("Comando desconhecido");
      Serial.println("A enviar codigo de erro 0x00");
      state = 1;
    }
   // }
  }

  void receiveEvent(int howMany) {
    while (Wire.available())
    {
      command = Wire.read();
    }
  }

  void requestEvent(int howMany)
  {
    Serial.println("A enviar resposta");
    Wire.write(answer);
    state = 0;
    command = 0;
    answer = 0;
  }
