#include "serial_interface.hpp"



SerialInterface::SerialInterface(NodeConfig &config)
{
    this->Serial = config.serial_config.serial;
}