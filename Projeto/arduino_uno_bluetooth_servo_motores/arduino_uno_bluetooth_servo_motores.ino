// Autor: Agnes Lages
//Descrição: código para movimentação do braço robótico através do
//           comando enviados por Bluetooth.
#include <Servo.h>
#include <SoftwareSerial.h>

#define servo1 2 // Pino do 1o servo motor (s1)
#define servo2 3 // Pino do 2o servo motor (s2)
#define servo3 6 // Pino do 3o servo motor (s3)
#define servo4 8 // Pino do 4o servo motor (s4)

Servo s1;
Servo s2;
Servo s3;
Servo s4;

int bluetoothT1 = 13; //Pino para o TX do Bluetooth
int bluetoothR1 = 12; //Pino para o RX do Bluetooth
SoftwareSerial bluetooth(bluetoothT1, bluetoothR1); //Carregar os pinos do bluetooth 
int pos1, pos2, pos3, pos4;
char data;

void setup() {
  //Inicializar a serial em arduino
  Serial.begin(9600);
  bluetooth.begin(9600);
  
  //Inicializar os Servos Motores em arduino
  s1.attach(2);//Inicia o pino do 1o Servo Motor
  s2.attach(3);//Inicia o pino do 2o Servo Motor
  s3.attach(6);//Inicia o pino do 3o Servo Motor
  s4.attach(8);//Inicia o pino do 4o Servo Motor
    
  pos1 = 0;
  pos2 = 0;
  pos3 = 0;
  pos4 = 90;

  s4.write(90);
}

void loop() {
  if (bluetooth.available()) // Válida se o Bluetooth está ativo
  {      
    while(bluetooth.available()) //Válida se recebeu alguma informação
     {
       data = (char)bluetooth.read(); //Ler a informação e converte para tipo char
     }    
  }
  
 switch (data) { //Validar a informção recebida por bluetooth ao servo motor
    case 'W':      
        pos1+=2; //Incrementar valor ao 1o Servo Motor  
        s1.write(pos1); //Escreve no 1o Servo o valor informado
        Serial.println(pos1); 
      break;
    case 'U':  
        pos1-=2;//Decrementar valor ao 1o Servo Motor    
        s1.write(pos1); //Escreve no 1o Servo o valor informado
        Serial.println(pos1); 
      break; 
    case 'L': 
        pos2+=2; //Incrementar valor ao 2o Servo Motor 
        s2.write(pos2); //Escreve no 2o Servo o valor informado
        Serial.println(pos2);    
      break;
    case 'R':      
        pos2-=2; //Decrementar valor ao 2o Servo Motor    
        s2.write(pos2); //Escreve no 2o Servo o valor informado
        Serial.println(pos2);    
      break; 
    case 'F':
        pos3+=2; //Incrementar valor ao 3o Servo Motor
        s3.write(pos3); //Escreve no 3o Servo o valor informado
        Serial.println(pos3);
      break;
    case 'B': 
        pos3-=2; //Decrementar valor ao 3o Servo Motor    
        s3.write(pos3); //Escreve no 3o Servo o valor informado
        Serial.println(pos3);
      break;    
       case 'V':
        pos4+=2; //Incrementar valor ao 4o Servo Motor
        s4.write(pos4); //Escreve no 4o Servo o valor informado
       Serial.println(pos4);        
      break;
    case 'X':     
        pos4-=2; //Decrementar valor ao 4o Servo Motor
        s4.write(pos4); //Escreve no 4o Servo o valor informado
        Serial.println(pos4);    
      break;    
  }             
  delay(100);
}
