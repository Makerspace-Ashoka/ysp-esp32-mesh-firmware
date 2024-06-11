#include "config.hpp"
#include "serial_interface.hpp"
#include "lighting.hpp"
// #include "mesh.hpp"

#define NUM_LED 5
#define LED_PIN 10
#define VERSION "2.0.0"
#define IS_ROOT_NODE false
#define BAUD_RATE 115200
#define NV_STORE_ON_SET true
//
#define DELAYED_BOOT_START 5000

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
void processLightingOnMessageReceive(JsonDocument &received_serial_mesh_json);

void sendMeshMessageCallback(JsonDocument &stringified_json_mesh);

void pathLighting();

// Class Instantiation
Scheduler user_scheduler;
Preferences prefs;
NodeConfig *config;
Mesh *mesh;
SerialInterface *serial_interface;
Lighting *lighting;

Task task_log_config(TASK_IMMEDIATE, TASK_ONCE, []()
                     { config->logConfig(); });

Task task_process_serial(TASK_IMMEDIATE, TASK_FOREVER, []()
                         { serial_interface->processSerial(); });


// Root Node Task 
// Task auto_dump_esp_counter(TASK_SECOND * 5, TASK_FOREVER, []()
//                            { Serial.printf("Node Count: %d\n", mesh->getNodesCount()); });

// Task auto_dump_esp_topology(TASK_SECOND * 5, TASK_FOREVER, []()
//                             { Serial.println(mesh->getTopology(true)); });

/*
    -using TaskCallback and TaskOnDisable-
    Positional Arguments:
    Task(unsigned long aInterval, long aIterations, TaskCallback aCallback, Scheduler* aScheduler, bool aEnable, TaskOnEnable aOnEnable, TaskOnDisable aOnDisable, bool aSelfdestruct);
*/
Task task_light_animate(TASK_IMMEDIATE, NUM_LED, []()
                        { lighting->lightAnimate(); }, NULL, false, NULL, []()
                        { lighting->lightReset(); });

Task task_path_lighting(TASK_IMMEDIATE, TASK_ONCE, &pathLighting);

void setup()
{
    Serial.begin(BAUD_RATE);

    ulong start_time = millis();

    while ((millis() - start_time) < DELAYED_BOOT_START)
    {
        Serial.printf(". ");
        delay(1000);
    }
    Serial.print("Starting Node...\n");

    if (!prefs.begin("node_config", false))
    {
        Serial.println("Failed to open Preferences");
        prefs.clear();
    }

    config = new NodeConfig(IS_ROOT_NODE, user_scheduler, LED_PIN, NUM_LED, Serial, VERSION, prefs, NV_STORE_ON_SET);
    mesh = new Mesh(*config);
    serial_interface = new SerialInterface(*config, *mesh);
    serial_interface->setSendMessageCallable(&sendMeshMessageCallback);
    lighting = new Lighting(*config, *mesh);

    delay(100);

    // Start the mesh
    mesh->init();
    mesh->onReceive(onReceiveCallback);

    user_scheduler.addTask(task_log_config);
    task_log_config.enableDelayed(1000);

    user_scheduler.addTask(task_process_serial);
    task_process_serial.enable();

    user_scheduler.addTask(task_light_animate);
    // task_light_animate.enable();

    user_scheduler.addTask(task_path_lighting);

    // user_scheduler.addTask(auto_dump_esp_counter);
    // auto_dump_esp_counter.enable();

    // user_scheduler.addTask(auto_dump_esp_topology);
    // auto_dump_esp_topology.enable();
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
    processLightingOnMessageReceive(received_serial_mesh_json);
}

void processLightingOnMessageReceive(JsonDocument &received_serial_mesh_json)
{
    if (received_serial_mesh_json["lighting"]["color"] != NULL)
    {
        lighting->setLightColor(received_serial_mesh_json["lighting"]["color"].as<String>());
        task_light_animate.enable();
    }
}

void sendMeshMessageCallback(JsonDocument &serial_json_mesh)
{
    String stringified_json_mesh;
    serializeJson(serial_json_mesh, stringified_json_mesh);
    mesh->sendMessage(0, stringified_json_mesh, true);
    if (serial_json_mesh["payload"]["HEX"] != "false")
    {
        serial_json_mesh["lighting"]["color"] = serial_json_mesh["payload"]["HEX"];
        lighting->setEndNodeId(serial_json_mesh["payload"]["to_node_id"].as<String>());
        lighting->setLightColor(serial_json_mesh["payload"]["HEX"].as<String>());
        task_light_animate.enable();
        task_path_lighting.enable();
    }
}

void pathLighting()
{
    lighting->pathFinder();
    lighting->sendPathLightingMessages();
}