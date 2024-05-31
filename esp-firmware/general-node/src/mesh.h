#include <painlessMesh.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <vector>
#include <sstream>

/*
 * This struct is used to pass the mesh configuration
 */
struct MeshConfig
{
    /*
     * The SSID of the mesh network
     */
    const char *ssid;

    /*
     * The password of the mesh network
     */
    const char *password;

    /*
     * The port of the mesh network
     */
    uint16_t port;
    /*
     * Whether the mesh network contains a root node
     */
    bool containsRoot;

    /*
     * Scheduler Instance to be used by the mesh network
     */
    Scheduler *scheduler;
};

/*
 * This struct is used to pass the message
 */
struct MessageParams
{
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
    Mesh(MeshConfig config);
    void init();

    /*
     * This function should be called in the main loop of the program to keep the mesh network running
     */
    void loop();

    /*
     * This function sends a message to a specific node or to all nodes in the mesh network
     * @param params: MessageParams struct containing the message to be sent, the destination node and whether the message is a broadcast
     */
    void sendMessage(MessageParams params);

    /*
     * This function returns the topology of the mesh network
     * @return: The topology of the mesh network in JSON format
     */
    JsonDocument getTopology();

    /*
     * This function returns the path to a specific node in the mesh network
     * @param endnode: The destination node
     * @return: A vector containing the path to the destination node
     */
    std::vector<uint32_t> getPathToNode(uint32_t endnode);

    /*
     * This function sets a callback function to be called when a message is received
     * @param callback: The callback function to be called when a message is received
     */
    void onMessageReceived(std::function<void(uint32_t, String &)> callback);
};