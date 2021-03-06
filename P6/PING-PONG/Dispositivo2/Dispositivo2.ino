#include <SPI.h>
#include <LoRa.h>

#define SX1278_SCK  18    // GPIO18  -- SX1278's SCK
#define SX1278_MISO 19   // GPIO19 -- SX1278's MISO//SPIQ//SDO
#define SX1278_MOSI 23   // GPIO23 -- SX1278's MOSI//SPID//SDI
#define SX1278_CS   5   // GPIO5 -- SX1278's CS
#define SX1278_RST  14   // GPIO14 -- SX1278's RESET
#define SX1278_DI0  26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

#define LORA_BAND   433E6 // Banda de radiofrecuencia LoRa para el módulo comprado

//////////////////////CONFIG 1///////////////////////////
byte localAddress = 8;     // Dirección dispositivo
byte destination = 18;     // Dirección de envío
int interval = 3000;       // Intervalo entre envios
String message = "¡HOLA!";    // Mensaje
///////////////////////////////////////////////////////



String outgoing;              // Mensaje saliente
byte msgCount = 0;            // Contador de mensajes salientes
long lastSendTime = 0;        // contador millis para tiempos

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // Inicio
  LoRa.write(destination);              // Escribir destino
  LoRa.write(localAddress);             // Escribir dirección dispositivo
  LoRa.write(msgCount);                 // Escribir contador mensaje
  LoRa.write(outgoing.length());        // Longitud del mensaje saliente
  LoRa.print(outgoing);                 // Escribir mensaje saliente
  LoRa.endPacket();                     // Finalizar paquete y enviar

  Serial.println("Sending message " + String(msgCount) + " to address: " + String(destination));
  Serial.println("Message: " + message);
  Serial.println();
  delay(1000);
  msgCount++;                          
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // Si no hay paquete, se realiza return

  // Lectura de bytes del paquete
  int recipient = LoRa.read();          // Dirección del recipiente
  byte sender = LoRa.read();            // Dirección del que envía
  byte incomingMsgId = LoRa.read();     
  byte incomingLength = LoRa.read();   

  String incoming = "";                

  while (LoRa.available()) {           
    incoming += (char)LoRa.read();     
  }

  if (incomingLength != incoming.length()) {   //Se realiza un check para comprobar si el mensaje cumple con la longitud del paquete
    Serial.println("error: message length does not match length");
    incoming = "message length error";
    return;                             
  }

  // Se realiza un check para comprobar si el destinatario no es este dispoisitivo
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    incoming = "message is not for me";
    return;                             // skip rest of function
  }
  // Si el mensaje es para este dispositivo, se escribe por pantalla los parámetros que queremos saber
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
  delay(1000);
}

void setup() {
  Serial.begin(115200);                   // initialize serial
  while (!Serial);
  SPI.begin(SX1278_SCK, SX1278_MISO, SX1278_MOSI, SX1278_CS);
  LoRa.setPins(SX1278_CS, SX1278_RST, SX1278_DI0);// set CS, reset, IRQ pin

  if (!LoRa.begin(LORA_BAND))
  {    Serial.println("LoRa init failed. Check your connections.");
    while (true);                 
  }
  LoRa.receive();
  Serial.println("LoRa init succeeded.");
  delay(1500);
}

void loop() {
  if (millis() - lastSendTime > interval) {
    sendMessage(message);
    lastSendTime = millis();            // timestamp the message
    interval = random(2000) + 1000;     // 2-3 seconds
    LoRa.receive();                     // go back into receive mode
  }
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    onReceive(packetSize);
  }
}
