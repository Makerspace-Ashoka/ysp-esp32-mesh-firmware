#include "mesh.hpp"

/**
 * @brief Start Painless Mesh
 *
 */
void Mesh::init()
{
    auto serial = this->nodeConfig->serial_config.serial;

    this->mesh.setDebugMsgTypes(ERROR | DEBUG);

    if (this->config->containsRoot)
    {
        this->mesh.setContainsRoot(true);
    }

    // Set Node to Root only if it's needed
    if (this->config->setRoot)
    {
        this->mesh.setRoot(true);
    }

    this->mesh.init(this->config->ssid.c_str(), this->config->password.c_str(), this->config->scheduler);
    // Update the ID in NodeConfig
    this->nodeConfig->setNodeId(this->mesh.getNodeId());
}

/**
 * @brief Subprocess
 *
 */
void Mesh::loop()
{
    this->mesh.update();
}

void Mesh::sendMessage(uint32_t to, String msg, bool is_broadcast)
{
    if (is_broadcast)
    {
        this->mesh.sendBroadcast(msg);
    }
    else
    {
        this->mesh.sendSingle(to, msg);
    }
}

String Mesh::getTopology(bool pretty = false)
{
    return this->mesh.subConnectionJson(pretty);
}

/**
 * @brief
 *
 * @param callback
 */
void Mesh::onReceive(void (*callback)(uint32_t from, String &msg))
{
    this->mesh.onReceive(callback);
}

/**
 * @brief
 *
 * @param node
 * @param end_node
 * @return vector<uint32_t>
 */
vector<uint32_t> Mesh::pathFinder(painlessmesh::protocol::NodeTree &node, uint32_t end_node)
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
    painlessmesh::protocol::NodeTree node = this->mesh.asNodeTree();
    return pathFinder(node, end_node);
}

uint32_t Mesh::getOwnNodeId()
{
    return this->mesh.getNodeId();
}

/**
 * @brief Construct a new Mesh:: Mesh object
 *
 * @param node_config
 */
Mesh::Mesh(NodeConfig &node_config)
{
    this->config = &node_config.mesh_config;
    this->nodeConfig = &node_config;
}
