#include "config.hpp"

NodeConfig::NodeConfig(String base_ssid, String base_password, bool isRoot, Scheduler &scheduler, uint8_t room_id, uint8_t led_pin, uint8_t led_count, HardwareSerial &serial, String version, bool nv_store_on_set)
{
    // TODO: Overwrite Default from NV Store

    this->base_ssid = base_ssid;
    this->base_password = base_password;

    // this->mesh_config.ssid = base_ssid;
    // this->mesh_config.password = base_password;

    // Room Config
    this->room_config.id = room_id;
    // Mesh Config
    this->mesh_config.containsRoot = true;
    this->mesh_config.setRoot = isRoot;
    this->mesh_config.scheduler = &scheduler;
    // Serial Config
    this->serial_config.serial = &serial;
    // Light Config
    this->light_config.effect_speed = 150;
    this->light_config.led_count = led_count;
    this->light_config.led_pin = led_pin;

    this->setWirelessCredentials();

    // Non - volatile config store
    this->nv_store_on_set = nv_store_on_set;

    this->prefs = new Preferences();

    // Make sure we're able to open the partition in read-write mode
    // assert(this->prefs->begin("default",false));
}

void NodeConfig::setNodeId(uint32_t node_id)
{
    this->node_id = node_id;
    // this->save();
}

uint32_t NodeConfig::getNodeId()
{
    return this->node_id;
}

void NodeConfig::setRoomId(uint8_t room_id)
{
    this->room_config.id = room_id;
    this->setWirelessCredentials();
    // this->save();
}

uint8_t NodeConfig::getRoomId()
{
    return this->room_config.id;
}

void NodeConfig::setWirelessCredentials()
{
    vector<String> generated_credentials = this->getWirelessCredentialsFromRoomId();

    this->mesh_config.ssid = generated_credentials[0];
    this->mesh_config.password = generated_credentials[1];
    // this->save();
}

vector<String> NodeConfig::getWirelessCredentialsFromRoomId()
{
    return {this->base_ssid + "-" + this->room_config.id, this->base_password + this->room_config.id};
}

void NodeConfig::setBaseNetworkCredentials(String base_ssid, String base_password)
{
    this->base_ssid = base_ssid;
    this->base_password = base_password;
    this->setWirelessCredentials();
}

vector<String> NodeConfig::getBaseNetworkCredentials()
{
    return {this->base_ssid, this->base_password};
}

vector<String> NodeConfig::getWirelessCredentials()
{
    return {this->mesh_config.ssid, this->mesh_config.password};
}

bool NodeConfig::save()
{
    this->prefs->putUInt("node-id", this->node_id);
    this->prefs->putInt("room-id", this->room_config.id);
    this->prefs->putString("ssid", this->mesh_config.ssid);
    this->prefs->putString("password", this->mesh_config.password);
}

void NodeConfig::logConfig()
{
    auto serial = this->serial_config.serial;

    serial->printf("========= Node Config  =========\n\n");
    serial->printf("Base SSID: %s\n", this->base_ssid.c_str());
    serial->printf("Base Password: %s\n", this->base_password.c_str());
    serial->printf("Node ID: %lu\n", this->node_id);
    serial->printf("Room ID: %d\n", this->room_config.id);
    serial->printf("Generated Mesh SSID: %s\n", this->mesh_config.ssid.c_str());
    serial->printf("Generated Mesh Password: %s\n", this->mesh_config.password.c_str());
    serial->printf("LED Count: %d\n", this->light_config.led_count);
    serial->printf("LED Pin: %d\n", this->light_config.led_pin);
    serial->printf("Version: %s\n", this->version.c_str());
    serial->printf("\n========= Node Config  =========\n");
}