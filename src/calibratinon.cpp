// ЦЕЙ КОД ТІЛЬКИ ДЛЯ КАЛІБРОВКИ
// #include "Arduino.h"

// const int dirPin = 2;
// const int stepPin = 3;
// const int enablePin = 13;
// const int stepsPerRevolution = 200;
// int delayTime = 640;

// int buttonFwd = 6;
// int buttonBack = 8;

// void setup()
// {
//     pinMode(stepPin, OUTPUT);
//     pinMode(dirPin, OUTPUT);
//     pinMode(enablePin, OUTPUT);

//     pinMode(buttonFwd, INPUT_PULLUP);
//     pinMode(buttonBack, INPUT_PULLUP);
//     Serial.begin(9600);
// }
// void move()
// {
//     for (int x = 0; x < stepsPerRevolution; x++)
//     {
//         digitalWrite(stepPin, HIGH);
//         delayMicroseconds(delayTime);
//         digitalWrite(stepPin, LOW);
//         delayMicroseconds(delayTime);
//     }
//     Serial.println("END");
// }
// void loop()
// {
//     bool f = !digitalRead(buttonFwd);
//     bool b = !digitalRead(buttonBack);
//     if (f)
//     {
//         digitalWrite(enablePin, LOW);
//         Serial.println("FORWARD");
//         digitalWrite(dirPin, HIGH);
//         move();
//     }
//     if (b)
//     {
//         digitalWrite(enablePin, LOW);
//         Serial.println("BACKWARD");
//         digitalWrite(dirPin, LOW);
//         move();
//     }
//     if (!f && !b)
//     {
//         digitalWrite(enablePin, HIGH);
//     }
// }
