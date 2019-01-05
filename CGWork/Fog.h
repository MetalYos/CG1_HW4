#pragma once

enum FogBlendingFunction
{
	FOG_BLENDING_LINEAR,
	FOG_BLENDING_EXP,
	FOG_BLENDING_EXP_SQUARED
};

struct FogParams
{
	int Color[3];
	double MinFogDistance;
	double MaxFogDistance;
	FogBlendingFunction BlendingFunction;
	double Exponent;

	FogParams()
		: MinFogDistance(10.0), MaxFogDistance(30.0),
		BlendingFunction(FOG_BLENDING_LINEAR), Exponent(0.33)
	{
		Color[0] = 100;
		Color[1] = 100;
		Color[2] = 100;
	}
};