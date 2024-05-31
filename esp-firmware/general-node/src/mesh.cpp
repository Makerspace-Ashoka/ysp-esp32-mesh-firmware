#include "mesh.h"

Mesh::Mesh(MeshConfig config)
{
    mesh.setDebugMsgTypes(ERROR | DEBUG);
    mesh.setContainsRoot(config.containsRoot);
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
    if (params.isBroadcast)
    {
        mesh.sendBroadcast(params.msg);
    }
    else
    {
        mesh.sendSingle(params.to, params.msg);
    }
}