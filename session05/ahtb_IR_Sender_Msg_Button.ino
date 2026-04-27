#include <IRremote.hpp>

#define IR_LED_PIN 4
#define BUTTON_PIN 3

String message = "HELLO";  // change this!

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  IrSender.begin(IR_LED_PIN);
  
  delay(100);
  Serial.println("ready"); 
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    sendMessage(message);
    Serial.print(message);
    Serial.println(" sent."); 
    delay(1000); // debounce / avoid repeat
  }
}

void sendMessage(String msg) {
  for (int i = 0; i < msg.length(); i++) {
    char c = msg[i];

    // send each character as 8-bit value
    IrSender.sendNEC((uint16_t)(uint8_t)c, 0, 0); 

    delay(100); // small gap between characters
  }

  // send termination character (newline)
  IrSender.sendNEC((uint16_t)(uint8_t)'\n', 0, 0);
}