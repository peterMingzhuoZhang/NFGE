//====================================================================================================
// Filename:	PerlinNoise.h
// Created by:	Peter Chan  (Peter Chan is Awesome)
// Resources:	https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
//				https://flafla2.github.io/2014/08/09/perlinnoise.html
//				https://www.redblobgames.com/maps/terrain-from-noise/
//====================================================================================================

#ifndef INCLUDED_MATH_PERLINNOISE_H
#define INCLUDED_MATH_PERLINNOISE_H

namespace NFGE {
namespace Math {

class PerlinNoise
{
public:
	PerlinNoise();
	PerlinNoise(uint32_t seed);

	float Get(float x, float y, float z);

private:
	float Fade(float t);
	float Grad(int hash, float x, float y, float z);

	std::vector<int> p;
};

} // namespace Math
} // namespace Engine

#endif // #ifndef INCLUDED_MATH_PERLINNOISE_H