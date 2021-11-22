#include <WiFi.h>
#include "time.h"

const char* ssid       = "vodafoneBA1157";
const char* password   = "SRULAGD6RHFQ4M5K";

const IPAddress serverIP(192, 168, 0, 13); // La dirección que desea visitar
uint16_t serverPort = 1080;         // Número de puerto del servidor

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

volatile int interruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}


int ValorEscritura = 0;

WiFiClient client;



void setup()
{
  Serial.begin(115200);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
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
  //printLocalTime();

}

void printLocalTime()
{
  if (interruptCounter > 0) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Hora no obtenida");
      return;
    }
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    Serial.println(&timeinfo, "%A, %d %B %Y %H:%M:%S");
    client.println(&timeinfo, "%A, %d %B %Y %H:%M:%S");
    //delay(1000);
  }
}

void accesoServidor()
{
  Serial.println("Intenta acceder al servidor");
  if (client.connect(serverIP, serverPort)) // Intenta acceder a la dirección de destino
  {
    Serial.println("Visita exitosa");
    client.println("Iniciado");
    ValorEscritura = 0;
    while (client.connected() || client.available()) // Si está conectado o se han recibido datos no leídos
    {
      if (client.available() > 0) // Si hay datos para leer
      {
        String escritura = client.readStringUntil('\n'); // Leer datos a nueva línea
        Serial.println(escritura);
        client.write(escritura.c_str()); // Devuelve los datos recibidos
        if (escritura.substring(0, 5) == "start") {
          ValorEscritura = 1;
          Serial.println(ValorEscritura);
        }
        while (ValorEscritura == 1) {
          printLocalTime();
          escritura = client.readStringUntil('\n');
          if (escritura.substring(0, 4) == "stop") {
            ValorEscritura = 0;
            Serial.println(ValorEscritura);
          }
        }
      }
    }
  }
  else
  {
    Serial.println("Acceso fallido");
    client.stop(); // Cerrar el cliente
  }
}

void loop()
{
  accesoServidor();

}
