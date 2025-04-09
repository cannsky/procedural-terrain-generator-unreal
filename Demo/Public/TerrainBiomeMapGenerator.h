#pragma once

#include "CoreMinimal.h"
#include "Biome.h"

class PROCEDURALLEVEL_API TerrainBiomeMapGenerator
{
public:
	TerrainBiomeMapGenerator();
	~TerrainBiomeMapGenerator();
    
    static void GenerateBiomeMap(TArray<int>& outBiomeMap, int gridSize, const TArray<FBiome>& biomes, int seed);
    static int DetermineBiomeForPoint(int x, int y, int gridSize, float noiseScale, int seed, const TArray<float>& biomeWeights, float totalWeight);
    static UTexture2D* CreateBiomeMapTexture(const TArray<int>& biomeMap, int gridSize);
};
