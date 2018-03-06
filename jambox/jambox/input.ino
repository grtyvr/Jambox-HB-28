#define BUTTON_1_PIN 4
#define BUTTON_2_PIN 5
#define BUTTON_3_PIN 16
#define BUTTON_4_PIN 17
#define BUTTON_5_PIN 18
#define BUTTON_6_PIN 19
#define BUTTON_7_PIN 21
#define BUTTON_8_PIN 23

#define POT_1_PIN 32
#define POT_2_PIN 33
#define POT_3_PIN 34
#define POT_4_PIN 35
#define POT_5_PIN 36

void input_init() {

    pinMode(BUTTON_1_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_2_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_3_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_4_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_5_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_6_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_7_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON_8_PIN, INPUT_PULLDOWN);

    analogReadResolution(10);

    pinMode(POT_1_PIN, INPUT);
    pinMode(POT_2_PIN, INPUT);
    pinMode(POT_3_PIN, INPUT);
    pinMode(POT_4_PIN, INPUT);
    pinMode(POT_5_PIN, INPUT);
}

bool input_button1() { return digitalRead(BUTTON_1_PIN) == HIGH; }
bool input_button2() { return digitalRead(BUTTON_2_PIN) == HIGH; }
bool input_button3() { return digitalRead(BUTTON_3_PIN) == HIGH; }
bool input_button4() { return digitalRead(BUTTON_4_PIN) == HIGH; }
bool input_button5() { return digitalRead(BUTTON_5_PIN) == HIGH; }
bool input_button6() { return digitalRead(BUTTON_6_PIN) == HIGH; }
bool input_button7() { return digitalRead(BUTTON_7_PIN) == HIGH; }
bool input_button8() { return digitalRead(BUTTON_8_PIN) == HIGH; }

int input_pot1() { return analogRead(POT_1_PIN); }
int input_pot2() { return analogRead(POT_2_PIN); }
int input_pot3() { return analogRead(POT_3_PIN); }
int input_pot4() { return analogRead(POT_4_PIN); }
int input_pot5() { return analogRead(POT_5_PIN); }
