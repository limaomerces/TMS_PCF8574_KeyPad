// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Nome do Projeto: Esp32_PCF8574_Teclado_Matricial
//  Objetivo: criar teclado matricial com o pcf8574
//  Autor: Tiago Mercês Silva
//  Email: tiago.merces@hotmail.com
//
//  Histórico de Atualizações:
//       27/04/2022 - 1.0   - Lançamento Inicial
//       13/10/2020 - 1.1   - Melhoria do debounce e otimização
//		 14/10/2020 - 1.1.1   - Bug Fix
//

#include <Wire.h>
#include <TMS_PCF8574_UserSetup.h>

byte PCF8574_Write;                           // Armazena os bits do PCF8574
bool PCF8574_teclado[16];                     // Valores do PCF8574 pós processamento para matrix
byte PCF8574_VALOR_Read[4];                   // 4 valores de cada loop para criar matrix
byte PCF8574_VALOR_ReadOld[4];                // Comparador
bool PCF8574_teclado_ajustado[16];            // Matriz corrigida para interface
int PCF8574_DAR;                              // Referência para ativar o Debounce


void PCF8574_Send() {
  Wire.beginTransmission(i2cAdress);
  Wire.write(PCF8574_Write);
  Wire.endTransmission();
}

void PCF8574_KeyBoardAjust(byte i) {       // Correção dos endereços do teclado
  switch (i) {
    case 0:
      PCF8574_teclado_ajustado[PCF8574_Key0] = PCF8574_teclado[0];
      break;
    case 1:
      PCF8574_teclado_ajustado[PCF8574_Key1] = PCF8574_teclado[1];
      break;
    case 2:
      PCF8574_teclado_ajustado[PCF8574_Key2] = PCF8574_teclado[2];
      break;
    case 3:
      PCF8574_teclado_ajustado[PCF8574_Key3] = PCF8574_teclado[3];
      break;
    case 4:
      PCF8574_teclado_ajustado[PCF8574_Key4] = PCF8574_teclado[4];
      break;
    case 5:
      PCF8574_teclado_ajustado[PCF8574_Key5] = PCF8574_teclado[5];
      break;
    case 6:
      PCF8574_teclado_ajustado[PCF8574_Key6] = PCF8574_teclado[6];
      break;
    case 7:
      PCF8574_teclado_ajustado[PCF8574_Key7] = PCF8574_teclado[7];
      break;
    case 8:
      PCF8574_teclado_ajustado[PCF8574_Key8] = PCF8574_teclado[8];
      break;
    case 9:
      PCF8574_teclado_ajustado[PCF8574_Key9] = PCF8574_teclado[9];
      break;
    case 10:
      PCF8574_teclado_ajustado[PCF8574_Key10] = PCF8574_teclado[10];
      break;
    case 11:
      PCF8574_teclado_ajustado[PCF8574_Key11] = PCF8574_teclado[11];
      break;
    case 12:
      PCF8574_teclado_ajustado[PCF8574_Key12] = PCF8574_teclado[12];
      break;
    case 13:
      PCF8574_teclado_ajustado[PCF8574_Key13] = PCF8574_teclado[13];
      break;
    case 14:
      PCF8574_teclado_ajustado[PCF8574_Key14] = PCF8574_teclado[14];
      break;
    case 15:
      PCF8574_teclado_ajustado[PCF8574_Key15] = PCF8574_teclado[15];
      break;
  }
}


bool PCF8574_teclado_old[16];
bool PCF8574_DebounceKey[16];
unsigned long PCF8574_millis[16];
int DeBounce8574contador;
bool PCF8574_Debounce_LOCK;

void TmsPCF8574_DeBounce() {
  byte DeBounce165contador = 0;
  for (byte i = 0; i < 16; i++) {
    if (PCF8574_teclado_old[i] != PCF8574_teclado[i]) {
      PCF8574_teclado_old[i] = PCF8574_teclado[i];
      PCF8574_DebounceKey[i] = 1;
      PCF8574_millis[i] = millis();
    }
    else if (PCF8574_DebounceKey[i] == 1) {
      if (millis() - PCF8574_millis[i] > PCF8574_DEBOUNCE) {
        PCF8574_teclado_old[i] = PCF8574_teclado[i];
        PCF8574_KeyBoardAjust(i);
        //CI74hc165_Teclado[i] = CI74hc165_Slot[i];
        PCF8574_DebounceKey[i] = 0;
      }
    }
    else {
      DeBounce8574contador++;
      if (DeBounce8574contador == 16) {
        PCF8574_Debounce_LOCK = 1;
      }
    }
  }
}

#ifdef DEBUG_PCF8574
bool PCF8574_teclado_ajustado_Debug[16];
void TmsPCF8574_Debug_KeyBoard() {
  for (byte i = 0; i < 16; i++) {
    if (PCF8574_teclado_ajustado_Debug[i] != PCF8574_teclado_ajustado[i]) {
      PCF8574_teclado_ajustado_Debug[i] = PCF8574_teclado_ajustado[i];
      Serial.print(i);
      Serial.print(" - ");
      Serial.println(PCF8574_teclado_ajustado[i]);
    }
  }
}
#endif

void TmsPCF8574_Main() {
#ifdef DEBUG_PCF8574
  TmsPCF8574_Debug_KeyBoard();
#endif
  for (byte i = 4; i < 8; i++) {
    bitWrite(PCF8574_Write, i, 0);
    PCF8574_Send();
    Wire.requestFrom(0x38, 1);
    if (Wire.available()) {
      PCF8574_VALOR_Read[i - 4] = Wire.read();
      if (PCF8574_VALOR_Read[i - 4] != PCF8574_VALOR_ReadOld[i - 4]) {
        PCF8574_VALOR_ReadOld[i - 4] = PCF8574_VALOR_Read[i - 4];
        for (byte i2 = 0; i2 < 4; i2++) {
          PCF8574_teclado[i2 + (4 * (i - 4))] = !bitRead(PCF8574_VALOR_Read[i - 4], i2);
        }
        PCF8574_Debounce_LOCK = 0;
		bitWrite(PCF8574_Write, i, 1);		// Reseta o ciclo
        PCF8574_Send();						// Reseta o ciclo
        break;
      }
      bitWrite(PCF8574_Write, i, 1);
    }
  }
  if (PCF8574_Debounce_LOCK == 0) TmsPCF8574_DeBounce();
}

void TmsPCF8574_Begin() {
  Wire.begin(PCF8574_SDA, PCF8574_SCL);
  Wire.setClock(PCF8574_Frequency);

  for (byte i = 0; i < 8; i++) {
    bitWrite(PCF8574_Write, i, 1);
  }
  PCF8574_Send();
}