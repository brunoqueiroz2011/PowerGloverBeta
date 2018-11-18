#include <stdlib.h>

#include <SoftwareSerial.h>
#include <Servo.h>

Servo motorBase, motorGarra, motorAsteDois, motorAsteTres;
#define SERVOBASE 7 //Pinos para o controle do Servo Motor Base
#define SERVOGARRA 6 //Pinos para o controle do Servo Motor Garra
#define SERVOASTEDOIS 5 //Pinos para o controle do Servo Motor Aste2
#define SERVOASTETRES 4 //Pinos para o controle do Servo Motor Aste3

#define rx 13
#define tx 12

SoftwareSerial bluetooth(rx,tx); // RX, TX

String posX,posY,posZ, isFechaGarra;
int intX,intY,intZ;
int oldX,oldY,oldZ;
int contPos = 0;

void setup() {  
  Serial.begin(9600);
  Serial.println("Aguardando...");  
  bluetooth.begin(9600);


  motorBase.attach(SERVOBASE);//Inicia o pino do Servo Motor
  motorGarra.attach(SERVOGARRA);//Inicia o pino do Servo Motor
  motorAsteDois.attach(SERVOASTEDOIS);//Inicia o pino do Servo Motor
  motorAsteTres.attach(SERVOASTETRES);//Inicia o pino do Servo Motor
  
  motorBase.write(90);//Inicia o pino do Servo Motor
 // motorGarra.write(0);//Inicia o pino do Servo Motor
 // motorAsteDois.write(0);//Inicia o pino do Servo Motor
 // motorAsteTres.write(0);//Inicia o pino do Servo Motor
}

void loop() {  
  if(bluetooth.available()){
    while(bluetooth.available()){
      char c = (char) bluetooth.read();    
      if(c == ','){  //Valida o Caractere recebido
        contPos++;  //caso seja verdadeiro significa que pode passar para o proximo eixo 
      }
      //O switch abaixo verifica a variavel contPos que significa qual eixo esta sendo recebido
      switch(contPos){
          case 0:                   
            posX += c;             
            break;
          case 1: 
            posY += c;
            break;
          case 2: 
            posZ += c;            
            break;
          case 3:
            isFechaGarra += c;
        }

        //Valida se já obteve todos os valores dos eixos
        if(contPos == 4){
          //Caso seja verdadeiro converte as variaveis String que estão com os valores em Int, 
          //zero o contPos, remove alguma inpureza no valor do eixo Y e limpa as variaveis String
          contPos = 0;
          intX = posX.toInt();
          //posY.remove(0,1);
          posY.replace(',','\n');
          intY = posY.toInt();
          posZ.remove(0,1);
          intZ = posZ.toInt();  
          cleanPos();
        }
    }   


  //Servo Motor Base
    if(oldX != intX){
      motorAsteDois.attach(pinoOrig);
      oldX = intX;
      motorAsteDois.write(intX);
    }else{
      motorAsteDois.attach(9);
    }

    //Servo Motor Base
    if(oldY != intY){
      motormotorAsteTresBase.attach(pinoOrig);
      oldY = intY;
      motorAsteTres.write(intY);
    }else{
      motorAsteTres.attach(10);
    }

    //Servo Motor Base
    if(oldZ != intZ){
      motorBase.attach(pinoOrig);
      oldZ = intZ;
      motorBase.write(intZ);
    }else{
      motorBase.attach(11);
    }

  int valor = 0;  
  if(valor >= 180){
    valor--;
  }else if(valor <= 0){
    valor++;
  }
  motorGarra.write(valor);

    
  }

//Essa funcção é chamada para atuar na Garra
AbrirFecharGarra(motorGarra);

oldX = inputValorMotor(motorAsteTres,oldX,intX,SERVOASTETRES,9);
oldY = inputValorMotor(motorAsteDois,oldY,intY,SERVOASTEDOIS,10);
oldZ = inputValorMotor(motorBase,oldZ,intZ,SERVOBASE,11);

}

//Limpa as Variaveis String que recebem a posição
void cleanPos(){
  posX = ""; 
  posY = "";
  posZ = "";
}

//Altera o valor do Servo da Garra para abrir ou fechar
void AbrirFecharGarra(Servo motor){
  int valor = 0;  
  if(valor >= 180){
    valor--;
  }else if(valor <= 0){
    valor++;
  }
  motor.write(valor);
}

//Generaliza o valor dos motores.
int inputValorMotor(Servo motor, int oldValor, int newValor, int pinoOrig, int pinoDest){
  //Servo Motor Base
  if(oldValor != newValor){
    motor.attach(pinoOrig);
    oldValor = newValor;
    motor.write(newValor);
  }else{
    motor.attach(pinoDest);
  }

  return oldValor;
}

