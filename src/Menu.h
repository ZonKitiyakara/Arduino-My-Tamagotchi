/*
  Menu.h
  ---------------------------------
  Menu system for the Arduino Tamagotchi project.

  Responsibilities:
    - Handle scroll and select button input
    - Store and update the currently selected menu item
    - Provide the selected ActionType to the main .ino file
    - Render the menu UI box on the OLED display

  Actions Supported:
    FEED, SLEEP, LOVE, PLAY, DATA

  Notes:
    - Uses simple state tracking to avoid repeated triggers
    - Menu items are rendered on the lower-right corner of the display
    - After getAction() is read, the action resets to NONE
*/

#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

enum ActionType {
  FEED,
  SLEEP,
  LOVE,
  PLAY,
  DATA,     
  NONE
};

class Menu {
public:
  Menu() {
    currentIndex = 0;
    currentAction = NONE;
  }

  void update(bool scrollPressed, bool selectPressed) {
    if (scrollPressed && !lastScrollState) {
      currentIndex = (currentIndex + 1) % itemCount;
    }

    if (selectPressed && !lastSelectState) {
      if (strcmp(menuItems[currentIndex], "Feed") == 0) {
        currentAction = FEED;
      } else if (strcmp(menuItems[currentIndex], "Sleep") == 0) {
        currentAction = SLEEP;
      } else if (strcmp(menuItems[currentIndex], "Love") == 0) {
        currentAction = LOVE;
      } else if (strcmp(menuItems[currentIndex], "Play") == 0) {
        currentAction = PLAY;
      }
      else if (strcmp(menuItems[currentIndex], "Data") == 0) {
  currentAction = DATA;
}

    }

    lastScrollState = scrollPressed;
    lastSelectState = selectPressed;
  }

  void render(Adafruit_SSD1306 &display) {
    int boxX = 128 - 55;
    int boxY = 64 - 18;
    display.drawRect(boxX, boxY, 54, 16, WHITE);
    display.setCursor(boxX + 4, boxY + 4);
    display.print(">");
    display.print(menuItems[currentIndex]);
  }

  ActionType getAction() {
    ActionType actionToReturn = currentAction;
    currentAction = NONE; // reset after reading
    return actionToReturn;
  }

private:
  const char* menuItems[5] = {"Feed", "Sleep", "Love", "Play", "Data"};  // Add "Play" here
  int currentIndex;
  const int itemCount = 5;  // Update count to 4
  bool lastScrollState = false;
  bool lastSelectState = false;
  ActionType currentAction;
};

#endif
