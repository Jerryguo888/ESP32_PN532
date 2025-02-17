#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532i2c(Wire);
NfcAdapter nfc(pn532i2c);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("PN532 NFC URL 寫入系統啟動");

  Wire.begin(21, 22);
  nfc.begin();
}

void loop() {
  Serial.println("請將 NFC 卡片放置於讀取區...");
  if (nfc.tagPresent()) {
    NdefMessage message = NdefMessage();
    message.addUriRecord("https://www.instagram.com/");  // 修改這裡的網址

    Serial.println("發現 NFC 卡片，正在寫入...");
    bool success = nfc.write(message);
    
    if (success) {
      Serial.println("寫入成功！");
    } else {
      Serial.println("寫入失敗，請確認卡片是否可寫入。");
    }
    delay(2000);
  }
}
