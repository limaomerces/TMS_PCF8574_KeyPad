//
// Produzido por: Tiago Mercês Silva
// Data: 27/04/2022
// 
//

#include <Wire.h>
#include <TMS_PCF8574_UserSetup.h>

byte PCF8574_Write;                           // Armazena os bits do PCF8574
bool PCF8574_teclado[16];                     // Valores do PCF8574 pós processamento para matrix
byte PCF8574_VALOR_Read[4];                   // 4 valores de cada loop para criar matrix
byte PCF8574_VALOR_ReadOld[4];                // Comparador
bool PCF8574_teclado_ajustado[16];            // Matriz corrigida para interface
unsigned long PCF8574_debounce_millis[16];    // Millis para debounce
bool PCF8574_Debounce_pre[16];                // Inicialzador do debounce
bool PCF8574_DbMillis_Lock[16];               // Evita loop de leitura ao ativar botão//
int PCF8574_DAR;                              // Referência para ativar o Debounce


void PCF8574_Send() {
  Wire.beginTransmission(i2cAdress);
  Wire.write(PCF8574_Write);
  Wire.endTransmission();
}

void PCF8574_KeyBoardAjust(byte i) {		   // Correção dos endereços do teclado
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

void TmsPCF8574_DeBounce() {								
  for (byte i = 0; i < 16; i++) {						
    if (PCF8574_teclado[i] == 1) {
      if (PCF8574_Debounce_pre[i] == 0) {					
        PCF8574_DbMillis_Lock[i] = 0;					
        PCF8574_debounce_millis[i] = millis();
        PCF8574_Debounce_pre[i] = 1;
      }
      if (PCF8574_DbMillis_Lock[i] == 0) {
        if (millis() - PCF8574_debounce_millis[i] >= PCF8574_DEBOUNCE) {
          PCF8574_KeyBoardAjust(i);
          PCF8574_DbMillis_Lock[i] = 1;
        }
      }
    }
    else if (PCF8574_Debounce_pre[i] == 1) {
      PCF8574_Debounce_pre[i] = 0;
	  PCF8574_KeyBoardAjust(i);
    }
  }
}

void TmsPCF8574_Main() { 
  PCF8574_DAR = 0;
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
      }
      else {
        PCF8574_DAR += PCF8574_VALOR_Read[i - 4];
      }
      bitWrite(PCF8574_Write, i, 1);
    }
    if (PCF8574_DAR != 780) {
      TmsPCF8574_DeBounce();
    }
  }
}

void TmsPCF8574_Begin() {
  Wire.begin(PCF8574_SDA, PCF8574_SCL);
  Wire.setClock(PCF8574_Frequency);

  for (byte i = 0; i < 8; i++) {
    bitWrite(PCF8574_Write, i, 1);
  }
  PCF8574_Send();
}