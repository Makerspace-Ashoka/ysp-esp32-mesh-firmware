#include <painlessMesh.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <vector>
#include <sstream>
#include <config.hpp>

using namespace std;



class Mesh
{
private:
    MeshConfig *config;
    painlessMesh mesh;
    NodeConfig *nodeConfig;

    vector<uint32_t> pathFinder (painlessmesh::protocol::NodeTree &node, uint32_t end_node);

public:
    /**
     * @brief The node ID of the mesh network
     */
    void init();

    /**
     * @brief This function should be called in the main loop of the program to keep the mesh network running
     */
    void loop();

    /**
     * @brief This function sends a message to a specific node or to all nodes in the mesh network
     * 
     * @param to destination node_id
     * @param msg payload
     * @param is_broadcast broadcast or single send using painlessMesh built in methods 
     */
    void sendMessage(uint32_t to, String msg, bool is_broadcast);

    /**
     * @brief Get the Topology object
     * 
     * Get the topology for the current mesh network. 
     * 
     * @return String topology of the mesh network. 
     */
    String getTopology(bool pretty);

    /**
     * @brief This function returns the path to a specific node in the mesh network
     * @param endNode: The destination node
     * @return: A vector containing the path to the destination node
     */
    vector<uint32_t> getPathToNode(uint32_t endNode);

    /**
     * @brief This function sets a callback function to be called when a message is received
     * @param callback: The callback function to be called when a message is received
     */
    void onReceive(void (*callback)(uint32_t, String &));

    uint32_t getOwnNodeId();

    /**
     * @brief Construct a new Mesh object
     * 
     * @param node_config 
     */
    Mesh(NodeConfig &node_config);
};