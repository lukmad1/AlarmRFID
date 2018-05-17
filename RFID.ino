
boolean sprawdzRFID(){


  String klucz1 ="43 69 b5 63";
  String klucz2 ="4d b6 2c 83";
  String klucz3 ="51 1d 01 89";
  String klucz4 ="d3 e6 be 1c";
  byte litera;
  String klucz;

  
  for(byte i=0; i<rfid.uid.size; i++){
    klucz.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    klucz.concat(String(rfid.uid.uidByte[i], HEX));
  }
  delay(500);
  Serial.println(klucz);
  if(klucz.substring(1) == klucz1 || klucz.substring(1) == klucz2 || klucz.substring(1) == klucz3 || klucz.substring(1) == klucz3){
    return true;
    Serial.println("Prawidlowy klucz");
  }else{
    return false;
    
    Serial.println("Nie prawidlowy klucz");
  }
}

