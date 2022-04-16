//
// Created by asus on 07/03/2022.
//
const int SW_UP = 2;
const int SW_DOWN = 3;
const int SW_WRN = 4;
const int LED_UP = 10;
const int LED_DOWN = 11;
const int LED_WRN = 12;
bool sw_up = false;
bool sw_down = false;
bool sw_wrn = false;

class Timer {
    unsigned long startTime = 0;
    bool is_started = false;
public:
    void start();
    void stop();
    void restart();
    unsigned long elapsed();
};

void Timer::start() {
    if (is_started) {
        return;
    }
    is_started = true;
    startTime = millis();
}

void Timer::stop(){
    is_started = false;
}

void Timer::restart() {
    start();
    stop();
}

unsigned long Timer::elapsed() {
    return millis() - startTime;
}

enum State {
    UNKNOWN, UP, DOWN, WARNING, WAIT_UP, WAIT_DOWN
};

String statesName[] = {"UNKNOWN", "UP", "DOWN", "WARNING", "WAIT_UP", "WAIT_DOWN"};


class Fsm {
    State _current_state = UNKNOWN;
public:
    void checkState(State source_state, State target_state, bool condition);

    State getCurrentState();

    String stateToString(State state);
};

void Fsm::checkState(State source_state, State target_state, bool condition) {
    if (source_state == _current_state && condition) {
        _current_state = target_state;
        Serial.print("Source state :");
        Serial.println(stateToString(source_state));
        Serial.print("Target state :");
        Serial.println(stateToString(target_state));
        Serial.println("--------");

    }
}

State Fsm::getCurrentState() {
    return _current_state;
}


String Fsm::stateToString(State state) {
    return statesName[state];

}

void getInputs() {
    sw_up = !digitalRead(SW_UP);
    sw_down = !digitalRead(SW_DOWN);
    sw_wrn = !digitalRead(SW_WRN);
    if (sw_up == 1) {
        Serial.print("sw_up:");
        Serial.println(sw_up);
    }
    if (sw_down == 1) {
        Serial.print("sw_down:");
        Serial.println(sw_down);
    }
    if (sw_wrn == 1) {
        Serial.print("sw_wrn:");
        Serial.println(sw_wrn);
    }
}

Fsm fsm;
Timer t1; //timer for up
Timer t2; //timer for down
Timer t3; //timer for wait up
Timer t4; //timer for wait down

void setState(int state) {
    // execute matching state

    //Tout eteindre avant de changer d'etat
    digitalWrite(LED_UP, LOW);
    digitalWrite(LED_DOWN, LOW);
    digitalWrite(LED_WRN, LOW);
    switch (state) {

        case UNKNOWN:
            break;
        case UP: //Moteur vers le haut
            digitalWrite(LED_UP, HIGH);
            t1.start();
            t2.stop();
            break;
        case WAIT_UP:
            digitalWrite(LED);
            break;
        case DOWN: //Moteur vers le bas
            digitalWrite(LED_DOWN, HIGH);
            t2.start();
            t1.stop();
            break;
        case WARNING:
            digitalWrite(LED_WRN, HIGH);
            break;
    }
}

void setup() {

    Serial.begin(9600);
    pinMode(SW_UP, INPUT_PULLUP);
    pinMode(SW_DOWN, INPUT_PULLUP);
    pinMode(SW_WRN, INPUT_PULLUP);
    pinMode(LED_UP, OUTPUT);
    pinMode(LED_DOWN, OUTPUT);
    pinMode(LED_WRN, OUTPUT);


}


void loop() {
// get all inputs
    getInputs();

// run fsm
    fsm.checkState(UNKNOWN, UP, true);
    fsm.checkState(UP, DOWN, sw_up);
    fsm.checkState(DOWN, UP, sw_down);
    fsm.checkState(UP, WARNING, sw_wrn || t1.elapsed() > 6000);
    fsm.checkState(DOWN, WARNING, sw_wrn || t1.elapsed() > 6000);

    setState(fsm.getCurrentState());

}
