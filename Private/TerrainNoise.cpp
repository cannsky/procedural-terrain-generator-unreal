#include "TerrainNoise.h"
#include "TerrainPerlinNoise.h"
#include "Math/UnrealMathUtility.h"
#include <cmath>

TerrainNoise::TerrainNoise() { }

TerrainNoise::~TerrainNoise() { }

float TerrainNoise::FBM(float x, float y, int octaves, float persistence, float frequency, int seed)
{
    float total = 0.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;
    float freq = frequency;
    // Run for each octave and apply perlin noise to get a sum of natural value
    for (int i = 0; i < octaves; i++)
    {
        total += TerrainPerlinNoise::PerlinNoise2D(x * freq, y * freq, seed) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        freq *= 2.0f;
    }
    // Divide the total amount generated to max value (amplitude)
    return total / maxValue;
}

float TerrainNoise::ValueNoise(float x, float y, int seed)
{
    // Get integer coordinates for the grid cell.
    int xi = FMath::FloorToInt(x);
    int yi = FMath::FloorToInt(y);
    // Compute the fractional parts for interpolation.
    float xf = x - xi;
    float yf = y - yi;
    // Retrieve perlin noise based random values at the four corners of the grid cell.
    float n00 = TerrainPerlinNoise::RandomFromCoords(xi, yi, seed);
    float n10 = TerrainPerlinNoise::RandomFromCoords(xi + 1, yi, seed);
    float n01 = TerrainPerlinNoise::RandomFromCoords(xi, yi + 1, seed);
    float n11 = TerrainPerlinNoise::RandomFromCoords(xi + 1, yi + 1, seed);
    // Bilinear interpolation using your custom lerp of the perlin noise
    float i1 = TerrainPerlinNoise::Lerp(n00, n10, xf);
    float i2 = TerrainPerlinNoise::Lerp(n01, n11, xf);
    float noiseVal = TerrainPerlinNoise::Lerp(i1, i2, yf);
    // Return noise value
    return noiseVal;
}

void TerrainNoise::GaussianKernel(const TArray<int>& biomeMap, int gridSize, int x, int y, int radius, float s, TArray<float>& outBiomeWeights, float& weightSum)
{
    // Initialize the weight sum to zero
    weightSum = 0.0f;
    // Loop over the kernel area.
    for (int nY = -radius; nY <= radius; nY++)
    {
        for (int nX = -radius; nX <= radius; nX++)
        {
            int sX = FMath::Clamp(x + nX, 0, gridSize);
            int sY = FMath::Clamp(y + nY, 0, gridSize);
            int index = sX + sY * (gridSize + 1);
            // Safeguard against an invalid index.
            int biomeIndex = biomeMap.IsValidIndex(index) ? biomeMap[index] : 0;
            // Calculate squared distance from the center.
            float distanceSq = static_cast<float>(nX * nX + nY * nY);
            // Compute the weight using the Gaussian function:
            // w(nX, nY) = exp(-(nX^2 + nY^2) / (2 * s^2))
            float weight = std::exp(-distanceSq / (2.0f * s * s));
            // Accumulate the weight for the specific biome.
            if (outBiomeWeights.IsValidIndex(biomeIndex)) outBiomeWeights[biomeIndex] += weight;
            // In case of an invalid biome index, add to the first element.
            else outBiomeWeights[0] += weight;
            // Increase sum with weight
            weightSum += weight;
        }
    }
}
