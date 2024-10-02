// Vect2D.h
#pragma once
#include <vector>

class Vect2D
{
public:
    int x;
    int z;

    Vect2D();

    Vect2D(int x, int z);

    Vect2D operator=(const Vect2D& right);

    std::vector<Vect2D> Neighbours(bool mode, int xlen, int zlen,
        const std::vector<std::vector<bool>>& visited) const;
};
