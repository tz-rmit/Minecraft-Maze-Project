// Vect2D.cpp
#include "Vect2D.h"
#include <random>
#include <iostream>
#include <cstdlib>
#include <ctime>

Vect2D::Vect2D() : x(0), z(0) {}

Vect2D::Vect2D(int x, int z) : x(x), z(z) {}

Vect2D Vect2D::operator=(const Vect2D& right) {
    x = right.x;
    z = right.z;
    return *this;
}

std::vector<Vect2D> Vect2D::Neighbours(bool mode, int xlen, int zlen, 
        const std::vector<std::vector<bool>>& visited) const {
    std::vector<Vect2D> neighbours;

    // UP
    if(x != 0 && !visited[x-1][z]) {
        neighbours.push_back(Vect2D(x-1, z));
    }

    int offset;
    // RIGHT
    if(z != zlen - 1 && !visited[x][z+1]) {
        if(mode == true) {
            offset = 0;
        } else {
            offset = std::rand() % (neighbours.size() + 1);
        }
        // insert the next neighbour 
        neighbours.insert(neighbours.end() - offset, Vect2D(x, z+1));
    }

    // DOWN
    if(x != xlen - 1 && !visited[x+1][z]) {
        if(mode == true) {
            offset = 0;
        } else {
            offset = std::rand() % (neighbours.size() + 1);
        }
        neighbours.emplace(neighbours.end() - offset, Vect2D(x+1, z));
    }

    // LEFT
    if(z != 0 && !visited[x][z-1]) {
        if(mode == true) {
            offset = 0;
        } else {
            offset = std::rand() % (neighbours.size() + 1);
        }
        neighbours.emplace(neighbours.end() - offset, Vect2D(x, z-1));
    }

    return neighbours;
}