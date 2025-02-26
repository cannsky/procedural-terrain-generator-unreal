#include "ProceduralTerrainActor.h"
#include "TerrainHeightmapGenerator.h"
#include "TerrainBiomeMapGenerator.h"
#include "TerrainMeshGenerator.h"
#include "TerrainTextureGenerator.h"

AProceduralTerrainActor::AProceduralTerrainActor()
{
    // Enable tick for this actor (Not used for now.)
    PrimaryActorTick.bCanEverTick = false;
    // Default terrain parameters.
    gridSize = 128;
    gridSpacing = 100.0f;
    // Seed for noise.
    noiseSeed = 1234;
    /*
    // Initial biome definitions
    // Closed for now (used for testing purposes.)
    FBiome biomeA;
    BiomeA.distribution = 0.0f;
    BiomeA.octaves = 4;
    BiomeA.persistence = 0.5f;
    BiomeA.frequency = 0.01f;
    BiomeA.heightScale = 0.5f;  // Lower height for grassland.

    FBiome biomeB;
    BiomeB.distribution = 1.0f;
    BiomeB.octaves = 8;           // level of detail.
    BiomeB.persistence = 0.3f;      // Lower persistence, less smoothing.
    BiomeB.frequency = 0.03f;       // sharper features.
    BiomeB.heightScale = 2.0f;      // height scale

    TerrainGeneratorData.Biomes.Add(biomeA);
    TerrainGeneratorData.Biomes.Add(biomeB);
    */
    // Create and attach the procedural mesh component.
    procMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    RootComponent = procMesh;
}

void AProceduralTerrainActor::BeginPlay()
{
    Super::BeginPlay();
    GenerateTerrain();
}

void AProceduralTerrainActor::GenerateTerrain()
{
    // Biome map and height map
    TArray<int32> biomeMap;
    TArray<float> heightmap;
    // Generate a biome map based on value noise
    TerrainBiomeMapGenerator::GenerateBiomeMap(biomeMap, gridSize, terrainGeneratorData.biomes, noiseSeed);
    // Generate a heightmap that blends biome noise with Gaussian Kernel, radius = 2
    TerrainHeightmapGenerator::GenerateHeightmapWithBiomes(heightmap, gridSize, terrainGeneratorData.biomes, biomeMap, 2, noiseSeed);
    // Mesh values required for assigning texture
    TArray<FVector> meshVertices;
    TArray<FVector> meshNormals;
    TArray<FVector2D> meshUVs;
    TArray<int> meshTriangles;
    TArray<FProcMeshTangent> meshTangents;
    TArray<FColor> meshVertexColors;
    // Generate the mesh of the terrain based on height map
    TerrainMeshGenerator::GenerateMesh(procMesh, heightmap, gridSize, gridSpacing, meshVertices, meshNormals, meshUVs, meshTriangles, meshTangents, meshVertexColors);
    // Apply texture generation (Simply, send textures to shader)
    TerrainTextureGenerator::GenerateTerrainTextures(procMesh, heightmap, biomeMap, gridSize, noiseSeed, terrainGeneratorData, meshVertices, meshNormals, meshUVs, meshTangents, baseTerrainMaterial);
}
