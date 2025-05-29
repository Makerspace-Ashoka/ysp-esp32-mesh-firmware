#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <TFT_eSPI.h>
#include "config.hpp"
#include "mesh.hpp"

class Display {
public:
    Display();

    void init(NodeConfig &config, Mesh &mesh);
    void update();
    void boot();
    String lastFromNode;
    String lastToNode;

private:
    Mesh *mesh;
    TFT_eSPI tft;
    int roomId;
    bool isRoot;
};

#endif // DISPLAY_HPP
