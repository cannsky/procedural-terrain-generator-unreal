// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MARCHINGCUBESLIGHT_API MarchingCubesFBM
{
public:
	MarchingCubesFBM();
	~MarchingCubesFBM();

	static float FBM(float x, float y, int octaves, float persistence, float frequency, int seed);
	static float FBM3D(float x, float y, float z, int octaves, float persistence, float frequency, int seed);
};
