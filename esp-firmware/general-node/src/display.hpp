#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <TFT_eSPI.h>
#include "config.hpp"
#include "mesh.hpp"

class Display {
public:
    Display(NodeConfig &config, Mesh &mesh);

    void init();
    void update();

private:
    Mesh *mesh;
    TFT_eSPI tft;
    int roomid;
};

#endif // DISPLAY_HPP
