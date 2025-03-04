#include <Wire.h>              // 引入 Wire 函式庫，提供 I2C 通訊功能
#include <PN532_I2C.h>         // 引入 PN532_I2C 函式庫，讓 PN532 能夠透過 I2C 進行通訊
#include <PN532.h>             // 引入 PN532 函式庫，提供更底層的 PN532 讀寫功能
#include <NfcAdapter.h>        // 引入 NfcAdapter 函式庫，簡化 NFC 讀寫操作

// **建立 PN532 I2C 物件**，指定使用 Wire (ESP32 I2C)
PN532_I2C pn532i2c(Wire);

// **建立 NFC 物件**，讓程式可以使用更高階的 NFC 操作
NfcAdapter nfc(pn532i2c);

void setup() {
  Serial.begin(115200);  // 設定序列埠 (Serial) 的波特率為 115200，方便用戶透過串列監視器查看輸出
  delay(1000);           // 延遲 1 秒，確保 ESP32 完全初始化
  Serial.println("PN532 NFC URL 寫入系統啟動");  // 顯示啟動訊息

  // **初始化 I2C，指定 SDA 和 SCL 腳位**
  Wire.begin(21, 22);  // 在 ESP32 上，SDA 預設為 GPIO 21，SCL 預設為 GPIO 22

  // **初始化 NFC 模組**
  nfc.begin();  // 這行會設定 PN532 模組並執行初始化
}

void loop() {
  Serial.println("請將 NFC 卡片放置於讀取區...");  // 提示用戶將 NFC 卡片放置於讀取範圍內

  // **檢查是否有 NFC 卡片存在**
  if (nfc.tagPresent()) {  // 如果偵測到 NFC 卡片，執行以下操作
    NdefMessage message = NdefMessage();  // 建立 NDEF 訊息物件
    message.addUriRecord("https://www.instagram.com/guo_jerry0122/");  // 新增 URI 記錄 (可修改為其他網址)

    Serial.println("發現 NFC 卡片，正在寫入...");  // 顯示寫入訊息
    bool success = nfc.write(message);  // 嘗試將 NDEF 訊息寫入 NFC 卡片

    // **確認寫入結果**
    if (success) {
      Serial.println("寫入成功！");  // 顯示成功訊息
    } else {
      Serial.println("寫入失敗，請確認卡片是否可寫入。");  // 顯示失敗訊息（可能是卡片為唯讀模式）
    }
    delay(2000);  // 延遲 2 秒，避免連續觸發
  }
}
