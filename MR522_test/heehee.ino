#include <Wire.h>
#include <Adafruit_PN532.h>

#define SDA_PIN 21  // ESP32 的 SDA 接 PN532 的 SDA
#define SCL_PIN 22  // ESP32 的 SCL 接 PN532 的 SCL

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  nfc.begin();  // 初始化 PN532
  
  // 持續尋找 PN532
  while (true) {
    Serial.println("Finding pn532...");
    uint32_t versiondata = nfc.getFirmwareVersion();
    
    if (versiondata) {
      Serial.println("find that shi");
      break;  // 找到就跳出迴圈
    }
    
    delay(1000);  // 每秒檢查一次
  }
}

void loop() {
  // 這裡可以加上讀取卡片的邏輯
}
