#include <Wire.h>  
#include <PN532_I2C.h>  
#include <PN532.h>  

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("PN532 I2C 連線檢查");

  Wire.begin(21, 22);  

  Serial.println("初始化 PN532...");
  nfc.begin();  

  while (true) {
    uint32_t versiondata = nfc.getFirmwareVersion();  
    if (versiondata) {  
      Serial.println("找到 PN532！");  
      break;  
    }
    Serial.println("未找到 PN532，請檢查接線...");  
    delay(3000);  
  }

  nfc.SAMConfig();
  Serial.println("請刷卡片...");
}

void loop() {
  uint8_t uid[7] = {0};  // 儲存 UID 的陣列（最多 7 個字節）
  uint8_t uidLength;  // UID 長度

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.println("讀取到卡片！");

    Serial.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX);  // 以十六進位格式輸出 UID
      if (i < uidLength - 1) Serial.print(" ");  
    }
    Serial.println();

    bool accessGranted = false;  

    // 比對 UID
    if (uidLength == 4 && uid[0] == 0x99 && uid[1] == 0xF0 && uid[2] == 0x1D && uid[3] == 0x03) {
      Serial.println("這是 P Diddy");
      Serial.println("hell nah get out pls");
      accessGranted = false;
    } 
    else if (uidLength == 7 && uid[0] == 0x04 && uid[1] == 0x15 && uid[2] == 0x91 && uid[3] == 0xEA &&
                          uid[4] == 0x5C && uid[5] == 0x61 && uid[6] == 0x1C) {
      Serial.println("o斑馬");
      accessGranted = true;
    } 
    else {
      Serial.println("未知的卡片");
      accessGranted = false;
    }

    // 開門判斷
    if (accessGranted) {
      Serial.println("門已開啟，請進！");
      // 如果有門鎖繼電器，可以用以下程式控制
      // digitalWrite(RELAY_PIN, HIGH);  // 開門
      delay(5000); // 模擬門保持開啟 5 秒
      // digitalWrite(RELAY_PIN, LOW);   // 關門
    } else {
      Serial.println("拒絕進入！");
    }

    delay(1000);
  } else {
    Serial.println("未讀取到卡片");
  }
}
