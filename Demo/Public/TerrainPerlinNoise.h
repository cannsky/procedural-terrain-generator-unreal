#pragma once

#include "CoreMinimal.h"

class PROCEDURALLEVEL_API TerrainPerlinNoise
{
public:
    static float PerlinNoise2D(float x, float y, int seed = 0);
    static float Fade(float t);
    static float Lerp(float a, float b, float t);
    static float Grad(int hash, float x, float y);
    static int FastFloor(float x);

    static int permutation[512];
    static bool isInitialized;
    static void InitializePermutation(int seed);
    static float RandomFromCoords(int x, int y, int seed);
};
