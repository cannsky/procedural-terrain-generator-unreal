#pragma once

#include "CoreMinimal.h"
#include "TerrainTexture.h"
#include "Biome.generated.h"

USTRUCT(BlueprintType)
struct FBiome
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
    float distribution = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
    int32 octaves = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
    float persistence = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
    float frequency = 0.01f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
    float heightScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biome")
    TArray<FTerrainTexture> textures;
};
