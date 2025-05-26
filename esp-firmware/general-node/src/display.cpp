#include "display.hpp"
#include <Arduino.h>




Display::Display() {
    // Constructor implementation can be empty or initialize members if needed
}

void Display::init(NodeConfig &config, Mesh &mesh) {
    this->roomId = config.getRoomId();
    this->mesh = &mesh;
    this->tft.init();
    this->tft.setRotation(1);
    this->tft.fillScreen(TFT_BLACK);
    this->tft.setTextColor(TFT_WHITE, TFT_BLACK);
    this->tft.setTextSize(2);
    this->tft.drawString("Display Initialized", 10, 10);
    this->tft.setCursor(10, 30);
    this->tft.printf("Room ID: %d\n", this->roomId);
}


void Display::update() {
    // Clear previous screen content

}
