// Include Libraries and Setup UUIDs

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "00000002-0000-0000-FDFD-FDFDFDFDFDFD"
#define CHARACTERISTIC_UUID_TEMP "00002A1C-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID_HUM  "00002A6F-0000-1000-8000-00805F9B34FB"

float readTemperature() {
  // Simulate reading temperature
  return 25.0 + (rand() % 10);  // Random value between 25 and 35
}

float readHumidity() {
  // Simulate reading humidity
  return 50.0 + (rand() % 10);  // Random value between 50 and 60
}

// Setup BLE Server
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristicTemp = NULL;
BLECharacteristic* pCharacteristicHum = NULL;
bool deviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32_BLE");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristicTemp = pService->createCharacteristic(
                       CHARACTERISTIC_UUID_TEMP,
                       BLECharacteristic::PROPERTY_READ |
                       BLECharacteristic::PROPERTY_NOTIFY
                     );
  pCharacteristicHum = pService->createCharacteristic(
                       CHARACTERISTIC_UUID_HUM,
                       BLECharacteristic::PROPERTY_READ |
                       BLECharacteristic::PROPERTY_NOTIFY
                     );
  pCharacteristicTemp->addDescriptor(new BLE2902());
  pCharacteristicHum->addDescriptor(new BLE2902());
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify...");

}


// Update Characteristics with Sensor Data

cpp
Copy code
void loop() {
  if (deviceConnected) {
    float temperature = readTemperature();
    float humidity = readHumidity();
    pCharacteristicTemp->setValue(temperature);
    pCharacteristicHum->setValue(humidity);
    pCharacteristicTemp->notify();
    pCharacteristicHum->notify();
    delay(1000);
  }
}


// Extra Credits: Transfer WiFi Credentials Over Bluetooth

cpp
Copy code
#define WIFI_SERVICE_UUID        "00000001-0000-0000-FDFD-FDFDFDFDFDFD"
#define CHARACTERISTIC_UUID_SSID "00002A0F-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID_PASS "00002A19-0000-1000-8000-00805F9B34FB"

BLECharacteristic* pCharacteristicSSID = NULL;
BLECharacteristic* pCharacteristicPASS = NULL;

void setupWiFiService(BLEServer* pServer) {
  BLEService *pService = pServer->createService(WIFI_SERVICE_UUID);
  pCharacteristicSSID = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_SSID,
                        BLECharacteristic::PROPERTY_WRITE
                      );
  pCharacteristicPASS = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_PASS,
                        BLECharacteristic::PROPERTY_WRITE
                      );
  pService->start();
}

void connectToWiFi() {
  std::string ssid = pCharacteristicSSID->getValue();
  std::string pass = pCharacteristicPASS->getValue();
  if (ssid.length() > 0 && pass.length() > 0) {
    Serial.printf("Connecting to WiFi SSID: %s, Password: %s\n", ssid.c_str(), pass.c_str());
    // Connect to WiFi
  }
}

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32_BLE");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  setupWiFiService(pServer);
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristicTemp = pService->createCharacteristic(
                       CHARACTERISTIC_UUID_TEMP,
                       BLECharacteristic::PROPERTY_READ |
                       BLECharacteristic::PROPERTY_NOTIFY
                     );
  pCharacteristicHum = pService->createCharacteristic(
                       CHARACTERISTIC_UUID_HUM,
                       BLECharacteristic::PROPERTY_READ |
                       BLECharacteristic::PROPERTY_NOTIFY
                     );
  pCharacteristicTemp->addDescriptor(new BLE2902());
  pCharacteristicHum->addDescriptor(new BLE2902());
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify...");
}

void loop() {
  if (deviceConnected) {
    float temperature = readTemperature();
    float humidity = readHumidity();
    pCharacteristicTemp->setValue(temperature);
    pCharacteristicHum->setValue(humidity);
    pCharacteristicTemp->notify();
    pCharacteristicHum->notify();
    delay(1000);
  }
  connectToWiFi();
}
