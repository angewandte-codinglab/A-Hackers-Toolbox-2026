#include <IRremote.hpp>

#define IR_RECEIVE_PIN 4

String receivedText = "";

void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
  IrReceiver.begin(IR_RECEIVE_PIN);
}

void loop() {
  if (IrReceiver.decode()) {

    uint32_t data = IrReceiver.decodedIRData.decodedRawData;

    // we only use lowest 8 bits (our character)
    char c = (char)(data & 0xFF);

    receivedText = c;
    Serial.println(receivedText);
    receivedText = ""; // reset

    IrReceiver.resume();
  }
}