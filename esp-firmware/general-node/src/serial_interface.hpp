/*
 * General
 * Send Message ( Actually a Broadcast)
 * Get Node_List
 * Get Own Node ID
 *
 * Debug
 * Get Topology
 * Config
 * Get Current Config
 * Set Wireless Credentials
 * Set Room ID
 */
#include <HardwareSerial.h>
#include <ArduinoJson.h>
#include <painlessMesh.h>
#include "config.hpp"
#include "mesh.hpp"

class SerialInterface
{
private:
    HWCDC *serial;
    Mesh *mesh;
    // String getReceivedMessages(int count=1);
    void sendMessage(String dest_node_id, String serial_payload);
    //  String getOwnNodeId();
public:
    /**
      * @brief logs the received payload on the serial interface
      *
      * @param payload  json object of the received message.
      */
    void displayLiveMessage(JsonDocument payload);

    /**
     * @brief Performs serial interface sub-routine. Put this in your void loop();
     *
     *  Performs the following when called.
     *      1. Checks the Serial Buffer
     *      2. Performs relevant action else discard
     */
    void processSerial();

    /**
     * @brief Construct a new serial interface object
     *
     * @param serial Reference to already instantiated HardwareSerial object
     */
    SerialInterface(NodeConfig &config, Mesh &mesh);
};
