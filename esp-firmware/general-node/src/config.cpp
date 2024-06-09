#include "config.hpp"

/**
 * @brief Construct a new Node Config:: Node Config object
 *
 * @param base_ssid Base wireless SSID
 * @param base_password Base Wireless PSK Key
 * @param port Wireless Mesh PORT
 * @param isRoot Weather this node is a root node (There can only be one)
 * @param scheduler Scheduler Instance
 * @param room_id Room Id of this Node
 * @param led_pin
 * @param led_count
 * @param serial
 * @param version
 * @param nv_store_on_set
 */
NodeConfig::NodeConfig(String base_ssid, String base_password, uint16_t port, bool isRoot, Scheduler &scheduler, uint8_t room_id, uint8_t led_pin, uint8_t led_count, HardwareSerial &serial, String version, bool nv_store_on_set)
{
    // TODO: Overwrite Default from NV Store



    this->base_ssid = base_ssid;
    this->base_password = base_password;

    // Room Config
    this->room_config.id = room_id;
    // Mesh Config
    this->setWirelessCredentials();
    this->mesh_config.containsRoot = true;
    this->mesh_config.setRoot = isRoot;
    this->mesh_config.scheduler = &scheduler;
    // Serial Config
    this->serial_config.serial = &serial;
    // Light Config
    this->light_config.effect_speed = 150;
    this->light_config.led_count = led_count;
    this->light_config.led_pin = led_pin;


    // Non - volatile config store
    this->nv_store_on_set = nv_store_on_set;

    this->prefs = new Preferences();

    // Make sure we're able to open the partition in read-write mode
    assert(this->prefs->begin("default",false));
}

void NodeConfig::setNodeId(uint32_t node_id)
{
    this->node_id = node_id;
    this->save();
}

uint32_t NodeConfig::getNodeId()
{
    return this->node_id;
}

void NodeConfig::setRoomId(uint8_t room_id)
{
    this->room_config.id = room_id;
    this->save();
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
    this->save();
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
    this->prefs->putUInt("node-id",this->node_id);
    this->prefs->putInt("room-id",this->room_config.id);
    this->prefs->putString("ssid",this->mesh_config.ssid);
    this->prefs->putString("password",this->mesh_config.password);
}
