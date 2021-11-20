#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>
#include <ArduinoJson.h>



#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

int scanTime = 5; //In seconds
BLEScan *pBLEScan;


BLEBeacon oBeacon = BLEBeacon();


//JSON
const int capacity = JSON_ARRAY_SIZE(100) + 100 * JSON_OBJECT_SIZE(4);
StaticJsonDocument<capacity> doc;
char datosChar[capacity];

void almacenaDatos() {
  JsonObject obj1 = doc.createNestedObject();
  obj1["major"] = ENDIAN_CHANGE_U16(oBeacon.getMajor());
  obj1["minor"] = ENDIAN_CHANGE_U16(oBeacon.getMinor());
  obj1["UUID"] = oBeacon.getProximityUUID().toString().c_str();
  obj1["TxPower"] = oBeacon.getSignalPower();
}

void escribeFichero() {
  serializeJson(doc, datosChar);
  doc.clear();
}


class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      if (advertisedDevice.haveManufacturerData() == true)
      {
        std::string strManufacturerData = advertisedDevice.getManufacturerData();

        uint8_t cManufacturerData[100];
        strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

        if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
        {
          Serial.println("Beacon Encontrado");
          oBeacon.setData(strManufacturerData);
          //Serial.printf("Estructura Beacon\n");
          //Serial.printf("Major: %d Minor: %d UUID: %s Power: %d\n", ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
          almacenaDatos();
          Serial.print("Fichero JSON: ");
          escribeFichero();
          Serial.println(datosChar);
        }
      }
      return;
    }
};



void setup()
{
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("Alba");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value
}

void loop()
{
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
  delay(2000);
}
