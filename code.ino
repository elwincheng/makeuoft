#include <Servo.h>

Servo servo;

bool clockwise = true;
int direction = 1; // Direction of servo motor (1 -> clockwise | -1 -> counter clockwise)
const int BUTTON_PIN = 10, SWITCH_PIN = 13;
int led = 0;
int pos = 0;
int buttonState = 0; // State of game button
int switchState = 0; // State of switching games button
int lastState = LOW ;
bool gameFlag = 0;

int lastSwitch = 0;

void clearLED() {
  for (int i = 1; i < 10; i++){
    digitalWrite(i, LOW);
  }
}

void setup() {
  tone(11, 2000, 500);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);
  servo.attach(12);
  randomSeed(analogRead(0));


}
  int score = 0;

// This game is based off of arcade game of the same name. Led lights light up and continously loop around. The
// player increases their score by one if they press the game button when the last LED is turned on
void cyclone() {
  int buttonState = 0;
  digitalWrite(1, LOW);

  for (int i = 2; i < 10; i++) { // For each led...
    digitalWrite(i-1, LOW);
    digitalWrite(i, HIGH);
    bool won = false;
    for (int j = 0; j < 500 - score *50; j++) { // Increase speed (decrease time at each led) proportional to score
      delay(1);
      switchState = digitalRead(SWITCH_PIN);
      int mills = millis();
      if (switchState == HIGH && mills - lastSwitch > 3000) { // prevent double pressing and switching game modes too often
        lastSwitch = mills;
        score = 0;
        gameFlag = !gameFlag;
        clearLED();
        led = random(7) + 2;
        digitalWrite(led, HIGH);
        return;
      }
      
      buttonState = digitalRead(BUTTON_PIN);
      if (buttonState == HIGH){ 
        if (i == 9) {
          if (!won){ // prevent double pressing
            tone(11, 2000, 500);
            score++;  
            won = true;
          }
          
        } else {
          lastState = LOW;
          tone(11, 500, 500);
          score = 0;
        }
      }
    
    }
  }
  digitalWrite(9, LOW);
}

// This game is based off of the Pop the Lock arcard game. The motor sweeps back and forth
// from 0 to 180 degrees. A random LED is lit up, and the goal of the player is to press the 
// game button once the motor's head
void lock() {
  delay(15 - score);
  switchState = digitalRead(SWITCH_PIN);
  int mills = millis();
  if (switchState == HIGH && mills - lastSwitch > 3000) { // check if user wants to change game modes
    lastSwitch = mills;
    score = 0;
    gameFlag = !gameFlag;
    clearLED();
    return;
  }
  pos += 1 * direction;

  if (pos == 180 || pos == 0) direction *= -1;
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) { 
    if (lastState == LOW) { // prevent double pressing
      int angle = servo.read();
      if ((led - 2) * 22 < angle && angle < (led - 2 + 1) * 22) {
        tone(11, 2000, 500);
        direction *= -1;
        score++;
        digitalWrite(led, LOW);
        led = random(0, 7) + 2;
        digitalWrite(led, HIGH);
        
      } else {
        tone(11, 500, 500);
        score = 0;
      } 
    }

    lastState = HIGH;
  } else {
    lastState = LOW;
  }
  servo.write(pos);
}

void loop() {
  if (gameFlag){
    lock();
  } else {
    cyclone();
  }


}