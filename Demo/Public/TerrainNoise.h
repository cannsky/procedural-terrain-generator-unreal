#pragma once

#include "CoreMinimal.h"

class PROCEDURALLEVEL_API TerrainNoise
{
public:
	TerrainNoise();
	~TerrainNoise();
    static float FBM(float x, float y, int octaves, float persistence, float frequency, int seed);
    static float ValueNoise(float x, float y, int seed);
    static void GaussianKernel(const TArray<int>& biomeMap, int gridSize, int x, int y, int radius, float s, TArray<float>& outBiomeWeights, float& outWeightSum);
};
