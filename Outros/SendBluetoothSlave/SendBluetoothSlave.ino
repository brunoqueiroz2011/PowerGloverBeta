#include <SoftwareSerial.h>
#include <Servo.h>

Servo motor;
#define SERVO 6 //Pinos para o controle do Servo Motor

SoftwareSerial bluetooth(0,1); // RX, TX
//char command[20];
String command;
int indexStr;

String posX,posY,posZ;
int intposX,intposY,intposZ;

float gyroPos = 0;
int servoPos = 1500;

void setup() {  
  Serial.begin(9600);
  Serial.println("Aguardando...");  
  bluetooth.begin(9600);

  motor.attach(SERVO);//Inicia o pino do Servo Motor
  //motor.writeMicroseconds(servoPos); 

  indexStr = 0;
}

void loop() {

  
  if (bluetooth.available()){
    while(bluetooth.available()){
      //command[indexStr] += (char)bluetooth.read();    
      command += (char)bluetooth.read();    
      indexStr++;
    } 
    Serial.println(command);        
  //01 234 567 891 234 567
  //X: 000 |Y: 000 |Z: 000
    
    posX = command[2] +command[3] +command[4];    
    posY = command[8] +command[9] +command[10];
    posZ = command[15]+command[16]+command[17];

    intposX = posX.toInt();
    intposY = posY.toInt(); 
    intposZ = posZ.toInt();

    //Serial.print(intposX);
    //Serial.print("|");
    //Serial.print(intposY);
    //Serial.print("|");
    //Serial.println(intposZ);
    
    
    //motor.write(intposX);           
  }    
   indexStr = 0;
  delay(2000);
}
