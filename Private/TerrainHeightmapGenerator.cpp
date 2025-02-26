#include "TerrainHeightmapGenerator.h"
#include "TerrainNoise.h"
#include "Math/UnrealMathUtility.h"

void TerrainHeightmapGenerator::GenerateHeightmapWithBiomes(TArray<float>& outHeightmap, int gridSize, const TArray<FBiome>& biomes, const TArray<int>& biomeMap, int gaussianRadius, int seed)
{
    outHeightmap.Empty();
    outHeightmap.SetNum((gridSize + 1) * (gridSize + 1));
    // Use a value s (sigma) based on the gaussian radius to compute Gaussian weights.
    float s = gaussianRadius / 2.0f;
    // Loop through every heightmap point.
    for (int y = 0; y <= gridSize; y++)
    {
        for (int x = 0; x <= gridSize; x++)
        {
            TArray<float> biomeWeights;
            biomeWeights.Init(0.0f, biomes.Num());
            float weightSum = 0.0f;
            // Compute the weights at (x, y) using the GaussianKernel function.
            TerrainNoise::GaussianKernel(biomeMap, gridSize, x, y, gaussianRadius, s, biomeWeights, weightSum);
            // Normalize weights so they sum to 1.
            NormalizeBiomeWeights(biomeWeights, weightSum);
            // Compute the blended height using FBM noise for (x, y) point.
            float blendedHeight = ApplyWeightedFBMToPoint(x, y, biomes, biomeWeights, seed);
            // Apply an overall terrain amplitude because FBM values are very small.
            float terrainAmplitude = 1000.0f;
            int outIndex = x + y * (gridSize + 1);
            outHeightmap[outIndex] = blendedHeight * terrainAmplitude;
        }
    }
}

void TerrainHeightmapGenerator::NormalizeBiomeWeights(TArray<float>& biomeWeights, float weightSum)
{
    if (weightSum > 0) for (int i = 0; i < biomeWeights.Num(); i++) biomeWeights[i] /= weightSum;
}

float TerrainHeightmapGenerator::ApplyWeightedFBMToPoint(int x, int y, const TArray<FBiome>& biomes, const TArray<float>& biomeWeights, int seed)
{
    // Create a blended height variable
    float blendedHeight = 0.0f;
    // Blend the height based on the FBM height * weight of each biome
    for (int i = 0; i < biomes.Num(); i++)
    {
        const FBiome& biome = biomes[i];
        float heightValue = TerrainNoise::FBM(x, y, biome.octaves, biome.persistence, biome.frequency, seed) * biome.heightScale;
        blendedHeight += heightValue * biomeWeights[i];
    }
    // Return new blended height
    return blendedHeight;
}
