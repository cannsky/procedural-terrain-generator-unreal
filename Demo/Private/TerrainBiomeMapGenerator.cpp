#include "TerrainBiomeMapGenerator.h"
#include "TerrainNoise.h"
#include "Math/UnrealMathUtility.h"

TerrainBiomeMapGenerator::TerrainBiomeMapGenerator()
{
}

TerrainBiomeMapGenerator::~TerrainBiomeMapGenerator()
{
}

void TerrainBiomeMapGenerator::GenerateBiomeMap(TArray<int>& outBiomeMap, int gridSize, const TArray<FBiome>& biomes, int seed)
{
    // Refresh the out biome map
    outBiomeMap.Empty();
    // Extend slightly (+1) beyond gridSize to properly interpolate edges
    outBiomeMap.SetNum((gridSize + 1) * (gridSize + 1));
    // Compute biome weights.
    // Used for detecting the biome index
    TArray<float> biomeWeights;
    biomeWeights.Init(0.0f, biomes.Num());
    float totalWeight = 0.0f;
    // Calculate total weight (Exclude negative values)
    for (int i = 0; i < biomes.Num(); i++)
    {
        float weight = FMath::Max(biomes[i].distribution, 0.0f);
        biomeWeights[i] = weight;
        totalWeight += weight;
    }
    // Scale noise coordinates to control patch size.
    float noiseScale = 10.f;
    // Loop through the grid.
    for (int y = 0; y <= gridSize; y++)
    {
        for (int x = 0; x <= gridSize; x++)
        {
            // Determine biome index for the current point
            int selectedBiome = DetermineBiomeForPoint(x, y, gridSize, noiseScale, seed, biomeWeights, totalWeight);
            // Set index as x + y since unreal is not supporting multidimensional arrays
            int index = x + y * (gridSize + 1);
            outBiomeMap[index] = selectedBiome;
        }
    }
}

int TerrainBiomeMapGenerator::DetermineBiomeForPoint(int x, int y, int gridSize, float noiseScale, int seed, const TArray<float>& biomeWeights, float totalWeight)
{
    // Compute normalized noise
    // This is required since Perlin noise performs better in small numbers
    float noise = TerrainNoise::ValueNoise((float)x / gridSize * noiseScale, (float)y / gridSize * noiseScale, seed);
    // Multiply noise with total weight so that biome index will be found faster
    float threshold = noise * totalWeight;
    float currentTotalWeight = 0.f;
    int selectedBiome = 0;
    // Find the biome index while iterating through all biome weights
    for (int i = 0; i < biomeWeights.Num(); i++)
    {
        currentTotalWeight += biomeWeights[i];
        if (threshold <= currentTotalWeight)
        {
            selectedBiome = i;
            break;
        }
    }
    return selectedBiome;
}

UTexture2D* TerrainBiomeMapGenerator::CreateBiomeMapTexture(const TArray<int>& biomeMap, int gridSize)
{
    // Create a transient texture with dimensions (gridSize+1) x (gridSize+1)
    UTexture2D* biomeTexture = UTexture2D::CreateTransient(gridSize + 1, gridSize + 1, PF_B8G8R8A8);
    if (!biomeTexture) return nullptr;
    // Set texture properties
    // Red channel is needed
    biomeTexture->SRGB = true;
    // Hard edges per biome
    biomeTexture->Filter = TF_Nearest;
    // Access the platform data using GetPlatformData().
    FTexturePlatformData* platformData = biomeTexture->GetPlatformData();
    if (platformData && platformData->Mips.Num() > 0)
    {
        FTexture2DMipMap& mip = platformData->Mips[0];
        int textureWidth = mip.SizeX;
        int textureHeight = mip.SizeY;
        // Lock the texture data for writing.
        void* data = mip.BulkData.Lock(LOCK_READ_WRITE);
        FColor* formattedImageData = static_cast<FColor*>(data);
        // Loop through each pixel
        for (int i = 0; i < biomeMap.Num(); i++)
        {
            // Get biome index
            int biomeIndex = biomeMap[i];
            FColor pixelColor;
            // Map each biome index to a color.
            switch (biomeIndex)
            {
                case 0:
                    pixelColor = FColor::Red;
                    break;
                case 1:
                    pixelColor = FColor::Green;
                    break;
                case 2:
                    pixelColor = FColor::Blue;
                    break;
                default:
                    pixelColor = FColor::Black;
                    break;
            }
            // Set the pixel color for formatted image data
            formattedImageData[i] = pixelColor;
        }

        // Unlock
        mip.BulkData.Unlock();
        // Update the texture resource.
        biomeTexture->UpdateResource();
    }

    return biomeTexture;
}
