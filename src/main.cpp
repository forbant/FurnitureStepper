#include <Arduino.h>

#define STEP_PIN 2
#define DIR_PIN 3
#define ENABLE_PIN 4
#define STOPPER_PIN 8

#define fwd_button 6
#define rev_button 7

// Кількість кроків назад коли впирається в кінцевик
#define REVERSE_STEPS 100

/*
    Время между импульсами в мксек. 1000 мксек = 1 мсек = 0.001 сек. Частота следования импульсов 1/0.001 = 1 кГц,   
    Не рекомендуется устанавливать время меньше 100 мсек, т.к. частота будет 10 кГц
*/
#define frequency 640

// Скільки мс треба тримати щоб перейшло в режим ручного керування 1000мс = 1с
#define holdThreshold 1000

int timer = 0;
int buttonHold = 0;
bool isMotorRunning = false;

bool stopper = false;
bool forwardButton = false;
bool backwardButton = false;

enum Direction {
    Forward = 0x1,
    Backward = 0x0
};

enum State {
    STANDBY,
    MANUAL_MOVE,
    AUTO_MOVE,
    REVERSE
};

State state;
Direction direction = Forward;

void move();
void readButtons();
void reverse();
void handleState();
void stopMotor();

void setup() {
    Serial.begin(9600);
    pinMode(fwd_button, INPUT_PULLUP);
    pinMode(rev_button, INPUT_PULLUP);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);

    digitalWrite(ENABLE_PIN, HIGH);

    state = digitalRead(STOPPER_PIN) ? STANDBY : REVERSE;
}

void loop() {
    readButtons();
    handleState();
    switch (state) {
    case STANDBY:
        stopMotor();
        break;
    case AUTO_MOVE:
    case MANUAL_MOVE:
        move();
        break;
    case REVERSE:
        reverse();
        state = STANDBY;
        break;
    default:
        Serial.println("Oops");
        break;
    }
}

void move() {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(frequency);
    digitalWrite(STEP_PIN, LOW);
}

void readButtons() {
    stopper = !digitalRead(STOPPER_PIN);
    forwardButton = !digitalRead(fwd_button);
    backwardButton = !digitalRead(rev_button);
}

void handleState() {
    if(stopper) {
        state = REVERSE;
        return;
    }

    if(forwardButton) {
        if(!isMotorRunning) {
            isMotorRunning = true;
            buttonHold = millis();
            digitalWrite(DIR_PIN, Forward);
            state = AUTO_MOVE;
        }
        if(isMotorRunning && millis() - buttonHold > holdThreshold) {
            state = MANUAL_MOVE;
        }
    } else {
        if(state == MANUAL_MOVE) {
            state = STANDBY;
        }
    }
}

void stopMotor() {
    isMotorRunning = false;
    digitalWrite(ENABLE_PIN, HIGH);
}

void reverse() {
    direction = static_cast<Direction>(direction ^ (Forward ^ Backward));
    digitalWrite(DIR_PIN, direction);
    for(size_t i = 0; i < REVERSE_STEPS; ++i) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(frequency);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(frequency);
    }
    direction = static_cast<Direction>(direction ^ (Forward ^ Backward));
}