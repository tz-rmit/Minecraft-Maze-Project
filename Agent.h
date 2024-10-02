#ifndef ASSIGN3_AGENT_H
#define ASSIGN3_AGENT_H

#include <iostream>
#include <mcpp/mcpp.h>
#include "Node.h"

#define MOVE_EAST mcpp::Coordinate(1,0,0)
#define MOVE_WEST mcpp::Coordinate(-1,0,0)
#define MOVE_SOUTH mcpp::Coordinate(0,0,1)
#define MOVE_NORTH mcpp::Coordinate(0,0,-1)

enum solveAlgorithm{
        RIGHT_HAND_FOLLOW,
        BREATH_FIRST_SEARCH,
};

enum AgentOrientation{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

class Agent {
private:
    mcpp::MinecraftConnection mc;
    AgentOrientation orient;
    mcpp::Coordinate currentCoord;
    mcpp::Coordinate prevCoord;
    bool mode;


public:
    Agent(mcpp::Coordinate startLoc, bool mode);
    ~Agent();

    void SolveMaze();
    void ShowShortestPath();

private:
    bool CheckForward();
    bool CheckRight();

    void TurnLeft();
    void TurnRight();

    void MoveForward();

    void RenderAgent();
    bool IsAtExit();

    bool CheckForwardLeft();
    bool CheckForwardRight();
    bool CheckForwardForwardLeft();
    bool CheckForwardForwardRight();

    void PrintStep(unsigned int& step);


    //EXTENSION CODE
    std::vector<mcpp::Coordinate> ShortestPath();
    std::vector<Node*> FindNeighbours(Node* node);
    bool CheckDirection(Node node, mcpp::Coordinate direction);



};

bool isOutsideMaze(const Node& node);




#endif //ASSIGN3_AGENT_H