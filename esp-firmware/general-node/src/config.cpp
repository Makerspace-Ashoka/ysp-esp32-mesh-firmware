#include "config.hpp"

NodeConfig::NodeConfig(String ssid, String password, uint16_t port = 555, bool isRoot = false, Scheduler &scheduler, uint8_t room_id, uint8_t led_pin, uint8_t led_count, HardwareSerial &serial, String version)
{
    // Mesh Config
    this->mesh_config.ssid = ssid;
    this->mesh_config.password = password;
    this->mesh_config.containsRoot = true;
    this->mesh_config.setRoot = isRoot;
    this->mesh_config.scheduler = &scheduler;
    // Serial Config
    this->serial_config.serial = &serial;
    // Room Config
    this->room_config.id = room_id;
    // Light Config
    this->light_config.effect_speed = 150;
    this->light_config.led_count = led_count;
    this->light_config.led_pin = led_pin;
}

void NodeConfig::setNodeId(uint32_t node_id)
{
    this->node_id = node_id;
}

uint32_t NodeConfig::getNodeId()
{
    return this->node_id;
}