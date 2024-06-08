#include "config.hpp"
#include "mesh.hpp"
#include "serial_interface.hpp"

Scheduler userScheduler;

#define MESH_SSID "whatyoulike1"
#define MESH_PASSWORD "soemthingsneeky"
#define MESH_PORT 5555
#define NUM_LED 5
#define LED_PIN 10
#define VERSION "2.0.0"
#define ROOM_ID 0
#define IS_ROOT_NODE false

// Class Instantiation
NodeConfig *config = new NodeConfig(
    MESH_SSID, MESH_PASSWORD, MESH_PORT, IS_ROOT_NODE, userScheduler, ROOM_ID, NUM_LED, LED_PIN, Serial, VERSION);

Mesh *mesh = new Mesh(*config);

SerialInterface* serial_interface = new SerialInterface(*config, *mesh);

void setup()
{
}

void loop()
{
    mesh->loop();
}