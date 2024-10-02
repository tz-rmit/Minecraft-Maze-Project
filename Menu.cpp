// Menu.cpp

#include "Agent.h"
#include "Menu.h"
#include "menuUtils.h"
#include <iostream>
#include <limits>

extern void teleportPlayer(const mcpp::Coordinate& coord);


Menu::Menu(bool mode, bool enhance): mode(mode), 
                                    enhance(enhance), 
                                    mc(),
                                    state(States::ST_Main), 
                                    maze(nullptr), 
                                    prevMaze(nullptr)
{
}

States Menu::getState() {
    return state;
}

void Menu::setState(States state) {
    this->state = state;
}

void Menu::MainMenu() {
    printStartText();
    setState(States::ST_Main);
    char selection = '0';
    while(getState() != States::ST_Exit) {
        printMainMenu();
        selection = '0';
        std::cin >> selection;
        if(selection == '1') {
            GenerateMenu();
        } else if(selection == '2') {
            if(prevMaze) {
                delete prevMaze;
            }
            if(maze) {
                maze->BuildMaze();
            } else {
                std::cout << "Error: No maze to build" << std::endl;
            }
        } else if(selection == '3') {
            SolveMenu();
        } else if(selection == '4') {
            printTeamInfo();
        } else if(selection == '5') {
            if(maze) {
                maze->Clean();
            }
            if(prevMaze) {
                prevMaze->Clean();
            }
            setState(States::ST_Exit);
        } else {
            if(std::cin.eof()) {
                setState(States::ST_Exit);
            } else {                
                printMainMenuError();
            }
        }
    }
    printExitMessage();
    return;
}

void Menu::GenerateMenu() {
    setState(States::ST_GenerateMaze);
    while(getState() == States::ST_GenerateMaze) {
        printGenerateMazeMenu();
        char selection = '0';
        std::cin >> selection;
        if(selection == '1') {
            if(maze) {
                prevMaze = maze;
            }
            maze = ReadMaze();
            setState(States::ST_Main);
        } else if(selection == '2') {
            if(maze) {
                prevMaze = maze;
            }
            maze = GenMaze();
            setState(States::ST_Main);
        } else if(selection == '3') {
            setState(States::ST_Main);
        } else {
            if(std::cin.eof()) {
                setState(States::ST_Exit);
            } else {                
                printMainMenuError();
            }
        }
    }
    return;
}

void Menu::SolveMenu() {
    setState(States::ST_SolveMaze);
    char selection = '0';
    while(getState() == States::ST_SolveMaze) {
        printSolveMazeMenu();
        std::cin >> selection;
        if(selection == '1') {
            mcpp::Coordinate tp;
            if(maze) {
                if(mode) {
                    tp = maze->BottomRightCoord();
                } else {
                    tp = maze->RandomCoord();
                }
            } else {
                std::cout << "Error: No maze to solve" << std::endl;
            }
            
            teleportPlayer(tp);
        } else if(selection == '2') {
            ShowRoute();
        } else if(selection == '3') {
            setState(States::ST_Main);
        } else {
            if(std::cin.eof()) {
                setState(States::ST_Exit);
            } else {                
                printSubMenuError();
            }
        }
    }
    return;
}
// ============================ END OF MENU ITEMS ==============================


Maze* Menu::ReadMaze() {
    mcpp::Coordinate basePoint;
    int xlen, zlen;

    bool success = false;
    while(!success) {
        success = ReadMazeInfo(basePoint, xlen, zlen);
    }

    success = false;
    std::vector<std::vector<char>> structure(xlen, std::vector<char>(zlen));
    while(!success) {
        success = ReadMazeStructure(structure);
    }   

    Maze* maze = new Maze(basePoint, xlen, zlen, mode, structure);

    if(maze) {
        std::cout << "Maze read successfully" << std::endl;
        maze->PrintMazeInfo();
    } else {
        std::cout << "Error: Maze not read successfully" << std::endl;
    }
    return maze;    
}

bool Menu::ReadMazeInfo(mcpp::Coordinate& basePoint, int& xlen, int& zlen) {
    bool success = false;
    while(!success) {
        std::cout << "Enter the base point of the maze:" << std::endl;
        int x, y, z;
        std::cin >> x >> y >> z;
        basePoint = mcpp::Coordinate(x, y, z);

        if(!VerifyHeight(x, y, z)) {
            std::cout << "Error: Given height does not match the terrain height" 
                << std::endl;
        } else {
            success = true;
        }
    }
    
    success = false;
    while(!success) {
        std::cout << "Enter the width(x) and length(z) of the maze:" 
            << std::endl;
        std::cin >> xlen >> zlen;

        if(xlen < 1 || zlen < 1) {
            std::cout << "Maze dimensions must both be positive numbers" 
            << std::endl;
        }
        
        else if(!(xlen % 2) || !(zlen % 2)) {
            std::cout << "Maze dimensions must both be odd" << std::endl;
        } 

        else {
            success = true;
        }
    }
    return success;
}

bool Menu::VerifyHeight(int x, int y, int z) {
    return mc.getHeight(x, z) + 1 == y ? true : false;
}

bool Menu::ReadMazeStructure(std::vector<std::vector<char>>& structure) {
    bool success = true;
    std::cout << "Enter the maze structure:" << std::endl;
    for(auto& row : structure) {
        if(success) {
            for(auto& col : row) {
                char input;
                std::cin >> input;
                if(input != '.' && input != 'x') {
                    std::cout << "Error: Incorrent input. " 
                        << "Please only enter either 'x' or '.'" 
                        << std::endl;
                    std::cout << "Please start again" << std::endl;
                    success = false;
                } else {
                    col = input;
                }
            }
        }
        
    }
    return success;    
}

Maze* Menu::GenMaze() {
    mcpp::Coordinate basePoint;
    int xlen, zlen;
    ReadMazeInfo(basePoint, xlen, zlen);
    
    Maze* maze = new Maze(basePoint, xlen, zlen, mode);
    if(maze) {
        std::cout << "Maze generated successfully" << std::endl;
        maze->PrintMazeInfo();
    } else {
        std::cout << "Error: Maze not read successfully" << std::endl;
    }
    
    return maze;
}

void Menu::ShowRoute() {
    if(maze) {
        if(!maze->IsPlayerInMaze()) {
            std::cout << "Player is not in the maze" <<std::endl;
            std::cout 
                << "Please select \"Solve Manually\" to place the player in " 
                << "the maze before selecting \"Show Escape Route\"" 
                << std::endl;
        } else {
            mcpp::Coordinate playerPos = mc.getPlayerPosition();
            // fixing minecraft/mcpp negative coord bug
            if(playerPos.x < 0) {
                playerPos.x--;
            }
            if(playerPos.z < 0) {
                playerPos.z--;
            }
            playerPos.y = mc.getHeight(playerPos.x, playerPos.z) + 1;
            Agent agent(playerPos, mode);
            if(enhance) {
                agent.ShowShortestPath();
            } else {
                agent.SolveMaze();
            }
        }
    } else {
        std::cout << "Error: No maze to solve" << std::endl;
    }
}
