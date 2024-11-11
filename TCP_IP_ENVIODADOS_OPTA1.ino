/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                PROGRAMAÇÃO MODBUS TPC/IP HOME ASSISTANT                                                      */
/*                                                                                                                                              */
/************************************************************************************************************************************************/





/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                            INCLUSÃO DAS BIBLIOTECAS NECESSÁRIAS PARA COMUNICAÇÃO                                             */
/*                                                                                                                                              */
/************************************************************************************************************************************************/

#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoRS485.h> 
#include <ArduinoModbus.h>
#include "OptaBlue.h"

#define PERIODIC_UPDATE_TIME 200
#define DELAY_AFTER6SETUP 200
/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                PROGRAMAÇÃO MODBUS TPC/IP HOME ASSISTANT                                                      */
/*                                                                                                                                              */
/************************************************************************************************************************************************/





/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                            INCLUSÃO DAS BIBLIOTECAS NECESSÁRIAS PARA COMUNICAÇÃO                                             */
/*                                                                                                                                              */
/************************************************************************************************************************************************/

#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoRS485.h> 
#include <ArduinoModbus.h>
#include "OptaBlue.h"

#define PERIODIC_UPDATE_TIME 200
#define DELAY_AFTER6SETUP 200


/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                CONFIGURAÇÃO DE MAC, IP E PORTA ETHERNET                                                      */
/*                                                             OPTA SECUNDÁRIO 2                                                                */
/************************************************************************************************************************************************/


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 68, 106);
EthernetServer ethServer(502);
ModbusTCPServer modbusTCPServer;


/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                       VARIÁVEIS (MEMÓRIAS) PARA ARMAZENAR O ESTADO DO BOTÃO DA ENTRADA I1                                    */
/*                                                                                                                                              */
/************************************************************************************************************************************************/


bool estadoButton = 0;


/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                          VARIÁVEIS (MEMÓRIAS) PARA INICIAR DETECTADO E RETORNA EM 0                                          */
/*                                                                   NÃO UTILIZADAS                                                             */
/************************************************************************************************************************************************/

int liga1 = 0;
int detectado = 0;
int retorna = 0;



/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                 INICIANDO COM OS CÓDIGOS DENTRO DO VOID SETUP                                                */
/*                                                                                                                                              */
/************************************************************************************************************************************************/



void setup() {
  
  //IMPRIME NO MONITOR SERIAL QUE O SERVIDOR ESTÁ SENDO INICIADO
  Serial.println("Iniciando Servidor TCP/IP");
  //INICIALIZA O SERVIDOR COM AS CONFIGURAÇÕES DE MAC E IP
  Ethernet.begin(mac, ip);

  //VERIFICA SE O OPTA TEM O HARDWARE ETHERNET/MODBUS
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Não foi encontrado hardware de Ethernet ou Modbus");
    if (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }

  //VERIFICA SE O CABO ETHERNET ESTÁ CONECTADO NO PRODUTO
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Cabo Ethernet não está conectado");
  }

  //INICIA O SERVIDOR MODBUS
  ethServer.begin();

  //VERIFICA SE A INICIALIZAÇÃO DO SERVIDOR FOI BEM SUCEDIDA, CASO CONTRÁRIO É ENVIADO UMA MENSAGEM DE ERRO NO MONITOR SERIAL
  if (!modbusTCPServer.begin()) {
    Serial.println("Falha ao Iniciar o servidor Modbus");
    if (1);
  }

  //INICIA A QUANTIDADE DE COILS EM 0 ATÉ 20
  modbusTCPServer.configureCoils(0, 20);

  //INICIA A QUANTIDADE DE HOLDING REGISTERS EM 0 ATÉ 8
  modbusTCPServer.configureHoldingRegisters(0, 9);

//Aguarda 1,5s
delay(5000); 

OptaController.begin();

 // Configurar o canal DAC para saída de tensão
  for (int device = 0; device < OptaController.getExpansionNum(); device++) {
    for (int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
      // Configure os canais ímpares como DAC de tensão
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         device,
                                         ch,
                                         OA_VOLTAGE_DAC,
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);
    }
  }
  
pinMode(D0, OUTPUT);        //SAÍDA 1 DO OPTA
pinMode(D1, OUTPUT);        //SAÍDA 2 DO OPTA
pinMode(D2, OUTPUT);        //SAÍDA 3 DO OPTA
pinMode(D3, OUTPUT);        //SAÍDA 4 DO OPTA
pinMode(A0, INPUT);         //ENTRADA I1 DO OPTA
pinMode(A1, INPUT);         //ENTRADA I2 DO OPTA
pinMode(A2, INPUT);         //ENTRADA I3 DO OPTA
pinMode(A3, INPUT);         //ENTRADA I4 DO OPTA
pinMode(A4, INPUT);         //ENTRADA I5 DO OPTA
pinMode(A5, INPUT);         //ENTRADA I6 DO OPTA
pinMode(A6, INPUT);         //ENTRADA I7 DO OPTA
pinMode(A7, INPUT);         //ENTRADA I8 DO OPTA
pinMode(LED_D0, OUTPUT);    //LED 1 DO OPTA
pinMode(LED_D1, OUTPUT);    //LED 2 DO OPTA
pinMode(LED_D2, OUTPUT);    //LED 3 DO OPTA
pinMode(LED_D3, OUTPUT);    //LED 4 DO OPTA
pinMode(LED_RESET, OUTPUT); //LED VERDE DE RESET OPTA
pinMode(LEDR, OUTPUT);      //LED VERMELHO DO OPTA 
pinMode(LED_USER, OUTPUT);  //LED AZUL DISPONÍVEL SOMENTE NA VERSÃO ADVANCED DO OPTA

}



/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                 INICIANDO COM OS CÓDIGOS DENTRO DO VOID LOOP                                                 */
/*                                                                                                                                              */
/************************************************************************************************************************************************/

void loop() {

  //ARMAZENA OS CLIENTES NA VARIÁVEL ETHERNETCLIENT
  EthernetClient client = ethServer.available();
  
  //VERIFICA SE UM CLIENTE SE CONECTOU
  if (client) {
    
    Serial.println("NOVO CLIENTE");

    //ACEITA A CONEXÃO DO CLIENTE NO SERVIDOR
    modbusTCPServer.accept(client);

    //ENQUANTO O CLIENTE ESTIVER CONECTADO, AS REQUISIÇÕES SÃO ENVIADAS E RECEBIDAS
    while (client.connected()) {
      modbusTCPServer.poll();
      

      //ATUALIZA A FUNÇÃO PARA LEITURA DAS COILS
      read_coils();

      //ATUALIZA A FUNÇÃO PARA VERIFICAÇÃO DAS ENTRADAS
      check_button();

      //ATUALIZA A FUNÇÃO PARA VERIFICAÇÃO DAS EXPANSÕES
      optaAnalogTask();
    }
    
    

    //CASO O CLIENTE SE DESCONECTE IRÁ SER IMPRESSO A MENSAGEM NO MONITOR SERIAL
    Serial.println("Cliente Desconectado");
  }

desconectado();
  
}


/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                          INICIA A FUNÇÃO READ_COILS                                                          */
/*                                                                                                                                              */
/************************************************************************************************************************************************/

void read_coils() {

  //ESSAS VARIÁVEIS DE BOTÕES SERVEM PARA TESTES E NÃO ESTÃO SENDO UTILIZADAS
  liga1 = modbusTCPServer.coilRead(1);
  //VARIÁVEL DETECTADO ESTÁ CONFIGURADA PARA SER A COIL 2
  detectado = modbusTCPServer.coilRead(2);
  //VARIÁVEL RETORNA ESTÁ CONFIGURADA PARA SER A COIL 3
  retorna = modbusTCPServer.coilRead(3);

}


/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                          INICIA A FUNÇÃO CHECK_BUTTON                                                        */
/*                                                                                                                                              */
/************************************************************************************************************************************************/


void optaAnalogTask(){
 
  static long int start = millis();
  if (millis() - start > PERIODIC_UPDATE_TIME){
    start = millis();
      //ENTRADA I1 OPTA SECUNDÁRIO 1
      uint16_t sensorValue1 = analogRead(A0);
      uint16_t dac_value1 = map(sensorValue1, 0, 1023, 0, 7490);
      AnalogExpansion exp = OptaController.getExpansion(0);
      Serial.println("Setting DAC value " + String(dac_value1) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I1
      exp.setDac(OA_CH_0, dac_value1);
      //ENVIO LEITURA I1 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(1, dac_value1);  // Armazena no Holding Register 1



      //ENTRADA I2 OPTA SECUNDÁRIO 1
      uint16_t sensorValue2 = analogRead(A1);
      uint16_t dac_value2 = map(sensorValue2, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value2) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I2
      exp.setDac(OA_CH_1, dac_value2);
      //ENVIO LEITURA I2 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(2, dac_value2);  // Armazena no Holding Register 1



      //ENTRADA I3 OPTA SECUNDÁRIO 1
      uint16_t sensorValue3 = analogRead(A2);
      uint16_t dac_value3 = map(sensorValue3, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value3) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I3
      exp.setDac(OA_CH_2, dac_value3);
      //ENVIO LEITURA I3 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(3, dac_value3);  // Armazena no Holding Register 1
      


      //ENTRADA I4 OPTA SECUNDÁRIO 1
      uint16_t sensorValue4 = analogRead(A3);
      uint16_t dac_value4 = map(sensorValue4, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value4) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I4
      exp.setDac(OA_CH_3, dac_value4);
      //ENVIO LEITURA I4 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(4, dac_value4);  // Armazena no Holding Register 1



      //ENTRADA I5 OPTA SECUNDÁRIO 1
      uint16_t sensorValue5 = analogRead(A4);
      uint16_t dac_value5 = map(sensorValue5, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value5) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I5
      exp.setDac(OA_CH_4, dac_value5);
      //ENVIO LEITURA I5 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(5, dac_value5);  // Armazena no Holding Register 1



      //ENTRADA I6 OPTA SECUNDÁRIO 1
      uint16_t sensorValue6 = analogRead(A5);
      uint16_t dac_value6 = map(sensorValue6, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value6) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I6
      exp.setDac(OA_CH_5, dac_value6);
      //ENVIO LEITURA I6 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(6, dac_value6);  // Armazena no Holding Register 1



      //ENTRADA I7 OPTA SECUNDÁRIO 1
      uint16_t sensorValue7 = analogRead(A6);
      uint16_t dac_value7 = map(sensorValue7, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value7) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I7
      exp.setDac(OA_CH_6, dac_value7);
      //ENVIO LEITURA I7 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(7, dac_value7);  // Armazena no Holding Register 1



      //ENTRADA I8 OPTA SECUNDÁRIO 1
      uint16_t sensorValue8 = analogRead(A7);
      uint16_t dac_value8 = map(sensorValue8, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value8) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I8
      exp.setDac(OA_CH_7, dac_value8);
      //ENVIO LEITURA I8 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(8, dac_value8);  // Armazena no Holding Register 1
      Serial.println("leitura dac8");
      Serial.println(dac_value8);

      OptaController.update();
  }
  
}

void desconectado() {
      
      modbusTCPServer.poll();
      
      //ATUALIZA A FUNÇÃO PARA LEITURA DAS COILS
      read_coils();

      //ATUALIZA A FUNÇÃO PARA VERIFICAÇÃO DAS ENTRADAS
      check_button();

      //ATUALIZA A FUNÇÃO PARA VERIFICAÇÃO DAS EXPANSÕES
      optaAnalogTask();
}


void check_button() {



//VARIÁVEL PARA CONTROLE VIA BOTÃO, NÃO ESTÁ SENDO UTILIZADO. SOMENTE PARA TESTES
estadoButton = digitalRead(A0);

if (estadoButton == HIGH){
  modbusTCPServer.coilWrite(0, 1);
  Serial.println("Output ON");
}else{
  modbusTCPServer.coilWrite(0, 0);
  Serial.println("Output OFF");
}

OptaController.update();
optaAnalogTask();
}

/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                CONFIGURAÇÃO DE MAC, IP E PORTA ETHERNET                                                      */
/*                                                             OPTA SECUNDÁRIO 2                                                                */
/************************************************************************************************************************************************/


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 68, 106);
EthernetServer ethServer(502);
ModbusTCPServer modbusTCPServer;


/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                       VARIÁVEIS (MEMÓRIAS) PARA ARMAZENAR O ESTADO DO BOTÃO DA ENTRADA I1                                    */
/*                                                                                                                                              */
/************************************************************************************************************************************************/


bool estadoButton = 0;


/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                          VARIÁVEIS (MEMÓRIAS) PARA INICIAR DETECTADO E RETORNA EM 0                                          */
/*                                                                   NÃO UTILIZADAS                                                             */
/************************************************************************************************************************************************/

int liga1 = 0;
int detectado = 0;
int retorna = 0;



/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                 INICIANDO COM OS CÓDIGOS DENTRO DO VOID SETUP                                                */
/*                                                                                                                                              */
/************************************************************************************************************************************************/



void setup() {
  
  //IMPRIME NO MONITOR SERIAL QUE O SERVIDOR ESTÁ SENDO INICIADO
  Serial.println("Iniciando Servidor TCP/IP");
  //INICIALIZA O SERVIDOR COM AS CONFIGURAÇÕES DE MAC E IP
  Ethernet.begin(mac, ip);

  //VERIFICA SE O OPTA TEM O HARDWARE ETHERNET/MODBUS
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Não foi encontrado hardware de Ethernet ou Modbus");
    if (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }

  //VERIFICA SE O CABO ETHERNET ESTÁ CONECTADO NO PRODUTO
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Cabo Ethernet não está conectado");
  }

  //INICIA O SERVIDOR MODBUS
  ethServer.begin();

  //VERIFICA SE A INICIALIZAÇÃO DO SERVIDOR FOI BEM SUCEDIDA, CASO CONTRÁRIO É ENVIADO UMA MENSAGEM DE ERRO NO MONITOR SERIAL
  if (!modbusTCPServer.begin()) {
    Serial.println("Falha ao Iniciar o servidor Modbus");
    if (1);
  }

  //INICIA A QUANTIDADE DE COILS EM 0 ATÉ 20
  modbusTCPServer.configureCoils(0, 20);

  //INICIA A QUANTIDADE DE HOLDING REGISTERS EM 0 ATÉ 8
  modbusTCPServer.configureHoldingRegisters(0, 9);

//Aguarda 1,5s
delay(5000); 

OptaController.begin();

 // Configurar o canal DAC para saída de tensão
  for (int device = 0; device < OptaController.getExpansionNum(); device++) {
    for (int ch = 0; ch < OA_AN_CHANNELS_NUM; ch++) {
      // Configure os canais ímpares como DAC de tensão
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         device,
                                         ch,
                                         OA_VOLTAGE_DAC,
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);
    }
  }
  
pinMode(D0, OUTPUT);        //SAÍDA 1 DO OPTA
pinMode(D1, OUTPUT);        //SAÍDA 2 DO OPTA
pinMode(D2, OUTPUT);        //SAÍDA 3 DO OPTA
pinMode(D3, OUTPUT);        //SAÍDA 4 DO OPTA
pinMode(A0, INPUT);         //ENTRADA I1 DO OPTA
pinMode(A1, INPUT);         //ENTRADA I2 DO OPTA
pinMode(A2, INPUT);         //ENTRADA I3 DO OPTA
pinMode(A3, INPUT);         //ENTRADA I4 DO OPTA
pinMode(A4, INPUT);         //ENTRADA I5 DO OPTA
pinMode(A5, INPUT);         //ENTRADA I6 DO OPTA
pinMode(A6, INPUT);         //ENTRADA I7 DO OPTA
pinMode(A7, INPUT);         //ENTRADA I8 DO OPTA
pinMode(LED_D0, OUTPUT);    //LED 1 DO OPTA
pinMode(LED_D1, OUTPUT);    //LED 2 DO OPTA
pinMode(LED_D2, OUTPUT);    //LED 3 DO OPTA
pinMode(LED_D3, OUTPUT);    //LED 4 DO OPTA
pinMode(LED_RESET, OUTPUT); //LED VERDE DE RESET OPTA
pinMode(LEDR, OUTPUT);      //LED VERMELHO DO OPTA 
pinMode(LED_USER, OUTPUT);  //LED AZUL DISPONÍVEL SOMENTE NA VERSÃO ADVANCED DO OPTA

}



/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                 INICIANDO COM OS CÓDIGOS DENTRO DO VOID LOOP                                                 */
/*                                                                                                                                              */
/************************************************************************************************************************************************/

void loop() {

  //ARMAZENA OS CLIENTES NA VARIÁVEL ETHERNETCLIENT
  EthernetClient client = ethServer.available();
  
  //VERIFICA SE UM CLIENTE SE CONECTOU
  if (client) {
    
    Serial.println("NOVO CLIENTE");

    //ACEITA A CONEXÃO DO CLIENTE NO SERVIDOR
    modbusTCPServer.accept(client);

    //ENQUANTO O CLIENTE ESTIVER CONECTADO, AS REQUISIÇÕES SÃO ENVIADAS E RECEBIDAS
    while (client.connected()) {
      modbusTCPServer.poll();
      

      //ATUALIZA A FUNÇÃO PARA LEITURA DAS COILS
      read_coils();

      //ATUALIZA A FUNÇÃO PARA VERIFICAÇÃO DAS ENTRADAS
      check_button();

      //ATUALIZA A FUNÇÃO PARA VERIFICAÇÃO DAS EXPANSÕES
      optaAnalogTask();
    }
    
    

    //CASO O CLIENTE SE DESCONECTE IRÁ SER IMPRESSO A MENSAGEM NO MONITOR SERIAL
    Serial.println("Cliente Desconectado");
  }

desconectado();
  
}


/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                          INICIA A FUNÇÃO READ_COILS                                                          */
/*                                                                                                                                              */
/************************************************************************************************************************************************/

void read_coils() {

  //ESSAS VARIÁVEIS DE BOTÕES SERVEM PARA TESTES E NÃO ESTÃO SENDO UTILIZADAS
  liga1 = modbusTCPServer.coilRead(1);
  //VARIÁVEL DETECTADO ESTÁ CONFIGURADA PARA SER A COIL 2
  detectado = modbusTCPServer.coilRead(2);
  //VARIÁVEL RETORNA ESTÁ CONFIGURADA PARA SER A COIL 3
  retorna = modbusTCPServer.coilRead(3);

}


/************************************************************************************************************************************************/
/*                                                                                                                                              */
/*                                                          INICIA A FUNÇÃO CHECK_BUTTON                                                        */
/*                                                                                                                                              */
/************************************************************************************************************************************************/


void optaAnalogTask(){
 
  static long int start = millis();
  if (millis() - start > PERIODIC_UPDATE_TIME){
    start = millis();
      //ENTRADA I1 OPTA SECUNDÁRIO 1
      uint16_t sensorValue1 = analogRead(A0);
      uint16_t dac_value1 = map(sensorValue1, 0, 1023, 0, 7490);
      AnalogExpansion exp = OptaController.getExpansion(0);
      Serial.println("Setting DAC value " + String(dac_value1) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I1
      exp.setDac(OA_CH_0, dac_value1);
      //ENVIO LEITURA I1 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(1, dac_value1);  // Armazena no Holding Register 1



      //ENTRADA I2 OPTA SECUNDÁRIO 1
      uint16_t sensorValue2 = analogRead(A1);
      uint16_t dac_value2 = map(sensorValue2, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value2) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I2
      exp.setDac(OA_CH_1, dac_value2);
      //ENVIO LEITURA I2 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(2, dac_value2);  // Armazena no Holding Register 1



      //ENTRADA I3 OPTA SECUNDÁRIO 1
      uint16_t sensorValue3 = analogRead(A2);
      uint16_t dac_value3 = map(sensorValue3, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value3) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I3
      exp.setDac(OA_CH_2, dac_value3);
      //ENVIO LEITURA I3 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(3, dac_value3);  // Armazena no Holding Register 1
      


      //ENTRADA I4 OPTA SECUNDÁRIO 1
      uint16_t sensorValue4 = analogRead(A3);
      uint16_t dac_value4 = map(sensorValue4, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value4) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I4
      exp.setDac(OA_CH_3, dac_value4);
      //ENVIO LEITURA I4 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(4, dac_value4);  // Armazena no Holding Register 1



      //ENTRADA I5 OPTA SECUNDÁRIO 1
      uint16_t sensorValue5 = analogRead(A4);
      uint16_t dac_value5 = map(sensorValue5, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value5) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I5
      exp.setDac(OA_CH_4, dac_value5);
      //ENVIO LEITURA I5 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(5, dac_value5);  // Armazena no Holding Register 1



      //ENTRADA I6 OPTA SECUNDÁRIO 1
      uint16_t sensorValue6 = analogRead(A5);
      uint16_t dac_value6 = map(sensorValue6, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value6) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I6
      exp.setDac(OA_CH_5, dac_value6);
      //ENVIO LEITURA I6 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(6, dac_value6);  // Armazena no Holding Register 1



      //ENTRADA I7 OPTA SECUNDÁRIO 1
      uint16_t sensorValue7 = analogRead(A6);
      uint16_t dac_value7 = map(sensorValue7, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value7) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I7
      exp.setDac(OA_CH_6, dac_value7);
      //ENVIO LEITURA I7 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(7, dac_value7);  // Armazena no Holding Register 1



      //ENTRADA I8 OPTA SECUNDÁRIO 1
      uint16_t sensorValue8 = analogRead(A7);
      uint16_t dac_value8 = map(sensorValue8, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value8) + " on expansion n. " + String(exp.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I8
      exp.setDac(OA_CH_7, dac_value8);
      //ENVIO LEITURA I8 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(8, dac_value8);  // Armazena no Holding Register 1
      Serial.println("leitura dac8");
      Serial.println(dac_value8);

      OptaController.update();
  }
  
}


void desconectado() {
      
      modbusTCPServer.poll();
      
      //ATUALIZA A FUNÇÃO PARA LEITURA DAS COILS
      read_coils();

      //ATUALIZA A FUNÇÃO PARA VERIFICAÇÃO DAS ENTRADAS
      check_button();

      //ATUALIZA A FUNÇÃO PARA VERIFICAÇÃO DAS EXPANSÕES
      optaAnalogTask();
}


void check_button() {



//VARIÁVEL PARA CONTROLE VIA BOTÃO, NÃO ESTÁ SENDO UTILIZADO. SOMENTE PARA TESTES
estadoButton = digitalRead(A0);

if (estadoButton == HIGH){
  modbusTCPServer.coilWrite(0, 1);
  Serial.println("Output ON");
}else{
  modbusTCPServer.coilWrite(0, 0);
  Serial.println("Output OFF");
}

OptaController.update();
optaAnalogTask();
}