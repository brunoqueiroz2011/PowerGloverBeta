//Autor: Bruno Queiroz
//Descrição: Programa que obtem os valores dos eixos do acelerometro e envia via Bluetooth.

#include <SoftwareSerial.h>
//Bibliotecas do Acelerometro
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" 
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;

#define OUTPUT_READABLE_YAWPITCHROLL

#define INTERRUPT_PIN 2

//Variaveis de Status do Controle do MPU
bool dmpReady = false;  // Set verdadeiro se o DMP foi bem sucedido
uint8_t mpuIntStatus;   // Mantém o status atual do byte de interrupção do MPU
uint8_t devStatus;      // Retorna status após cada operação do dispositivo (0 = sucesso,! 0 = erro)
uint16_t packetSize;    // Tamanho esperado do pacote DMP (o padrão é 42 bytes)
uint16_t fifoCount;     // Contagem de todos os bytes atualmente em FIFO
uint8_t fifoBuffer[64]; // Buffer de armazenamento FIFO

//Variaveis de orientação do movimento.
Quaternion q;           // [w, x, y, z]         Quaternion container
VectorInt16 aa;         // [x, y, z]            Medidas do sensor de aceleração
VectorInt16 aaReal;     // [x, y, z]            Medidas de sensor de aceleração sem gravidade
VectorInt16 aaWorld;    // [x, y, z]            Medições de sensor de aceleração de quadro mundial
VectorFloat gravity;    // [x, y, z]            Vetor de Gravidade
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container e vetor de gravidade

int8_t angleX;
int8_t angleY;
int8_t angleZ;

// ================================================================
// ===           ROTINA DE DETECÇÃO DE INTERRUPÇÕES             ===
// ================================================================
volatile bool mpuInterrupt = false;     // indica se o pino de interrupção MPU foi alto
void dmpDataReady() {
    mpuInterrupt = true;
}

//Bibliotecas do Bluetooth
//Variavel Bluetooth e os pinos utilizados pelo mesmo.
SoftwareSerial bluetooth(8,9); // RX, TX

void setup() {  
  Serial.begin(9600);               //Inicializando a Comunicação Serial
  Serial.println("Conectando...");
  // Configuração do Bluetooth    
  bluetooth.begin(9600);            //Configurando a velocidade da porta serial do Bluetooth

  // Configuração do Acelerometro
  // Junte-se ao barramento I2C (a biblioteca I2Cdev não faz isso automaticamente)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      Wire.setClock(400000); // Relógio de 400kHz I2C. Comente esta linha se tiver dificuldades de compilação
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif

  while (!Serial);

  // initialize device
  //Serial.println(F("Iniciazando o dispositivos I2C..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  // Verificando as Conexões
  //Serial.println(F("Testando a conexão dos dispositivos..."));
  //Serial.println(mpu.testConnection() ? F("MPU6050 conexão bem sucedida") : F("MPU6050 falha na conexão"));

  delay(1000);

  // Aguarda e configura o DMP
  //Serial.println(F("Iniciazando o DMP..."));
  devStatus = mpu.dmpInitialize();

  // Ajustando a sensibilidade do giroscópio.
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 padrão de fábrica para o meu chip de teste

  // verifique se funcionou (retorna 0 em caso afirmativo)
  if (devStatus == 0) {
      // ligue o DMP, agora que está pronto
      //Serial.println(F("Habilidando o DMP..."));
      mpu.setDMPEnabled(true);

      // Ativar a detecção de interrupção do Arduino
      //Serial.println(F("Ativando a detecção de interrupção (interrupção externa do Arduino 0)..."));
      attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus();

      // Setar nosso flag DMP Ready para que a função main loop () saiba que não há problemas em usá-lo
      //Serial.println(F("DMP pronto! Esperando pela primeira interrupção..."));
      dmpReady = true;

      // Obtenha o tamanho do pacote DMP esperado para comparação posterior
      packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
      // ERROR!
      // 1 = carga inicial da memória falhou
      // 2 = Falha na atualização da configuração do DMP
      // (se vai quebrar, geralmente o código será 1)
      //Serial.print(F("Inicialização do DMP falhou (código "));
      //Serial.print(devStatus);
      //Serial.println(F(")"));
  }
  
}

void loop() {
  //se a programação falhar, saia do método loop
  if (!dmpReady) return;

  // redefini o sinalizador de interrupção e obter byte INT_STATUS
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // obter contagem atual de FIFO
  fifoCount = mpu.getFIFOCount();

  // Verifique se há estouro
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      // redefinir para que possamos continuar
      mpu.resetFIFO();
      //Serial.println(F("FIFO overflow!"));

  // Caso contrário, verifique se há interrupção de dados pronta do DMP (isso deve acontecer com frequência)
  } else if (mpuIntStatus & 0x02) {
      // Aguarde o comprimento correto dos dados disponíveis, deve ser uma espera MUITO curta
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

      // leia um pacote da FIFO
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      
      //rastreia a contagem FIFO aqui, caso haja -> 1 pacote disponível (isso nos permite ler imediatamente mais sem esperar por uma interrupção)

      fifoCount -= packetSize;

      #ifdef OUTPUT_READABLE_YAWPITCHROLL
        // exibir ângulos de Euler em graus
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);        

        angleX = int(ypr[2]*180/M_PI)+90;
        
        angleY = int(ypr[1]*180/M_PI)+90;

        angleZ = int(ypr[0]*180/M_PI)+90;       
    
      #endif         
      
  }

  //Serial.print("X:");
  Serial.print(angleX);
  //Serial.print("|Y:");
  Serial.print(angleY);
  //Serial.print("|Z:");
  Serial.println(angleZ);       

  delay(150);
}
