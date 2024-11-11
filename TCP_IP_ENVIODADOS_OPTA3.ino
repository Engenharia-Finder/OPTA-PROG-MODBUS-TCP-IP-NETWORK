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
/*                                                            OPTA SECUNDÁRIO 3                                                                 */
/************************************************************************************************************************************************/


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 68, 108);
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

  //INICIA A QUANTIDADE DE HOLDING REGISTERS EM 0 ATÉ 20
  modbusTCPServer.configureHoldingRegisters(0, 20);
//Aguarda 1,5s
delay(5000); 

OptaController.begin();


      //CONFIGURAÇÃO NA EXP0 COMO DAC (SOMENTE OA_CH0 - I1)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         0,
                                         OA_CH_0,
                                         OA_VOLTAGE_DAC,
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);

      //CONFIGURAÇÃO NA EXP0 COMO DAC (SOMENTE OA_CH1 - I2)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         0,
                                         OA_CH_1,
                                         OA_VOLTAGE_DAC,
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);

      //CONFIGURAÇÃO NA EXP0 COMO DAC (SOMENTE OA_CH2 - I3)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         0,
                                         OA_CH_2,
                                         OA_VOLTAGE_DAC,
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);

      //CONFIGURAÇÃO NA EXP0 COMO DAC (SOMENTE OA_CH3 - I4)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         0,
                                         OA_CH_3,
                                         OA_VOLTAGE_DAC,
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);

      //CONFIGURAÇÃO NA EXP0 COMO DAC (SOMENTE OA_CH4 - O1)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         0,
                                         OA_CH_4,
                                         OA_VOLTAGE_DAC,
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);

      //CONFIGURAÇÃO NA EXP0 COMO DAC (SOMENTE OA_CH5 - I5)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         0,
                                         OA_CH_5,
                                         OA_VOLTAGE_DAC,
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);

      //CONFIGURAÇÃO NA EXP0 COMO DAC (SOMENTE OA_CH6 - I6)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         0,
                                         OA_CH_6,
                                         OA_VOLTAGE_DAC,
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);

      //CONFIGURAÇÃO NA EXP0 COMO DAC (SOMENTE OA_CH7 - I7)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         0,
                                         OA_CH_7,
                                         OA_VOLTAGE_DAC,
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);

/* -------------------------------------------------------------------------- */

      //CONFIGURAÇÃO NA EXP1 COMO LEITURA (SOMENTE OA_CH0 - I1) 
      AnalogExpansion::beginChannelAsAdc(OptaController, 1, // ENDEREÇO DA EXPANSÃO
                                                        OA_CH_0, // CANAL DE LEITURA
                                                        OA_VOLTAGE_ADC, // TIPO LEITURA (ADC)
                                                        true, // habilitar pull down
                                                        false, // disable rejection
                                                        false, // disable diagnostic
                                                        0); // disable averaging

      //CONFIGURAÇÃO NA EXP1 COMO LEITURA (SOMENTE OA_CH1 - I2)
      AnalogExpansion::beginChannelAsAdc(OptaController, 1, // ENDEREÇO ​​DA EXPANSÃO
                                                        OA_CH_1, // CANAL DE LEITURA
                                                        OA_VOLTAGE_ADC, // TIPO LEITURA (ADC)
                                                        true, // enable pull down
                                                        false, // disable rejection
                                                        false, // disable diagnostic
                                                        0); // disable averaging
                                                        

      //CONFIGURAÇÃO NA EXP1 COMO LEITURA (SOMENTE OA_CH2 - I3)
      AnalogExpansion::beginChannelAsAdc(OptaController, 1, // ENDEREÇO ​​DA EXPANSÃO
                                                        OA_CH_2, // CANAL DE LEITURA
                                                        OA_VOLTAGE_ADC, // TIPO LEITURA (ADC)
                                                        true, // enable pull down
                                                        false, // disable rejection
                                                        false, // disable diagnostic
                                                        0); // disable averaging


      //CONFIGURAÇÃO NA EXP1 COMO LEITURA (SOMENTE OA_CH3 - I4)
      AnalogExpansion::beginChannelAsAdc(OptaController, 1, // ENDEREÇO ​​DA EXPANSÃO
                                                        OA_CH_3, // CANAL DE LEITURA
                                                        OA_VOLTAGE_ADC, // TIPO LEITURA (ADC)
                                                        true, // enable pull down
                                                        false, // disable rejection
                                                        false, // disable diagnostic
                                                        0); // disable averaging

      //CONFIGURAÇÃO NA EXP1 COMO LEITURA (SOMENTE OA_CH4 - O1)
      AnalogExpansion::beginChannelAsAdc(OptaController, 1, // ENDEREÇO ​​DA EXPANSÃO
                                                        OA_CH_4, // CANAL DE LEITURA
                                                        OA_VOLTAGE_ADC, // TIPO LEITURA (ADC)
                                                        true, // enable pull down
                                                        false, // disable rejection
                                                        false, // disable diagnostic
                                                        0); // disable averaging 

      //CONFIGURAÇÃO NA EXP1 COMO LEITURA (SOMENTE OA_CH5 - I5)
      AnalogExpansion::beginChannelAsAdc(OptaController, 1, // ENDEREÇO ​​DA EXPANSÃO
                                                        OA_CH_5, // CANAL DE LEITURA
                                                        OA_VOLTAGE_ADC, // TIPO LEITURA (ADC)
                                                        true, // enable pull down
                                                        false, // disable rejection
                                                        false, // disable diagnostic
                                                        0); // disable averaging

/* -------------------------------------------------------------------------- */

      //CONFIGURAÇÃO NA EXP2 COMO DAC (SOMENTE OA_CH0 - I1)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         2, //ENDEREÇO DA EXPANSÃO
                                         OA_CH_0, // CANAL DE SAÍDA DAC
                                         OA_VOLTAGE_DAC, // TIPO SAÍDA (DAC)
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);

      //CONFIGURAÇÃO NA EXP2 COMO DAC (SOMENTE OA_CH1 - I2)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         2, //ENDEREÇO DA EXPANSÃO
                                         OA_CH_1, // CANAL DE SAÍDA DAC
                                         OA_VOLTAGE_DAC, // TIPO SAÍDA (DAC)
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);


      //CONFIGURAÇÃO NA EXP2 COMO DAC (SOMENTE OA_CH2 - I3)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         2, //ENDEREÇO DA EXPANSÃO
                                         OA_CH_2, // CANAL DE SAÍDA DAC
                                         OA_VOLTAGE_DAC, // TIPO SAÍDA (DAC)
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);


      //CONFIGURAÇÃO NA EXP2 COMO DAC (SOMENTE OA_CH3 - I4)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         2, //ENDEREÇO DA EXPANSÃO
                                         OA_CH_3, // CANAL DE SAÍDA DAC
                                         OA_VOLTAGE_DAC, // TIPO SAÍDA (DAC)
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);


      //CONFIGURAÇÃO NA EXP2 COMO DAC (SOMENTE OA_CH4 - O1)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         2, //ENDEREÇO DA EXPANSÃO
                                         OA_CH_4, // CANAL DE SAÍDA DAC
                                         OA_VOLTAGE_DAC, // TIPO SAÍDA (DAC)
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);


      //CONFIGURAÇÃO NA EXP2 COMO DAC (SOMENTE OA_CH5 - I5)
      AnalogExpansion::beginChannelAsDac(OptaController,
                                         2, //ENDEREÇO DA EXPANSÃO
                                         OA_CH_5, // CANAL DE SAÍDA DAC
                                         OA_VOLTAGE_DAC, // TIPO SAÍDA (DAC)
                                         true,
                                         false,
                                         OA_SLEW_RATE_0);

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
delay(5000);
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

      AnalogExpansion exp0 = OptaController.getExpansion(0); //CONFIGURA EXPANSÃO 0 COMO EXP0
      AnalogExpansion exp1 = OptaController.getExpansion(1); //CONFIGURA EXPANSÃO 1 COMO EXP1
      AnalogExpansion exp2 = OptaController.getExpansion(2); //CONFIGURA EXPANSÃO 2 COMO EXP2

      //ENTRADA I1 OPTA SECUNDÁRIO 3
      uint16_t sensorValue1 = analogRead(A0);
      uint16_t dac_value1 = map(sensorValue1, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value1) + " on expansion n. " + String(exp0.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I1
      exp0.setDac(OA_CH_0, dac_value1);
      //ENVIO LEITURA I1 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(1, dac_value1);  // Armazena no Holding Register 1
      OptaController.update();


      //ENTRADA I2 OPTA SECUNDÁRIO 3
      uint16_t sensorValue2 = analogRead(A1);
      uint16_t dac_value2 = map(sensorValue2, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value2) + " on expansion n. " + String(exp0.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I2
      exp0.setDac(OA_CH_1, dac_value2);
      //ENVIO LEITURA I2 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(2, dac_value2);  // Armazena no Holding Register 1
      OptaController.update();


      //ENTRADA I3 OPTA SECUNDÁRIO 3
      uint16_t sensorValue3 = analogRead(A2);
      uint16_t dac_value3 = map(sensorValue3, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value3) + " on expansion n. " + String(exp0.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I3
      exp0.setDac(OA_CH_2, dac_value3);
      //ENVIO LEITURA I3 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(3, dac_value3);  // Armazena no Holding Register 1
      OptaController.update();


      //ENTRADA I4 OPTA SECUNDÁRIO 3
      uint16_t sensorValue4 = analogRead(A3);
      uint16_t dac_value4 = map(sensorValue4, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value4) + " on expansion n. " + String(exp0.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I4
      exp0.setDac(OA_CH_3, dac_value4);
      //ENVIO LEITURA I4 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(4, dac_value4);  // Armazena no Holding Register 1
      OptaController.update();


      //ENTRADA I5 OPTA SECUNDÁRIO 3
      uint16_t sensorValue5 = analogRead(A4);
      uint16_t dac_value5 = map(sensorValue5, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value5) + " on expansion n. " + String(exp0.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I5
      exp0.setDac(OA_CH_4, dac_value5);
      //ENVIO LEITURA I5 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(5, dac_value5);  // Armazena no Holding Register 1
      OptaController.update();


      //ENTRADA I6 OPTA SECUNDÁRIO 3
      uint16_t sensorValue6 = analogRead(A5);
      uint16_t dac_value6 = map(sensorValue6, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value6) + " on expansion n. " + String(exp0.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I6
      exp0.setDac(OA_CH_5, dac_value6);
      //ENVIO LEITURA I6 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(6, dac_value6);  // Armazena no Holding Register 1
      OptaController.update();


      //ENTRADA I7 OPTA SECUNDÁRIO 3
      uint16_t sensorValue7 = analogRead(A6);
      uint16_t dac_value7 = map(sensorValue7, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value7) + " on expansion n. " + String(exp0.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I7
      exp0.setDac(OA_CH_6, dac_value7);
      //ENVIO LEITURA I7 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(7, dac_value7);  // Armazena no Holding Register 1
      OptaController.update();


      //ENTRADA I8 OPTA SECUNDÁRIO 3
      uint16_t sensorValue8 = analogRead(A7);
      uint16_t dac_value8 = map(sensorValue8, 0, 1023, 0, 7490);
      Serial.println("Setting DAC value " + String(dac_value8) + " on expansion n. " + String(exp0.getIndex()));
      //ENVIA O MESMO SINAL DE ENTRADA PARA A SAÍDA I8
      exp0.setDac(OA_CH_7, dac_value8);
      //ENVIO LEITURA I8 PARA O OPTA PRINCIPAL
      modbusTCPServer.holdingRegisterWrite(8, dac_value8);  // Armazena no Holding Register 1
      Serial.println("leitura dac8");
      Serial.println(dac_value8);
      OptaController.update();

/* -------------------------------------------------------------------------- */     

   if (exp1 && exp2) {


      // I1 -> O1
      uint16_t dac_value9 = map(exp1.analogRead(OA_CH_0), 0, 65535, 0, 7490);
      exp2.setDac(OA_CH_0, dac_value9);
      Serial.println("Espelhando valor " + String(dac_value9) + " para OA_CH_0");
      modbusTCPServer.holdingRegisterWrite(11, dac_value9);  // Armazena no Holding Register 1

      // I2 -> O2
      uint16_t dac_value10 = map(exp1.analogRead(OA_CH_1), 0, 65535, 0, 7490);
      exp2.setDac(OA_CH_1, dac_value10);
      Serial.println("Espelhando valor " + String(dac_value10) + " para OA_CH_1");
      modbusTCPServer.holdingRegisterWrite(12, dac_value10);  // Armazena no Holding Register 1

      // I3 -> O3
      uint16_t dac_value11 = map(exp1.analogRead(OA_CH_2), 0, 65535, 0, 7490);
      exp2.setDac(OA_CH_2, dac_value11);
      Serial.println("Espelhando valor " + String(dac_value11) + " para OA_CH_2");
      modbusTCPServer.holdingRegisterWrite(13, dac_value11);  // Armazena no Holding Register 1

      // I4 -> O4
      uint16_t dac_value12 = map(exp1.analogRead(OA_CH_3), 0, 65535, 0, 7490);
      exp2.setDac(OA_CH_3, dac_value12);
      Serial.println("Espelhando valor " + String(dac_value12) + " para OA_CH_3");
      modbusTCPServer.holdingRegisterWrite(14, dac_value12);  // Armazena no Holding Register 1

      // I5 -> O5
      uint16_t dac_value13 = map(exp1.analogRead(OA_CH_4), 0, 65535, 0, 7490);
      exp2.setDac(OA_CH_4, dac_value13);
      Serial.println("Espelhando valor " + String(dac_value13) + " para OA_CH_4");
      modbusTCPServer.holdingRegisterWrite(15, dac_value13);  // Armazena no Holding Register 1

      // I6 -> O6
      uint16_t dac_value14 = map(exp1.analogRead(OA_CH_5), 0, 65535, 0, 7490);
      exp2.setDac(OA_CH_5, dac_value14);
      Serial.println("Espelhando valor " + String(dac_value14) + " para OA_CH_5");
      modbusTCPServer.holdingRegisterWrite(16, dac_value14);  // Armazena no Holding Register 1

      OptaController.update();
    }

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