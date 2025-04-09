// TerrainTextureGenerator.cpp
#include "TerrainTextureGenerator.h"
#include "TerrainBiomeMapGenerator.h"
#include "TerrainNoise.h"
#include "Math/UnrealMathUtility.h"

void TerrainTextureGenerator::GenerateTerrainTextures(
    UProceduralMeshComponent* ProcMesh,
    const TArray<float>& Heightmap,
    const TArray<int>& BiomeMap,
                                                      int GridSize,
                                                      int NoiseSeed,
    const FTerrainGeneratorData& GeneratorData,
    const TArray<FVector>& StoredVertices,
    const TArray<FVector>& StoredNormals,
    const TArray<FVector2D>& StoredUVs,
    const TArray<FProcMeshTangent>& StoredTangents,
    UMaterialInterface* BaseTerrainMaterial)
{
    // Check if the base material is added or not, very important to check
    if (!BaseTerrainMaterial)
    {
        UE_LOG(LogTemp, Warning, TEXT("Base Terrain Material is not assigned!"));
        return;
    }
    // Create a dynamic material instance from the base material.
    UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(BaseTerrainMaterial, ProcMesh);
    // Create a biome mask texture from the biome map.
    UTexture2D* BiomeMaskTexture = TerrainBiomeMapGenerator::CreateBiomeMapTexture(BiomeMap, GridSize);
    // Assign shader values
    if (BiomeMaskTexture) DynMaterial->SetTextureParameterValue(FName("BiomeMask"), BiomeMaskTexture);
    if (GeneratorData.biomes.Num() > 0 && GeneratorData.biomes[0].textures.Num() > 0) DynMaterial->SetTextureParameterValue(FName("TextureA"), GeneratorData.biomes[0].textures[0].texture);
    if (GeneratorData.biomes.Num() > 1 && GeneratorData.biomes[1].textures.Num() > 0) DynMaterial->SetTextureParameterValue(FName("TextureB"), GeneratorData.biomes[1].textures[0].texture);
    // Assign the dynamic material to the procedural mesh.
    ProcMesh->SetMaterial(0, DynMaterial);
}
