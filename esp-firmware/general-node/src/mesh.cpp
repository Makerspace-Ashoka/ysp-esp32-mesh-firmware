#include "mesh.hpp"

/**
 * @brief Start Painless Mesh
 *
 */
void Mesh::init()
{
    mesh.init(config.ssid, config.password, config.scheduler, config.port);
}

/**
 * @brief Subprocess
 *
 */
void Mesh::loop()
{
    mesh.update();
}

void Mesh::sendMessage(uint32_t to = 0, String msg, bool is_broadcast=false)
{
    if (is_broadcast)
    {
        mesh.sendBroadcast(msg);
    }
    else
    {
        mesh.sendSingle(to, msg);
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
vector<uint32_t> pathFinder(painlessmesh::protocol::NodeTree &node, uint32_t end_node)
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
