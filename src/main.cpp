#include <Homie.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 4
#define RESET_BTN 16

MFRC522 mfrc522(SS_PIN, RST_PIN);

String lastUid = "";

void sendMqttMessage(const char *topic, const char *message) {
  std::string payload("{\"id\":\"");
  payload.append(Homie.getConfiguration().deviceId);
  payload.append("\", \"message\":\"");
  payload.append(message);
  payload.append("\"}");

  Homie.getMqttClient().publish(topic, 1, true, payload.c_str());
}

void setupHandler() {
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

  if (uid != lastUid) {
    sendMqttMessage("ingosi", uid.c_str());
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
  Homie.setResetTrigger(RESET_BTN, LOW, 2000);
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  Homie.setup();
}

void loop() {
  Homie.loop();
}
