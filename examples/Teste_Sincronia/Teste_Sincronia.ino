#include "TMS_PCF8574.h"

void setup() {
  Serial.begin(115200);
  TmsPCF8574_Begin();

}
void loop() {
	TmsPCF8574_Main();
  for (byte i = 0; i < 16; i++) {
    if (PCF8574_teclado_ajustado[i] == 1) {
      Serial.print("Botão = ");
      Serial.println(i);
      PCF8574_teclado_ajustado[i] = 0;
    }
  }
}