#include "BluetoothSerial.h"

BluetoothSerial SerialBT;  
int ledPin = 2;            

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);  
  SerialBT.begin("ESP32-BT-Slave");  
  Serial.println("Perangkat dimulai, sekarang Anda dapat memasangkan dengan Bluetooth!");
}

void loop() {
  if (SerialBT.hasClient()) {
    Serial.println("Master terhubung!");
    SerialBT.println("Slave Terhubung");  
  }

  if(SerialBT.available()){  
    int receivedData = SerialBT.readStringUntil('\n').toInt();  

    if (receivedData == 1) {
      digitalWrite(ledPin, HIGH);  
      Serial.println("LED MENYALA");
    } 
    else if (receivedData == 0) {
      digitalWrite(ledPin, LOW);  
      Serial.println("LED MATI");
    }
  }
}
