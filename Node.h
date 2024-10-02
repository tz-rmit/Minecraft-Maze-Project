#pragma once

#include <mcpp/mcpp.h>

struct Node {
    mcpp::Coordinate coord;
    Node* prevNode;

    bool operator!=(const Node& right) const {
        return !(coord == right.coord);
    }
};