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
  Serial.println("請將 RFID 卡片靠近..."); // 提示用戶將 RFID 卡片靠近感應區

  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // 建立一個陣列來儲存 UID（卡片唯一識別碼）
  uint8_t uidLength; // 變數 uidLength 用來存放 UID 長度（幾個位元組）

  // 嘗試讀取靠近的 RFID 卡片
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.println("找到 RFID 卡片！"); // 成功偵測到卡片

    uint8_t dataBlock[16] = "Hello RFID!"; // 要寫入的字串，最多 16 個字元（MIFARE Classic 每個 block 只能存 16 bytes）
    
    uint8_t blockNum = 4; // 設定要寫入的 block 編號（MIFARE Classic 資料區從 block 4 開始）
    uint8_t keyA[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // 預設密鑰（出廠時通常都是 0xFF FF FF FF FF FF）

    // 使用 Key A 來驗證區塊是否可以存取
    if (nfc.mifareclassic_AuthenticateBlock(uid, uidLength, blockNum, 0, keyA)) {
      // 如果驗證成功，嘗試寫入資料到 blockNum
      if (nfc.mifareclassic_WriteDataBlock(blockNum, dataBlock)) {
        Serial.println("寫入成功！"); // 成功寫入資料
      } else {
        Serial.println("寫入失敗！"); // 可能發生寫入錯誤，例如存取權限不足
      }
    } else {
      Serial.println("區塊驗證失敗！"); // 如果驗證密鑰失敗，可能密鑰不正確或區塊無法存取
    }

    delay(2000); // 等待 2 秒，避免連續寫入導致卡片頻繁存取
  }
}

