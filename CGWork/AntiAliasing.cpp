#pragma once

#include "AntiAliasing.h"

/* ----------------------------------------- 3X3 Filters -----------------------------------------*/
Mat3 AntiAliasing::BoxFilter3X3 = Mat3(
	1, 1, 1,
	1, 1, 1,
	1, 1, 1
) * (1.0 / 9.0);

Mat3 AntiAliasing::TriangleFilter3X3 = Mat3(
	1, 2, 1,
	2, 4, 2,
	1, 2, 1
) * (1.0 / 16.0);

Mat3 AntiAliasing::GaussianFilter3X3 = Mat3(
	1, 2, 1,
	2, 5, 2,
	1, 2, 1
) * (1.0 / 17.0);

Mat3 AntiAliasing::SincFilter3X3 = Mat3(
	2, 3, 2,
	3, 4, 3,
	2, 3, 2
) * (1.0 / 24.0);

/* ----------------------------------------- 5X5 Filters -----------------------------------------*/
Mat5 AntiAliasing::BoxFilter5X5 = Mat5(
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1
) * (1.0 / 25.0);

Mat5 AntiAliasing::TriangleFilter5X5 = Mat5(
	1, 2, 3, 2, 1,
	2, 4, 6, 4, 2,
	3, 6, 9, 6, 3,
	2, 4, 6, 4, 2,
	1, 2, 3, 2, 1
) * (1.0 / 81.0);

Mat5 AntiAliasing::GaussianFilter5X5 = Mat5(
	1, 1, 1, 1, 1,
	1, 2, 4, 2, 1,
	1, 4, 10, 4, 1,
	1, 2, 4, 2, 1,
	1, 1, 1, 1, 1
) * (1.0 / 50.0);

Mat5 AntiAliasing::SincFilter5X5 = Mat5(
	-2, -1, 0, -1, -2,
	-1,  4, 6,  4, -1,
	 0,  6, 9,  6,  0,
	-1,  4, 6,  4, -1,
	-2, -1, 0, -1, -2
) * (1.0 / 33.0);