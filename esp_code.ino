#include <WiFi.h>
#include <ArduinoOTA.h>
#include "DHT.h"

// Dane Wi-Fi
const char* ssid = "MagentaWLAN-UVKN";
const char* password = "26343427342537254858";

// Konfiguracja pinu i typu czujnika
#define DHTPIN 18      // GPIO18 (D18)
#define DHTTYPE DHT11  // Typ czujnika: DHT11 lub DHT22
#define SOIL_SENSOR_PIN 34  // GPIO34 (D34)
DHT dht(DHTPIN, DHTTYPE);

// Deklaracja pinu diody
const int ledPin = 2; // Pin GPIO 2 dla wbudowanej diody LED

// Zmienna czasu do odczytu
unsigned long previousMillis = 0;
const long interval = 3000; // Odczyt co 3 sekundy

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Inicjalizacja diody LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Wyłącz diodę LED

  // Łączenie z Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nPołączono z Wi-Fi!");
  Serial.print("Adres IP: ");
  Serial.println(WiFi.localIP());

  // Inicjalizacja OTA
  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "Flash" : "SPIFFS";
    Serial.println("Rozpoczęto aktualizację: " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nAktualizacja zakończona!");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Postęp: %u%%\r", (progress * 100) / total);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Błąd [%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Błąd autoryzacji");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Błąd rozpoczęcia");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Błąd połączenia");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Błąd odbioru");
    else if (error == OTA_END_ERROR) Serial.println("Błąd zakończenia");
  });

  ArduinoOTA.setPassword("pawelkoks");
  ArduinoOTA.begin();
  Serial.println("OTA gotowe!");
}

void loop() {
  ArduinoOTA.handle(); // Obsługa OTA

  unsigned long currentMillis = millis();

  // Odczyt danych co 3 sekundy
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Odczyt wilgotności i temperatury
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Błąd odczytu z czujnika!");
    } else {
      float tempHalf = round(temperature * 2) / 2.0; // Temperatury co 0.5 stopnia
      Serial.print("Wilgotność powietrza: ");
      Serial.print(humidity);
      Serial.println(" %");

      Serial.print("Temperatura: ");
      Serial.print(tempHalf);
      Serial.println(" *C");
    }

    // Odczyt z czujnika wilgotności gleby
    int soilValue = analogRead(SOIL_SENSOR_PIN);
    float soilPercentage = map(soilValue, 0, 4095, 100, 0);

    Serial.print("Wilgotność gleby: ");
    Serial.print(soilPercentage);
    Serial.println(" %");
  }
}
