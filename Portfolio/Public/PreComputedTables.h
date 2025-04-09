// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MARCHINGCUBESLIGHT_API PreComputedTables
{
public:
	PreComputedTables();
	~PreComputedTables();

	// Precomputed tables for the Marching Cubes algorithm
	// Source: https://github.com/SebLague/Marching-Cubes/blob/master/Assets/Scripts/Compute/Includes/MarchTables.compute
	
	static const int edges[256];

	static const int triangulation[256][16];

	static const int cornerIndicesFromEdge[12][2];
};