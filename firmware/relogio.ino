void setup_relogio(){
  int tries = 0;

  while (tries < max_tries) {  
    if (rtc.begin()){
      Serial.println("RTC initialized");
      
      if (atualizar_hora_no_setup){
        rtc.adjust(DateTime(__DATE__,__TIME__));
        Serial.println("RTC time updated");
      }
      
      break;
    }
    delay(2);
    tries += 1;
  } 

  if (tries == max_tries){
    logger_has_error = true;
    if (log_level <= L_ERROR) 
      Serial.println( "ERROR: Cannot find RTC" );
    digitalWrite(led_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);                       // wait for a second
    digitalWrite(led_pin, LOW);    // turn the LED off by making the voltage LOW
  }
  
}



void get_data_time(){
  now = rtc.now();
  sprintf(date_time_buf, "%04u-%02u-%02u %02u:%02u:%02u", now.year(),now.month(),now.day(),now.hour(),now.minute(),now.second());
  if (log_level <= L_INFO) 
    Serial.println(date_time_buf);
}

void update_time(String dt){
  uint32_t t;
  t = dt.toInt();
  rtc.adjust(DateTime(t));
  get_data_time();
}
