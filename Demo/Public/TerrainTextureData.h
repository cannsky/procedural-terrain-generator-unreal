#pragma once

#include "CoreMinimal.h"
#include "TerrainTexture.h"
#include "TerrainTextureData.generated.h"

USTRUCT(BlueprintType)
struct FTerrainTextureData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Generator Data")
    TArray<FTerrainTexture> textures;
};
