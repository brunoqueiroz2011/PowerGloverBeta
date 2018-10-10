//Carrega a biblioteca Wire
#include<Wire.h>
//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
//Define os pinos que serÃ£o utilizados para ligaÃ§Ã£o ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


//Variaveis Globais
//Endereco I2C do MPU6050
const int MPU=0x68;  
//Variaveis para armazenar valores dos sensores
int AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


void setup() {
  Serial.begin(9600);  
  //Define o nÃºmero de colunas e linhas do LCD
  lcd.begin(16, 2);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 

  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);

  delay(1000);
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor
  Wire.requestFrom(MPU,14,true);  
  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  //Tmp=Wire.read()<<8|Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)


  
  //Limpa a tela
  lcd.clear();
  //Valor de X na tela
  lcd.setCursor(0,0);
  lcd.print("X=");
  lcd.print(AcX);

  //Valor de Y na tela
  lcd.setCursor(0,1);
  lcd.print("Y=");
  lcd.print(AcY);

  //Valor de Z na tela
  lcd.setCursor(6,0);
  lcd.print("Z=");
  lcd.print(AcZ);

//Valor de X na tela
  lcd.setCursor(6,1);
  lcd.print("X=");
  lcd.print(GyX);

  delay(1000);
}

