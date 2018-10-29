
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(0,1); // RX, TX

int cont;
void setup() {  
  cont = 0;
  Serial.begin(115200);
  Serial.println("Conectando...");
  bluetooth.begin(115200);
}

void loop() {
  cont += 1;
  Serial.print("Chegou...");
  Serial.println(cont);  

  delay(500);
}
