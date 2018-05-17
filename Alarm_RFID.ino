#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define PIR1  7     //pin out sr04
#define gsmTx 2     //gsm TX pin
#define gsmRx 3     //gsm RX pin
#define SS_PIN 10  //RFID SlaveSelect pin
#define RST_PIN 9   //RFID reset pin
#define LCD_ADRESS 0x27 //Adres konwertera lcd i2c
MFRC522 rfid(SS_PIN, RST_PIN);   //inicjalizacja RFID


boolean alarm_uzbrojony = true;
boolean pierwsze_wyzwolenie = true;
boolean wywolano_alarm = true;
boolean flaga_alarmu = false;
boolean flaga_przelaczenia = false;
int timer_rozbrojenia, timer_alarmu;
int timer_wyjscia;
int timer_przelaczenia;
//////////////////////////////////////////////GSM//////////////////////////////////
char odczyt[200];
SoftwareSerial gsm(gsmTx, gsmRx);
String telefon = "";
String telefon2 = "";
int flaga_sms = 0;

//////////////////////////////////////LCD/////////////////////////////////////

LiquidCrystal_I2C lcd(LCD_ADRESS, 2,  1,  0,  4,  5,  6,  7,  3, POSITIVE); 
String nowy_kod_ekran;
String pin_ekran;
String kod_ekran;

//////////////////////////////////RFID//////////////////////////////////
String klucz1 ="";
String klucz2 = "";
String klucz3 = "";
boolean dostep = false;


void setup() {
 delay(5000); //czas na zalogowanie gsm
  pinyPIR();
  
  lcd.begin(16, 2);
  lcd.noBacklight();
//  gsm.begin(9600);
//  delay(200);
//  gsmCommand("AT+CMGF=1");
//  delay(100);
//  gsmCommand("AT+CSCS=\"GSM\"");
//  delay(100);
//  gsmSMS(telefon, "Inicjalizacja alarmu");
//  delay(5000);
//  gsmSMS(telefon2, "Inicjalizacja alarmu");
  Serial.begin(9600);
 
  SPI.begin();
  rfid.PCD_Init();
  
  
}

void loop() {

while(alarm_uzbrojony){
  
//  odczytanie();
//
//Serial.println(odczyt);
//  if (strstr(odczyt, "+CMTI: \"SM\",1") && flaga_sms == 0) {
//    gsm.println("AT+CMGR=1");
//    flaga_sms = 1;
//  } 
//  else if (strstr(odczyt, "+CMTI: \"SM\"") && flaga_sms == 0){
//     gsm.println("AT+CMGDA=\"DEL ALL\"");
//  }
//
//
//  if ((strstr(odczyt, "600013973") || strstr(odczyt, "504117487")) && strstr(odczyt, "Off") && flaga_sms == 1) {
//    dostep = true;
//    delay(100);
//    flaga_sms = 2;
//  }
//
//
//  if (  ((!strstr(odczyt, "600013973") || !strstr(odczyt, "504117487"))  || ((strstr(odczyt, "600013973") || !strstr(odczyt, "504117487")) && !strstr(odczyt, "Off"))) && (!strstr(odczyt, "AT+CMGR=1")) && (!strstr(odczyt, "+CMTI: \"SM\",1"))  && flaga_sms == 1) {
//    gsm.println("AT+CMGDA=\"DEL ALL\"");
//    flaga_sms = 0;
//   // delay(10);
//  }
//
//  if (flaga_sms == 2) {
//    gsm.println("AT+CMGDA=\"DEL ALL\"");
//    gsm.flush();
//    flaga_sms = 0;
//    delay(100);
//    gsmSMS(telefon, "Alarm rozbrojony zdalnie");
//    delay(5000);
//    gsmSMS(telefon2, "Alarm rozbrojony zdalnie");
//  }
//
//  
//  



  if(dostep){
    alarm_uzbrojony = false;
    pierwsze_wyzwolenie = true;
    wywolano_alarm = true;
    flaga_alarmu = false;
    timer_przelaczenia = millis();
    lcd.backlight();
    lcd.clear();
    dostep = false;
    lcd.print("Przyznano dostep");
    delay(2000);
    lcd.clear();
    lcd.print("Rozbrojono alarm!");
    delay(1000);
    lcd.noBacklight();
    }



  if(alarm_uzbrojony && odczytPIR(PIR1)){
  while(pierwsze_wyzwolenie){
      timer_rozbrojenia = millis();
        pierwsze_wyzwolenie = false;
        flaga_alarmu = true;
        lcd.backlight();
        Serial.println("Wykryto ruch!");
  }
}

    timer_alarmu = (10 - ((millis() - timer_rozbrojenia) / 1000));
    if (timer_alarmu > 500) timer_alarmu = 0;


    
    if (timer_alarmu == 0 && flaga_alarmu) {
      while (wywolano_alarm) {
//        gsmSMS (telefon, "Wywolano alarm!!");
//        delay(5000);
//        gsmSMS (telefon2, "Wywolano alarm!!");
        wywolano_alarm = false;
        lcd.print("Wywolano alarm!");
        //digitalWrite(BRZECZYk,HIGH)
      }
    }


  if(!rfid.PICC_IsNewCardPresent()){
    break;
  }
  if(!rfid.PICC_ReadCardSerial()){
    break;
  }
    dostep = sprawdzRFID();


  
}


if(10-((millis()-timer_przelaczenia)/1000) == 0){
  flaga_przelaczenia = true;
}



while(!alarm_uzbrojony && flaga_przelaczenia){
  
//    odczytanie();
//  if (strstr(odczyt, "+CMTI: \"SM\",1") && flaga_sms == 0) {
//    gsm.println("AT+CMGR=1");
//    flaga_sms = 1;
// 
//  } 
//  else if (strstr(odczyt, "+CMTI: \"SM\"") && flaga_sms == 0){
//     gsm.println("AT+CMGDA=\"DEL ALL\"");
//
//  }
//
//
//  if ((strstr(odczyt, "600013973") || strstr(odczyt, "504117487")) && strstr(odczyt, "On") && flaga_sms == 1) {
//
//      dostep = true;
//      delay(100);
//      flaga_sms = 2;
//  }
//
//  delay(5);
//  if (  ((!strstr(odczyt, "600013973") || !strstr(odczyt, "504117487"))  || ((strstr(odczyt, "600013973`") || !strstr(odczyt, "504117487")) && !strstr(odczyt, "On"))) && (!strstr(odczyt, "AT+CMGR=1")) && (!strstr(odczyt, "+CMTI: \"SM\",1"))  && flaga_sms == 1) {
//     gsm.println("AT+CMGDA=\"DEL ALL\"");
//    flaga_sms = 0;
//  
//  }
//
//  if (flaga_sms == 2) {
//    gsm.println("AT+CMGDA=\"DEL ALL\"");
//    gsm.flush();
//    flaga_sms = 0;
//    delay(100);
//    gsmSMS(telefon, "Alarm uzbrojony zdalnie");
//    delay(5000);
//    gsmSMS(telefon2, "Alarm uzbrojony zdalnie");
//
//  }

Serial.println(dostep);
  if(dostep){
    lcd.clear();
    dostep = false;
    lcd.backlight();
    lcd.print("60s do wyjscia!");
    timer_wyjscia = millis();
  }

  if( 10 -((millis() - timer_wyjscia)/1000) == 0){
    alarm_uzbrojony = true;
    flaga_przelaczenia = false;
    lcd.clear();
    lcd.noBacklight();
    
  }



  
    if(!rfid.PICC_IsNewCardPresent()){
    break;
  }
  if(!rfid.PICC_ReadCardSerial()){
    break;
  }
  dostep = sprawdzRFID();
}

  
  

}
