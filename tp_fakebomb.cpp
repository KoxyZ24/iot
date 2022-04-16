// définition des broches du décodeur 7 segments
// (vous pouvez changer les numéros si vous voulez)
const int bit_A = 2;
const int bit_B = 3;
const int bit_C = 4;
const int bit_D = 5;
const byte rows = 3; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
};
byte rowPins[rows] = {12, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[cols] = {9, 8, 14}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
// définitions des broches des transistors pour chaque afficheur
const int alim_ten = 6; // les tens
const int alim_unity = 7;   // les units

void setup() {
    // Les broches sont toutes des sorties
    pinMode(bit_A, OUTPUT);
    pinMode(bit_B, OUTPUT);
    pinMode(bit_C, OUTPUT);
    pinMode(bit_D, OUTPUT);
    pinMode(alim_ten, OUTPUT);
    pinMode(alim_unity, OUTPUT);

    // Les broches sont toutes mises à l'état bas
    digitalWrite(bit_A, LOW);
    digitalWrite(bit_B, LOW);
    digitalWrite(bit_C, LOW);
    digitalWrite(bit_D, LOW);
    digitalWrite(alim_ten, LOW);
    digitalWrite(alim_unity, LOW);
}

void loop() // fonction principale
{
    // boucle qui permet de compter de 0 à 99 (= 100 valeurs)
    if (disarmaCode())
        for (char i = 60; i >= 0; i--) {
            // appel de la fonction affichage avec envoi du number à display
            display_number(i);
        }
    exit(0);
}

bool disarmaCode(const solve) {
    int randNum = rand() % (19 - 11 + 1) + 11;

    if (solve == randNum) {
        return true;
    } else {
        return false;
    }
    return false;
}


// fonction permettant d'display un number sur deux afficheurs
void display_number(char number) {
    long temps; // variable utilisée pour savoir le temps écoulé...
    char unity = 0, ten = 0; // variable pour chaque afficheur

    if (number > 9) // si le number reçu dépasse 9
    {
        ten = number / 10; // on récupère les tens
    }

    unity = number - (ten * 10); // on récupère les unités

    temps = millis(); // on récupère le temps courant

    // tant qu'on a pas affiché ce digit pendant au moins 500 millisecondes
    // permet donc de pouvoir lire le number affiché
    while ((millis() - temps) < 1000) {
        // on affiche le number

        // d'abord les tens pendant 10 ms

        // le transistor de l'afficheur des tens est saturé,
        // donc l'afficheur est allumé
        digitalWrite(alim_ten, HIGH);
// on appel la fonction qui permet d'display le digit ten
        display(ten);
        // l'autre transistor est bloqué et l'afficheur éteint
        digitalWrite(alim_unity, LOW);
        delay(250);

        // puis les unités pendant 10 ms

        // on éteint le transistor allumé
        digitalWrite(alim_ten, LOW);
        // on appel la fonction qui permet d'display le digit unité
        display(unity);
        // et on allume l'autre
        digitalWrite(alim_unity, HIGH);
        delay(250);
    }
}

// fonction écrivant sur un seul afficheur
// on utilise le même principe que vu plus haut
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