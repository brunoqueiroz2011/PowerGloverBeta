#include <SoftwareSerial.h>

SoftwareSerial bluetooth(0,1); // RX, TX


void setup() {  
  delay(500);
  Serial.begin(9600);
  Serial.println("Conectando...");
  bluetooth.begin(9600);
}

void loop() {

  // Talvez quando não esteja usando os pinos RX e TX do arduino.    
  //bluetooth.write('A'); 

  Serial.print("Chegou...");
  Serial.println("");
  
  delay(500);
}
