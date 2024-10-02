#include "Agent.h"
#include <chrono>
#include <thread>
#include <unordered_set>
#include <queue>
#include <algorithm>

Agent::Agent(mcpp::Coordinate startLoc, bool mode)
        : mc(), 
        orient(AgentOrientation::NORTH),
        currentCoord(startLoc), 
        prevCoord(startLoc),
        mode(mode)
{
}

Agent::~Agent()
{
}

bool Agent::CheckForward() {
    bool open = false;
    mcpp::Coordinate checkCoord = currentCoord;
    if(orient == AgentOrientation::NORTH) {
        checkCoord = checkCoord + MOVE_NORTH;
    } else if(orient == AgentOrientation::EAST) {
        checkCoord = checkCoord + MOVE_EAST;
    } else if(orient == AgentOrientation::SOUTH) {
        checkCoord = checkCoord + MOVE_SOUTH;
    } else if(orient == AgentOrientation::WEST) {
        checkCoord = checkCoord + MOVE_WEST;
    }

    if(mc.getBlock(checkCoord) == mcpp::Blocks::AIR) {
        open = true;
    }
    return open;
}

bool Agent::CheckRight() {
    bool open = false;
    mcpp::Coordinate checkCoord = currentCoord;
    if(orient == AgentOrientation::NORTH) {
        checkCoord = checkCoord + MOVE_EAST;
    } else if(orient == AgentOrientation::EAST) {
        checkCoord = checkCoord + MOVE_SOUTH;
    } else if(orient == AgentOrientation::SOUTH) {
        checkCoord = checkCoord + MOVE_WEST;
    } else if(orient == AgentOrientation::WEST) {
        checkCoord = checkCoord + MOVE_NORTH;
    }

    if(mc.getBlock(checkCoord) == mcpp::Blocks::AIR) {
        open = true;
    }
    return open;
}

void Agent::TurnLeft() {
    if(orient == AgentOrientation::NORTH) {
        orient = AgentOrientation::WEST;
    } else if(orient == AgentOrientation::EAST) {
        orient = AgentOrientation::NORTH;
    } else if(orient == AgentOrientation::SOUTH) {
        orient = AgentOrientation::EAST;
    } else if(orient == AgentOrientation::WEST) {
        orient = AgentOrientation::SOUTH;
    }
}

void Agent::TurnRight() {
    if(orient == AgentOrientation::NORTH) {
        orient = AgentOrientation::EAST;
    } else if(orient == AgentOrientation::EAST) {
        orient = AgentOrientation::SOUTH;
    } else if(orient == AgentOrientation::SOUTH) {
        orient = AgentOrientation::WEST;
    } else if(orient == AgentOrientation::WEST) {
        orient = AgentOrientation::NORTH;
    }
}

void Agent::MoveForward() {
    prevCoord = currentCoord;
    if(orient == AgentOrientation::NORTH) {
        currentCoord = currentCoord + MOVE_NORTH;
    } else if(orient == AgentOrientation::EAST) {
        currentCoord = currentCoord + MOVE_EAST;
    } else if(orient == AgentOrientation::SOUTH) {
        currentCoord = currentCoord + MOVE_SOUTH;
    } else if(orient == AgentOrientation::WEST) {
        currentCoord = currentCoord + MOVE_WEST;
    }
    RenderAgent();
}

void Agent::RenderAgent() {
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
    mc.setBlock(prevCoord, mcpp::Blocks::AIR);
    mc.setBlock(currentCoord, mcpp::Blocks::LIME_CARPET);
}

bool Agent::IsAtExit() {
    bool out = true;
    out = CheckForwardLeft();
    if(out) {
        out = CheckForward();
    }
    if(out) {
        out = CheckForwardRight();
    }
    if(out) {
        out = CheckForwardForwardLeft();
    }
    if(out) {
        out = CheckForwardForwardRight();
    }
    return out;    
}

void Agent::SolveMaze() {
    if(mode) {
        // in test mode turn player clockwise until there is a wall on the right
        // if there are no walls around then continue as normal
        for(int i = 0; CheckRight() && i < 4; i++) {
            TurnRight();
        }
    }
    
    mc.setBlock(currentCoord, mcpp::Blocks::LIME_CARPET);
    unsigned int step = 1;
    PrintStep(step);
    while (!IsAtExit()) {
        if (CheckRight()) {
            TurnRight();
            MoveForward();
            PrintStep(step);
        } else if (CheckForward()) {
            MoveForward();
            PrintStep(step);
        } else {
            TurnLeft();
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    mc.setBlock(currentCoord, mcpp::Blocks::AIR);
}

bool Agent::CheckForwardLeft() {
    bool open = true;
    mcpp::Coordinate checkCoord = currentCoord;
    if(orient == AgentOrientation::NORTH) {
        checkCoord = checkCoord + MOVE_NORTH;
        checkCoord = checkCoord + MOVE_WEST;
    } else if(orient == AgentOrientation::EAST) {
        checkCoord = checkCoord + MOVE_EAST;
        checkCoord = checkCoord + MOVE_NORTH;
    } else if(orient == AgentOrientation::SOUTH) {
        checkCoord = checkCoord + MOVE_SOUTH;
        checkCoord = checkCoord + MOVE_EAST;
    } else if(orient == AgentOrientation::WEST) {
        checkCoord = checkCoord + MOVE_WEST;
        checkCoord = checkCoord + MOVE_SOUTH;
    }
    if(mc.getBlock(checkCoord) == mcpp::Blocks::ACACIA_WOOD_PLANK) {
        open = false;
    }
    return open;
}

bool Agent::CheckForwardRight() {
    bool open = true;
    mcpp::Coordinate checkCoord = currentCoord;
    if(orient == AgentOrientation::NORTH) {
        checkCoord = checkCoord + MOVE_NORTH;
        checkCoord = checkCoord + MOVE_EAST;
    } else if(orient == AgentOrientation::EAST) {
        checkCoord = checkCoord + MOVE_EAST;
        checkCoord = checkCoord + MOVE_SOUTH;
    } else if(orient == AgentOrientation::SOUTH) {
        checkCoord = checkCoord + MOVE_SOUTH;
        checkCoord = checkCoord + MOVE_WEST;
    } else if(orient == AgentOrientation::WEST) {
        checkCoord = checkCoord + MOVE_WEST;
        checkCoord = checkCoord + MOVE_SOUTH;
    }
    if(mc.getBlock(checkCoord) == mcpp::Blocks::ACACIA_WOOD_PLANK) {
        open = false;
    }
    return open;
}

bool Agent::CheckForwardForwardLeft() {
    bool open = true;
    mcpp::Coordinate checkCoord = currentCoord;
    if(orient == AgentOrientation::NORTH) {
        checkCoord = checkCoord + MOVE_NORTH + MOVE_NORTH;
        checkCoord = checkCoord + MOVE_WEST;
    } else if(orient == AgentOrientation::EAST) {
        checkCoord = checkCoord + MOVE_EAST + MOVE_EAST;
        checkCoord = checkCoord + MOVE_NORTH;
    } else if(orient == AgentOrientation::SOUTH) {
        checkCoord = checkCoord + MOVE_SOUTH + MOVE_SOUTH;
        checkCoord = checkCoord + MOVE_EAST;
    } else if(orient == AgentOrientation::WEST) {
        checkCoord = checkCoord + MOVE_WEST + MOVE_WEST;
        checkCoord = checkCoord + MOVE_SOUTH;
    }
    if(mc.getBlock(checkCoord) == mcpp::Blocks::ACACIA_WOOD_PLANK) {
        open = false;
    }
    return open;
}

bool Agent::CheckForwardForwardRight() {
    bool open = true;
    mcpp::Coordinate checkCoord = currentCoord;
    if(orient == AgentOrientation::NORTH) {
        checkCoord = checkCoord + MOVE_NORTH + MOVE_NORTH;
        checkCoord = checkCoord + MOVE_EAST;
    } else if(orient == AgentOrientation::EAST) {
        checkCoord = checkCoord + MOVE_EAST + MOVE_EAST;
        checkCoord = checkCoord + MOVE_SOUTH;
    } else if(orient == AgentOrientation::SOUTH) {
        checkCoord = checkCoord + MOVE_SOUTH + MOVE_SOUTH;
        checkCoord = checkCoord + MOVE_WEST;
    } else if(orient == AgentOrientation::WEST) {
        checkCoord = checkCoord + MOVE_WEST + MOVE_WEST;
        checkCoord = checkCoord + MOVE_SOUTH;
    }
    if(mc.getBlock(checkCoord) == mcpp::Blocks::ACACIA_WOOD_PLANK) {
        open = false;
    }
    return open;
}

void Agent::PrintStep(unsigned int& step) {
    std::cout << "[STEP " << std::to_string(step) << "]: (" << currentCoord.x
        << ", " << currentCoord.y << ", " << currentCoord.z << ")" << std::endl;
    step++;
}


//===============================EXTENSION CODE=================================
void Agent::ShowShortestPath() {
    std::vector<mcpp::Coordinate> path = ShortestPath();
    prevCoord = path[0];
    unsigned int step = 1;

    for(mcpp::Coordinate coord : path) {
        currentCoord = coord;
        RenderAgent();
        PrintStep(step);
        prevCoord = coord;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(750));
    mc.setBlock(currentCoord, mcpp::Blocks::AIR);
}

struct NodeComparator {
    mcpp::Coordinate coord;

    NodeComparator(mcpp::Coordinate coord) : coord(coord) {}

    bool operator()(const Node* node) const {
        return node->coord == coord;
    }
};

std::vector<mcpp::Coordinate> Agent::ShortestPath() { 
    std::vector<mcpp::Coordinate> path;
    std::unordered_set<Node*> visited;
    std::queue<Node*> queue;
    Node* start = new Node{currentCoord, nullptr};
    queue.push(start);

    bool exit = false;

    while(!exit && !queue.empty()) {
        Node* current = queue.front();
        queue.pop();
        visited.insert(current);

        if(isOutsideMaze(*current)) {
            exit = true;
            while(current) {
                path.insert(path.begin(), current->coord);
                current = current->prevNode;
            }
        }

        if(!exit) {
            std::vector<Node*> neighbours = FindNeighbours(current);
            for(Node* neighbour : neighbours){
                mcpp::Coordinate coord = neighbour->coord;
                NodeComparator comp(coord); 
                if(std::find_if(visited.begin(), visited.end(), comp) 
                        == visited.end()) {
                    queue.push(neighbour);
                }
            }
        }
    }
    return path;
}

std::vector<Node*> Agent::FindNeighbours(Node* node) {
    std::vector<Node*> neighbours;
    mcpp::Coordinate directions[] = {MOVE_NORTH, MOVE_EAST, 
                                        MOVE_SOUTH, MOVE_WEST};

    for(const mcpp::Coordinate& dir : directions) {
        if(CheckDirection(*node, dir)) {
            neighbours.push_back(new Node{node->coord + dir, node});
        }
    }
    return neighbours;
}

bool Agent::CheckDirection(Node node, mcpp::Coordinate direction) {
    bool open = false;
    mcpp::Coordinate checkCoord = node.coord + direction;
    if(mc.getBlock(checkCoord) == mcpp::Blocks::AIR) {
        open = true;
    }
    return open;
}

bool isOutsideMaze(const Node& node) {
    mcpp::MinecraftConnection mc;
    bool isSet1Empty = true;
    bool isSet2Empty = true;

    // array of relative block coords to check
    mcpp::Coordinate set1Delta[] = 
        {MOVE_NORTH, MOVE_NORTH + MOVE_EAST, MOVE_EAST};
    mcpp::Coordinate set2Delta[] = 
        {MOVE_SOUTH, MOVE_SOUTH + MOVE_WEST, MOVE_WEST};

    // if any of the three blocks has acacia then the set is false
    for(int i = 0; i < 3; i++) {
        mcpp::Coordinate set1 = node.coord + set1Delta[i];
        mcpp::Coordinate set2 = node.coord + set2Delta[i];
        
        if(mc.getBlock(set1) == mcpp::Blocks::ACACIA_WOOD_PLANK) {
            isSet1Empty = false;
        }
        if(mc.getBlock(set2) == mcpp::Blocks::ACACIA_WOOD_PLANK) {
            isSet2Empty = false;
        }
    }    

    // one set must be fully open to be outside the maze
    return isSet1Empty || isSet2Empty;
}