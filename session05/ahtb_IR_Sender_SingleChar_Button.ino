#include <IRremote.hpp>

#define IR_LED_PIN 4
#define BUTTON_PIN 3


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  IrSender.begin(IR_LED_PIN);
  
  delay(100);
  Serial.println("ready"); 
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    sendMessage();
    Serial.println(" sent."); 
    delay(1000); // debounce / avoid repeat
  }
}

void sendMessage() {
  IrSender.sendNEC((uint16_t)(uint8_t)'E', 0, 0);
}