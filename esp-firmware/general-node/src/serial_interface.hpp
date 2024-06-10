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
#include "mesh.hpp"

class SerialInterface
{
private:
    HardwareSerial *serial;
    Mesh *mesh;
    uint32_t nodeId;
    NodeConfig *nodeConfig;
    void (*sendMessageCallable)(JsonDocument &serial_json_mesh);

    void sendResponse(JsonDocument &response_serial_json, JsonDocument &incoming_serial_json);
    void sendMessage(JsonDocument &incoming_serial_json);
    void setRoomId(JsonDocument &incoming_serial_json);
    void setBaseNetworkCredentials(JsonDocument &incoming_serial_json);
    void sendTopology(bool pretty, JsonDocument &incoming_serial_json);
    //  String getOwnNodeId();

    void getOwnNodeId(JsonDocument &incoming_serial_json);
    void getRoomId(JsonDocument &incoming_serial_json);
    void getWirelessCredentials(JsonDocument &incoming_serial_json);
    void getBaseNetworkCredentials(JsonDocument &incoming_serial_json);

public:

    void setSendMessageCallable(void (*sendMessageCallback)(JsonDocument &stringified_json_mesh));
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
