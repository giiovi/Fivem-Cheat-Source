#include "includes.hpp"
#include <sstream>

#include "native.hpp"
#include "func.hpp"
#include <fa_solid_900.h>

#include <cmath>

namespace Drawing
{
	float Distance(Vector3 A, Vector3 B) {
		return sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y, 2) + pow(B.z - A.z, 2));
	}

	inline float distance_2d(float fromX, float fromY, float toX, float toY)
	{
		float a = abs(fromX - toX);
		float b = abs(fromY - toY);
		return sqrt((a * a) + (b * b));
	}

	inline bool in_fov(int fovX, int fovY, int aimFOV, int x, int y)
	{
		float dist = sqrt(abs(fovX - x) * abs(fovX - x) + abs(fovY - y) * abs(fovY - y));
		return (dist < aimFOV) ? true : false;
	}

	inline ImVec2 WorldToScreen(Vector3 WorldPos) // bu oyun'u çökertiyor.
	{
		ImVec2 tempVec2;
		GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD((float)WorldPos.x, (float)WorldPos.y, (float)WorldPos.z, &tempVec2.x, &tempVec2.y);

		tempVec2.x *= RAIDOS::SizeX;
		tempVec2.y *= RAIDOS::SizeY;
		return tempVec2;
	}

	inline Vector3 GetBonePos(uint64_t cPed, int wMask)
	{
		const auto boneManager = *reinterpret_cast<__m128**>(cPed + 0x180);
		const auto boneVector = *reinterpret_cast<__m128i*>(cPed + 0x430 + wMask * 0x10);
		const auto v1 = _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(boneVector, 0)), *boneManager);
		const auto v2 = _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(boneVector, 0x55)), boneManager[1]);
		const auto v3 = _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(boneVector, 0xAA)), boneManager[2]);
		const auto v4 = _mm_add_ps(_mm_add_ps(v1, boneManager[3]), v2);
		return Vector3::FromM128(_mm_add_ps(v3, v4));
	}
	inline ImVec2 GetBonePosW2S(uint64_t cPed, int wMask)
	{
		const auto boneManager = *reinterpret_cast<__m128**>(cPed + 0x180);
		const auto boneVector = *reinterpret_cast<__m128i*>(cPed + 0x430 + wMask * 0x10);
		const auto v1 = _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(boneVector, 0)), *boneManager);
		const auto v2 = _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(boneVector, 0x55)), boneManager[1]);
		const auto v3 = _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(boneVector, 0xAA)), boneManager[2]);
		const auto v4 = _mm_add_ps(_mm_add_ps(v1, boneManager[3]), v2);
		return WorldToScreen(Vector3::FromM128(_mm_add_ps(v3, v4)));
	}
};