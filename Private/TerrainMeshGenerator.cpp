#include "TerrainMeshGenerator.h"

TerrainMeshGenerator::TerrainMeshGenerator()
{
}

TerrainMeshGenerator::~TerrainMeshGenerator()
{
}

void TerrainMeshGenerator::GenerateMesh(UProceduralMeshComponent* procMesh, const TArray<float>& heightmap, int gridSize, float gridSpacing, TArray<FVector>& outVertices, TArray<FVector>& outNormals, TArray<FVector2D>& outUVs, TArray<int>& outTriangles, TArray<FProcMeshTangent>& outTangents, TArray<FColor>& outVertexColors)
{
    // Reset all arrays.
    outVertices.Empty();
    outNormals.Empty();
    outUVs.Empty();
    outTriangles.Empty();
    outTangents.Empty();
    outVertexColors.Empty();
    // Generate mesh components
    for (int y = 0; y <= gridSize; y++)
        for (int x = 0; x <= gridSize; x++)
            GenerateMeshComponents(x, y, gridSize, gridSpacing, heightmap, outVertices, outNormals, outUVs, outTangents, outVertexColors);
    // Generate mesh triangles for each point (two triangles per point).
    // Since we have a grid baed approach, we need two triangles to form a square
    for (int y = 0; y < gridSize; y++)
        for (int x = 0; x < gridSize; x++)
            GenerateMeshTriangles(x, y, gridSize, outTriangles);
    // For now initialize all normals to zero (Duplicate?)
    outNormals.Init(FVector::ZeroVector, outVertices.Num());
    // Create the mesh section with the generated arrays.
    procMesh->CreateMeshSection(0, outVertices, outTriangles, outNormals, outUVs, outVertexColors, outTangents, true);
}

void TerrainMeshGenerator::GenerateMeshComponents(int x, int y, int gridSize, float gridSpacing, const TArray<float>& heightmap, TArray<FVector>& outVertices, TArray<FVector>& outNormals, TArray<FVector2D>& outUVs, TArray<FProcMeshTangent>& outTangents, TArray<FColor>& outVertexColors)
{
    int index = x + y * (gridSize + 1);
    float height = heightmap.IsValidIndex(index) ? heightmap[index] : 0.0f;
    outVertices.Add(FVector(x * gridSpacing, y * gridSpacing, height));
    outUVs.Add(FVector2D((float)x / gridSize, (float)y / gridSize));
    // No normals for now.
    outNormals.Add(FVector::ZeroVector);
    outTangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
    outVertexColors.Add(FColor::White);
}

void TerrainMeshGenerator::GenerateMeshTriangles(int x, int y, int gridSize, TArray<int>& outTriangles)
{
    // Form a square with two triangles
    int index = x + y * (gridSize + 1);
    // First triangle.
    outTriangles.Add(index);
    outTriangles.Add(index + gridSize + 1);
    outTriangles.Add(index + 1);
    // Second triangle.
    outTriangles.Add(index + 1);
    outTriangles.Add(index + gridSize + 1);
    outTriangles.Add(index + gridSize + 2);
}
