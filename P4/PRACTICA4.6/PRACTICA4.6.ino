#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "time.h"

WiFiClient client;

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//
const char* ssid       = "vodafoneBA1157";
const char* password   = "SRULAGD6RHFQ4M5K";

//const char* ssid       = "MiA2";
//const char* password   = "25208230t";

struct tm timeinfo;
int h, m, s, h_reset = 0, m_reset = 0, s_reset = 0; //Hora, minuto y segundo
String hora_S;

bool flag;

String proceso(const String& var) {
  Serial.println(var);
  if (var == "STATE" || var == "RESET") {
    getLocalTime(&timeinfo);
    h = timeinfo.tm_hour;
    m = timeinfo.tm_min;
    s = timeinfo.tm_sec;

    if (flag) { //PULSAMOS RESET
      h_reset = h;
      m_reset = m;
      s_reset = s;
      flag = false;
    }
    //Hora mostrada
    h = h - h_reset;
    m = m - m_reset;
    s = s - s_reset;
    if (s < 0) {
      s = s + 60;
      m = m - 1;
    }

    if (s < 10) { //Para añadir 0 a la izquierda
      hora_S = String(":0" + String(s));
    }
    else {
      hora_S = String(":" + String(s));
    }
    if (m < 10) {
      hora_S = String(":0" + String(m) + hora_S);
    }
    else {
      hora_S = String(":" + String(m) + hora_S);
    }
    if (h < 10) {
      hora_S = String("0" + String(h) + hora_S);
    }
    else {
      hora_S = String(String(h) + hora_S);
    }
    return hora_S;
  }
  return String();
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
  // Route to set GPIO to LOW
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, proceso);
    flag = true;
  });
  // Start server
  server.begin();
}




void loop() {

}
