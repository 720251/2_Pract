#include <MPU9250_asukiaaa.h>
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22

MPU9250_asukiaaa sensor;

volatile int interruptCounter;
int totalInterruptCounter;
int contadorSerial;
int contadorLed;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {

  Wire.begin(SDA_PIN, SCL_PIN);

  sensor.setWire(&Wire);
  sensor.beginAccel();

  Serial.begin(115200);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);

  pinMode(12, OUTPUT);

}

void leeSensor() {
  if (interruptCounter > 0) {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    totalInterruptCounter++;
    contadorSerial++;
    contadorLed++;
    if (totalInterruptCounter == 100) {
      sensor.accelUpdate();
      totalInterruptCounter = 0;
    }
    if (contadorSerial == 1000) {
      Serial.println("Valores de aceleración:");
      Serial.println("X: " + String(sensor.accelX()));
      Serial.println("Y: " + String(sensor.accelY()));
      Serial.println("Z: " + String(sensor.accelZ()));
      Serial.println("Aceleración: " + String(sensor.accelSqrt()));
      if (digitalRead(12) == LOW) {
        digitalWrite(12, HIGH);
      }
      contadorSerial = 0;
    }
    if (digitalRead(12) == HIGH && contadorLed == 1200) {
      digitalWrite(12, LOW);
      contadorLed = 0;
    }

  }
}


void loop() {

  leeSensor();

}
