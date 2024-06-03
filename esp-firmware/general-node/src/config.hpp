/*
    * Node Config:

        * Light Config:
            * Led Count
            * Led Pin
            *! NeoPixelBus Object ( TBD)

        * Serial Config:
            * Serial Object

        * Mesh Config:
            * SSID
            * PSK
            * Port
            * Contains Root
            * Set Root
            * Scheduler ( User instantiated )

        * Room Config:
            * Room ID
            * Description (optional)
*/

#include <Arduino.h>
#include <string.h>
#include <painlessMesh.h>
/*
 * Config related to painless mesh
 */

class MeshConfig
{
public:
    /*
     * The SSID of the mesh network
     */
    String ssid;

    /*
     * The password of the mesh network
     */
    String password;

    /*
     * The port of the mesh network
     */
    uint16_t port;
    /*
     * Whether the mesh network contains a root node
     */
    bool containsRoot;

    /*
     * Set if this is the root node
     */
    bool setRoot;

    /*
     * Scheduler Instance to be used by the mesh network
     */
    Scheduler *scheduler;
};

class LightConfig
{
public:
    /*
     * To Be Defined Exactly
     */
    uint8_t effect_speed;
    /*
     * NeoPixel Pin number
     */
    uint8_t led_pin;
    /*
     * NeoPixel Pin number
     */
    uint8_t led_count;
};

class SerialConfig
{
public:
    HardwareSerial *serial;
};

/*
 * Contains all info that might vary from node to node ( ssid, number of led's etc)
 */

class RoomConfig
{
public:
    uint8_t id;
};

class NodeConfig
{
public:
    /*
     * Version Number of the Program
     * TODO: AutoGenerate
     */
    String version;
    /*
     * Room Config
     */
    RoomConfig room_config;
    /*
     * Mesh Config
     */
    MeshConfig mesh_config;

    /*
     * LED Config
     * NeoPixel Related Config
     */
    LightConfig light_config;

    /*
     * Serial Config
     * All user config variables pertaining to serial communication bit ( Baud Rate, etc)
     */
    SerialConfig serial_config;

    // Constructor
    NodeConfig(String ssid, String password, uint16_t port = 555, bool isRoot = false, Scheduler *scheduler, uint8_t room_id, uint8_t led_pin, uint8_t led_count, HardwareSerial *serial, String version);
};