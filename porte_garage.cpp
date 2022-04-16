const int BT1 = 2;
const int SW_OPEN = 3;
const int SW_CLOSE = 4;
const int LED_OPEN = 11; // GATE OPEN
const int LED_WORKING = 12; // OPENING OR CLOSING GATE
const int LED_CLOSE = 13; // GATE CLOSE OR LOCK (BLINKING)
bool sw_open = false;
bool sw_close = false;
bool bt1 = false;
bool lock = false;
bool gate = false;

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

void Timer::stop() {
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
    UNKNOWN,
    CLOSE,
    OPEN,
    WAIT_CLOSE,
    WAIT_OPEN,
    WAIT_RELEASE_CLOSE,
    WAIT_RELEASE_OPEN,
    LOCK,
    WARNING,
    BT_PRESS,
    BT_RELEASE,
    BT_NOTPRESS
};

String statesName[] = {"UNKNOWN", "CLOSE", "OPEN", "WAIT_CLOSE", "WAIT_OPEN", "WAIT_RELEASE_CLOSE", "WAIT_RELEASE_OPEN",
                       "LOCK", "WARNING", "BT_PRESS", "BT_RELEASE", "BT_NOTPRESS"};

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
        // Serial.print("Source state :");
        // Serial.println(stateToString(source_state));
        // Serial.print("Target state :");
        // Serial.println(stateToString(target_state));
        // Serial.println("--------");

    }
}

State Fsm::getCurrentState() {
    return _current_state;
}


String Fsm::stateToString(State state) {
    return statesName[state];
}

void getInputs() {
    sw_close = !digitalRead(SW_CLOSE);
    sw_open = !digitalRead(SW_OPEN);
    bt1 = !digitalRead(BT1);
    /* if (sw_close == 1) {
         // Serial.print("sw_close :");
         // Serial.println(sw_close);
     }
     if (sw_open == 1) {
         // Serial.print("sw_open :");
         // Serial.println(sw_open);
     }
     if (bt1 == 1) {
         // Serial.print("bt1 :");
         // Serial.println(bt1);
     }*/
}

Fsm fsm;
Timer t1; // Time for close
Timer t2; // Time for wait open
Timer t3; // Time for open
Timer t4; // Time for wait close
Timer t5; // Time for lock
Timer bt1_timer; //Time for button

void lockBlinking()  // Clignotant
    t5.start();
    digitalWrite(LED_CLOSE, LOW);
    if (t5.elapsed() > 300) {
        digitalWrite(LED_CLOSE, HIGH);
    }
}

void setState(int state) {


    digitalWrite(LED_CLOSE, LOW);
    digitalWrite(LED_OPEN, LOW);
    digitalWrite(LED_WORKING, LOW);

    switch (state) {
        case UNKNOWN:
            break;
        case CLOSE:
            digitalWrite(LED_CLOSE, HIGH);
            t1.start();
            t5.stop();
            break;
        case WAIT_RELEASE_OPEN:
            digitalWrite(LED_WORKING, HIGH);
            t1.stop();
            t2.start();
            break;
        case WAIT_OPEN:
            digitalWrite(LED_WORKING, HIGH);
            t2.stop();
            break;
        case OPEN:
            digitalWrite(LED_OPEN, HIGH);
            t3.start();
            break;
        case WAIT_RELEASE_CLOSE:
            digitalWrite(LED_WORKING, HIGH);
            t3.stop();
            t4.start();
            break;
        case WAIT_CLOSE:
            digitalWrite(LED_WORKING, HIGH);
            t4.stop();
            break;
        case LOCK:
            lockBlinking();
            t1.stop();
            t5.start();
            break;
        case WARNING:
            digitalWrite(LED_CLOSE, HIGH);
            digitalWrite(LED_OPEN, HIGH);
            digitalWrite(LED_WORKING, HIGH);
            break;
        case BT_NOTPRESS:
            bt1_timer.stop();
            break;
        case BT_PRESS:
            bt1_timer.start();
            break;
        case BT_RELEASE:
            if (bt1_timer.elapsed() >= 2000 && bt1_timer.elapsed() < 5000) {
                gate = true;
            }
            if (bt1_timer.elapsed() >= 5000) {
                lock = true;
            }
            break;

    }
}

void setup() {
    // Serial.begin(9600);
    pinMode(SW_CLOSE, INPUT_PULLUP);
    pinMode(SW_OPEN, INPUT_PULLUP);
    pinMode(BT1, INPUT_PULLUP);
    pinMode(LED_CLOSE, OUTPUT);
    pinMode(LED_OPEN, OUTPUT);
    pinMode(LED_WORKING, OUTPUT);
}

void loop() {
    //get all inputs
    getInputs();


    fsm.checkState(BT_NOTPRESS, BT_PRESS, bt1);
    fsm.checkState(BT_PRESS, BT_RELEASE, !bt1);
    fsm.checkState(BT_RELEASE, BT_NOTPRESS, !bt1);


    //CLOSE & OPEN FSM
    fsm.checkState(UNKNOWN, CLOSE, true);

    fsm.checkState(CLOSE, WAIT_RELEASE_OPEN, gate);
    fsm.checkState(WAIT_RELEASE_OPEN, WAIT_OPEN, t2.elapsed() <= 5000); //ouverture
    fsm.checkState(WAIT_OPEN, OPEN, sw_open);

    fsm.checkState(OPEN, WAIT_RELEASE_CLOSE, gate);
    fsm.checkState(WAIT_RELEASE_CLOSE, WAIT_CLOSE, t4.elapsed() <= 5000);
    fsm.checkState(WAIT_CLOSE, CLOSE, sw_close);

    // LOCK & UNLOCK FSM
    fsm.checkState(CLOSE, LOCK, lock);
    fsm.checkState(LOCK, CLOSE, lock);

    //WARNING FSM
    fsm.checkState(WAIT_RELEASE_OPEN, WARNING, t2.elapsed() > 5000);
    fsm.checkState(WAIT_RELEASE_CLOSE, WARNING, t4.elapsed() > 5000);

    setState(fsm.getCurrentState());

}