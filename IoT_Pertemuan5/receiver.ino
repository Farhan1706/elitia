#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>

/* LORA START */
// Tentukan pin yang digunakan oleh modul receiver
#define LORA_AURORA_V2_NSS 15
#define LORA_AURORA_V2_RST 0
#define LORA_AURORA_V2_DIO0 27
#define LORA_AURORA_V2_EN 32
/* LORA END */

/* FIREBASE START */
// FIREBASE HEADER
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"  // Provide the token generation process info.
#include "addons/RTDBHelper.h"   // Provide the RTDB payload printing info and other helper functions.

// FIREBASE SETUP
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Ini wifi"
#define WIFI_PASSWORD "12345678"
/* 2. Define the API Key */
#define API_KEY "AIzaSyC9py_1MVxcUT19FvWV-UETK-CjqjCpFvc"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://modul5-9bb30-default-rtdb.firebaseio.com/"

// Function List
void firebaseSetFloatAsync(String, float);
/* FIREBASE END */

int id;
float got_value;
float float_value;
int int_value;

void setup() {
  Serial.begin(115200);

  /* WIFI START */
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  /* WIFI END */

  /* FIREBASE START */
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;  // Assign RTDB API Key

  /*For anonymous account: Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase success");
    // digitalWrite(LED_BUILTIN, LOW);
    signupOK = true;
  } else {
    String firebaseErrorMessage = config.signer.signupError.message.c_str();
    Serial.printf("%s\n", firebaseErrorMessage);
  }

  config.database_url = DATABASE_URL;                  // Assign rtdb url
  config.token_status_callback = tokenStatusCallback;  // Set callback

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  /* FIREBASE END */

  /* LORA START */
  // Inisialisasi mode pin LoRa
  pinMode(LORA_AURORA_V2_EN, OUTPUT);
  // LoRa chip dinyalakan
  digitalWrite(LORA_AURORA_V2_EN, HIGH);

  // Setup modul receiver LoRa
  LoRa.setPins(LORA_AURORA_V2_NSS, LORA_AURORA_V2_RST,
               LORA_AURORA_V2_DIO0);

  // Menggunakan frekuensi 915E6
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }

  // Tentukan sync word, di sini kita menggunakan 0xF3

  Serial.println("LoRa Initializing OK!");
  /* LORA END */
}

void loop() {
  LoRa.setSyncWord(0xF3);
  receiveLoRa();

  LoRa.setSyncWord(0xF4);
  receiveLoRa();
}

void receiveLoRa() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaPacket = LoRa.readString();
      Serial.println(LoRaPacket);

      // Parse ID
      int hashIndex = LoRaPacket.indexOf('#');
      String idStr = LoRaPacket.substring(0, hashIndex);
      id = idStr.toInt();

      // Parse float_value
      int nextHashIndex = LoRaPacket.indexOf('#', hashIndex + 1);
      String float_valueTemp = LoRaPacket.substring(hashIndex + 1, nextHashIndex);
      float_value = float_valueTemp.toFloat();

      // Parse int_value
      String int_valueTemp = LoRaPacket.substring(nextHashIndex + 1);
      int_value = int_valueTemp.toInt();

      Serial.println(id);
      Serial.println(float_value);
      Serial.println(int_value);

      Serial.println("---------");
    }

    firebaseSetFloatAsync(String("/" + String(id) + "/float_value/"), float_value);
    firebaseSetIntAsync(String("/" + String(id) + "/int_value/"), int_value);

    Serial.println("=======================");
  }
}

void firebaseSetFloatAsync(String databaseDirectory, float value) {
  // Write an Int number on the database path test/int
  if (Firebase.RTDB.setFloatAsync(&fbdo, databaseDirectory, value)) {
    Serial.print("PASSED: ");
    Serial.println(value);
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

void firebaseSetIntAsync(String databaseDirectory, int value) {
  // Write an Int number on the database path test/int
  if (Firebase.RTDB.setIntAsync(&fbdo, databaseDirectory, value)) {
    Serial.print("PASSED: ");
    Serial.println(value);
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}