#include <SoftwareSerial.h>

SoftwareSerial bluetooth(0,1); // RX, TX
String command = "";

void setup() {  
  Serial.begin(9600);
  Serial.println("Aguardando...");  
  bluetooth.begin(9600);

}

void loop() {
  
  if (bluetooth.available()){
    while(bluetooth.available()){
      command += (char)bluetooth.read();    
    }    
    Serial.println(command);
    command = "";
  }    
   
  delay(150);
}
