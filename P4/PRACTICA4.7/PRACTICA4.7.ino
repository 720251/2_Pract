#include <ArduinoJson.h>
#include "time.h"
#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESP32_FTPClient.h>

//INTERRUPCION
volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

//WIFI
//const char* ssid       = "vodafoneBA1157";
//const char* password   = "SRULAGD6RHFQ4M5K";

const char* ssid       = "MiA2";
const char* password   = "25208230t";

//MARCA TEMPORAL
const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

struct tm timeinfo;
String hora;

//FTP
char ftp_server[] = "155.210.150.77";
char ftp_user[]   = "rsense";
char ftp_pass[]   = "rsense";
ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass, 5000, 2);

//JSON
const int capacity = JSON_ARRAY_SIZE(100) + 100 * JSON_OBJECT_SIZE(4);
StaticJsonDocument<capacity> doc;
char datosChar[capacity];

void almacenaDatos() {
  JsonObject obj1 = doc.createNestedObject();
  obj1["t"] = random(10, 30);
  obj1["u"] = "C";
  obj1["h"] = timeinfo.tm_hour;
  obj1["m"] = timeinfo.tm_min;
  obj1["s"] = timeinfo.tm_sec;
}

void escribeFichero() {
  serializeJson(doc, datosChar);
  doc.clear();
}

void mandaFichero() {
  ftp.OpenConnection();
  ftp.ChangeWorkDir("/rsense/720251");
  ftp.InitFile("Type A");
  ftp.NewFile("720251_211122.json");
  ftp.Write(datosChar);
  ftp.CloseFile();
  ftp.CloseConnection();
}

void setup() {
  Serial.begin(115200);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10000000, true);
  timerAlarmEnable(timer);

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false); // Desactiva la suspensión de wifi en modo STA para mejorar la velocidad de respuesta
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void printLocalTime() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Hora no obtenida");
    return;
  }
  //Serial.println(&timeinfo, "%A, %d %B %Y %H:%M:%S");
}


void loop() {
  printLocalTime();
  if (interruptCounter > 0) {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    almacenaDatos();
    Serial.print("Fichero JSON: ");
    escribeFichero();
    Serial.println(datosChar);
    mandaFichero();
    Serial.println();
  }


}
