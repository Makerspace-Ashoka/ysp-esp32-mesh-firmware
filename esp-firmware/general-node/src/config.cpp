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
NodeConfig::NodeConfig(String base_ssid, String base_password, uint16_t port = 555, bool isRoot = false, Scheduler &scheduler, uint8_t room_id, uint8_t led_pin, uint8_t led_count, HWCDC &serial, String version, bool nv_store_on_set = false)
{
    // Mesh Config
    vector<String> creds = this->getWirelessCredentialsFromRoomId(base_ssid, base_password, room_id);

    // Make sure we got both sssid and password on index 0 and 1 respectively
    assert(creds.size() == 2);

    this->mesh_config.ssid = creds[0];
    this->mesh_config.password = creds[1];
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

vector<String> NodeConfig::getWirelessCredentialsFromRoomId(String base_ssid, String base_password, uint8_t room_id)
{
    return {base_ssid + "-" + room_id, base_password + "-" + room_id};
}
