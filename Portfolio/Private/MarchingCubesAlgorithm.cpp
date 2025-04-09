// Fill out your copyright notice in the Description page of Project Settings.

#include "MarchingCubesAlgorithm.h"
#include "PreComputedTables.h"
#include "MarchingCubesPerlinNoise.h"
#include "MarchingCubesFBM.h"


MarchingCubesAlgorithm::MarchingCubesAlgorithm()
{

}

MarchingCubesAlgorithm::~MarchingCubesAlgorithm()
{

}

// This method was a test method to calculate the distance from the center of the volume
// Since initial volume data was a sphere for testing purposes
float MarchingCubesAlgorithm::CalculateDistance(float x, float y, float z)
{
    // Calculate normalized coordinates (0.0 to 1.0)
    float nx = static_cast<float>(x) / width;
    float ny = static_cast<float>(y) / height;
    float nz = static_cast<float>(z) / depth;
    // Center coordinates (-0.5 to 0.5)
    float cx = nx - 0.5f;
    float cy = ny - 0.5f;
    float cz = nz - 0.5f;
    // Calculate distance from center (for spherical shape)
    // This is a simple square root calculation
    return FMath::Sqrt(cx * cx + cy * cy + cz * cz);
}

// This method calculates the vertex index based on the coordinates
int MarchingCubesAlgorithm::CalculateVertexIndex(int x, int y, int z)
{
    // Calculate the vertex index based on the coordinates
    return x + y * width + z * width * height;
}

// This method initializes the volume data with a 3D terrain volume data
void MarchingCubesAlgorithm::InitializeVolumeData(
    int width, 
    int height, 
    int depth, 
    float isoLevel, 
    float bigScale,
    float smallScale,
    int seedParam,
    float heightScale,
    float persistence,
    float octaves)
{
    Initialize(width, height, depth, isoLevel);
    // Create FBM instance
    MarchingCubesFBM fbm;
    // Parameters
    int seed = seedParam;
    float terrainHeight = height * heightScale;
    float mountainHeight = height * (heightScale - 0.3f);
    // Generate volume data
    for (int z = 0; z < depth; z++)
    {
        for (int x = 0; x < width; x++)
        {
            // Use 2D FBM for the base terrain heightmap
            float bigFBM = fbm.FBM(x * bigScale, z * bigScale, octaves, persistence, 1.0f, seed);
            // Add smaller details with another FBM pass
            float smallFBM = fbm.FBM(x * smallScale, z * smallScale, octaves, persistence, 1.0f, seed + 5000);
            // Calculate terrain height
            float terrainY = terrainHeight + bigFBM * mountainHeight + smallFBM * heightScale * 10.0f;
            // Process each vertical column
            for (int y = 0; y < height; y++)
            {
                // Calculate base density
                float baseDensity = terrainY - y;
                // Use 3D FBM for detailed features
                float noise3D = fbm.FBM3D(x * 0.05f, y * 0.05f, z * 0.05f,
                    octaves - 1, persistence,
                    1.0f, seed + 1000);
                // Calculate final density with 3D features
                float density = baseDensity + noise3D;
                // Store the final density value
                volumeData[CalculateVertexIndex(x, y, z)] = density;
            }
        }
    }
}

// Set parameters for the marching cubes algorithm (this params will be used outside volume data initialization)
void MarchingCubesAlgorithm::Initialize(int32 width, int32 height, int32 depth, float isoLevel)
{
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->isoLevel = isoLevel;
    volumeDataSize = width * height * depth;
    volumeData.SetNum(volumeDataSize);
}

// Linear interpolation function.
FVector MarchingCubesAlgorithm::PerformLinearInterpolation(float corners[8], FVector cornerPositions[8], int edgeIndex)
{
    // Get the corner indices for the edge
    int corner1 = PreComputedTables::cornerIndicesFromEdge[edgeIndex][0];
    int corner2 = PreComputedTables::cornerIndicesFromEdge[edgeIndex][1];
    // Linear interpolation between the two corners
    float t = 0.5f;
    if (FMath::Abs(corners[corner2] - corners[corner1]) > 1e-6)
        t = (isoLevel - corners[corner1]) / (corners[corner2] - corners[corner1]);
    // Ensure t is between 0 and 1 (Normalize)
    t = FMath::Clamp(t, 0.0f, 1.0f);
    // Get the positions of the two corners
    FVector pos1 = cornerPositions[corner1];
    FVector pos2 = cornerPositions[corner2];
    // Interpolate to find the intersection point
    return pos1 + t * (pos2 - pos1);
}

float MarchingCubesAlgorithm::CalculateCornerValue(int x, int y, int z)
{
    // Check if the coordinates are within bounds
    bool isOutOfBounds = (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth);
    // If the coordinates are out of bounds, return 0.0f
    if (isOutOfBounds) return 0.0f;
    // Calculate the index in the 1D array
    int vertexIndex = CalculateVertexIndex(x, y, z);
    // Check if the index is valid and return the corresponding value
    if (vertexIndex >= 0 && vertexIndex < volumeData.Num()) return volumeData[vertexIndex];
    // If the index is invalid, return 0.0f
    return 0.0f;
}

int MarchingCubesAlgorithm::CalculateCubeIndex(float corners[8])
{
    // Set the cube index to 0
    int cubeIndex = 0;
    // Iterate through the corners and set the cube index based on the iso level
    for (int i = 0; i < 8; ++i)
        if (corners[i] < isoLevel) cubeIndex |= (1 << i);
    // Return the calculated cube index
    return cubeIndex;
}

void MarchingCubesAlgorithm::ProcessCube(int x, int y, int z)
{
    // Cube corner indices
    //    4 ------- 5
    //   /|        /|
    //  7 ------- 6 |
    //  | |       | |
    //  | 0 ------| 1
    //  |/        |/
    //  3 ------- 2
    // Calculate the corner values for the cube
    float corners[8] = 
    {
        CalculateCornerValue(x, y, z),
        CalculateCornerValue(x + 1, y, z),
        CalculateCornerValue(x + 1, y, z + 1),
        CalculateCornerValue(x, y, z + 1),
        CalculateCornerValue(x, y + 1, z),
        CalculateCornerValue(x + 1, y + 1, z),
        CalculateCornerValue(x + 1, y + 1, z + 1),
        CalculateCornerValue(x, y + 1, z + 1)
    };
    // Calculate the cube index based on the corner values
    int cubeIndex = CalculateCubeIndex(corners);
    // In order to increase the performance of the algorithm,
    // Skip the cube if the cube index is 0 or 255
    // Since these cubes do not need to be processed
    if (cubeIndex == 0 || cubeIndex == 255) return;
    // Get the intersected edge mask for the cube index
    int edgeMask = PreComputedTables::edges[cubeIndex];
    // Calculate the corner positions (For unit cube)
    FVector cornerPositions[8] = 
    {
        FVector(x, y, z),
        FVector(x + 1, y, z),
        FVector(x + 1, y, z + 1),
        FVector(x, y, z + 1),
        FVector(x, y + 1, z),
        FVector(x + 1, y + 1, z),
        FVector(x + 1, y + 1, z + 1),
        FVector(x, y + 1, z + 1)
    };
    // Calculate the intersection points for the edges
    FVector intersectionPoints[12];
    // Iterate through the edges and calculate the intersection points
    for (int i = 0; i < 12; ++i)
    {
        // Check if the edge is intersected
        // If the edge is not intersected, skip it
        // Since not intersected edges do not need to be processed
        if (!(edgeMask & (1 << i))) continue;
        // Calculate the intersection point for the edge
        intersectionPoints[i] = PerformLinearInterpolation(corners, cornerPositions, i);
    }
    // Get the triangulation for the cube index
    const int* triangulation = PreComputedTables::triangulation[cubeIndex];
    // Iterate through the triangulation and add triangles
    for (int i = 0; triangulation[i] != -1; i += 3)
    {
        // Get triangle vertices (a, b and c)
        FVector a = intersectionPoints[triangulation[i]];
        FVector b = intersectionPoints[triangulation[i + 1]];
        FVector c = intersectionPoints[triangulation[i + 2]];
        // Add the triangle to the mesh data
        AddTriangle(a, b, c);
    }
}

void MarchingCubesAlgorithm::ProcessVolume()
{
    // Iterate through the volume data
    for (int z = 0; z < depth - 1; ++z)
        for (int y = 0; y < height - 1; ++y)
            for (int x = 0; x < width - 1; ++x)
                ProcessCube(x, y, z);
}

void MarchingCubesAlgorithm::AddTriangle(FVector a, FVector b, FVector c)
{
    // Get the vertex index for the new triangle
    int vertexIndex = meshData.vertices.Num();
    // Add the triangle vertices to the mesh data
    meshData.vertices.Add(a);
    meshData.vertices.Add(b);
    meshData.vertices.Add(c);
    // Add triangle indices
    meshData.triangles.Add(vertexIndex);
    meshData.triangles.Add(vertexIndex + 1);
    meshData.triangles.Add(vertexIndex + 2);
    // Calculate normal
    FVector normal = FVector::CrossProduct(b - a, c - a).GetSafeNormal();
    // Add normals for each vertex
    meshData.normals.Add(normal);
    meshData.normals.Add(normal);
    meshData.normals.Add(normal);
}

void MarchingCubesAlgorithm::GenerateMesh()
{
    // Clear any existing mesh data
    meshData.vertices.Reset();
    meshData.triangles.Reset();
    meshData.normals.Reset();
    // Process the volume data to generate the mesh
    ProcessVolume();
}

UProceduralMeshComponent* MarchingCubesAlgorithm::CreateMeshComponent(AActor* owner, bool createCollision)
{
    if (!owner) return nullptr;
    // Create the procedural mesh component
    UProceduralMeshComponent* procMesh = NewObject<UProceduralMeshComponent>(owner);
    procMesh->RegisterComponent();
    procMesh->AttachToComponent(owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    // Prepare additional mesh data required by procedural mesh component
    TArray<FVector2D> UV0;
    TArray<FColor> vertexColors;
    TArray<FProcMeshTangent> tangents;
    // UVs for texturing (Very simple UV mapping)
    for (int i = 0; i < meshData.vertices.Num(); i++)
    {
        FVector vertex = meshData.vertices[i];
        // Create simple UV mapping based on XY position
        UV0.Add(FVector2D(vertex.X / width, vertex.Y / height));
        // Add default vertex color (white) - (Not very white but let's keep it for now.)
        vertexColors.Add(FColor::White);
        // Calculate tangents (can be improved for better texturing)
        FVector tangentX = FVector(1.0f, 0.0f, 0.0f);
        tangents.Add(FProcMeshTangent(tangentX, false));
    }
    // Create the mesh section
    procMesh->CreateMeshSection(
        0,
        meshData.vertices,
        meshData.triangles,
        meshData.normals,
        UV0,
        vertexColors,
        tangents,
        createCollision
    );
    return procMesh;
}