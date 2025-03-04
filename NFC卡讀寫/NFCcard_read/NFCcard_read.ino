#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532i2c(Wire);
NfcAdapter nfc(pn532i2c);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("PN532 I2C NFC 讀取程式啟動");

  Wire.begin(21, 22); // 根據實際接線設置 SDA 和 SCL
  nfc.begin();
}

void loop() { 
  Serial.println("請將 NFC 卡片靠近..."); // 在序列埠監視器顯示提示訊息，要求使用者靠近 NFC 卡片

  if (nfc.tagPresent()) { // 檢查是否有 NFC 卡片靠近
    NfcTag tag = nfc.read(); // 讀取 NFC 卡片的資訊並儲存到 tag 物件中
    Serial.println("讀取到 NFC 卡片！"); // 顯示成功偵測到 NFC 卡片的訊息

    if (tag.hasNdefMessage()) { // 檢查卡片是否包含 NDEF 訊息
      NdefMessage message = tag.getNdefMessage(); // 取得卡片內的 NDEF 訊息

      for (int i = 0; i < message.getRecordCount(); i++) { // 迴圈讀取卡片內所有的 NDEF 訊息記錄
        NdefRecord record = message.getRecord(i); // 取得第 i 筆 NDEF 記錄
        int payloadLength = record.getPayloadLength(); // 取得此記錄的資料長度
        uint8_t payload[payloadLength]; // 建立一個 uint8_t 陣列來儲存 NDEF 訊息內容
        record.getPayload(payload); // 讀取 NDEF 訊息並存入 payload 陣列

        Serial.print("讀取到文字: "); // 顯示標題，準備輸出 NDEF 訊息的內容
        for (int j = 3; j < payloadLength; j++) { // 從索引 3 開始讀取，跳過前 3 個位元（通常是語言碼）
          Serial.print((char)payload[j]); // 轉換 byte 為 char 並輸出到序列埠監視器
        }
        Serial.println(); // 讀取結束後換行
      }
    } else {
      Serial.println("此卡片無 NDEF 訊息"); // 如果卡片沒有 NDEF 訊息，顯示提示訊息
    }
  }

  delay(2000); // 延遲 2 秒，避免過度重複讀取
}
