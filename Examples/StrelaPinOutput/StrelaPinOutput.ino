// Подключим библиотеку для работы с I2C-расширителем портов
#include <Wire.h> 
// Подключим библиотеку Strela
#include <Strela.h> 
void setup() {

  uPinMode(P9, OUTPUT);  // это Arduino GPIO. Мы настроим его на выход
  uPinMode(P10, OUTPUT); // а это — I²C-GPIO. Его мы тоже настроим на выход
}

void loop() {
  uDigitalWrite(P9, HIGH);  // подадим на P9 высокий уровень
  uDigitalWrite(P10, HIGH); // подадим на P10 высокий уровень

  delay(1000);

  uDigitalWrite(P9, LOW);   // подадим на P9 низкий уровень
  uDigitalWrite(P10, LOW);  // подадим на P10 низкий уровень

  delay(1000);
}