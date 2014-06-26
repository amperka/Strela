//Arduino Bluetooth RC Car in Strela
//Original App: 
//https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller

//Скетч для управления роботом при помощи мобильного телефона
//Установите на Android-телефон приложение Arduino Bluetooth RC Car
//https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller
//Подключите Bluetooth Bee на площадку для модуля беспроводной связи
//Прошейте скетч в стрелу, запустите приложение на телефоне и создайте Bluetooth-соединение с модулем
//

#include <Wire.h> // Библиотека для работы с I2C
#include <Strela.h> // Библиотека для работы со Стрелой

int velocity = 0;  //Здесь будет храниться значение скорости

int defaultSpeed = 100; // это число мы будем использовать в логике поворотов

void setup()
{
  Serial1.begin(9600);     //Bluetooth Bee по умолчанию использует эту скорость
  motorConnection(1, 0);   // Я неправильно прикрутил один мотор
  //поэтому, чтобы их не перекручивать
  //можно воспользоваться этой функцией.
  //Направление вращения мотора 1 будет изменено.
}

void loop()
{
  if (Serial1.available() > 0)    //Если появились новые команды
  {
    control();                    //вызываем функцию управления
  }
  //Здесь можно написать ещё много своего кода
}

void control()  // функция управления
{
  char dataIn = Serial1.read();  //Считаем значение пришедшей команды
  
  if (dataIn == 'F')         //Если пришла команда "F"
    drive(velocity, velocity);   //едем вперёд

  else if (dataIn == 'B')    //или если пришла команда "B"
    drive(-velocity, -velocity); //едем назад

  else if (dataIn == 'L')    //или если пришла команда "L"
    drive(-velocity, velocity);  //поворачиваем налево на месте

  else if (dataIn == 'R')    //или если пришла команда "R"
    drive(velocity, -velocity);  //поворачиваем направо на месте

  else if (dataIn == 'I')    //или если пришла команда "I", едем вперёд и направо
    drive(defaultSpeed + velocity, defaultSpeed - velocity);

  else if (dataIn == 'J')    //или если пришла команда "J", едем назад и направо
    drive(-defaultSpeed - velocity, -defaultSpeed + velocity);

  else if (dataIn == 'G')   //или если пришла команда "I", едем вперёд и налево
    drive(defaultSpeed - velocity, defaultSpeed + velocity);

  else if (dataIn == 'H')   //или если пришла команда "H", едем назад и налево
    drive(-defaultSpeed + velocity, -defaultSpeed - velocity);

  else if (dataIn == 'S')   //или если пришла команда "S", стоим
    drive(0, 0);

  else if (dataIn == 'U')   //или если "U", зажигаем "передние фары"
  {
    uDigitalWrite(L2, HIGH);
    uDigitalWrite(L3, HIGH);
  }
  else if (dataIn == 'u')   //или если "u", гасим "передние фары"
  {
    uDigitalWrite(L2, LOW);
    uDigitalWrite(L3, LOW);
  }
  else if (dataIn == 'W')   //или если "W", зажигаем "задние фары"
  {
    uDigitalWrite(L1, HIGH);
    uDigitalWrite(L4, HIGH);
  }
  else if (dataIn == 'w')   ////или если "w", гасим "задние фары"
  {
    uDigitalWrite(L1, LOW);
    uDigitalWrite(L4, LOW);
  }
  
  // если к нам пришло значение от 0 до 9
  else if (((dataIn - '0') >= 0) && ((dataIn - '0') <= 9)) 
    velocity = (dataIn - '0') * 25; //сохраняем новое значение скорости

  else if (dataIn == 'q') //если "q" - полный газ!
    velocity = 255;
}