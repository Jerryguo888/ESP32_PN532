#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

void setup() {
  Serial.begin(115200);
  Serial.println("PN532 NFC/RFID 判別系統啟動");

  Wire.begin(21, 22);
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("未找到 PN532，請檢查接線！");
    while (1);
  }

  Serial.println("找到 PN532！");
  nfc.SAMConfig();
  Serial.println("請刷 NFC/RFID 卡片...");
}

void loop() {
  uint8_t uid[7] = {0};  // 儲存 UID
  uint8_t uidLength;     // UID 長度

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.print("讀取到卡片，UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX);
      if (i < uidLength - 1) Serial.print(" ");  // 用空格分隔 UID
    }
    Serial.println();

    // 判斷卡片類型
    if (uidLength == 4) {
      Serial.println("判斷結果: 這是 MIFARE Classic (RFID) 卡");
    } else if (uidLength == 7) {
      Serial.println("判斷結果: 這是 MIFARE Ultralight / NFC 卡");
    } else {
      Serial.println("未知類型的卡片");
    }

    Serial.println("----------------------------");
    delay(1000);  // 等待 1 秒後繼續讀取
  }
}