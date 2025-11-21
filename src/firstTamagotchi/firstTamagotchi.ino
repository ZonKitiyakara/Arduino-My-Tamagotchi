/*
  MyTamagotchi.ino
  -----------------
  Main program file for an Arduino-based virtual pet (Tamagotchi-style) 
  running on a 128x64 OLED display.

  Features:
    - Menu-driven actions: Feed, Sleep, Love, Play, View Data
    - Idle bobbing animation
    - Stat tracking: Hunger, Happiness, Sleep (0–10)
    - Time-based stat decay every 3 minutes
    - Action log counters
    - Bitmap-based sprites rendered with Adafruit_SSD1306

  Hardware:
    - Arduino Uno/Nano
    - 128x64 OLED (I2C, address 0x3C)
    - Scroll button (pin 3) and Select button (pin 2)

  Author: Zon Kitiyakara
  Date: 2025
*/

#include "MyPetBitmap.h"
#include "Steak.h"
#include "Menu.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Button pins
const int scrollButton = 3; // Feed = scroll
const int selectButton = 2; // Love = select

// Pet stats
int hunger = 5;
int happiness = 5;
int sleep = 5;

//Counters
int feedCount = 0;
int sleepCount = 0;
int loveCount = 0;
int playCount = 0;

// Love message
bool showLove = false;
unsigned long stateTimer = 0;

// Bobbing animation
int bobOffset = 0;
bool goingDown = true;
unsigned long lastBobTime = 0;

// Stat decay
unsigned long lastDecayTime = 0;
const unsigned long decayInterval = 180000; // 3 minutes

// Menu system
Menu menu;

void setup() {
  pinMode(scrollButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  Serial.begin(9600);
}

void loop() {
  // Button logic
  bool selectPressed = digitalRead(selectButton) == LOW;
  bool scrollPressed = digitalRead(scrollButton) == LOW;

  // Update menu system
  menu.update(scrollPressed, selectPressed);

  // Handle selected action
  ActionType selected = menu.getAction();

  if (selected == FEED) {
    feedCount++;
    for (int i = 0; i < 6; i++) {
      display.clearDisplay();

      // Pet animation
      display.drawBitmap(0, 14 + (i % 2) * 2, epd_bitmap_TomogatchiPetZon__2_, 32, 46, WHITE);

      // Steak image
      display.drawBitmap(80, 20 + (i % 2) * 2, steakBitmap, 40, 40, WHITE);

      // Text
      display.setCursor(36, 0);
      if (i % 2 == 0)
        display.println("Chomp!");
      else
        display.println("Yum!");

      display.display();
      delay(300);
    }

    hunger = min(hunger + 1, 10);
    happiness = min(happiness + 1, 10);
  }

  if (selected == SLEEP) {
    sleepCount++;
  // Clear screen and show Zzz
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(40, 26); // Center-ish
  display.println("Zzz...");
  display.display();

  // Pause to show sleep effect
  delay(3000); // 3 seconds
  display.setTextSize(1);  // Reset text size back to normal


  // Update stats
  sleep = min(sleep + 1, 10);
  happiness = min(happiness + 1, 10);
}

  if (selected == LOVE) {
    loveCount++;
    showLove = true;
    stateTimer = millis();
    happiness = min(happiness + 1, 10);
  }

  if (selected == PLAY) {
  playCount++;
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 26);
  display.println("Excerising!!!");
  display.display();
  delay(3000);
  display.setTextSize(1);
  happiness = min(happiness + 2, 10);
}
if(selected == DATA)
{
  if (selected == DATA) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Action Log:");
  display.print("Fed: "); display.println(feedCount);
  display.print("Slept: "); display.println(sleepCount);
  display.print("Loved: "); display.println(loveCount);
  display.print("Played: "); display.println(playCount);
  display.display();
  delay(4000); // Display for 4 seconds
}


}


  // Bobbing effect
  if (millis() - lastBobTime > 150) {
    bobOffset += goingDown ? 1 : -1;
    if (bobOffset >= 3 || bobOffset <= 0) goingDown = !goingDown;
    lastBobTime = millis();
  }

  // Stat decay
  if (millis() - lastDecayTime > decayInterval) {
    if (hunger > 0) hunger--;
    if (happiness > 0) happiness--;
    if (sleep > 0) sleep--;
    lastDecayTime = millis();
  }

  // Hide love message after 3 sec
  if (showLove && millis() - stateTimer > 3000) {
    showLove = false;
  }

  // ==== Display ====
  display.clearDisplay();

  if (showLove) {
    display.setCursor(0, 0);
    display.println("Love <3 Love <3 Love <3");
  } else {
    // Menu box
    menu.render(display);

    // Draw pet
    display.drawBitmap(0, 18 + bobOffset, epd_bitmap_TomogatchiPetZon__2_, 32, 46, WHITE);

    // Draw hunger bar
    display.setCursor(36, 10);
    display.print("Hunger");
    display.drawRect(80, 10, 40, 6, WHITE);
    display.fillRect(81, 11, map(hunger, 0, 10, 0, 38), 4, WHITE);

    // Happiness bar
    display.setCursor(36, 20);
    display.print("Happy");
    display.drawRect(80, 20, 40, 6, WHITE);
    display.fillRect(81, 21, map(happiness, 0, 10, 0, 38), 4, WHITE);

    // Sleep bar
    display.setCursor(36, 30);
    display.print("Sleep");
    display.drawRect(80, 30, 40, 6, WHITE);
    display.fillRect(81, 31, map(sleep, 0, 10, 0, 38), 4, WHITE);
  }

  display.display();
  delay(100);
}