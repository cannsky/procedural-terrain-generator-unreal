#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

class PROCEDURALLEVEL_API TerrainMeshGenerator
{
public:
	TerrainMeshGenerator();
	~TerrainMeshGenerator();
    
    static void GenerateMesh(UProceduralMeshComponent* procMesh, const TArray<float>& heightmap, int gridSize, float gridSpacing, TArray<FVector>& outVertices, TArray<FVector>& outNormals, TArray<FVector2D>& outUVs, TArray<int>& outTriangles, TArray<FProcMeshTangent>& outTangents, TArray<FColor>& outVertexColors);
    static void GenerateMeshComponents(int x, int y, int gridSize, float gridSpacing, const TArray<float>& heightmap, TArray<FVector>& outVertices, TArray<FVector>& outNormals, TArray<FVector2D>& outUVs, TArray<FProcMeshTangent>& outTangents, TArray<FColor>& outVertexColors);
    static void GenerateMeshTriangles(int x, int y, int gridSize, TArray<int>& outTriangles);
};
