#include "config.h"

int contador = 0;

#define IO_LOOP_DELAY 5000
unsigned long actualizacion = 0;

AdafruitIO_Feed *contador = io.feed("contador");

void setup() {
  Serial.begin(115200);
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  contador->onMessage(Mensaje);

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
  contador->get();
}

void loop() {
  io.run();

  if (millis() > (actualizacion + IO_LOOP_DELAY)) {
    Serial.print("Enviado -> ");
    Serial.println(contador);
    contador->save(contador);

    contador++;

    actualizacion = millis();
  }

}

void Mensaje(AdafruitIO_Data *data) {

  Serial.print("Recibido <- ");
  Serial.println(data->value());

}
