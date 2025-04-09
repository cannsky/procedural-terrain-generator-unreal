// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MARCHINGCUBESLIGHT_API MarchingCubesPerlinNoise
{
public:
    static float PerlinNoise2D(float x, float y, int seed = 0);
    static float PerlinNoise3D(float x, float y, float z, float scale, int seed = 0);
    
    static float Fade(float t);
    static float Lerp(float a, float b, float t);
    static float Grad(int hash, float x, float y);
    static int FastFloor(float x);

    static int permutation[512];
    static bool isInitialized;
    static void InitializePermutation(int seed);
    static float RandomFromCoords(int x, int y, int seed);
};
