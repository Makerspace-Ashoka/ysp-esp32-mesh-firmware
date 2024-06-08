#include "serial_interface.hpp"



SerialInterface::SerialInterface(NodeConfig &config, Mesh &mesh)
{
    this->serial = config.serial_config.serial;
    this->mesh = &mesh;
}


void SerialInterface::sendMessage(String dest_node_id, String serial_payload)
{
    unsigned long destination_node = ; 
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