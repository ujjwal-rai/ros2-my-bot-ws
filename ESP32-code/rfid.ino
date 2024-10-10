#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 0

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

Servo gateServo;  // Create a servo object to control the gate servo
int servoPin = 17; // Choose the pin to which the servo is connected

void servoGateOpen() {
  // Open the gate by moving the servo to 90 degrees
  gateServo.write(180);
  delay(1000); // Adjust delay as needed for servo to reach position
}

void servoGateClose() {
  // Close the gate by moving the servo to 0 degrees
  gateServo.write(0);
  delay(1000); // Adjust delay as needed for servo to reach position
}

void rfidGateOpen() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Print UID of the card
    Serial.print("UID tag :");
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      content.concat((char)mfrc522.uid.uidByte[i]);
    }
    Serial.println(content);

    // Check for specific RFID tag
    if (content == "123456") { // Modify this to match your RFID tag
      Serial.println("Access granted!");
      servoGateOpen(); // Open the gate
      servoGateClose(); 
    } else {
      Serial.println("Access denied!");
    }

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
  }
}


void setup() {
    Serial.begin(9600);
    gateServo.attach(servoPin);
    SPI.begin(); // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 
}

void loop() {
  rfidGateOpen();
}
