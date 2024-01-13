#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHTPIN D2    
#define DHTTYPE DHT11

#define GAS A0 

DHT dht(DHTPIN, DHTTYPE);

 char *ssid = "numair";
 char *password = "1234567879";

 char *YOUR_CHANNEL_ID = "your_channel_id_here";
 char *YOUR_THINGSPEAK_API_KEY = "your_api_key_here";

void setup() {
  Serial.begin(9600);

  connectToWiFi();

  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int gasValue = analogRead(GAS);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Gas Value: ");
  Serial.println(gasValue);
  updateThingSpeak(temperature, humidity, gasValue);

  delay(5000);
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

   
    Serial.println("\nFailed to connect to Wi-Fi. Please check your credentials or try again later.");
   
  
}

void updateThingSpeak(float temperature, float humidity, int gasValue) {
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, gasValue);

  unsigned long channelID = atol(YOUR_CHANNEL_ID);
  int hi = ThingSpeak.writeFields(channelID, YOUR_THINGSPEAK_API_KEY);

  if (hi == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Failed to send data to ThingSpeak. HTTP code: " + String(hi));
  }

  delay(2000);
}
