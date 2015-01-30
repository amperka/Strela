// Подключим библиотеку для работы с I2C-расширителем портов
#include <Wire.h>
// Подключим библиотеку Strela
#include <Strela.h>

void setup() {
}

void loop() {

  // Шаговый двигатель управляется при помощи
  // функции stepperMotor(int stepsToMove, int stepDelay);
  // stepsToMove — это количество шагов которые должен
  // сделать шаговый двигатель. 
  // Если это число положительное — мотор крутится в одну сторону,
  // если отрицательное - в другую.
  // stepDelay — это пауза между шагами в миллисекундах.
  // От этой величины зависит скорость вращения мотора.
  // Чем больше пауза, тем медленнее вращается мотор
  // Функция stepperMotor() управляет врщением мотора только в
  // полушаговом режиме. В этом режиме шаговый двигатель
  // развивает наибольшую мощность и точность позиционирования.
  
  // Быстро шагаем 200 раз вперёд
  stepperMotor(200, 5);
  
  // Медленно шагаем 200 раз назад.
  // Мотор вернётся в исходное положение
  stepperMotor(-200, 10);

}