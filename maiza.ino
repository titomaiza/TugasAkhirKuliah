#include <CTBot.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <FirebaseESP32.h>
#include <DHT.h>         // dht11 temperature and humidity sensor library
#define FIREBASE_HOST "https://takhir-f0aab-default-rtdb.asia-southeast1.firebasedatabase.app"  // the project name address from firebase id
#define FIREBASE_AUTH "RTkUcK4D5dc6JK6rTb3naQtry8BOpPJIo5haZmoS"  // the secret key generated from firebase
String token = "5433035702:AAE5xMBqITaPaFnFAEPnWcaB126uOICORSg";
const int id = 941386715;
#define WIFI_SSID "Nooderlicht bawah"
#define WIFI_PASSWORD "noorderlicht" 
//#define WIFI_SSID "H"
//#define WIFI_PASSWORD "Bandung39" 
#define Photoresistor 36
#define DHTPIN 5                // what digital pin we're connected to
#define DHTTYPE DHT21              // select dht type as DHT 11 or DHT22
#define RELAY_PIN_1 21             // lampu
#define RELAY_PIN_2 19             // kipas
#define RELAY_PIN_3 18             // humidifier
#define ANALOG_THRESHOLD  3000     // batas cahaya
#define KELEMBABAN_THRESHOLD 72  // batas pelembab
#define KELEMBABAN_THRESHOLD_TELE 72
#define SUHU_THRESHOLD 28          // batas suhu
#define SUHU_THRESHOLD_TELE 27

CTBot myBot;
DHT dht(DHTPIN, DHTTYPE);
FirebaseData fbdo;

void setup() {
  Serial.begin(9600);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);    
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                  //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);     // connect to firebase
  dht.begin();                            //Start reading dht sensor
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  
  //koneksi ke wifi
  myBot.wifiConnect(WIFI_SSID, WIFI_PASSWORD);
  //set token
  myBot.setTelegramToken(token);

  //cek koneksi
  if(myBot.testConnection())
  Serial.println("Connection successful");
  else
  Serial.println("Connection failed");
}
void loop() {
  //baca pesan masuk dari telegram
  TBMessage msg;
  if(myBot.getNewMessage(msg))
  {
    //tampilan di serial monitor
    Serial.println("Pesan Masuk : " + msg.text);
  }
float t = dht.readTemperature();
  if (t > SUHU_THRESHOLD)
    digitalWrite(RELAY_PIN_2, HIGH); // turn on Relay
  else
    digitalWrite(RELAY_PIN_2, LOW);
 
 if (t > 28.00) {
//    Serial.println("Suhu");
    String temp = "Air temperature above 28°C";
//    temp += int(SUHU_THRESHOLD_TELE);
//    temp += " °C\n";
    myBot.sendMessage(id, temp, "");
 }
 
  int analogValue = analogRead(Photoresistor);
  if (analogValue < ANALOG_THRESHOLD)
    digitalWrite(RELAY_PIN_1, HIGH); // turn on Relay
  else
    digitalWrite(RELAY_PIN_1, LOW);

  float h = dht.readHumidity();
  if (h > KELEMBABAN_THRESHOLD)
    digitalWrite(RELAY_PIN_3, LOW); // turn on Relay
  else
    digitalWrite(RELAY_PIN_3, HIGH);
    delay(3000);
    digitalWrite(RELAY_PIN_3, LOW);
    delay(500);
  
  if (h < 72.00) {
//    Serial.println("Kelembaban");
    String temp = "Air humidity below 72%";
//    temp += int(KELEMBABAN_THRESHOLD_TELE);
//    temp += " RH\n";
    myBot.sendMessage(id, temp, "");
 }

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("AnalogLDR: ");
  Serial.print(analogValue);
  Serial.print(" ");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("°C  ");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%  ");

  Firebase.setFloat( fbdo,"greenhouse/Temperature", t);

  Firebase.setFloat ( fbdo,"greenhouse/Humidity", h);

  Firebase.setInt ( fbdo,"greenhouse/AnalogLDR", analogValue);


//  delay(57500);
  delay(5000);  
}
