/*
 * General related:
 */

static int max_tries = 5; // numero maximo de tentativas para tentar se conectar com os perifericos
String file_name = "/";
static int L_INFO = 1, L_WARNING = 2, L_ERROR = 3, L_DISABLED = 4;
int log_level = L_ERROR; 
bool listar_sd = false;
bool enviar_arquivo_sd = false;
int arquivo_a_enviar = 0;
const int led_pin = 32; 
bool lido = false;
bool logger_has_error = false;


/*
 * SD card related:
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"

/*
 * Termopar related:
 */
#include "Nanoshield_Termopar.h"
#define CS_termopar 15
Nanoshield_Termopar tc(CS_termopar, TC_TYPE_VOLTAGE_GAIN_32, TC_AVG_OFF);
char internal[20] = "";
char external[20] = "";
float external_float = 0;
float previously_external = 0;

/*
 * RTC related (clock):
 */
#include "RTClib.h"
RTC_DS3231 rtc;
char date_time_buf[19];
static bool atualizar_hora_no_setup = false;
DateTime now;


/* INA219 related (Power) */
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;
float power_mW_float = 0;
float previouly_power = 0;
char power_mW[8] = "";
bool calibrated = false;


/* Bluetooth related */
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID   "ab0828b1-198e-4351-b779-901fa0e0371e"
#define CHARACTERISTIC_UUID_RX_atualizar_hora  "914e6957-a448-490b-ae63-227c9d210cc2" // Atualizar hora
#define CHARACTERISTIC_UUID_TX_enviar  "50e9c8fc-4ddd-426c-9b63-cefd2034f881" // Enviar arquivo
#define CHARACTERISTIC_UUID_TX_listar  "1d23c96e-efd0-42d0-8301-bfe5035bafbe" // Listar arquivos
#define CHARACTERISTIC_UUID_RX_signal  "3ebb2155-5ca0-4362-b14a-393255d50f25" // 
#define CHARACTERISTIC_UUID_RX_power  "1b312471-1b52-47c1-a129-cdbdf0825a3f" // 

BLECharacteristic *pCharacteristicAtualizarHora; //através desse objeto iremos enviar dados para o client
BLECharacteristic *pCharacteristicEnviar; //através desse objeto iremos enviar dados para o client
BLECharacteristic *pCharacteristicListarArquivos; //através desse objeto iremos enviar a lista de arquivos do SD
BLECharacteristic *pCharacteristicSignal;
BLECharacteristic *pCharacteristicPower; 
 
 
void setup ()  
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("----------------------------------------------");
  Serial.println("-----------LOGGER DE FLUXO DE SEIVA-----------");
  Serial.println("--------------DESENVOLVIDO POR----------------");
  Serial.println("------------RENNAN ANDRES PALOSCHI------------");
  Serial.println("-----------------Versão 6.0-------------------");
  Serial.println("----------------------------------------------");
  Serial.println();
  Serial.println();

  pinMode(led_pin, OUTPUT);
  
  setup_relogio();
  setup_ina219();
  setup_termocoupler(); // Configuração do termopar
  setup_sd();
  setup_bluetooth(); 


  if (logger_has_error == false){
    digitalWrite(led_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(2000);                       // wait for a second
    digitalWrite(led_pin, LOW);    // turn the LED off by making the voltage LOW
  }
 
  get_data_time();
  file_name += String(date_time_buf);
  file_name += String(".csv");
  file_name.replace(':', '_');
 
  writeFile(SD, file_name.c_str(), "data_time, termocouple(mV), probe_power(mW), board_temperature(°C)\n");
  get_data_time();
  
}

void loop () 
{


  get_data_time();
  read_termocoupler();
  get_probe_power();
 
  // só faz a gravação no SD de 10 em 10 minutos
  if ((now.minute() == 0) or (now.minute() % 10 == 0)){
    if (lido == false){
      Serial.println("----------------------------------------------");
      Serial.print("Hora da leitura:\t");
      Serial.println(date_time_buf);
      sd_write_log();
      lido = true;
    }
  } else {
    // caso não esteja no periodo de leitura, seta o lido como falso
    lido = false;
  }
  
    // Verifica se foi solicitado para listar os arquivos do SD
    if (listar_sd) {
      listDir(SD, "/", 0);
      listar_sd=false;
    }
    // Verifica se foi solicitado para enviar algum arquivo
    if (enviar_arquivo_sd){
      transmit_file(SD, "/", 0, arquivo_a_enviar);
      enviar_arquivo_sd = false;
    }
    
    // Verifica se houve mudança no sinal e atualiza na caracteristica
    if (previously_external != external_float){
      transmit_data(external, pCharacteristicSignal);
      previously_external = external_float;
    }
    
    // Verifica se houve mudança no aquecimento e atualiza na caracteristica
    if (previouly_power != power_mW_float){
      transmit_data(power_mW, pCharacteristicPower);
      previouly_power = power_mW_float;
    }
    delay(100);
}
