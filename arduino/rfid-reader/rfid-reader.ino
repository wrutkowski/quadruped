/* Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9     
#define SS_PIN          10    

MFRC522 mfrc522(SS_PIN, RST_PIN);

byte keyringRFID[4] = {0xF5, 0x68, 0xCD, 0x65};
byte cardRFID[4] = {0x68, 0x4D, 0xEC, 0x10};
byte readCard[4];

void setup(void) {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();    
//    mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
    Serial.println("Ready!");
}

void loop() {
    if (getID()) {
        if (checkRFID(readCard, keyringRFID)) {
            Serial.println("keyring detected");
        } else if (checkRFID(readCard, cardRFID)) {
            Serial.println("card detected");
        }
    }
}

uint8_t getID() {
    // Getting ready for Reading PICCs
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
        return 0;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
        return 0;
    } 
    for ( uint8_t i = 0; i < 4; i++) {  //
        readCard[i] = mfrc522.uid.uidByte[i];
//        Serial.print(readCard[i], HEX);
    }
  
    mfrc522.PICC_HaltA(); // Stop reading
    return 1;
}

boolean checkRFID ( byte a[], byte b[] ) {
    boolean match = false;
    if ( a[0] != 0 ) {
        match = true;       // Assume they match at first
    }
    
    for (uint8_t k = 0; k < 4; k++ ) {
        if (a[k] != b[k]) {     // IF a != b then set match = false, one fails, all fail
            match = false;
        }
    }
    return match;
}
