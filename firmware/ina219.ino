//static float calibrated_v = 320.00;
//static float acc_error = calibrated_v * 0.02;

void get_probe_power(){
  //float shuntvoltage = 0;
  //float busvoltage = 0;
  //float current_mA = 0;
  //float loadvoltage = 0;

  //shuntvoltage = ina219.getShuntVoltage_mV();
  //busvoltage = ina219.getBusVoltage_V();
  //current_mA = ina219.getCurrent_mA();
  power_mW_float = ina219.getPower_mW();
  dtostrf(power_mW_float, 6, 2, power_mW);
  //loadvoltage = busvoltage + (shuntvoltage / 1000);
  
//  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
//  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
//  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
//  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  if (log_level <= L_INFO) {
    Serial.print("Power:         "); 
    Serial.print(power_mW); Serial.println(" mW");
  }
}


void setup_ina219(){
  int tries = 0;
  while (tries < max_tries) {  
    if (ina219.begin()){
      Serial.println("ina219 initialized");
      break;
    }
    delay(2);
    tries += 1;
  } 

  if (tries == max_tries){
    logger_has_error = true;
    if (log_level <= L_ERROR) 
      Serial.println( "ERROR: Cannot find ina219" );
    digitalWrite(led_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);                       // wait for a second
    digitalWrite(led_pin, LOW);    // turn the LED off by making the voltage LOW
    delay(200);  
    digitalWrite(led_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);                       // wait for a second
    digitalWrite(led_pin, LOW);    // turn the LED off by making the voltage LOW
  }
}
