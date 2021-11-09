#include <WiFi.h>
#include "time.h"

const char* ssid       = "vodafoneBA1157";
const char* password   = "SRULAGD6RHFQ4M5K";

const IPAddress serverIP(192, 168, 0, 17); // La dirección que desea visitar
uint16_t serverPort = 1080;         // Número de puerto del servidor

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

WiFiClient client;

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Hora no obtenida");
    return;
  }
  Serial.println(&timeinfo, "%A, %d %B %Y %H:%M:%S");
  client.println(&timeinfo, "%A, %d %B %Y %H:%M:%S");
  delay(1000);
}

void accesoServidor()
{
  Serial.println("Intenta acceder al servidor");
  if (client.connect(serverIP, serverPort)) // Intenta acceder a la dirección de destino
  {
    Serial.println("Visita exitosa");
    client.println("Iniciado");                    // Enviar datos al servidor
    while (client.connected() || client.available()) // Si está conectado o se han recibido datos no leídos
    {
      printLocalTime();
      if (client.available()>0) // Si hay datos para leer
      {
        String escritura = client.readStringUntil('\n'); // Leer datos a nueva línea
        Serial.println(escritura);
        client.write(escritura.c_str()); // Devuelve los datos recibidos
      }
    }
  }
  else
  {
    Serial.println("Acceso fallido");
    client.stop(); // Cerrar el cliente
  }
}

void setup()
{
  Serial.begin(115200);

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
  printLocalTime();

}

void loop()
{
  accesoServidor();

}
