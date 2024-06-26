#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <numeric>

///////////////////////////////////////////////////////////////////////////////
// PerlinNoise class
///////////////////////////////////////////////////////////////////////////////
class PerlinNoise 
{
    //=========================================================================
    // list of friend classes and methods.
    //=========================================================================
    /* NOTHING */
private:
    //=========================================================================
    // private variables.
    //=========================================================================
    std::vector<int> p;

    //=========================================================================
    // private methods
    //=========================================================================
    // �C�[�W���O�֐�
    double fade(double t);

    // ���`���
    double lerp(double t, double a, double b);

    // ���z�x�N�g���ƈʒu�x�N�g����
    double grad(int hash, double x, double y, double z);

public:
    //=========================================================================
   // public variables.
   //=========================================================================
    /* NOTHING */

    //=========================================================================
    // public methods.
    //=========================================================================
    PerlinNoise();

    double noise(double x, double y, double z);

    // Compute Perlin noise at coordinates x, y, z with specific octaves and persistence
    double octaveNoise(double x, double y, double z, int octaves, double persistence);
    
};