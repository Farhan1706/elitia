#include "BluetoothSerial.h"

BluetoothSerial SerialBT;  
String targetSlave = "ESP32-BT-Slave";  
int buttonPin = 16;        
int buttonState = 0;
int lastButtonState = 0;
bool connected = false;    

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);  
  Serial.println("Memulai master...");
  
  if (SerialBT.connect(targetSlave)) {  
    Serial.println("Terhubung ke perangkat slave: " + targetSlave);
    connected = true;
  } else {
    Serial.println("Gagal menghubungkan ke perangkat slave: " + targetSlave);
  }
}

void loop() {
  if (connected) {  
    buttonState = digitalRead(buttonPin);  
    
    if (buttonState == HIGH && lastButtonState == LOW) {
      SerialBT.println(1);  
      Serial.println("Tombol ditekan: 1 dikirim (LED NYALA)");
      delay(500);  
    } else if (buttonState == LOW && lastButtonState == HIGH) {
      SerialBT.println(0);  
      Serial.println("Tombol dilepas: 0 dikirim (LED MATI)");
      delay(500);  
    }
    
    lastButtonState = buttonState;  
  } else {
    Serial.println("Tidak terhubung ke perangkat slave.");
    delay(1000);
  }
}
