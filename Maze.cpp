#include "Maze.h"
#include <random>
#include <ctime>
#include <list>
#include <thread>
#include <chrono>
#include <string>
#include <unordered_set>

extern void teleportPlayer(const mcpp::Coordinate& coord);

Maze::Maze(): basePoint(mcpp::Coordinate()), xlen(), zlen(), mode(), 
    structure(), overriddenBlocks()    
{
}

Maze::Maze(mcpp::Coordinate basePoint, int xlen, 
                                    int zlen,
                                    bool mode,
                                    std::vector<std::vector<char>> structure)
                                    : basePoint(basePoint), 
                                    xlen(xlen), 
                                    zlen(zlen), 
                                    mode(mode), 
                                    structure(structure) {}


Maze::Maze(mcpp::Coordinate basePoint, int xlen, 
                                    int zlen,
                                    bool mode)
                                    : basePoint(basePoint), 
                                    xlen(xlen), 
                                    zlen(zlen), 
                                    mode(mode)
{
    GenerateRandomMaze();
}

Maze::~Maze()
{
    Clean();
}

void Maze::GenerateRandomMaze() {
    std::srand(std::time(nullptr));
    // get blank maze
    structure = BlankMaze(xlen, zlen);

    std::vector<std::vector<bool>> 
        visited(xlen, std::vector<bool>(zlen, false));

    // get random starting point
    Vect2D start;
    if(mode == true) {
        start.x = 1;
        start.z = 0;
    } else {
        start = RandStartPoint();
    }
    structure[start.x][start.z] = '.';

    if(start.x != 0) {
        start.x = (start.x - 1)/2;
    }
    if(start.z != 0) {
        start.z = (start.z - 1)/2;
    }
    
    RecursiveBacktracking(visited, start);
}

void Maze::RecursiveBacktracking(std::vector<std::vector<bool>>& visited, 
        const Vect2D prevNode) {
    visited[prevNode.x][prevNode.z] = true;
    
    // add neighbours in random order 
    std::vector<Vect2D> neighbours = prevNode.Neighbours(mode, (xlen-1)/2, 
                                                        (zlen-1)/2, visited);

    for(auto iter = neighbours.begin(); iter != neighbours.end(); iter++) {
        // if node has not yet been visited
        if(visited[iter->x][iter->z] == false) {
            // add to visted
            visited[iter->x][iter->z] = true;

            // get the positions of two given nodes in the actual maze
            int x = iter->x * 2 + 1;
            int z = iter->z * 2 + 1;

            int prevX = prevNode.x * 2 + 1;
            int prevZ = prevNode.z * 2 + 1;

            // find the coord of the wall between the two points
            int wallX = (prevX + x) / 2;
            int wallZ = (prevZ + z) / 2;

            // change relevant wall('x') to '.'
            structure[wallX][wallZ] = '.';
            
            // recurse
            RecursiveBacktracking(visited, *iter);
        }
    }
}

std::vector<std::vector<char>> BlankMaze(int xlen, int zlen) {
    std::vector<std::vector<char>> struc(xlen, std::vector<char>(zlen, '.'));

    for(int row = 0; row < xlen; row++) {
        for(int col = 0; col < zlen; col++) {
            if(row % 2 == 0 || col % 2 == 0) {
                struc[row][col] = 'x';
            }
        }
    }

    return struc;
}

Vect2D Maze::RandStartPoint() {
    Vect2D start;
    // int perim = ((xlen - 2) * 2 + (zlen - 2) * 2) - 1;
    int perim = (xlen-1) + (zlen-1) - 1;
    // range from 0 to perim-1)
    int ran = (rand() % perim);
    if(ran < (zlen - 1)/2) {
        start.x = 0;
        // *2+1 so that coord is now for full maze
        start.z = ran * 2 + 1;
    } else if(ran < zlen - 1) {
        start.x = xlen - 1;
        start.z = 2 * ran - zlen + 2;
    } else if(ran < zlen - 1 + (xlen - 1)/2) {
        start.x = 2 * (ran - zlen) + 3;
        start.z = 0;
    } else {
        start.x = 2 * (ran - zlen + 2) - xlen;
        start.z = zlen - 1;
    }
    return start;
}

void Maze::PrintMazeInfo() {
    std::cout << std::endl;
    std::cout << "**Printing Maze**" << std::endl;
    std::cout << "BasePoint: (" << basePoint.x << ", "
            << basePoint.y << ", "
            << basePoint.z << ") " << std::endl;
    std::cout << "Structure:" << std::endl;
    for(auto row : structure) {
        for(auto col : row) {
            std::cout << col;
        }
        std::cout << std::endl;
    }
    std::cout << "**End Printing Maze**" << std::endl;
}

void Maze::BuildMaze() {
    mcpp::Coordinate tp = basePoint;
    tp.y += 10;
    teleportPlayer(tp);
    
    mcpp::MinecraftConnection mc;

    mcpp::Coordinate start = basePoint + mcpp::Coordinate(0, -1, 0);
    mcpp::Coordinate end = start + mcpp::Coordinate(xlen - 1, 0, zlen - 1);
    std::vector<std::vector<int>> heights = mc.getHeights(start, end);

    // get min and max heights
    int min = basePoint.y - 1;
    int max = basePoint.y - 1;
    for(auto row : heights) {
        for(auto col : row) {
            if(col > max) max = col;
            if(col < min) min = col;
        }
    }
    min++;
    if(max < basePoint.y + 2) max = basePoint.y + 2;

    mcpp::Coordinate saveStart = start;
    saveStart.y = min;
    mcpp::Coordinate saveEnd = end;
    saveEnd.y = max;

    // save the blocks in that area
    cleanStart = saveStart;
    overriddenBlocks = mc.getBlocks(saveStart, saveEnd);

    // flatten above
    mcpp::Coordinate flatStart = saveStart;
    flatStart.y = basePoint.y;
    mcpp::Coordinate flatEnd = saveEnd;
    flatEnd.y = max;
    mc.setBlocks(flatStart, flatEnd, mcpp::Blocks::AIR);

    heights = mc.getHeights(start, end);

    // raise terrain below
    for(int row = 0; row < xlen; row++) {
        for(int col = 0; col < zlen; col++) {
            if(heights[row][col] < basePoint.y - 1) {
                //fill
                mcpp::Coordinate 
                    coord = basePoint + mcpp::Coordinate(row, 0, col);
                coord.y = heights[row][col];
                mcpp::BlockType block = mc.getBlock(coord);
                for(int i = 0; i < basePoint.y - heights[row][col]; i++) {
                    mc.setBlock(coord + mcpp::Coordinate(0, i, 0), block);
                }
            }
        }
    }

    // build walls
    BuildWalls();
}

void Maze::BuildWalls() {
    mcpp::MinecraftConnection mc;

    for(int row = 0; row < xlen; row++) {
        for(int col = 0; col < zlen; col++) {
            for(int y = 0; y < 3; y++){
                if(structure[row][col] == 'x') {
                    // set to acacia
                    mc.setBlock(basePoint + mcpp::Coordinate(row, y, col), 
                        mcpp::Blocks::ACACIA_WOOD_PLANK);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
        }
    }
}

void Maze::Clean() {
    mcpp::MinecraftConnection mc;
    int xIndex = 0;
    for(auto x : overriddenBlocks) {
        int yIndex = 0;
        for(auto y : x) {
            int zIndex = 0;
            for(auto block : y) {
                mcpp::Coordinate offset(yIndex, xIndex, zIndex);
                mc.setBlock(cleanStart + offset, block);
                zIndex++;
            }
            yIndex++;
        }
        xIndex++;
    }
}

mcpp::Coordinate Maze::RandomCoord() {
    std::srand(std::time(nullptr));
    int x = 0;
    int z = 0;
    while(structure[x][z] == 'x') {
        x = rand() % xlen;
        z = rand() % zlen;
    }
    return mcpp::Coordinate(basePoint.x + x, basePoint.y, basePoint.z + z);
}

mcpp::Coordinate Maze::BottomRightCoord() {
    return mcpp::Coordinate(basePoint.x + xlen - 2, 
            basePoint.y + 1, basePoint.z + zlen - 2);
}

bool Maze::IsPlayerInMaze() {
    mcpp::MinecraftConnection mc;
    bool inside = true;
    mcpp::Coordinate playerPos = mc.getPlayerPosition();
    // fixing minecraft/mcpp negative coord bug
    if(playerPos.x < 0) {
        playerPos.x--;
    }
    if(playerPos.z < 0) {
        playerPos.z--;
    }
    if(playerPos.y > basePoint.y + 3 || playerPos.y < basePoint.y) {
        inside = false;
    } else if(playerPos.x > basePoint.x + xlen || playerPos.x < basePoint.x) {
        inside = false;
    } else if(playerPos.z > basePoint.z + zlen || playerPos.z <= basePoint.z) {
        inside = false;
    }
    return inside;
}

void Maze::SolveManually() {
    mcpp::MinecraftConnection mc;
    std::srand(std::time(nullptr));
    int x = 0;
    int z = 0;

    while(structure[x][z] != '.') {
        x = (rand() % xlen - 2) + 1;
        z = (rand() % zlen - 2) + 1;
    }

    mcpp::Coordinate tp(basePoint.x + x, basePoint.y + 1, basePoint.z + 1);

    teleportPlayer(tp);
}

//=================================ENHANCEMENTS=================================
