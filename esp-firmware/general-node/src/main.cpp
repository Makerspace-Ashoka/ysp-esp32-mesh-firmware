#include "config.hpp"
#include "serial_interface.hpp"
// #include "mesh.hpp"

Scheduler userScheduler;

#define MESH_SSID "whatyoulike"
#define MESH_PASSWORD "soemthingsneeky"
#define MESH_PORT 5555
#define NUM_LED 5
#define LED_PIN 10
#define VERSION "2.0.0"
#define ROOM_ID 0
#define IS_ROOT_NODE false
#define BAUD_RATE 115200


void onReceiveCallback(uint32_t from_node_id, String &received_stringified_mesh_json);

// Class Instantiation
NodeConfig *config = new NodeConfig(MESH_SSID, MESH_PASSWORD, MESH_PORT, IS_ROOT_NODE, userScheduler, ROOM_ID, LED_PIN, NUM_LED, Serial, VERSION, false);

Mesh *mesh = new Mesh(*config);

SerialInterface *serial_interface = new SerialInterface(*config, *mesh);


void setup()
{
    Serial.begin(BAUD_RATE);
    mesh->init();
    mesh->onReceive(onReceiveCallback);
}

void loop()
{
    mesh->loop();
    serial_interface->processSerial();
}

void onReceiveCallback(uint32_t from_node_id, String &received_stringified_mesh_json)
{
    JsonDocument received_serial_mesh_json;
    deserializeJson(received_serial_mesh_json, received_stringified_mesh_json);
    serial_interface->displayLiveMessage(received_serial_mesh_json);
}