//====================================================================================================
// Filename:	PerlinNoise.cpp
// Created by:	Peter Chan  (Peter Chan is Awesome)
//====================================================================================================

#include "Precompiled.h"
#include "NFGEMath.h"

using namespace NFGE::Math;

PerlinNoise::PerlinNoise()
{
	// Initialize the permutation vector with the reference values
	p = {
		151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
		8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
		35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
		134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
		55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,
		18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
		250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
		189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,
		43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,
		97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
		107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}

PerlinNoise::PerlinNoise(uint32_t seed)
{
	// Generate a new permutation vector based on the value of seed
	p.resize(256);

	// Fill vector with values from 0 to 255
	std::iota(p.begin(), p.end(), 0);

	// Initialize a random engine with seed
	std::default_random_engine engine(seed);

	// Shuffle using the above random engine
	std::shuffle(p.begin(), p.end(), engine);

	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}

float PerlinNoise::Get(float x, float y, float z)
{
	// Find the unit cube that contains the point
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	// Find relative x, y, z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x, y, z
	float u = Fade(x);
	float v = Fade(y);
	float w = Fade(z);

	// Hash coordinates of the 8 cube corners
	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	// Add blended results from 8 corners of cubeLerp
	float u0 = Grad(p[AA], x, y, z);
	float u1 = Grad(p[BA], x - 1, y, z);
	float u2 = Grad(p[AB], x, y - 1, z);
	float u3 = Grad(p[BB], x - 1, y - 1, z);

	float u4 = Grad(p[AA + 1], x, y, z - 1);
	float u5 = Grad(p[BA + 1], x - 1, y, z - 1);
	float u6 = Grad(p[AB + 1], x, y - 1, z - 1);
	float u7 = Grad(p[BB + 1], x - 1, y - 1, z - 1);

	float v0 = Lerp(u0, u1, u);
	float v1 = Lerp(u2, u3, u);
	float v2 = Lerp(u4, u5, u);
	float v3 = Lerp(u6, u7, u);

	float w0 = Lerp(v0, v1, v);
	float w1 = Lerp(v2, v3, v);

	float res = Lerp(w0, w1, w);
	return (res + 1.0f) * 0.5f;
}

float PerlinNoise::Fade(float t)
{
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise::Grad(int hash, float x, float y, float z)
{
	int h = hash & 0xF;
	// Convert lower 4 bits of hash into 12 gradient directions
	float u = h < 8 ? x : y;
	float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}