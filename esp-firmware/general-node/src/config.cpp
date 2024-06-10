#include "config.hpp"

#define RW_MODE false
#define RO_MODE true

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

    auto serial = this->serial_config.serial;

    if (this->prefs->begin("default", RW_MODE))
    {
        if (!this->prefs->putUInt("node-id", this->node_id))
        {
            this->prefs->end();
            serial->printf("node-id save failed\n");
            return false;
        }
        if (!this->prefs->putInt("room-id", this->room_config.id))
        {
            this->prefs->end();
            serial->printf("room-id save failed\n");
            return false;
        }
        if (!this->prefs->putString("base-ssid", this->mesh_config.ssid.c_str()))
        {
            this->prefs->end();
            serial->printf("ssid save failed\n");
            return false;
        }
        if (!this->prefs->putString("base_password", this->mesh_config.password.c_str()))
        {
            this->prefs->end();
            serial->printf("password save failed\n");
            return false;
        }
        this->prefs->end();
        serial->printf("Node Config Saved\n");
        return true;
    }

    return false;
}

bool NodeConfig::load(string name = "defualt")
{
    auto serial = this->serial_config.serial;

    uint8_t failed = 0;
    if (this->prefs->begin("default", RO_MODE))
    {
        if (this->prefs->isKey("node-id"))
        {
            this->node_id = this->prefs->getUInt("node-id", 0);
        }
        else
        {
            failed++;
        }
        if (this->prefs->isKey("room-id"))
        {
            this->room_config.id = this->prefs->getInt("room-id", 0);
        }
        else
        {
            failed++;
        }
        if (this->prefs->isKey("base-ssid"))
        {
            this->mesh_config.ssid = this->prefs->getString("base_ssid", "");
        }
        else
        {
            failed++;
        }
        if (this->prefs->isKey("base-password"))
        {
            this->mesh_config.password = this->prefs->getString("base-password", "");
        }
        else
        {
            failed++;
        }

        this->prefs->end();

        if (failed > 0)
        {
            serial->printf("Node Config Load Failed\n");
            return false;
        }
        serial->printf("Node Config Loaded\n");
        return true;
    }
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