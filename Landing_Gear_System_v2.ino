#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#include <Bounce2.h>

#define HORIZONTAL_BUTTON_PIN 2
#define VERTICAL_BUTTON_PIN 3
#define GREEN_LED_PIN 4
#define AMBER_LED_PIN 5
#define RED_LED_PIN 6
#define RGB_LED_RED_PIN 7
#define RGB_LED_GREEN_PIN 8
#define RGB_LED_BLUE_PIN 9
#define BUZZER_PIN 10
#define GREEN_LED_PIN2 11


hd44780_I2Cexp lcd;            // create an instance of the hd44780_I2Cexp class

Bounce horizontalButton = Bounce();
Bounce verticalButton = Bounce();

int cycleCount = 0;
bool verticalPressed = false;
bool horizontalPressed = false;

void setup() {
  lcd.begin(16, 2, 0x27);
  lcd.init();                   // Initialize the display using hd44780_I2Cexp library
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("LANDING GEAR");
  lcd.setCursor(0, 1);
  lcd.print("CYCLE METER");
  delay(3000);
  lcd.clear();

  lcd.print("Cycles: 0");

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN2, OUTPUT);
  pinMode(AMBER_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(RGB_LED_RED_PIN, OUTPUT);
  pinMode(RGB_LED_GREEN_PIN, OUTPUT);
  pinMode(RGB_LED_BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  horizontalButton.attach(HORIZONTAL_BUTTON_PIN, INPUT_PULLUP);
  horizontalButton.interval(13); // Debounce interval in milliseconds

  verticalButton.attach(VERTICAL_BUTTON_PIN, INPUT_PULLUP);
  verticalButton.interval(13); // Debounce interval in milliseconds
}

void loop() {
  horizontalButton.update();
  verticalButton.update();

  // Check if the horizontal button is pressed
  if (horizontalButton.fell()) {
    horizontalPressed = true;
    // Turn on the green LEDs
    digitalWrite(GREEN_LED_PIN2, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(AMBER_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    
  }

  // Check if the vertical button is pressed
  if (verticalButton.fell()) {
    verticalPressed = true;
    // Turn on the green LEDs
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(AMBER_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN2, LOW);
  }

  // Check if a complete cycle is detected
  if (horizontalPressed && verticalPressed) {
    cycleCount++;
    lcd.setCursor(8, 0);
    lcd.print("   ");
    lcd.setCursor(8, 0);
    lcd.print(cycleCount);

    // Reset the button flags
    verticalPressed = false;
    horizontalPressed = false;

    // Turn off the green LEDs
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(AMBER_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN2, LOW);
  }

   // Check if the landing gear is in transit
  if (horizontalButton.read() == HIGH && verticalButton.read() == HIGH) {
    // Beep the buzzer every 200ms
     digitalWrite(AMBER_LED_PIN, HIGH);
    tone(BUZZER_PIN, 1000);
    delay(100);
    noTone(BUZZER_PIN);
delay(150);
    // Turn on the amber LED
   
     digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(AMBER_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN2, LOW);
  } else {
    digitalWrite(AMBER_LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  // Control the RGB LED based on the cycle count
  if (cycleCount >= 0 && cycleCount <= 7) {
    // Green color
    analogWrite(RGB_LED_RED_PIN, 0);
    analogWrite(RGB_LED_GREEN_PIN, 255);
    analogWrite(RGB_LED_BLUE_PIN, 0);
  } else if (cycleCount >= 8 && cycleCount <= 9) {
    // Amber color
    analogWrite(RGB_LED_RED_PIN, 255);
    analogWrite(RGB_LED_GREEN_PIN, 150);
    analogWrite(RGB_LED_BLUE_PIN, 0);
  } else if (cycleCount >= 10) {
    // Red color
    analogWrite(RGB_LED_RED_PIN, 255);
    analogWrite(RGB_LED_GREEN_PIN, 0);
    analogWrite(RGB_LED_BLUE_PIN, 0);
    // Reset the cycle count to 0
    cycleCount = 0;
    lcd.setCursor(8, 0);
    lcd.print("   ");
    lcd.setCursor(8, 0);
    lcd.print(cycleCount);
    delay(3000);
  }
}
