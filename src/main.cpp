#include <Homie.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 4
#define RESET_BTN 0
#define BLUE_LED 16
#define GREEN_LED 2
#define YELLOW_LED 15

MFRC522 mfrc522(SS_PIN, RST_PIN);

String lastUid = "";
unsigned int timerLastUid;

//LED part
boolean onB = false;
int bBlinks = -1;
int currentBBlink = -1;
unsigned long nextBBlinkTimer = 0;
unsigned long onBDuration;
unsigned long timerB;
boolean onG = false;
unsigned int onGDuration;
unsigned int timerG;
boolean onY = false;
unsigned int onYDuration;
unsigned int timerY;

void blueLedOn(unsigned long duration) {
  onB = true;
  timerB = millis();
  onBDuration = duration;
  digitalWrite(BLUE_LED, HIGH);
}

void blueLedBlink(int blinks, unsigned long duration) {
  bBlinks = blinks;
  currentBBlink = 0;
  blueLedOn(duration);
}

void greenLedOn(unsigned long duration) {
  onG = true;
  timerG = millis();
  onGDuration = duration;
  digitalWrite(GREEN_LED, HIGH);
}

void yellowLedOn(unsigned long duration) {
  onY = true;
  timerY = millis();
  onYDuration = duration;
  digitalWrite(YELLOW_LED, HIGH);
}

void ledHandler() {
  if (onB && (millis() - timerB) >= onBDuration) {
    onB = false;
    digitalWrite(BLUE_LED, LOW);
    if (bBlinks != -1) {
      nextBBlinkTimer = millis() + onBDuration;
    }
  }
  if (bBlinks != -1) {
    if (currentBBlink < bBlinks) {
      if (millis() >= nextBBlinkTimer) {
        blueLedOn(onBDuration);
        bBlinks++;
      }
    } else {
      bBlinks = -1;
      currentBBlink = -1;
    }
  }
  if (onG && (millis() - timerG) >= onGDuration) {
    onG = false;
    digitalWrite(GREEN_LED, LOW);
  }
  if (onY && (millis() - timerY) >= onYDuration) {
    onY = false;
    digitalWrite(YELLOW_LED, LOW);
  }
}

// MQTT part
void sendMqttMessage(const char *topic, const char *message) {
  std::string payload(message);

  Homie.getMqttClient().publish(topic, 1, true, payload.c_str());
}

void onMqttConnect(bool sessionPresent) {
  Serial.println(sessionPresent);
  Homie.getMqttClient().subscribe(Homie.getConfiguration().deviceId, 2);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  String msg = payload;
  int msgCode = msg.toInt();

  /**
   * 1:  Error
   * 11: RFID not found
   * 12: Item already returned
   * 20: Item registered
   * 21: Item returned
   * 22: Item borrowed
   * 30: Borrower registered
   * 31: Borrower card checked
   * 32: Borrower card checked-out 
   */
  switch (msgCode) {
    case 1:
      Serial << "Error" << endl;
      blueLedBlink(3, 200UL);
      break;
    case 11:
      break;
    case 12:
      break;
    case 20:
      break;
    case 21:
      break;
    case 22:
      break;
    case 30:
      break;
    case 31:
      break;
    case 32:
      break;
  }
}

void setupHandler() {
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, LOW);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);
  pinMode(YELLOW_LED, OUTPUT);
  digitalWrite(YELLOW_LED, LOW);

  blueLedOn(2000UL);
  greenLedOn(1500UL);
  yellowLedOn(1000UL);

  sendMqttMessage("register", "setup");
}

void loopHandler() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }

  if (uid != lastUid ||  millis() > (timerLastUid + 5000)) {
    timerLastUid = millis();
    sendMqttMessage("ingosi", uid.c_str());
    blueLedOn(500UL);
    lastUid = uid;
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  SPI.begin();
  mfrc522.PCD_Init();

  Homie_setFirmware("ingosi", "0.1.0");
  Homie_setBrand("Ingosi");
  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);
  Homie.setResetTrigger(RESET_BTN, LOW, 2000);
  Homie.setLedPin(BLUE_LED, HIGH);
  Homie.getMqttClient().onConnect(onMqttConnect);
  Homie.getMqttClient().onMessage(onMqttMessage);

  Homie.setup();
}

void loop() {
  Homie.loop();
  ledHandler();
}
