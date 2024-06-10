#include "config.hpp"

#define RW_MODE false
#define RO_MODE true

NodeConfig::NodeConfig(bool isRoot, Scheduler &scheduler, uint8_t led_pin, uint8_t led_count, HardwareSerial &serial, String version, Preferences &prefs, bool nv_store_on_set)
{
    this->prefs = &prefs;
    this->loadSavedConfigOrSetDefault();

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
    // Version Num
    this->version = version;

    this->setWirelessCredentials();
    // Non - volatile config store
    this->nv_store_on_set = nv_store_on_set;
}

void NodeConfig::setNodeId(uint32_t node_id)
{
    this->node_id = node_id;
}

uint32_t NodeConfig::getNodeId()
{
    return this->node_id;
}

void NodeConfig::loadSavedConfigOrSetDefault()
{
    auto prefs = this->prefs;
    auto serial = this->serial_config.serial;

    // if (!prefs->begin("node_config", RO_MODE))
    // {
    //     serial->printf("Failed to open Preferences\n");
    //     prefs->clear();
    //     prefs->end();
    // }

    // prefs->end();

    this->room_config.id = prefs->getInt("room_id", this->default_room_id);
    this->base_ssid = prefs->getString("base_ssid", this->default_base_ssid);
    this->base_password = prefs->getString("base_password", this->default_base_password);
}

void NodeConfig::setRoomId(uint8_t room_id)
{
    this->room_config.id = room_id;
    this->setWirelessCredentials();

    if (this->nv_store_on_set)
    {
        this->save();
    }
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

    if (this->nv_store_on_set)
    {
        this->save();
    }
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
    auto serial = this->serial_config.serial;
    auto prefs = this->prefs;

    // if (!prefs->begin("node_config", RW_MODE))
    // {
    //     serial->printf("Failed to open Preferences\n");
    //     return false;
    // }

    vector<String> failed_to_save;
    if (!prefs->putInt("room_id", this->room_config.id))
    {
        failed_to_save.push_back("room_id");
    }
    if (!prefs->putString("base_ssid", this->base_ssid))
    {
        failed_to_save.push_back("base_ssid");
    }
    if (!prefs->putString("base_password", this->base_password))
    {
        failed_to_save.push_back("base_password");
    }

    // prefs->end();

    if (failed_to_save.size() > 0)
    {
        serial->printf("Failed to save the following keys: ");
        for (auto key : failed_to_save)
        {
            serial->printf("%s, ", key.c_str());
        }
        serial->printf("\n");
        return false;
    }

    serial->printf("Node Config Saved\n");
    return true;
}

void NodeConfig::logConfig()
{
    auto serial = this->serial_config.serial;

    serial->printf("========= Node Config  =========\n\n");
    serial->printf("# Loaded from Flash\n");
    serial->printf("Base SSID: %s\n", this->base_ssid.c_str());
    serial->printf("Base Password: %s\n", this->base_password.c_str());
    serial->printf("Room ID: %d\n", this->room_config.id);
    serial->printf("\n# Loaded from Program Space\n");
    serial->printf("Node ID: %lu\n", this->node_id);
    serial->printf("Generated Mesh SSID: %s\n", this->mesh_config.ssid.c_str());
    serial->printf("Generated Mesh Password: %s\n", this->mesh_config.password.c_str());
    serial->printf("LED Count: %d\n", this->light_config.led_count);
    serial->printf("LED Pin: %d\n", this->light_config.led_pin);
    serial->printf("Version: %s\n", this->version.c_str());
    serial->printf("\n========= Node Config  =========\n");
}