#include <Arduino.h>

#define STEP_PIN 3      // ПІН STEP
#define DIR_PIN 2      // ПІН DIR
#define ENABLE_PIN 13    // ПІН EN
#define STOPPER_PIN_1 4 //  КІНЦЕВИК 1
#define STOPPER_PIN_2 5 //  КІНЦЕВИК 2

#define fwd_button 6 // КНОПКА ВПЕРЕД
#define rev_button 8 // КНОПКА НАЗАД

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
    pinMode(fwd_button, INPUT_PULLUP);
    pinMode(rev_button, INPUT_PULLUP);
    pinMode(STOPPER_PIN_1, INPUT_PULLUP);
    pinMode(STOPPER_PIN_2, INPUT_PULLUP);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);

    digitalWrite(ENABLE_PIN, HIGH);

    state = STANDBY;
    isMotorRunning = false;
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

    forwardButton = !digitalRead(fwd_button);
    backwardButton = !digitalRead(rev_button);
}

void handleState()
{
    if (!isMotorRunning && !stopper1 && forwardButton)
    {
        state = FORWARD;
        digitalWrite(DIR_PIN, Direction::Forward);
        isMotorRunning = true;
        digitalWrite(ENABLE_PIN, LOW);
    }
    if (!isMotorRunning && !stopper2 && backwardButton)
    {
        state = BACKWARD;
        digitalWrite(DIR_PIN, Direction::Backward);
        isMotorRunning = true;
        digitalWrite(ENABLE_PIN, LOW);
    }
}

void stopMotor()
{
    state = STANDBY;
    isMotorRunning = false;
    digitalWrite(ENABLE_PIN, HIGH);
}
