# Session 05


![](../images/esp32-c3-supermini-schematics.jpg)



## 1. Hacking an infrared remote

Infrared (IR) LEDs work the same as regular ones, but they emit light that can not be seen by the human eye. 
An IR remote works by using an infrared LED to send coded pulses of invisible light to a receiver in your device. The receiver interprets these pulses as specific commands, such as turning the TV On/Off, changing the channel or adjusting the volume.

### Accessing the IR-LED

1. **Get access to the insides if the remote.**
   Find a way to open up the remote. Most consist of two plastic shells that are either held together by plastic clips or small screws. 

2. **Examine the IR-LED.**
   If written on the Circuit Board note down or mark the polarity. If not, either find out with a multimeter or by locating the "Anvil" (The bigger "plate" inside the bulb)

   ![LED Polarity](../images/led-polarity2.jpg)

3. **Wire the LED**
   a. Solder a cable directly to each soldering point on the PCB. Connect the other ends to the Arduino – Pin 4 and Ground
   ![Soldered Cables to IR LED](../images/soldered-cables-to-ir-led.jpg)

   b. **Or** desolder the LED by applying heat to both soldering points at the same time. When the solder liquifies carfully pull out the LED with a pair of tweezers. After getting the LED out successfully mark polarity and connect on the breadboard to Pin 4 and Ground.

![Soldering Kit](../images/solderingkit.jpg)


### Installing an external library (IRremote)

External Arduino libraries are add-on packages created by others that provide extra functionality not included in the basic Arduino software, such as controlling sensors, displays, or communication protocols. They make it easier to use complex hardware or features by offering pre-written code that you can include at the top of your own Arduino sketches.

We will install a library that makes coded infrared communication easier.

1. In the left menu column click the 3rd button from top for "Arduino Libraries".
2. Search for "IRremote".
3. Install "IRremote" by shirriff.

![Installing external libraries](../images/install-external-libraries.png)
![Button Schematic Interal Pullup](../images/button_circuit-internal-pullup.jpg)

### Send out a signal with a button press.

1. **Include the library**
   ```cpp
   #include <IRremote.hpp>
   ```
2. **Define the Pins**
   ```cpp
   #define IR_LED_PIN 4
   #define BUTTON_PIN 3
   ```

3. **Setup Function**
   Set up the PinMode for the Button and begin serial communication. `IrSender.begin(IR_LED_PIN)` sets our LED Pin to the correct PinMode for IR Communication.
   ```cpp
   void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    Serial.begin(9600);

    IrSender.begin(IR_LED_PIN);
    
    delay(100);
    Serial.println("ready"); 
    }
    ```

4. **sendMessage function to send a single char**
   Create a new function below `void loop()`. We will call this function in the loop afterwards. The `IrSender.sendNEC()` function sends out the letter 'E'
   
   ```cpp
   void sendMessage() {
    IrSender.sendNEC((uint16_t)(uint8_t)'E', 0, 0);
   }
   ```

5. **Send message on button press**
   ```cpp
   void loop() {
      if (digitalRead(BUTTON_PIN) == LOW) {
        sendMessage();
        Serial.println("sent."); 
        delay(1000); // avoid repeat
      }
    }
    ```

<details>
<summary>**Full Code**</summary>

```cpp
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
```

</details>

### Send out a String
Extend the example from above. Right now there is no way for the receiver to define the end of a message. This can be helped by sending a specific character that states that the string ended. A bit like saying "over" on a walkie talkie. 

We will use the character for "newline" `\n`.

1. **Define the Message**
   ```cpp
   String message = "HELLO"; //change this!
   ```

2. Setup stays the same.
3. Inside `void loop()` we need to pass the string variable to our sendMessage() function by writing the variable name ("message") inside the curved brackets.
   
   While we're at it – let's print that to the serial port aswell
   ```cpp
   sendMessage(message);

   Serial.print(message);
   Serial.println(" sent."); 
   ```
4. In our `sendMessage()` function we need to find out the length of our string and then send each character seperately. 
   
   ```cpp
   for (int i = 0; i < msg.length(); i++) {
   char c = msg[i];

   // send each character as 8-bit value
   IrSender.sendNEC((uint16_t)(uint8_t)c, 0, 0); 

   delay(100); // small gap between characters
   }

   ```
  
5. Send out the termination character
   ```cpp
   IrSender.sendNEC((uint16_t)(uint8_t)'\n', 0, 0);
   ```

<details>
<summary>**Full Code**</summary>

```cpp
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
```

</details>

## 2. TV-B Gone

This example loops through all "Power Off" codes found on this website: https://tasmota.github.io/docs/Codes-for-IR-Remotes/#sony-bd-s1500

```cpp
#include <IRremote.hpp>

#define IR_LED_PIN 4
#define BUTTON_PIN 3

// Tuning knobs
const uint16_t pauseMs = 250;
const uint16_t debounceButton = 800;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  IrSender.begin(IR_LED_PIN);
  delay(100);

  Serial.println("ready");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    blastAllPowerCodes();
    delay(debounceButton);
  }
}

void blastAllPowerCodes() {
  Serial.println("Sending power codes...");

  // From https://tasmota.github.io/docs/Codes-for-IR-Remotes/

  // Samsung (SAMSUNG protocol)
  IrSender.sendSamsung(0xE0E0, 0x40, 2); delay(pauseMs);
  IrSender.sendSamsung(0xE0E0, 0x99, 2); delay(pauseMs);
  IrSender.sendSamsung(0xE0E0, 0x19, 2); delay(pauseMs);

  // Sony SIRC (Tasmota Data packed as (address<<7)|command)
  IrSender.sendSony((uint16_t)(0x750 >> 7), (uint8_t)(0x750 & 0x7F), 2, 12); delay(pauseMs);
  IrSender.sendSony((uint16_t)(0xF50 >> 7), (uint8_t)(0xF50 & 0x7F), 2, 12); delay(pauseMs);
  IrSender.sendSony((uint16_t)(0xA90 >> 7), (uint8_t)(0xA90 & 0x7F), 2, 12); delay(pauseMs);
  IrSender.sendSony((uint16_t)(0xA8B47 >> 7), (uint8_t)(0xA8B47 & 0x7F), 2, 20); delay(pauseMs);

  // NEC (address = upper 16 bits, command = bits 15..8)
  IrSender.sendNEC(0x00FE, 0xA8, 2); delay(pauseMs);
  IrSender.sendNEC(0x0008, 0x00, 2); delay(pauseMs);
  IrSender.sendNEC(0x2662, 0xBA, 2); delay(pauseMs);
  IrSender.sendNEC(0x00FF, 0x30, 2); delay(pauseMs);
  IrSender.sendNEC(0x10C8, 0xE1, 2); delay(pauseMs);
  IrSender.sendNEC(0xCC00, 0x00, 2); delay(pauseMs);
  IrSender.sendNEC(0xE13E, 0xA4, 2); delay(pauseMs);
  IrSender.sendNEC(0xE13E, 0x13, 2); delay(pauseMs);
  IrSender.sendNEC(0x7E81, 0x7E, 2); delay(pauseMs);
  IrSender.sendNEC(0x7E81, 0xFE, 2); delay(pauseMs);
  IrSender.sendNEC(0x00FF, 0x00, 2); delay(pauseMs);
  IrSender.sendNEC(0x10ED, 0x00, 2); delay(pauseMs);
  IrSender.sendNEC(0x10ED, 0x40, 2); delay(pauseMs);
  IrSender.sendNEC(0x00FF, 0x00, 2); delay(pauseMs);
  IrSender.sendNEC(0x00FF, 0x80, 2); delay(pauseMs);
  IrSender.sendNEC(0x01FE, 0x48, 2); delay(pauseMs);
  IrSender.sendNEC(0x01FE, 0x58, 2); delay(pauseMs);
  IrSender.sendNEC(0x08E7, 0x62, 2); delay(pauseMs);

  Serial.println("Done.");

}
````








