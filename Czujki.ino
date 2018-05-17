void pinyPIR(){
  pinMode(PIR1, INPUT);
}


boolean odczytPIR(int PIR_pin){

if(digitalRead(PIR_pin)){
  return true;
  }
  else
    return false;
}

