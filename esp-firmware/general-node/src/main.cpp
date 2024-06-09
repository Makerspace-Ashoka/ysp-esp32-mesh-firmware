#include "config.hpp"
#include "serial_interface.hpp"

Scheduler userScheduler;

#define MESH_SSID "whatyoulike"
#define MESH_PASSWORD "soemthingsneeky"
#define MESH_PORT 5555
#define NUM_LED 5
#define LED_PIN 10
#define VERSION "2.0.0"
#define ROOM_ID 0
#define IS_ROOT_NODE false

// Class Instantiation
NodeConfig *config = new NodeConfig(MESH_SSID, MESH_PASSWORD, MESH_PORT, false, userScheduler, ROOM_ID, LED_PIN, NUM_LED, Serial, VERSION, false);

Mesh *mesh = new Mesh(*config);

SerialInterface *serial_interface = new SerialInterface(*config, *mesh);

void setup()
{
}

void loop()
{
    mesh->loop();
}