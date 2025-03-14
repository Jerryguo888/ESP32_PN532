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
  
  nfc.SAMConfig();
  Serial.println("請刷 RFID 卡片...");
}

void loop() {
  Serial.println("請將 RFID 卡片靠近...");

  uint8_t uid[] = { 0, 0, 0, 0}; // UID buffer
  uint8_t uidLength;

if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.println("找到 RFID 卡片！");

    uint8_t blockNum = 4; // 讀取 Block 4（MIFARE Classic 的一般數據區塊）
    uint8_t keyA[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // 預設密鑰（MIFARE Classic 預設的 Key A）
    uint8_t dataBlock[16]; // 用來存放讀取到的 16 Byte 資料

    // 嘗試使用 Key A 驗證 Block 4
    if (nfc.mifareclassic_AuthenticateBlock(uid, uidLength, blockNum, 0, keyA)) {
        // 驗證成功，開始讀取 Block 4
        if (nfc.mifareclassic_ReadDataBlock(blockNum, dataBlock)) {
            Serial.print("讀取內容: ");

            bool hasContent = false; // 用來檢查是否有有效資料
            for (int i = 0; i < 16; i++) {
                if (dataBlock[i] != 0x00 && dataBlock[i] != 0xFF) { // 過濾掉全 0 或全 F 的空白區塊
                    Serial.print((char)dataBlock[i]); // 以字元方式輸出資料
                    hasContent = true; // 代表至少有一個有效字元
                }
            }

            if (!hasContent) {
                Serial.println("[無有效資料]"); // 如果都是空白內容，顯示「無有效資料」
            } else {
                Serial.println(); // 若有內容則換行
            }
        } else {
            Serial.println("讀取失敗！"); // 若讀取失敗，顯示錯誤訊息
        }
    } else {
        Serial.println("區塊驗證失敗！"); // 若驗證失敗，顯示錯誤訊息
    }

    delay(2000); // 避免連續讀取，讓使用者有時間看到輸出
  }

}
