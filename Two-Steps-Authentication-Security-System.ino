#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include<LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN); 
 
byte nuidPICC[4];

LiquidCrystal_I2C LCD(0x27,16,2);

const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};
byte rowPins[ROWS] = {6,7,5,2}; 
byte colPins[COLS] = {A0,A1,A2,A3}; 

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

int iscorrect=0;
char correctpassword[6]={'A','#','1','2','*','B'};
char actualpassword[6];
int index=0;

void setup() { 
  Serial.begin(9600);
  SPI.begin(); 
  rfid.PCD_Init(); 
pinMode(3,OUTPUT);  
pinMode(4,OUTPUT);
pinMode(8,OUTPUT);
LCD.init();
LCD.backlight();
}

void verified(){
digitalWrite(3,HIGH);
digitalWrite(4,LOW);
noTone(8);
}

void noTverified(){
digitalWrite(3,LOW);
digitalWrite(4,HIGH);
tone(8,1500);
}
 
 void normal(){
digitalWrite(3,LOW);
digitalWrite(4,LOW);
noTone(8);
 }

void loop() {

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

if(nuidPICC[0]==0x1A && nuidPICC[1]==0x19 && nuidPICC[2]==0xC2 && nuidPICC[3]==0x01){
  Serial.println("code verified");
  verified();
  delay(5000);
  normal();
iscorrect=1;
  }

else {
  Serial.println("code not verified");
  noTverified();
  delay(5000);
  normal();
  iscorrect=0;
}

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  }

char Key = customKeypad.getKey();
 if (Key){
  Serial.println(Key);
  actualpassword[index]=Key;
  if(actualpassword[index]==correctpassword[index]){
    index++;
  }
  else{
    if(iscorrect==1){
      LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("WRONG !");
delay(3000);
LCD.clear();
index=0;
}

  }
if(index==6 && iscorrect==1){
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("WELCOME !");
  delay(5000);
  LCD.clear();
index=0;
iscorrect=0;
}
}
 }

