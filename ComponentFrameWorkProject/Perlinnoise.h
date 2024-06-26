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
    static std::vector<int> p;
    static unsigned int seed;

    //=========================================================================
    // private methods
    //=========================================================================
    // イージング関数
    double fade(double t);

    // 線形補間
    double lerp(double t, double a, double b);

    // 勾配ベクトルと位置ベクトルの
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
    static void Init();
    static void SetSeed(unsigned int seed_);
};