// Arduino
// GLOBAL
class Timer {
    unsigned long startTime = 0;
    public:
        void start();
        unsigned long elapsed();
};
void Timer::start() {
    startTime = millis();
}
unsigned long Timer::elapsed() {
    return millis() - startTime;
}
Timer t;
void setup() {
    Serial.begin(9600);
    t.start();
    delay(1000);
    // t.elapsed();
      Serial.print("\n");
    Serial.print(t.elapsed());
      Serial.print("\n");
}
void loop() {
  // rien
}