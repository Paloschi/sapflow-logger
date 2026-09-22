float board_probe_bias = -0.155;

void setup_termocoupler() {
  tc.begin();
  tc.read();
  if (tc.hasError()) {
    logger_has_error = true;
    printErrorsTc();
    digitalWrite(led_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);                       // wait for a second
    digitalWrite(led_pin, LOW);    // turn the LED off by making the voltage LOW
    delay(200);  
    digitalWrite(led_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);                       // wait for a second
    digitalWrite(led_pin, LOW);    // turn the LED off by making the voltage LOW
    delay(200);  
    digitalWrite(led_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);                       // wait for a second
    digitalWrite(led_pin, LOW);    // turn the LED off by making the voltage LOW
  }
}

void printErrorsTc() {
  if (log_level <= L_ERROR){
    if (tc.isOpen()) {
      Serial.println(F("Open circuit"));
    } else if (tc.isOverUnderVoltage()) {
      Serial.println(F("Overvoltage/Undervoltage"));
    } else if (tc.isInternalOutOfRange()) {
      Serial.println(F("Internal temperature (cold junction) out of range)"));
    } else if (tc.isExternalOutOfRange()) {
      Serial.println(F("External temperature (hot junction) out of range"));
    }
  }
}

void read_termocoupler() {

  if (log_level <= L_INFO) Serial.print("Termocoupler:\t");
  tc.read();
  if (tc.hasError()) {
    printErrorsTc();
    strcpy(internal, "ERROR");
    strcpy(external, "ERROR");
  } else {
    dtostrf(tc.getInternal(), 6, 1, internal);
    external_float = (tc.getExternal() * 1000) - (board_probe_bias);
    dtostrf(external_float, 10, 6, external);
    if (log_level <= L_INFO) Serial.println(String(external) + " mV");
  }
}
