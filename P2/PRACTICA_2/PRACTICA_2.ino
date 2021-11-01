#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif


#include <MPU9250_asukiaaa.h>
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22

MPU9250_asukiaaa sensor;

void Muestreo( void *pvParameters );
void EnvioDatos( void *pvParameters );
void Led( void *pvParameters );

void setup() {

  xTaskCreatePinnedToCore(
    Muestreo
    ,  "Muestreo"
    ,  1024
    ,  NULL
    ,  3
    ,  NULL
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    EnvioDatos
    ,  "EnvioDatos"
    ,  1024
    ,  NULL
    ,  2
    ,  NULL
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    Led
    ,  "Led"
    ,  1024
    ,  NULL
    ,  1
    ,  NULL
    ,  ARDUINO_RUNNING_CORE);

}


void loop() {

}


void Led(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  /*
    Led
    Enciende un Led cada segundo, lo apaga tras 200 ms y se vuelve a encender cada segundo.
    El Led debe estar conectado al pin 12

  */

  pinMode(12, OUTPUT);

  for (;;)
  {
    vTaskDelay(1000 / portTICK_RATE_MS);
    digitalWrite(12, HIGH);
    vTaskDelay(200 / portTICK_RATE_MS);
    digitalWrite(12, LOW);
  }
}

void Muestreo(void *pvParameters)
{
  (void) pvParameters;

  /*
    Muestreo
    Lee la aceleración de un sensor MPU9250 cada 100 ms.

  */
  Wire.begin(SDA_PIN, SCL_PIN);

  sensor.setWire(&Wire);
  sensor.beginAccel();

  for (;;)
  {
    sensor.accelUpdate();
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

void EnvioDatos(void *pvParameters)
{
  (void) pvParameters;

  /*
    EnvioDatos
    Imprime en pantalla la aceleración de un sensor MPU9250 cada segundo

  */
  Serial.begin(115200);

  for (;;)
  {
    Serial.println("Valores de aceleración:");
    Serial.println("X: " + String(sensor.accelX()));
    Serial.println("Y: " + String(sensor.accelY()));
    Serial.println("Z: " + String(sensor.accelZ()));
    Serial.println("Aceleración: " + String(sensor.accelSqrt()));
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}
