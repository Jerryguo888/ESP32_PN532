#include <Wire.h>  // 引入 Wire 庫，用於 I2C 通訊
#include <PN532_I2C.h>  // 引入 PN532 I2C 介面函式庫
#include <PN532.h>  // 引入 PN532 主函式庫
#include <NfcAdapter.h>  // 引入 NFC 介面函式庫

// 建立 PN532 I2C 物件，並使用 Wire 進行通訊
PN532_I2C pn532i2c(Wire);
// 建立 NfcAdapter 物件，負責 NFC 讀寫操作
NfcAdapter nfc(pn532i2c);

void setup() {
  Serial.begin(115200);  // 設定序列埠通訊速率為 115200，方便除錯
  delay(1000);  // 延遲 1 秒，確保序列埠初始化完成
  Serial.println("PN532 I2C 連線檢查");

  Wire.begin(21, 22);  // 初始化 I2C，ESP32 預設 SDA (GPIO 21)、SCL (GPIO 22)

  Serial.println("初始化 PN532...");
  nfc.begin();  // 初始化 NFC 介面

  Serial.println("請刷 NFC 卡片...");
}

void loop() {
  Serial.println("請將 NFC 卡片靠近感應區...");

  if (nfc.tagPresent()) {  // 檢測是否有 NFC 卡片
    Serial.println("偵測到 NFC 卡片！");

    // 建立要寫入的 NDEF 訊息
    NdefMessage message = NdefMessage();
    message.addTextRecord("Hello NFC!"); // 添加一個文字記錄

    Serial.println("正在寫入 NFC 卡片...");
    bool success = nfc.write(message);  // 嘗試寫入 NFC 卡片

    if (success) {
      Serial.println("寫入成功！");
    } else {
      Serial.println("寫入失敗！");
    }

    delay(2000); // 延遲 2 秒，避免連續寫入同一張卡片
  }
}

