#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Biome.h"
#include "TerrainGeneratorData.h"
#include "ProceduralTerrainActor.generated.h"

UCLASS()
class PROCEDURALLEVEL_API AProceduralTerrainActor : public AActor
{
    GENERATED_BODY()

public:
    AProceduralTerrainActor();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    int gridSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float gridSpacing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    int octaves;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float persistence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float frequency;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    FTerrainGeneratorData terrainGeneratorData;
    
    UPROPERTY(EditAnywhere, Category = "Terrain Generation")
    int noiseSeed = 1234;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    UMaterialInterface* baseTerrainMaterial;
    
    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    UProceduralMeshComponent* procMesh;

private:
    void GenerateTerrain();
};
