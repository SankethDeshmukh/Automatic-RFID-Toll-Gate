#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// ---------------- PIN DEFINITIONS ----------------
#define SS_PIN      10
#define RST_PIN      9
#define IR_SENSOR    7
#define SERVO_PIN    6
#define LED_GREEN    3
#define LED_RED      4

// ---------------- OBJECTS ----------------
MFRC522 rfid(SS_PIN, RST_PIN);
Servo gate;

// ---------------- VALID RFID UID ----------------
// Replace these values with your actual RFID card UID
byte validUID[] = {0x12, 0x34, 0x56, 0x78};

// Maximum time to wait for vehicle to pass
const unsigned long VEHICLE_TIMEOUT = 10000; // 10 seconds


// ---------------- UID COMPARISON FUNCTION ----------------
bool compareUID(byte *cardUID, byte *validUID, byte length) {

  for (byte i = 0; i < length; i++) {
    if (cardUID[i] != validUID[i]) {
      return false;
    }
  }

  return true;
}


// ---------------- SETUP ----------------
void setup() {

  Serial.begin(9600);

  // Initialize SPI
  SPI.begin();

  // Initialize RFID reader
  rfid.PCD_Init();

  // Attach servo
  gate.attach(SERVO_PIN);

  // Pin modes
  pinMode(IR_SENSOR, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Initial LED state
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);

  // Keep gate closed at startup
  gate.write(0);

  Serial.println("Automatic RFID Toll Gate System");
  Serial.println("System Ready...");
}


// ---------------- MAIN LOOP ----------------
void loop() {

  // Wait for a new RFID card
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Read RFID card
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // ---------------- DISPLAY CARD UID ----------------
  Serial.print("Card UID: ");

  for (byte i = 0; i < rfid.uid.size; i++) {

    if (rfid.uid.uidByte[i] < 0x10) {
      Serial.print("0");
    }

    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }

  Serial.println();


  // ---------------- CHECK RFID UID ----------------
  bool accessGranted = false;

  // Check UID length first
  if (rfid.uid.size == sizeof(validUID)) {

    accessGranted = compareUID(
      rfid.uid.uidByte,
      validUID,
      sizeof(validUID)
    );
  }


  // ==================================================
  //              ACCESS GRANTED
  // ==================================================
  if (accessGranted) {

    Serial.println("Access Granted");

    // Green ON
    digitalWrite(LED_GREEN, HIGH);

    // Red OFF
    digitalWrite(LED_RED, LOW);

    // Open gate
    gate.write(90);

    Serial.println("Gate Open");

    // Wait 2 seconds before checking vehicle
    delay(2000);


    // ---------------- WAIT FOR VEHICLE TO PASS ----------------
    Serial.println("Waiting for vehicle to pass...");

    unsigned long startTime = millis();

    // Assumption:
    // IR sensor = LOW when vehicle is detected
    // IR sensor = HIGH when vehicle has passed

    while (digitalRead(IR_SENSOR) == LOW) {

      // Prevent the program from getting stuck forever
      if (millis() - startTime >= VEHICLE_TIMEOUT) {

        Serial.println("IR sensor timeout.");
        break;
      }

      delay(50);
    }


    // Close gate
    gate.write(0);

    Serial.println("Gate Closed");

    // Turn OFF green LED
    digitalWrite(LED_GREEN, LOW);
  }


  // ==================================================
  //              ACCESS DENIED
  // ==================================================
  else {

    Serial.println("Access Denied");

    // Red ON
    digitalWrite(LED_RED, HIGH);

    // Green OFF
    digitalWrite(LED_GREEN, LOW);

    // Keep gate closed
    gate.write(0);

    delay(2000);

    // Red OFF
    digitalWrite(LED_RED, LOW);
  }


  // Stop communication with RFID card
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  // Small delay before reading another card
  delay(500);
}