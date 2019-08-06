// Подключим библиотеку для работы с I2C-расширителем портов
#include <Wire.h> 
// Подключим библиотеку Strela
#include <Strela.h>
void setup() {

  uPinMode(P9, INPUT);  // это Arduino GPIO. Мы настроим его на вход
  uPinMode(P10, INPUT); // а это — I²C-GPIO. Его мы тоже настроим на вход

  Serial.begin(9600); //Подготовим к работе Serial-порт

  while (!Serial) { // Так как Strela рботает на том же контроллере, что и Arduino Leonardo
    ;               // необходимо дождаться подключения виртуального Serial-порта
  }
}

void loop() {
  Serial.print("P9 - ");
  //Считаем значение с P9
  // и отправим его в последовательный порт
  Serial.println(uDigitalRead(P9)); 
  
  Serial.print("P10 - ");
  //Считаем значение с P10
  // и отправим его в последовательный порт
  Serial.println(uDigitalRead(P10)); 
  
  delay(500);
}