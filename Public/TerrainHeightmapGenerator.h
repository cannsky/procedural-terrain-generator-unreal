#pragma once

#include "CoreMinimal.h"
#include "TerrainNoise.h"
#include "Biome.h"

class PROCEDURALLEVEL_API TerrainHeightmapGenerator
{
public:
    TerrainHeightmapGenerator();
    ~TerrainHeightmapGenerator();

    static void GenerateHeightmapWithBiomes(TArray<float>& outHeightmap, int gridSize, const TArray<FBiome>& biomes, const TArray<int>& biomeMap, int radius, int seed);
    static void NormalizeBiomeWeights(TArray<float>& biomeWeights, float weightSum);
    static float ApplyWeightedFBMToPoint(int x, int y, const TArray<FBiome>& biomes, const TArray<float>& biomeWeights, int seed);
};
