/**
 * @file Levitador_ultrasonico.ino
 * Modificado por: Saulo Aislan
 * @brief Firmware responsável por gerar uma onda de 80kHz no sensor ultrassonico.
 * @version 0.1
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#if defined(__arm__)   // Familia Arduino
#elif defined(__AVR__) // Familia Arduino e outras placas AVR
#else
  #error A Placa selecionada nao e suportada, selecione placas da familia Arduino.
#endif

byte TP = 0b10101010; // Alterna os valores entre 0 e 1, para setar nas portas

/**
 * @brief Interrupcao por comparacao de igualdade no TIMER1
 * Onde inverte o valor da porta a cada ciclo
 */
ISR(TIMER1_COMPA_vect)
{
  // PORTC contem os bits das portas 0 a 7 analogicas ou as portas PC
  PORTC = TP; // Envie o valor de TP para as portas PORTC
  TP = ~TP; // Inverter TP para a proxima execucao
}

void setup()
{
  // Registrador DDRC (Data Direction Register) das portas PC (PC1,PC2,PC3..)
  DDRC = 0b11111111; // Define todas as portas PC (PC1,PC2,PC3..) analogicas como saida (OUTPUT)
  
  noInterrupts(); // Desabilita a interrupcao

  // Configuracao do TIMER1
  TCCR1A = 0; // Registrador TCCR1A configurado para o timer1
  TCCR1B = 0; // Registrador TCCR1B configurado para o timer1
  TCNT1 = 0; // Zera o contador TCNT1 de 16bits
  OCR1A = 200; // Define o registrador de comparacao OCR1A (16MHz / 200 = onda quadrada de 80kHz -> onda completa de 40kHz)
  
  TCCR1B |= (1 << WGM12); // Modo CTC (WGM12 bit para o modo de geracao de onda)
  TCCR1B |= (1 << CS10); // Define prescaler para 1 = sem prescaling (CS10 bit para o clock interno - prescaler)
  // TIMSK1 máscara do registrador de interrupção do temporizador/contador
  TIMSK1 |= (1 << OCIE1A); // Habilita a interrupcao por comparação pela igualdade (OCIE1A bit que ativar interrupção de comparação pela igualdade do temporizador/contador1)

  interrupts(); // Ativa interrupcao 
}

void loop() {}
