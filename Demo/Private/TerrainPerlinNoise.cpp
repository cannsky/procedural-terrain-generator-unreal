#include "TerrainPerlinNoise.h"
#include "Math/UnrealMathUtility.h"

/*
 This perlin noise is improved version of my C# perlin noise:
 https://github.com/cannsky/perlin-noise-procedural-city-generation/blob/main/PerlinNoise.cs
 */

// Set static variables
bool TerrainPerlinNoise::isInitialized = false;
int TerrainPerlinNoise::permutation[512];

// This is randomized perlin, an another approach would be finding a good permutation table pattern.
// But for now random perlin is used, this will not give very randomized results because of FBM and biomes.
void TerrainPerlinNoise::InitializePermutation(int seed)
{
    TArray<int> p;
    p.SetNumUninitialized(256);
    // Fill initial permutation with values 0-255.
    for (int i = 0; i < 256; i++) p[i] = i;
    // Shuffle using the provided seed.
    FRandomStream RandStream(seed);
    for (int i = 255; i >= 0; i--)
    {
        int swapIndex = RandStream.RandRange(0, i);
        int temp = p[i];
        p[i] = p[swapIndex];
        p[swapIndex] = temp;
    }
    // Duplicate the permutation vector.
    for (int i = 0; i < 256; i++)
    {
        permutation[i] = p[i];
        permutation[256 + i] = p[i];
    }
    isInitialized = true;
}

int TerrainPerlinNoise::FastFloor(float x)
{
    return (x >= 0) ? (int)x : (int)x - 1;
}

// Fade, S(t) = 6t^5 - 15t^4 + 10t^3
float TerrainPerlinNoise::Fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// Linear Interpolation, Lerp(a, b, t) = a + t(b - a)
float TerrainPerlinNoise::Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

// Gradient Function
float TerrainPerlinNoise::Grad(int hash, float x, float y)
{
    // Convert lower 3 bits of hash code into 8 possible directions.
    int h = hash & 7;
    float u = h < 4 ? x : y;
    float v = h < 4 ? y : x;
    // Depending on the bits, return either positive or negative contributions.
    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

float TerrainPerlinNoise::PerlinNoise2D(float x, float y, int seed)
{
    // Initialize the permutation table
    if (!isInitialized) InitializePermutation(seed);
    // Find unit grid cell containing the point.
    int X = FastFloor(x) & 255;
    int Y = FastFloor(y) & 255;
    // Get relative x and y coordinates within that cell.
    float xf = x - FastFloor(x);
    float yf = y - FastFloor(y);
    // Compute fade curves for x and y.
    float u = Fade(xf);
    float v = Fade(yf);
    // Hash coordinates of the 4 corners.
    int aa = permutation[permutation[X] + Y];
    int ab = permutation[permutation[X] + Y + 1];
    int ba = permutation[permutation[X + 1] + Y];
    int bb = permutation[permutation[X + 1] + Y + 1];
    // Add blended results from 4 corners of the square.
    float x1 = Lerp(Grad(aa, xf, yf), Grad(ba, xf - 1, yf), u);
    float x2 = Lerp(Grad(ab, xf, yf - 1), Grad(bb, xf - 1, yf - 1), u);
    float y1 = Lerp(x1, x2, v);
    // Return the noise
    return y1;
}

float TerrainPerlinNoise::RandomFromCoords(int x, int y, int seed)
{
    // Ensure the permutation table is initialized.
    if (!isInitialized) InitializePermutation(seed);
    // Combine the coordinates.
    int index = permutation[(permutation[x & 255] + (y & 255)) & 255];
    // Normalize the index (0-255) to a float in the range [0,1].
    return static_cast<float>(index) / 255.0f;
}
