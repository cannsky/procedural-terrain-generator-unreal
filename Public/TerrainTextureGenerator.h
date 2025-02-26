#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralMeshComponent.h"
#include "TerrainGeneratorData.h"

class PROCEDURALLEVEL_API TerrainTextureGenerator
{
public:
    static void GenerateTerrainTextures(
            UProceduralMeshComponent* procMesh,
            const TArray<float>& heightmap,
            const TArray<int>& biomeMap,
            int gridSize,
            int noiseSeed,
            const struct FTerrainGeneratorData& generatorData,
            const TArray<FVector>& storedVertices,
            const TArray<FVector>& storedNormals,
            const TArray<FVector2D>& storedUVs,
            const TArray<FProcMeshTangent>& storedTangents,
            UMaterialInterface* baseTerrainMaterial);
};
