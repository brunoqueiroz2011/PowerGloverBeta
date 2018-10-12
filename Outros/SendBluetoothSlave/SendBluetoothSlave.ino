#include <SoftwareSerial.h>

SoftwareSerial bluetooth(0,1); // RX, TX
String command = "";

void setup() {
  delay(500);
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
   
  // Read user input if available.  
  //if (Serial.available())
  //{  
    //delay(100); // The DELAY!  
    //bluetooth.write(Serial.read());  
  //}  

  delay(500);
}
