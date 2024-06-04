#include <painlessMesh.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <vector>
#include <sstream>
#include <config.hpp>

using namespace std;

/*
 * This class is used to pass the message
 */
class MessageParams
{
public:
    /*
     * The destination node
     */
    uint32_t to;

    /*
     * The message to be sent
     */
    String msg;

    /*
     * Whether the message is a broadcast
     */
    bool isBroadcast;
};

class Mesh
{
private:
    MeshConfig config;
    painlessMesh mesh;

public:
    /*
     * The node ID of the mesh network
     */
    void init();

    /*
     * This function should be called in the main loop of the program to keep the mesh network running
     */
    void loop();

    /*
     * This function sends a message to a specific node or to all nodes in the mesh network
     * @param params: MessageParams class containing the message to be sent, the destination node and whether the message is a broadcast
     */
    void sendMessage(MessageParams params);

    /*
     * This function returns the topology of the mesh network
     * @return: The topology of the mesh network in JSON format
     */
    JsonDocument getTopology();

    /*
     * This function returns the path to a specific node in the mesh network
     * @param endNode: The destination node
     * @return: A vector containing the path to the destination node
     */
    vector<uint32_t> getPathToNode(uint32_t endNode);

    /*
     * This function sets a callback function to be called when a message is received
     * @param callback: The callback function to be called when a message is received
     */
    void onReceive(void (*callback)(uint32_t, String &));

    /*
     * Constructor
     */
    Mesh(NodeConfig *node_config);
};