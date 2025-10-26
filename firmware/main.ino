#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 🔑 Config WiFi
const char* ssid = "NETFLIX_HD_SinLag";
const char* password = "D3v0ps#N3tw0rk2025";

// 🌦️ API OpenWeatherMap
const String city = "Bogota";
const String apiKey = "24e6bb7d702ebfa414bcbac7b6d3c241";
const String units = "metric";   // Celsius
const String lang = "es";        // Español

// ☁️ ThingSpeak
String serverUpdate = "http://api.thingspeak.com/update";
String apiKeyWrite = "OWXK8AF3NXEIAUIR";
String serverRead = "https://api.thingspeak.com/channels/3132847/fields/1/last.json?api_key=G974JJ8XW07YP5ZX";


unsigned long lastTime = 0;
unsigned long timerDelay = 60000; // cada 60 segundos
int modo = 1; // 1=clima, 2=sistema, 3=frecuencia rápida

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // Inicializar pantalla OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Conectando WiFi...");
  display.display();

  // Conectar WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("WiFi conectado!");
  display.display();
  delay(1000);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      // 🧠 1️⃣ Leer comando remoto de ThingSpeak
      HTTPClient http;
      http.begin(serverRead);
      int httpResponseCode = http.GET();
      if (httpResponseCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(512);
        deserializeJson(doc, payload);
        String comando = doc["field1"];
        Serial.println("Comando recibido: " + comando);

        if (comando == "1") modo = 1;              // Modo clima
        else if (comando == "2") modo = 2;         // Modo sistema
        else if (comando == "3") {                 // Cambiar frecuencia
          modo = 3;
          timerDelay = 10000;                      // cada 10s
        }
      }
      http.end();

      // 🌦️ 2️⃣ Obtener datos reales del clima
      HTTPClient weather;
      String weatherURL = "http://api.openweathermap.org/data/2.5/weather?q=" 
                          + city + "&appid=" + apiKey + "&units=" + units + "&lang=" + lang;
      weather.begin(weatherURL);
      int weatherCode = weather.GET();

      float temp = 0.0;
      int humidity = 0;
      String description = "sin datos";

      if (weatherCode == 200) {
        String wpayload = weather.getString();
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, wpayload);
        temp = doc["main"]["temp"];
        humidity = doc["main"]["humidity"];
        description = doc["weather"][0]["description"].as<String>();
        Serial.println("Clima obtenido correctamente");
      } else {
        Serial.println("Error al obtener datos del clima");
      }
      weather.end();

      // 📤 3️⃣ Enviar datos a ThingSpeak
      HTTPClient httpSend;
      String httpRequestData = "api_key=" + apiKeyWrite +
        "&field1=" + String(temp) +
        "&field2=" + String(humidity) +
        "&field3=" + String(WiFi.RSSI());
      httpSend.begin(serverUpdate);
      httpSend.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int sendCode = httpSend.POST(httpRequestData);
      Serial.println("Datos enviados -> HTTP " + String(sendCode));
      httpSend.end();

      // 🖥️ 4️⃣ Mostrar información en OLED
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.print("Modo: "); display.println(modo);

      if (modo == 1) { // Clima
        display.setCursor(0, 15);
        display.print("Temp: "); display.print(temp); display.println(" C");
        display.setCursor(0, 30);
        display.print("Hum: "); display.print(humidity); display.println(" %");
        display.setCursor(0, 45);
        display.println(description);
      }
      else if (modo == 2) { // Sistema
        display.setCursor(0, 15);
        display.print("WiFi: "); display.println(WiFi.SSID());
        display.setCursor(0, 30);
        display.print("RSSI: "); display.print(WiFi.RSSI()); display.println(" dBm");
        display.setCursor(0, 45);
        display.print("Uptime: "); display.println(millis() / 1000);
      }
      else if (modo == 3) {
        display.setCursor(0, 15);
        display.println("Modo rapido");
        display.setCursor(0, 30);
        display.println("Act cada 10s");
      }

      display.display();
    } 
    else {
      Serial.println("WiFi desconectado");
      WiFi.reconnect();
    }
    lastTime = millis();
  }
}
