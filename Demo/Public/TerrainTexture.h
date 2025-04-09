#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "TerrainTexture.generated.h"

USTRUCT(BlueprintType)
struct FTerrainTexture
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Texture")
    UTexture2D* texture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Texture")
    float minHeight = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain Texture")
    float maxHeight = 1000.f;
};
