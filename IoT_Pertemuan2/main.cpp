#include <Arduino.h>

#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
/* FIREBASE START */
// FIREBASE LIBRARY
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h" // Provide the token generation process info.
#include "addons/RTDBHelper.h" // Provide the RTDB payload printing info and other helper functions.
// FIREBASE SETUP
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;
/* 1. Define the WiFi credentials */
#define WIFI_SSID "RUMAH"
#define WIFI_PASSWORD "HomeC7A4"
/* 2. Define the API Key */
#define API_KEY "AIzaSyB8vZQNPFB3HUUy06Cm4HNI87SS-f6-riY"
/* 3. Define the RTDB URL */
#define DATABASE_URL "https://coba-c7eac-default-rtdb.firebaseio.com/"

void firebaseSetInt(String, int);
void firebaseSetFloat(String, float);
void firebaseSetString(String databaseDirectory, String value);
String firebaseGetString(String databaseDirectory);

// Root directory
String device_root = "/";
/* FIREBASE END */
int int_value = 0;
float float_value = 0;
char char_value = 'a';
int pinPB = 16;
String statusLED;
void setup() {
 Serial.begin(115200);
 pinMode(LED_BUILTIN, OUTPUT);
 pinMode(pinPB,INPUT);
 digitalWrite(LED_BUILTIN, HIGH);
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
 config.api_key = API_KEY; // Assign RTDB API Key
 /*For anonymous account: Sign up */
 if (Firebase.signUp(&config, &auth, "", "")) {
 Serial.println("Firebase success");
 digitalWrite(LED_BUILTIN, LOW);
 signupOK = true;
 } else {
 String firebaseErrorMessage = config.signer.signupError.message.c_str();
 Serial.printf("%s\n", firebaseErrorMessage);
 }
config.database_url = DATABASE_URL; // Assign rtdb url
 config.token_status_callback = tokenStatusCallback; // Set callback
 Firebase.begin(&config, &auth);
 Firebase.reconnectWiFi(true);
 /* FIREBASE END */
}

void loop() {
/* Write */
  int nilaiPB = analogRead(pinPB);
  if (nilaiPB == HIGH) {
  firebaseSetInt("led", 1);
 }else{
  firebaseSetInt("led", 0);
 }

//Read
 statusLED = firebaseGetString("led");
 if (statusLED == "1") {
 digitalWrite(LED_BUILTIN, HIGH);
 } else {
 digitalWrite(LED_BUILTIN, LOW);
 }
 delay(500);
}
String firebaseGetString(String databaseDirectory) {
 if (Firebase.RTDB.getString(&fbdo, databaseDirectory)) {
 if (fbdo.dataType() == "string") {
 String stringValue = fbdo.stringData();
 return stringValue;
 }
 } else {
 Serial.println(fbdo.errorReason());
 }
}
void firebaseSetInt(String databaseDirectory, int value) {
 // Write an Int number on the database path test/int
 if (Firebase.RTDB.setInt(&fbdo, databaseDirectory, value)) {
 Serial.print("PASSED: ");
 Serial.println(value);
 } else {
 Serial.println("FAILED");
 Serial.println("REASON: " + fbdo.errorReason());
 }
}
void firebaseSetString(String databaseDirectory, String value) {
 // Write a string on the database path
 if (Firebase.RTDB.setString(&fbdo, databaseDirectory, value)) {
 Serial.print("PASSED: ");
 Serial.println(value);
 } else {
 Serial.println("FAILED");
 Serial.println("REASON: " + fbdo.errorReason());
 }
}
