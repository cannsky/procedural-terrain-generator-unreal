// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

/**
 * 
 */
class MARCHINGCUBESLIGHT_API MarchingCubesAlgorithm
{
public:
	MarchingCubesAlgorithm();
	~MarchingCubesAlgorithm();

	struct MeshData
	{
		TArray<FVector> vertices;
		TArray<int32> triangles;
		TArray<FVector> normals;
	};

	void Initialize(int32 width, int32 height, int32 depth, float isoLevel);
	
	void InitializeVolumeData(int32 width, int32 height, int32 depth, float isoLevel, float bigScale, float smallScale, int seedParam, float heightScale, float persistence, float octaves);
	void SetIsoLevel(float isoLevel);
	void GenerateMesh();
	UProceduralMeshComponent* CreateMeshComponent(AActor* owner, bool createCollision);

	float CalculateDistance(float x, float y, float z);
	int CalculateVertexIndex(int x, int y, int z);
	float CalculateCornerValue(int x, int y, int z);
	int CalculateCubeIndex(float corners[8]);
	void ProcessCube(int x, int y, int z);
	void ProcessVolume();
	void AddTriangle(FVector a, FVector b, FVector c);
	FVector PerformLinearInterpolation(float corners[8], FVector cornerPositions[8], int edgeIndex);
	
private:
	// 3D volume data
	TArray<float> volumeData;
	// 3D volume data size
	int volumeDataSize;
	// Iso level
	float isoLevel;
	// Mesh data
	MeshData meshData;
	// Dimensions of the volume data
	int width, height, depth;
};
