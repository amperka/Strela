// Подключим библиотеку для работы с I2C-расширителем портов
#include <Wire.h>
// Подключим библиотеку Strela
#include <Strela.h>
void setup() {
}

void loop() {
  for (byte i = S1; i <= S4; ++i) // Будем проверять состояние кнопок по очереди
  {
    byte buttonNumber = i - S1;         //Определим номер текущей кнопки
    bool buttonState = uDigitalRead(i); //Считаем положение кнопки
    
    if (buttonState)          //Если она была нажата...
      tone(BUZZER, 1000, 50); //...пикнем пищалкой с частотой 1000 Гц, 50 мс
 
    //Подадим считанное значение с кнопки на светодиод с тем же номером
   uDigitalWrite(L1 + buttonNumber, buttonState);
  }
}