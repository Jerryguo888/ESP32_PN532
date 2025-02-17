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
  Serial.println("請將 NFC 卡片靠近...");

  if (nfc.tagPresent()) {
    NfcTag tag = nfc.read();
    Serial.println("讀取到 NFC 卡片！");

    if (tag.hasNdefMessage()) {
      NdefMessage message = tag.getNdefMessage();
      for (int i = 0; i < message.getRecordCount(); i++) {
        NdefRecord record = message.getRecord(i);
        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);

        Serial.print("讀取到文字: ");
        for (int j = 3; j < payloadLength; j++) { // 跳過語言碼部分
          Serial.print((char)payload[j]);
        }
        Serial.println();
      }
    } else {
      Serial.println("此卡片無 NDEF 訊息");
    }
  }

  delay(2000); // 避免重複讀取
}
