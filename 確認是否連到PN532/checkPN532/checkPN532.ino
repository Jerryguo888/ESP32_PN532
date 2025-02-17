#include <Wire.h>  // 引入 Wire 庫，用於 I2C 通訊
#include <PN532_I2C.h>  // 引入 PN532 I2C 介面函式庫
#include <PN532.h>  // 引入 PN532 主函式庫

// 建立 PN532 I2C 物件，並使用 Wire 進行通訊
PN532_I2C pn532i2c(Wire);
// 建立 PN532 物件，並將 I2C 介面物件傳入
PN532 nfc(pn532i2c);

void setup() {
  Serial.begin(115200);  // 設定序列埠通訊速率為 115200，方便除錯
  delay(1000);  // 延遲 1 秒，確保序列埠初始化完成
  Serial.println("PN532 I2C 連線檢查");  // 印出訊息，表示程式開始執行

  Wire.begin(21, 22);  // 初始化 I2C，ESP32 預設 SDA (GPIO 21)、SCL (GPIO 22)

  Serial.println("初始化 PN532...");  // 顯示 PN532 初始化訊息
  nfc.begin();  // 初始化 PN532

  // 進入無窮迴圈，直到成功找到 PN532 為止
  while (true) {
    uint32_t versiondata = nfc.getFirmwareVersion();  // 取得 PN532 韌體版本

    if (versiondata) {  // 若成功取得版本資訊
      Serial.println("找到 PN532！");  // 顯示成功找到 PN532 的訊息
      break;  // 跳出迴圈
    }

    Serial.println("未找到 PN532，請檢查接線...");  // 若未找到 PN532，提示檢查接線
    delay(3000);  // 延遲 3 秒後再重試
  }
}

void loop() {
  // 連線檢查已完成，無需執行任何操作
}
