# TMS_PCF8574_KeyPad

//Instruções:
1. Adicionar "TmsPCF8574_Begin()" no "setup()"
2. Adicionar "TmsPCF8574_Main()" no "loop()"


// Teste 
1. Com o circuito montado no pcf8574, carregue o "Teste_Sincronia.ino" no esp32
2. - Abra o monitor serial, pressione os botões e veja se o resultado está correto.

// Calibrar
- Caso a numeração não esteja compatível com o seus botões, Exemplo: Numero 2 indicando o número 4, etc..

1. Com o circuito montado no pcf8574, carregue o "Ref_Codigo.ino" no esp32
2. - Abra o monitor serial, pressione as teclas e anote o botão pressionado e a referência exibida.
3. - Abra o "TMS_PCF8574_UserSetup.h", altere as informações necessárias
	3.1. - Substitua a numeração dos botões, pelos que foram anotados no monitor serial.

5. - Para referenciar o código no seu projeto, utilize a matriz: PCF8574_teclado_ajustado[16].

6. - Caso tenha dúvidas de como executar o código no seu projeto, veja o exemplo: "KeyPad.ino"
