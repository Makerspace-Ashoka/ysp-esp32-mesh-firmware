#include "mesh.hpp"

Mesh::Mesh(MeshConfig config)
{
    mesh.setDebugMsgTypes(ERROR | DEBUG);
    mesh.setContainsRoot(config.containsRoot);
    this->nodeId = mesh.getNodeId();
}

void Mesh::init()
{
    mesh.init(config.ssid, config.password, config.scheduler, config.port);
}

void Mesh::loop()
{
    mesh.update();
}

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

void Mesh::onReceive(void (*callback)(uint32_t from, String &msg))
{
    mesh.onReceive(callback);
}


vector<uint32_t> Mesh::getPathToNode(uint32_t endNode)
{

    return vector<uint32_t>();
}