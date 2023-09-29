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
    Час між імпульсами у мксек. 1000 мксек = 1 мсек = 0.001 сек. Частота проходження імпульсів 1/0.001 = 1 кГц,
     Не рекомендується встановлювати час менше 100 мсек, оскільки частота буде 10 кГц
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
    pinMode(STOPPER_PIN, INPUT_PULLUP);
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
    delayMicroseconds(frequency);
}

void readButtons() {
    stopper = !digitalRead(STOPPER_PIN);
    Serial.print("stopper:"); Serial.println(stopper);
    forwardButton = !digitalRead(fwd_button);
    Serial.print("forwardButton:"); Serial.println(forwardButton);
    backwardButton = !digitalRead(rev_button);
    Serial.print("backwardButton:"); Serial.println(backwardButton);
}

void handleState() {
    if(stopper) {
        state = REVERSE;
        Serial.println("REVERSE");
        return;
    }

    if(forwardButton) {
        if(!isMotorRunning) {
            digitalWrite(ENABLE_PIN, LOW);
            isMotorRunning = true;
            buttonHold = millis();
            digitalWrite(DIR_PIN, Forward);
            state = AUTO_MOVE;
            Serial.println("AUTO_MOVE");
        }
        if(isMotorRunning && (millis() - buttonHold) > holdThreshold) {
            state = MANUAL_MOVE;
            Serial.println("MANUAL_MOVE");
        }
    } else {
        if(state == MANUAL_MOVE) {
            state = STANDBY;
            Serial.println("STANDBY");
        }
    }
}

void stopMotor() {
    Serial.println("stopMotor");
    isMotorRunning = false;
    digitalWrite(ENABLE_PIN, HIGH);
}

void reverse() {
    Serial.println("reverse start");
    Serial.print("direction1"); Serial.println(direction);
    direction = static_cast<Direction>(direction ^ (Forward ^ Backward));
    Serial.print("direction2"); Serial.println(direction);
    digitalWrite(DIR_PIN, direction);
    for(size_t i = 0; i < REVERSE_STEPS; ++i) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(frequency);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(frequency);
    }
    Serial.print("direction3"); Serial.println(direction);
    direction = static_cast<Direction>(direction ^ (Forward ^ Backward));
    Serial.print("direction4"); Serial.println(direction);
    Serial.println("reverse stop");
}
