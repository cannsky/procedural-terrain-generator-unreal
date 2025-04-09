// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingCubesActor.h"
#include "MarchingCubesAlgorithm.h"

// Sets default values
AMarchingCubesActor::AMarchingCubesActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMarchingCubesActor::BeginPlay()
{
	Super::BeginPlay();
	
	CreateMesh();
}

// Create the mesh
void AMarchingCubesActor::CreateMesh()
{
    // If mesh component already exists, clear it
    if (meshComponent) meshComponent->ClearMeshSection(0);
	// Initialize the marching cubes algorithm
    MarchingCubesAlgorithm marchingCubes;
    marchingCubes.InitializeVolumeData(
        GridWidth, 
        GridHeight,
        GridDepth,
        IsoLevel,
        BigScale,
        SmallScale,
        Seed,
        HeightScale,
        Persistence,
        Octaves
    );
    marchingCubes.GenerateMesh();
	// Create the procedural mesh component
    meshComponent = marchingCubes.CreateMeshComponent(this, false);
    // Set up materials
    if (Material) meshComponent->SetMaterial(0, Material);
}

// Called every frame
void AMarchingCubesActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}