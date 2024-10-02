// Menu.h

#pragma once

#include <mcpp/mcpp.h>
#include "Maze.h"

enum States {
    ST_Main = 0,
    ST_GenerateMaze,
    ST_BuildMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit
};

class Menu {
private:
    bool mode;
    bool enhance;
    mcpp::MinecraftConnection mc;
    States state;
    Maze* maze;
    Maze* prevMaze;
    
public:
    Menu(bool mode, bool enhance);

    States getState();

    void setState(States state);

    void StartUp();

    void MainMenu();

    void GenerateMenu();

    void SolveMenu();

private:
    Maze* ReadMaze();

    bool ReadMazeInfo(mcpp::Coordinate& basePoint, int& xlen, int& zlen);

    bool VerifyHeight(int x, int y, int z);

    bool ReadMazeStructure(std::vector<std::vector<char>>& structure);

    Maze* GenMaze();

    void ShowRoute();
};
