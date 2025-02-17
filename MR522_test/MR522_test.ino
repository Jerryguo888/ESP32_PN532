#include <Wire.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ   (2)
#define PN532_RESET (3)

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup(void) {
    Serial.begin(115200);
    Serial.println("NFC Writer Initialized");

    nfc.begin();
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("Didn't find PN53x board");
        while (1);
    }

    Serial.println("Found PN53x board");
    nfc.SAMConfig();
}

void loop(void) {
    Serial.println("Waiting for an NFC card...");
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
    uint8_t uidLength;

    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
        Serial.println("NFC tag detected!");
        Serial.print("UID: ");
        for (uint8_t i = 0; i < uidLength; i++) {
            Serial.print(uid[i], HEX);
            Serial.print(" ");
        }
        Serial.println();

        if (writeURLToTag("https://www.example.com")) {
            Serial.println("✅ URL successfully written!");
        } else {
            Serial.println("❌ Failed to write URL.");
        }

        delay(2000);
    }
}

// ✅ 寫入 NDEF URL 到 Mifare Ultralight 卡片
bool writeURLToTag(const char *url) {
    uint8_t urlPrefix = 0x03; // NDEF TLV (Type-Length-Value) Record
    uint8_t urlLength = strlen(url) + 5; // 訊息長度
    uint8_t data[32] = {
        urlPrefix, urlLength,
        0xD1, 0x01, (uint8_t)(strlen(url) + 1), // NDEF Header
        0x55, 0x01 // URI Identifier Code (0x01 = "http://www.")
    };

    memcpy(data + 7, url, strlen(url));

    Serial.println("Writing URL to NFC tag...");
    if (nfc.mifareultralight_WritePage(4, data) &&
        nfc.mifareultralight_WritePage(5, data + 4) &&
        nfc.mifareultralight_WritePage(6, data + 8)) {
        return true;
    }

    return false;
}
