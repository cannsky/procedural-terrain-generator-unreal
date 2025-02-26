#pragma once

#include "CoreMinimal.h"
#include "Biome.h"
#include "TerrainGeneratorData.generated.h"

USTRUCT(BlueprintType)
struct FTerrainGeneratorData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Generator Data")
    TArray<FBiome> biomes;
};
