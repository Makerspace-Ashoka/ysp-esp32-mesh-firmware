#include "mesh.hpp"

/**
 * @brief 
 * 
 */
void Mesh::init()
{
    mesh.init(config.ssid, config.password, config.scheduler, config.port);
}

/**
 * @brief 
 * 
 */
void Mesh::loop()
{
    mesh.update();
}

/**
 * @brief 
 * 
 * @param params 
 */
void Mesh::sendMessage(MessageParams params)
{

    // TODO: Add Destination Node ID to the message string

    if (params.isBroadcast)
    {
        mesh.sendBroadcast(params.msg);
    }
    else
    {
        mesh.sendSingle(params.to, params.msg);
    }
}

/**
 * @brief 
 * 
 * @param callback 
 */
void Mesh::onReceive(void (*callback)(uint32_t from, String &msg))
{
    mesh.onReceive(callback);
}

/**
 * @brief 
 * 
 * @param node 
 * @param end_node 
 * @return vector<uint32_t> 
 */
vector<uint32_t> pathFinder(painlessmesh::protocol::NodeTree &node,uint32_t end_node)
{
    if (node.nodeId == end_node)
    {
        return {node.nodeId};
    }

    if (node.subs.size())
    {
        for (painlessmesh::protocol::NodeTree &child : node.subs)
        {
            vector<uint32_t> path = pathFinder(child, end_node);
            if (!path.empty())
            {
                path.push_back(node.nodeId);
                return path;
            }
        }
    }
    return {};
}
/**
 * @brief 
 * 
 * @param end_node 
 * @return vector<uint32_t> 
 */
vector<uint32_t> Mesh::getPathToNode(uint32_t end_node)
{
    painlessmesh::protocol::NodeTree node = mesh.asNodeTree();
    return pathFinder(node, end_node);

}
 
/**
 * @brief Construct a new Mesh:: Mesh object
 * 
 * @param node_config 
 */
Mesh::Mesh(NodeConfig &node_config)
{
    this->config = node_config.mesh_config;

    mesh.setDebugMsgTypes(ERROR | DEBUG);

    if (config.containsRoot)
    {
       mesh.setContainsRoot(true);
    }

    // Set Node to Root only if it's needed
    if (config.setRoot)
    {
        mesh.setRoot(true);
    }

    // Update the ID in NodeConfig
    node_config.setNodeId(mesh.getNodeId());
}
