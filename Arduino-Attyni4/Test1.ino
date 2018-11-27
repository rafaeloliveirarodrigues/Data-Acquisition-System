/*
  Autores:
  Carlos Garcia
  Diogo Candeias
  Rafael Rodrigues

  Title:
  SAD P1

*/
int LdrLeft = A3;
int LdrRight = A2;
int ledPinA = A0;
int ledPinV = A1;
int SensorValueLeft = 0;
int SensorValueRight = 0;
int MotorLeft = 9;
int MotorRight = 10;
int MotorEnable = 8;
char on ;

void setup() {
  //Serial.begin(9600);
  pinMode(ledPinA, OUTPUT);
  pinMode(ledPinV, OUTPUT);
  pinMode(MotorRight, OUTPUT);
  pinMode(MotorLeft, OUTPUT);
  pinMode(MotorEnable, OUTPUT);

}

void loop() {

 // on = Serial.read();
  digitalWrite(MotorEnable, HIGH);
  SensorValueLeft = analogRead(LdrLeft);
  SensorValueRight = analogRead(LdrRight);
 /* if (on == 'L') {

    Serial.println("Sensor ldr1:");
    Serial.println(sensorValue);
    delay(500);
    Serial.println("Sensor ldr2:");
    Serial.println(sensorValue2);
  }*/
  
  if (SensorValueLeft > SensorValueRight + 50) {
    digitalWrite(ledPinA, HIGH);
    digitalWrite(ledPinV, LOW);
    digitalWrite(MotorLeft, HIGH);
    digitalWrite(MotorRight, LOW);
    delay(100);
    //digitalWrite(MotorLeft, LOW);
  }

else  if (SensorValueRight > SensorValueLeft + 50) {
    digitalWrite(ledPinV, HIGH);
    digitalWrite(ledPinA, LOW);
    digitalWrite(MotorRight, HIGH);
    digitalWrite(MotorLeft, LOW);
    delay(100);
    //digitalWrite(MotorRight, LOW);
  }

else{
  digitalWrite(MotorLeft, LOW);
  digitalWrite(MotorRight, LOW);
  }

}
