#include "serial_interface.hpp"

using namespace std;

SerialInterface::SerialInterface(NodeConfig &config, Mesh &mesh)
{
    this->serial = config.serial_config.serial;
    this->mesh = &mesh;
    this->nodeId = config.getNodeId();
}


void SerialInterface::sendMessage(vector<String> &serial_payload)
{
    
    JsonDocument payload;
    unsigned long destination_node = stoul(.c_str(), nullptr, 10);
    payload["from"] = this->nodeId;
    payload["to"] = destination_node;

    this->mesh->sendMessage(,serial_payload);
}

void SerialInterface::displayLiveMessage(JsonDocument payload)
{
    serializeJson(payload,this->serial);
}

void SerialInterface::processSerial()
{
    if (this->serial->available())
    {

        // process the commands
    }
}