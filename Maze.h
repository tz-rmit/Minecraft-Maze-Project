#ifndef ASSIGN_MAZE_H
#define ASSIGN_MAZE_H

#include <vector>
#include <stack>
#include <mcpp/mcpp.h>
#include <tuple>
#include <list>
#include "Vect2D.h"

enum Direction { UP, DOWN, LEFT, RIGHT };

class Maze
{
private:
    mcpp::Coordinate basePoint;
    int xlen; 
    int zlen;
    bool mode;
    std::vector<std::vector<char>> structure;
    std::vector<std::vector<std::vector<mcpp::BlockType>>> overriddenBlocks;
    mcpp::Coordinate cleanStart;

public:
    Maze();
    
    Maze(mcpp::Coordinate basePoint, int xlen, int zlen, bool mode,
                                std::vector<std::vector<char>> structure);

    Maze(mcpp::Coordinate basePoint, int xlen, 
                                        int zlen,
                                        bool mode);

    ~Maze();

    void SolveManually();
    void PrintMazeInfo();
    void BuildMaze();
    void Clean();
    mcpp::Coordinate RandomCoord();
    mcpp::Coordinate BottomRightCoord();
    bool IsPlayerInMaze();
    
    void GenerateRandomMaze();


private:
    void ClearBuildZone(std::stack<std::tuple<mcpp::BlockType, mcpp::Coordinate>>&);
    void BuildPlatform(std::stack<std::tuple<mcpp::BlockType, mcpp::Coordinate>>&);
    void BuildWalls();

    void RecursiveBacktracking(std::vector<std::vector<bool>>& visited, const Vect2D prevNode);

    Vect2D RandStartPoint();

};

std::vector<std::vector<char>> BlankMaze(int xlen, int zlen);

#endif //ASSIGN_MAZE_H
