#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "time.h"

const int ledPin = 12;

WiFiClient client;

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char* ssid       = "vodafoneBA1157";
const char* password   = "SRULAGD6RHFQ4M5K";

String printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Hora no obtenida");
  }
  String Hora = String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec);
  return (Hora);
}

String printResetTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Hora no obtenida");
  }
  timeinfo.tm_hour = 00;
  timeinfo.tm_min = 00;
  timeinfo.tm_sec = 00;
  String Hora = String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec);
  return (Hora);
}

String proceso(const String& var)
{
  Serial.println(var);
  if (var == "HORA")
  {
    return printLocalTime();
  }
  else if (var == "RESET")
  {
    return printResetTime();
  }
}


void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected");
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, proceso);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  // Route to set GPIO to HIGH
  server.on("/hora", HTTP_GET, [](AsyncWebServerRequest * request) {
    //digitalWrite(ledPin, HIGH);
    request->send(SPIFFS, "/index.html", String(), false, proceso);
  });

  // Route to set GPIO to LOW
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest * request) {
    //digitalWrite(ledPin, LOW);
    request->send(SPIFFS, "/index.html", String(), false, proceso);
  });
  // Start server
  server.begin();
}




void loop() {

}
