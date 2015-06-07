#pragma once

inline float safe_acos(float t)
{
	if (t > 1.f) t = 1.f;
	if (t < -1.f) t = -1.f;
	return acos(t);
}
