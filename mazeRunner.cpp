#include <iostream>
#include <mcpp/mcpp.h>
#include <cstring>
#include "Maze.h"
#include "Agent.h"
#include "Menu.h"

#define NORMAL_MODE 0
#define TESTING_MODE 1

int main(int argc, char** argv){
    bool mode = NORMAL_MODE;
    bool enhance = false;
    //read Mode
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-testmode") == 0) {
            std::cout << "[Test mode]" << std::endl;
            mode = TESTING_MODE;
        } else if(strcmp(argv[i], "-e") == 0 || 
                strcmp(argv[i], "-enhancements") == 0 ||
                strcmp(argv[i], "-enhance") == 0) {
            std::cout << "[Enhancements enabled]" << std::endl;
            enhance = true;
        }
    }

    mcpp::MinecraftConnection mc; 
    mc.doCommand("time set day");
    mc.doCommand("gamerule randomTickSpeed 0");

    Menu menu(mode, enhance);
    menu.MainMenu();

    return EXIT_SUCCESS;
}

void teleportPlayer(const mcpp::Coordinate& coord) {
    mcpp::MinecraftConnection mc;
    std::string command = "teleport @p ";
    command += std::to_string(coord.x) + " ";
    command += std::to_string(coord.y) + " ";
    command += std::to_string(coord.z);
    mc.doCommand(command);
}