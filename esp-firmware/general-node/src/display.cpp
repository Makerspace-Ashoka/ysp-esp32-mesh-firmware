#include "display.hpp"
#include <Arduino.h>

Display::Display(NodeConfig &config, Mesh &mesh) : tft(TFT_eSPI()), mesh(&mesh) {
    roomid = config.getRoomId();
}

void Display::init() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
}

void Display::update() { // Clear previous screen content
    tft.setCursor(10, 10);
    tft.println("Room ID: " + String(roomid));
    tft.setCursor(10, 30);
    tft.println("Node Count: " + String(mesh->getNodesCount()));  // replace with actual function if different
}
