// Dalam receiver dan transmitter, menggunakan 2 library yaitu SPI.h dan LoRa.h by Sandeep Mistry
#include <SPI.h>
#include <LoRa.h>

// Tentukan pin yang digunakan oleh modul receiver
#define LORA_AURORA_V2_NSS 15
#define LORA_AURORA_V2_RST 0
#define LORA_AURORA_V2_DIO0 27
#define LORA_AURORA_V2_EN 32
#define LORA_TX_POWER 20
#define LORA_SPREADING_FACTOR 12

int POT = 25;

void setup() {
  // Inisialisasi mode pin LoRa
  pinMode(LORA_AURORA_V2_EN, OUTPUT);
  pinMode(POT, INPUT);
  // LoRa chip dinyalakan
  digitalWrite(LORA_AURORA_V2_EN, HIGH);

  // Inisialisasi Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");
  // setup modul receiver LoRa
  LoRa.setPins(LORA_AURORA_V2_NSS, LORA_AURORA_V2_RST,
               LORA_AURORA_V2_DIO0);

  // Ganti LoRa.begin(---E-) argument dengan frekuensi yang cocok di tempat kamu
  // 433E6 for Asia
  // 866E6 for Europe
  // 915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setTxPower(LORA_TX_POWER, PA_OUTPUT_PA_BOOST_PIN);
  LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);

  // Ganti sync word (0xF3) menyesuaikan transceiver
  // sync word memastikan LoRa tidak mendapatkan data dari LoRa transceivers yang lain
  // memiliki range dari 0-0xFF
  LoRa.setSyncWord(0xA4);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // Input dari potensio dimasukkan ke dalam varibel analogInput
  // Mapping nilai analog dari 0-4095 menjadi 0-33
  // Dibagi 10 agar menyesuaikan nilai volt 0-3.3
  int analogInput  = analogRead(POT);
  int voltInt = map(analogInput, 0, 4095, 0, 33);
  float voltFloat = (float)voltInt / 10;
  Serial.print("Volatge: ");
  Serial.println(voltFloat);

  Serial.print("Sending packet: ");
  Serial.println(analogInput);
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(analogInput);
  LoRa.endPacket();
  delay(5000);
}