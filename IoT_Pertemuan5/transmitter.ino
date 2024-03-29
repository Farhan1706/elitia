// Kodingan transmitter lora
#include <SPI.h>
#include <LoRa.h>
// Tentukan pin yang digunakan oleh modul sender
#define LORA_AURORA_V2_NSS 15
#define LORA_AURORA_V2_RST 0
#define LORA_AURORA_V2_DIO0 27
#define LORA_AURORA_V2_EN 32
#define LORA_TX_POWER 20
#define LORA_SPREADING_FACTOR 12
// Variabel yang akan dimasukkan ke dalam paket data dan dikirim

float float_value = 0;
int int_value = 0;
const int id = 2;  // Nilai id disesuaikan dengan device_id

void setup() {
  // Inisialisasi mode pin LoRa
  pinMode(LORA_AURORA_V2_EN, OUTPUT);
  // LoRa chip dinyalakan
  digitalWrite(LORA_AURORA_V2_EN, HIGH);
  // Inisialisasi Serial Monitor
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("LoRa Sender");
  // setup modul receiver LoRa
  LoRa.setPins(LORA_AURORA_V2_NSS, LORA_AURORA_V2_RST, LORA_AURORA_V2_DIO0);
  // Ganti LoRa.begin(---E-) argument dengan frekuensi yang cocok di tempat kamu
  // 433E6 for Asia
  // 866E6 for Europe
  // 915E6 for North America
  while (!LoRa.begin(920E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setTxPower(LORA_TX_POWER, PA_OUTPUT_PA_BOOST_PIN);
  LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
  // Ganti sync word (0xF3) menyesuaikan transceiver
  // sync word memastikan LoRa tidak mendapatkan data dari LoRa transceivers yang lain
    // memiliki range dari 0-0xFF
    LoRa.setSyncWord(0xF2);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // Packet dibentuk dengan membuat variabel string
  String packet_to_send = String(String(id) + "#" + String(float_value) + "#" + String(int_value));
  Serial.print("Sending packet: ");
  Serial.println(packet_to_send);
  // Packet dikirim ke Gateway
  LoRa.beginPacket();
  LoRa.print(packet_to_send);
  LoRa.endPacket();
  // Increment
  float_value = float_value + 0.3;
  int_value++;
  if(int_value > 50){
    float_value = 0;
    int_value = 0;
  }
  // Set delay selama 1 detik (opsional)
  delay(1000);
}