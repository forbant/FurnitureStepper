#include <Arduino.h>

#define DEBUG_MSG // ЗАКОМЕНТУЙ ЦЕЙ РЯДОК ЯКЩО ВСЕ ПРАЦЮЄ

#define STEP_PIN 1      // ПІН STEP
#define DIR_PIN 2       // ПІН DIR
#define ENABLE_PIN 3    // ПІН EN
#define STOPPER_PIN_1 4 //  КІНЦЕВИК 1
#define STOPPER_PIN_2 5 //  КІНЦЕВИК 2

#define fwd_button 6 // КНОПКА ВПЕРЕД
#define rev_button 7 // КНОПКА НАЗАД

// Кількість кроків назад коли впирається в кінцевик
#define REVERSE_STEPS 100

/*
    Час між імпульсами у мксек. 1000 мксек = 1 мсек = 0.001 сек. Частота проходження імпульсів 1/0.001 = 1 кГц,
    Не рекомендується встановлювати час менше 100 мсек, оскільки частота буде 10 кГц
*/
#define frequency 640

bool isMotorRunning = false;

bool stopper1 = false;
bool stopper2 = false;
bool forwardButton = false;
bool backwardButton = false;

enum Direction
{
    Forward = 0x1,
    Backward = 0x0
};

enum State
{
    STANDBY,
    FORWARD,
    BACKWARD
};

State state;
Direction direction = Forward;

void move();
void readButtons();
void handleState();
void stopMotor();

void setup()
{
    #ifdef DEBUG_MSG
        Serial.begin(9600);
    #endif
    pinMode(fwd_button, INPUT_PULLUP);
    pinMode(rev_button, INPUT_PULLUP);
    pinMode(STOPPER_PIN_1, INPUT_PULLUP);
    pinMode(STOPPER_PIN_2, INPUT_PULLUP);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);

    digitalWrite(ENABLE_PIN, HIGH);

    state = STANDBY;
}

void loop()
{
    readButtons();
    handleState();
    switch (state)
    {
    case STANDBY:
        stopMotor();
        break;
    case FORWARD:
        if (!stopper1)
        {
            move();
        } else
        {
            stopMotor();
        }
        break;
    case BACKWARD:
        if(!stopper2)
        {
            move();
        } else
        {
            stopMotor();
        }
        break;
    default:
        #ifdef DEBUG_MSG
            Serial.println("Oops");
        #endif
        break;
    }
}

void move()
{
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(frequency);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(frequency);
}

void readButtons()
{
    stopper1 = !digitalRead(STOPPER_PIN_1);
    stopper2 = !digitalRead(STOPPER_PIN_2);
#ifdef DEBUG_MSG
    Serial.print("stopper1:"); Serial.println(stopper1);
    Serial.print("stopper2:"); Serial.println(stopper2);
#endif

    forwardButton = !digitalRead(fwd_button);
    backwardButton = !digitalRead(rev_button);
#ifdef DEBUG_MSG
    Serial.print("forwardButton:"); Serial.println(forwardButton);
    Serial.print("backwardButton:"); Serial.println(backwardButton);
#endif
}

void handleState()
{
    if (!isMotorRunning && !stopper1 && forwardButton)
    {
        state = FORWARD;
        digitalWrite(DIR_PIN, Direction::Forward);
        isMotorRunning = true;
    }
    if (!isMotorRunning && !stopper2 && backwardButton)
    {
        state = BACKWARD;
        digitalWrite(DIR_PIN, Direction::Backward);
        isMotorRunning = true;
    }
}

void stopMotor()
{
    #ifdef DEBUG_MSG
        Serial.println("stopMotor");
    #endif
    state = STANDBY;
    isMotorRunning = false;
    digitalWrite(ENABLE_PIN, HIGH);
}
