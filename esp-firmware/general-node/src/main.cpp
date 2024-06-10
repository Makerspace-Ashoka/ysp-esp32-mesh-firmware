#include "config.hpp"
#include "serial_interface.hpp"
// #include "mesh.hpp"

#define NUM_LED 5
#define LED_PIN 10
#define VERSION "2.0.0"
#define IS_ROOT_NODE false
#define BAUD_RATE 115200

/*
    TODO:
        * Save and Load Config
        * Lighting
        * Mesh class rename to MeshHelper
        * Root
            * auto topology
            * node numbers
            *

*/

void onReceiveCallback(uint32_t from_node_id, String &received_stringified_mesh_json);

// Class Instantiation
Scheduler user_scheduler;

NodeConfig *config = new NodeConfig(IS_ROOT_NODE, user_scheduler, LED_PIN, NUM_LED, Serial, VERSION, false);

Mesh *mesh = new Mesh(*config);

SerialInterface *serial_interface = new SerialInterface(*config, *mesh);

Task task_log_config(TASK_IMMEDIATE, TASK_ONCE, []()
                     { config->logConfig(); });

Task task_process_serial(TASK_IMMEDIATE, TASK_FOREVER, []()
                         { serial_interface->processSerial(); });

void setup()
{
    Serial.begin(BAUD_RATE);
    delay(2000);

    config->resetToDefault();

    Serial.println("Starting Node");
    mesh->init();
    mesh->onReceive(onReceiveCallback);

    user_scheduler.addTask(task_log_config);
    task_log_config.enableDelayed(1000);

    user_scheduler.addTask(task_process_serial);
    task_process_serial.enable();
}

void loop()
{
    mesh->loop();
}

void onReceiveCallback(uint32_t from_node_id, String &received_stringified_mesh_json)
{
    JsonDocument received_serial_mesh_json;
    deserializeJson(received_serial_mesh_json, received_stringified_mesh_json);
    serial_interface->displayLiveMessage(received_serial_mesh_json);
}