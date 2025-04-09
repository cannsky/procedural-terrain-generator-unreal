// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingCubesFBM.h"
#include "MarchingCubesPerlinNoise.h"

MarchingCubesFBM::MarchingCubesFBM()
{
}

MarchingCubesFBM::~MarchingCubesFBM()
{
}

float MarchingCubesFBM::FBM(float x, float y, int octaves, float persistence, float frequency, int seed)
{
    float total = 0.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;
    float freq = frequency;
    // Run for each octave and apply perlin noise to get a sum of natural value
    for (int i = 0; i < octaves; i++)
    {
        total += MarchingCubesPerlinNoise::PerlinNoise2D(x * freq, y * freq, seed) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        freq *= 2.0f;
    }
    // Divide the total amount generated to max value (amplitude)
    return total / maxValue;
}

float MarchingCubesFBM::FBM3D(float x, float y, float z, int octaves, float persistence, float frequency, int seed)
{
    float total = 0.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;
    float freq = frequency;
    // Run for each octave and apply perlin noise to get a sum of natural value
    for (int i = 0; i < octaves; i++)
    {
        total += MarchingCubesPerlinNoise::PerlinNoise3D(x * freq, y * freq, z * freq, seed + i * 1000) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        freq *= 2.0f;
    }
    // Divide the total amount generated to max value (amplitude)
    return total / maxValue;
}