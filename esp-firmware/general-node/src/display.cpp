#include "display.hpp"
#include <Arduino.h>




Display::Display() {
    // Constructor implementation can be empty or initialize members if needed
}

void Display::init(NodeConfig &config, Mesh &mesh) {
    this->roomId = config.getRoomId();
    this->mesh = &mesh;
    this->lastFromNode = -1; // Initialize to -1 to indicate no messages received yet
    this->lastToNode = -1; // Initialize to -1 to indicate no messages sent yet
    this->tft.init();
    this->tft.setRotation(1);
    this->tft.fillScreen(TFT_BLACK);
    this->tft.setTextColor(TFT_WHITE, TFT_BLACK);
    this->tft.setTextSize(2);
    this->tft.drawString("Display Initialized", 10, 10);
    this->tft.setCursor(10, 30);
    this->tft.printf("Room ID: %d\n", this->roomId);
    delay(2000); // Delay to allow the display to show the initialization message
    this->tft.fillScreen(TFT_BLACK);


}


void Display::update() {
    this->tft.setCursor(10,10);
    this->tft.println("Room ID: "+String(roomId));
    this->tft.setCursor(10, 30);
    this->tft.println("Node Count: " + String(this->mesh->getNodesCount()));
    this->tft.setCursor(10, 50);
    this->tft.println("Last Msg From: " + String(this->lastFromNode));
    this->tft.setCursor(10, 70);
    this->tft.println("Last Msg To: " + String(this->lastToNode));
    this->tft.setCursor(10, 150);
    this->tft.printf("Free Heap: %d", ESP.getFreeHeap());
    
}
