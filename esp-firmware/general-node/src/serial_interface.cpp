#include "serial_interface.hpp"

using namespace std;

SerialInterface::SerialInterface(NodeConfig &config, Mesh &mesh)
{
    this->serial = config.serial_config.serial;
    this->mesh = &mesh;
    this->nodeId = config.getNodeId();
}

vector<String> SerialInterface::splitString(const String &str)
{
    vector<String> split_string;
    int start = 0;
    int end = 0;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
        {
            end = i;
            split_string.push_back(str.substring(start, end));
            start = i + 1;
        }
    }
    split_string.push_back(str.substring(start, str.length()));
    return split_string;
}

void SerialInterface::sendMessage(vector<String> &serial_payload_split)
{
    JsonDocument payload;
    String stringified_json_payload;
    unsigned long destination_node = stoul(serial_payload_split[1].c_str(), nullptr, 10);
    payload["from"] = this->nodeId;
    payload["to"] = destination_node;
    payload["HEX"] = serial_payload_split[2];
    payload["msg"] = serial_payload_split[3];
    serializeJson(payload, stringified_json_payload);

    this->mesh->sendMessage(0, stringified_json_payload, true);
}

void SerialInterface::sendTopology()
{
    
}

void SerialInterface::displayLiveMessage(JsonDocument payload)
{
    serializeJson(payload,this->serial);
}

void SerialInterface::processSerial()
{
    if (this->serial->available())
    {
        String command = this->serial->readStringUntil('\n');
        command.trim();
        vector<String> serial_payload_split = splitString(command);
        if (serial_payload_split[0] == "ping")
        {
            sendMessage(serial_payload_split);
        }
        else if (serial_payload_split[0] == "capture-topology")
        {

        }
        

        // process the commands
    }
}