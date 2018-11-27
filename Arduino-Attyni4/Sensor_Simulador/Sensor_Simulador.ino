#include <Wire.h>

void setup()
{
  Wire.begin(0x0A);
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent);
  Serial.begin(9600); // 
  Serial.println("setup");
}

int command;
int answer;
int state = 0;

void loop()
{
       

  if (state == 0) {
    if (command == 0x33) {
      answer = random(1,2);
      state = 1;
      Serial.println("Recebeu comando 0x33");
      Serial.print("Valor aleatorio é ");
      Serial.println(answer);
    } else if (command == 0x66) {
      answer = random(3,4);
      Serial.print("Valor aleatorio é ");
      Serial.println(answer);
      state = 1;
      Serial.println("Recebeu comando 0x66");
    } else if (command != 0x00){
      answer = 0x13;
      Serial.println("Comando desconhecido");
      Serial.println("A enviar codigo de erro 0x13");
      state = 1;
    }
  }
}

void receiveEvent(int howMany)
{
  while(Wire.available()) 
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
}
