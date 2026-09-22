bool deviceConnected = false; //controle de dispositivo conectado

void transmit_data(const char * message, BLECharacteristic * pCharacteristic){
  const int max_buff = 20;
  int m_len = float(strlen(message));
  int n_tranmitions =  m_len/max_buff + !!(m_len%max_buff);
  char buf[21];
  
  for (int i=0; i<n_tranmitions; i++){
    
    strncpy_P(buf, message + (i*max_buff), (i+1)*max_buff);
    buf[20] = '\0';
    pCharacteristic->setValue(buf);
    pCharacteristic->notify();
    delay(12);
  }
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
 
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      BLEDevice::startAdvertising();    
    };
};

class MyCharacteristic_atualizar_hora: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristicAtualizarHora->getValue(); //pega o valor

      String valor = "";
      if (rxValue.length() > 0) {
        Serial.print("Hora recebida: ");
        for (int i = 0; i < rxValue.length(); i++){
          valor =  valor + rxValue[i];
        }
        Serial.println(valor);
      }
      update_time(valor);
        
    
    };  
};


class MyCharacteristic_listar_arquivos: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      listar_sd=true;
    };  
};

class MyCharacteristic_enviar_arquivo: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristicEnviar->getValue(); //pega o valor

      String valor = "";
      if (rxValue.length() > 0) {
        for (int i = 0; i < rxValue.length(); i++){
          valor =  valor + rxValue[i];
        }
        Serial.println(valor);
      }
      
      enviar_arquivo_sd=true;
      arquivo_a_enviar = valor.toInt();
    };  
};

void setup_bluetooth(){
  
    // Create the BLE Device
    BLEDevice::init("SAPFLOW DEVICE 004"); // nome do dispositivo bluetooth
 
    // Create the BLE Server
    BLEServer *pServer = BLEDevice::createServer(); //cria um BLE server 
 
    pServer->setCallbacks(new MyServerCallbacks()); //seta o callback do server

    /* ------------------------ Configuração da características de atualização -------------------------- */
    BLEService *pService = pServer->createService(SERVICE_UUID);
    
    // Create a BLE Characteristic para enviar dados
    pCharacteristicEnviar = pService->createCharacteristic(
                                                      CHARACTERISTIC_UUID_TX_enviar,
                                                      BLECharacteristic::PROPERTY_READ   |
                                                      BLECharacteristic::PROPERTY_WRITE  |
                                                      BLECharacteristic::PROPERTY_NOTIFY |
                                                      BLECharacteristic::PROPERTY_INDICATE
                                                      );
                                                      
    pCharacteristicEnviar->addDescriptor(new BLE2902());
    pCharacteristicEnviar->setCallbacks(new MyCharacteristic_enviar_arquivo());

    // Create a BLE Characteristic para atualizar a hora
    pCharacteristicAtualizarHora = pService->createCharacteristic(
                                                      CHARACTERISTIC_UUID_RX_atualizar_hora,
                                                      BLECharacteristic::PROPERTY_READ   |
                                                      BLECharacteristic::PROPERTY_WRITE  |
                                                      BLECharacteristic::PROPERTY_NOTIFY |
                                                      BLECharacteristic::PROPERTY_INDICATE
                                                      );

    pCharacteristicAtualizarHora->addDescriptor(new BLE2902());
    pCharacteristicAtualizarHora->setCallbacks(new MyCharacteristic_atualizar_hora());

    // Create a BLE Characteristic para listar or arquivos do SD
    pCharacteristicListarArquivos = pService->createCharacteristic(
                                                      CHARACTERISTIC_UUID_TX_listar,
                                                      BLECharacteristic::PROPERTY_READ   |
                                                      BLECharacteristic::PROPERTY_WRITE  |
                                                      BLECharacteristic::PROPERTY_NOTIFY |
                                                      BLECharacteristic::PROPERTY_INDICATE
                                                      );

    pCharacteristicListarArquivos->addDescriptor(new BLE2902());
    pCharacteristicListarArquivos->setCallbacks(new MyCharacteristic_listar_arquivos());

    // Create a BLE Characteristic para atualizar o sinal
    pCharacteristicSignal = pService->createCharacteristic(
                                                      CHARACTERISTIC_UUID_RX_signal,
                                                      BLECharacteristic::PROPERTY_READ   |
                                                      BLECharacteristic::PROPERTY_NOTIFY
                                                      );

//    pCharacteristicSignal->addDescriptor(new BLE2902());

    // Create a BLE Characteristic para atualizar o power
    pCharacteristicPower = pService->createCharacteristic(
                                                      CHARACTERISTIC_UUID_RX_power,
                                                      BLECharacteristic::PROPERTY_READ   |
                                                      BLECharacteristic::PROPERTY_NOTIFY
                                                      );

//    pCharacteristicPower->addDescriptor(new BLE2902());
    



    
    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();    

    BLEDevice::setPower(ESP_PWR_LVL_P7);
}
