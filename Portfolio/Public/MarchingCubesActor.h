// Marching Cubes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "MarchingCubesActor.generated.h"

UCLASS()
class MARCHINGCUBESLIGHT_API AMarchingCubesActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMarchingCubesActor();
    UProceduralMeshComponent* meshComponent;
	// Material to apply to the generated mesh (Dummy material for now)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes")
    UMaterialInterface* Material;
    // Parameters for the marching cubes algorithm
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes", meta = (ClampMin = "2", UIMin = "2"))
    int GridWidth = 512;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes", meta = (ClampMin = "2", UIMin = "2"))
    int GridHeight = 16;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes", meta = (ClampMin = "2", UIMin = "2"))
    int GridDepth = 512;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes")
    float Scale = 0.1f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes")
    float Persistence = 0.5;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes")
    int Octaves = 5;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes")
    float BigScale = 0.03f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes")
    float SmallScale = 0.1f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes")
    float HeightScale = 0.7f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes")
    int Seed = 1234;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float IsoLevel = 1000000.0f;
    // Create mesh function accessible in the editor (WBP accessible)
    UFUNCTION(BlueprintCallable, Category = "Marching Cubes")
    void CreateMesh();
    // Generate collision for the mesh (Not very useful for now)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marching Cubes")
    bool GenerateCollision = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
