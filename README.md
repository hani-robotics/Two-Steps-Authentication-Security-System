# 🔓 Two-Steps-Authentication-Security-System:

   A dual-authentication access control system combining RFID card verification with a keypad-entered password, built as the **capstone project** for a summer-long Arduino learning journey.

**Next step ---> ESP32**

## Overview:

The system verifies identity in two independent steps : an **RFID** card scan,followed by a password entered on a **keypad**.
Access is only confirmed when both checks succeed. The two inputs are handled independently in the main loop rather than sequentially blocking one another, since a user cannot physically scan a card and type a password at the exact same instant.

## How it works:

-**Unrecognized card** ==> Triggers an alarm (Red LED + Buzzer) for 5 seconds. No message shown on LCD.

-**Correct RFID tag** ==> Activates the Green LED and enables password entry.

-**Correct password** ==> Displays "WELCOME !".

-**Wrong password** ==> Displays "WRONG !".


## Finite State Machine diagram :

[ https://github.com/hani-robotics/Two-Steps-Authentication-Security-System/blob/main/finite.png ]

## Hardware components:

-Arduino Uno

-MFRC522 RFID Reader

-RFID tag & card

-I2C LCD 16x2

-Active Buzzer

-Red LED + Green LED

-Breadboard

-Keypad (4x4)

-3 resistors (220 Ω)

## Software needed:

-Arduino IDE

-"MFRC522" Library

-"Keypad" Library

-"LiquidCrystal_I2C" Library

## connections:

| Connection   | Pin         | 
|--------------|-------------|
|  RST         | 9           |
|  SS          | 10          |
|  MOSI        | 11          |
|  MISO        | 12          |
|  SCK         | 13          |
|  SDA         | A4          |
|  SCL         | A5          |
|  ROW Pins    | 6,7,5,2     |
|  Cols pins   | A0,A1, A2,A3|
|  Buzzer      | 8           |
|  Green LED   | 3           |
|  Red LED     | 4           |


RST, SS, MOSI, MISO & SCK are RFID's pins (SPI protocol).

SDA, SCL are LCD's pins (I2C protocol).

Row pins & Cols pins are keypad's protocols.


## Debugging & Problem solving :

1-**Keypad was completely unresponsive, despite being correctly defined in code**:

The RFID block used early **return** statement when no card was present. Since **loop()** runs top to bottom, hitting **return** exits the entire functions immediately **So whenever no card was on the reader (the vast majority of the time)**, the keypad-reading code further down never even executed.
**/Fix:** Remove the **return** statements and wrapped the RFID logic in a conditional block instead, so a missing card no longer prevents the rest of **loop()** from running.



2-**Remembering the result of the last card scan**:

The system needed to **remember** whether the last scanned card was valid, even after the card was removed from the reader-since scanning a card and typing a password cannot happen at the same instant.
**/Fix:** Introduced a persistent **iscorrect** flag. It resets to **0** whenever an incorrect card is scanned(blocking password entry entirely), and is set to **1** when the correct card is scanned(unlocking the password stage).



3-**Keypad input wasn't being read reliably**:

This came down to where the keypad-reading code sat relative to the RFID logic.
**/Fix:** Placed the keypad-reading instructions to run right after the RFID instructions in **loop()**, so both are checked on every iteration without either blocking the other.



4-**Verifying that the entered password matches the real one**:

**Fix:** Defined two arrays, a fixed one holding the correct password, and a second one that fills up as the user types. Each keypress is compared, in real time, against the correct password's character advances **index** to next keystroke. An incorrect character immediately resets **index** to 0,forcing the user to restart the entry from the beginning.




5-**Buzzer wasn't producing sound**

The buzzer stayed silent even though it was wired to it's designated pin and triggerd correctly in code.
**/Fix:** Added a resistor in series with the buzzer, which resolved the issue, likely a current limiting requirement that the direct pin connection alone didn't satisfy. 


## What I learned:

-**delay()** freezes, it doesn't wait. During a 'delay()' the entire program stops, including sensors, & inputs.

-you have to know exactly the position of every instruction in your code, for example: whether to write it inside or outside the condition .


## Future Improvements:

-Replace **delay()** by **millis()** for fully non-blocking timing.

-Switching from **Arduino** to **ESP32** for more GPIO & wireless capability.

-Add a servo motor for physical lock simulation.

-Store authorized RFID tags in EEPROM instead of hardcoding them.

## DEMO:

Watch the entire demo in linkedIn :
