#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 0
#define SS_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

void setup() {
  Serial.begin(9600); // Initialize serial communication.
  SPI.begin(); // Init SPI bus.
  mfrc522.PCD_Init(); // Init MFRC522 card.
}

void loop() {
  // Look for new cards.
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Card detected.");

    // Print UID of the card.
    Serial.print("UID tag :");
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      content.concat((char)mfrc522.uid.uidByte[i]);
    }
    Serial.println(content);

    // Modify the tag value (for demonstration purposes).
    String newContent = "123456"; // New value to write to the tag.

    // Write the new value to the tag.
    MFRC522::StatusCode status = mfrc522.MIFARE_Write(0, (byte*)newContent.c_str(), newContent.length());
    if (status == MFRC522::STATUS_OK) {
      Serial.println("Tag value updated successfully.");
    } else {
      Serial.println("Error updating tag value.");
    }

    // Halt PICC.
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD.
    mfrc522.PCD_StopCrypto1();

    delay(1000); // Delay before reading again.
  }
}
