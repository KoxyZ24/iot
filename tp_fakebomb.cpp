#include <Keypad.h>

const int bit_A = 2;
const int bit_B = 3;
const int bit_C = 4;
const int bit_D = 5;

const int alim_ten = 6; //  tens
const int alim_unity = 7;   // units

char password[4]; // number of characters in our password
int currentLength = 0; //defines which number we are currently writing
int second = 12; // number second
char entered[4];

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 3; //three columns
char keys[ROW_NUM][COLUMN_NUM] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
};
byte pin_rows[ROW_NUM] = {12, 11, 10, 9}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {8, 14, 15}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

void setup() {
    Serial.begin(9600);
    pinMode(bit_A, OUTPUT);
    pinMode(bit_B, OUTPUT);
    pinMode(bit_C, OUTPUT);
    pinMode(bit_D, OUTPUT);
    pinMode(alim_ten, OUTPUT);
    pinMode(alim_unity, OUTPUT);

    digitalWrite(bit_A, LOW);
    digitalWrite(bit_B, LOW);
    digitalWrite(bit_C, LOW);
    digitalWrite(bit_D, LOW);
    digitalWrite(alim_ten, LOW);
    digitalWrite(alim_unity, LOW);
    pinMode(13, OUTPUT);
    Serial.println("Press * to start !");
}

void loop() {
    char key = keypad.getKey();
    if (key == '*') {
        Serial.print("\nEnter Password : ");
        while (currentLength < 4) {
            char key2 = keypad.getKey();
            countdown();
            if (key2 != NO_KEY) {
                Serial.print(key2);
                entered[currentLength] = key2;
                currentLength++;
            }
        }
        if (currentLength == 4) { // Password 0000 (yes yes its secure)
            if (entered[0] == '0' && entered[1] == '0' && entered[2] == '0' && entered[3] == '0') {
                Serial.println("\nWell play you survive");
                exit(0);
                tetrisTheme();
            } else {
                Serial.println("\nWrong Password! Press * to retry.");
                currentLength = 0;
            }
        }
    }
}

void countdown() {
    second--;
    display_number(second);
    switch (second) {
        case 10:
            tone(13, 3136, 200);
            delay(500);
            break;
        case 0:
            Serial.println("\nKABOOM!(Yes, you die)");
            tetrisTheme();
            exit(0);
    }
}

// Function who permit displaying numbers on 2 screens
void display_number(char number) {
    long temps; // variable used to know the elapsed time
    char unity = 0, ten = 0; // variable for each display

    if (number > 9) // if the number received is more than 9
    {
        ten = number / 10; // get the tens
    }

    unity = number - (ten * 10); // get the units
    temps = millis(); // get the current time

    while ((millis() - temps) < 1000) {
        // TENS
        digitalWrite(alim_ten, HIGH);
        display(ten);
        digitalWrite(alim_unity, LOW);
        delay(50);
        // UNITY
        digitalWrite(alim_ten, LOW);
        display(unity);
        digitalWrite(alim_unity, HIGH);
        delay(50);
    }
}

void display(char digit) {
    digitalWrite(bit_A, LOW);
    digitalWrite(bit_B, LOW);
    digitalWrite(bit_C, LOW);
    digitalWrite(bit_D, LOW);

    if (digit >= 8) {
        digitalWrite(bit_D, HIGH);
        digit = digit - 8;
    }
    if (digit >= 4) {
        digitalWrite(bit_C, HIGH);
        digit = digit - 4;
    }
    if (digit >= 2) {
        digitalWrite(bit_B, HIGH);
        digit = digit - 2;
    }
    if (digit >= 1) {
        digitalWrite(bit_A, HIGH);
        digit = digit - 1;
    }
}

// TETRIS SONG
void tetrisTheme() {
    tone(13, 2637, 200);
    delay(400);
    tone(13, 1975, 200);
    delay(200);
    tone(13, 2093, 200);
    delay(200);
    tone(13, 2349, 200);
    delay(400);
    tone(13, 2093, 200);
    delay(200);
    tone(13, 1975, 200);
    delay(200);
    tone(13, 1760, 200);
    delay(400);
    tone(13, 1760, 200);
    delay(200);
    tone(13, 2093, 200);
    delay(200);
    tone(13, 2637, 200);
    delay(400);
    tone(13, 2349, 200);
    delay(200);
    tone(13, 2093, 200);
    delay(200);
    tone(13, 1975, 200);
    delay(400);
    tone(13, 1975, 200);
    delay(200);
    tone(13, 2093, 200);
    delay(200);
    tone(13, 2349, 200);
    delay(400);
    tone(13, 2637, 200);
    delay(400);
    tone(13, 2093, 200);
    delay(400);
    tone(13, 1760, 200);
    delay(400);
    tone(13, 1760, 200);
    delay(800);
    tone(13, 1760, 200);
    delay(400);
    tone(13, 2349, 200);
    delay(200);
    tone(13, 2794, 200);
    delay(200);
    tone(13, 3520, 200);
    delay(400);
    tone(13, 3136, 200);
    delay(200);
    tone(13, 2794, 200);
    delay(200);
    tone(13, 2637, 200);
    delay(600);
    tone(13, 2093, 200);
    delay(200);
    tone(13, 2637, 200);
    delay(400);
    tone(13, 2349, 200);
    delay(200);
    tone(13, 2093, 200);
    delay(200);
    tone(13, 1975, 200);
    delay(400);
    tone(13, 1975, 200);
    delay(200);
    tone(13, 2093, 200);
    delay(200);
    tone(13, 2349, 200);
    delay(400);
    tone(13, 2637, 200);
    delay(400);
    tone(13, 2093, 200);
    delay(400);
    tone(13, 1760, 200);
    delay(400);
    tone(13, 1760, 200);
    delay(800);
}