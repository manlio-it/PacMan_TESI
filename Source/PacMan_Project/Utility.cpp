#include "Utility.h"


bool Utility::InRange(const FVector& VectorToCheck, const FVector& VectorA, const FVector& VectorB)
{

	//Checking if Actor/Pawn is in range between two FVector
	bool InRangeX = (VectorToCheck.X >= FMath::Min(VectorA.X, VectorB.X)) && (VectorToCheck.X <= FMath::Max(VectorA.X, VectorB.X));
	bool InRangeY = (VectorToCheck.Y >= FMath::Min(VectorA.Y, VectorB.Y)) && (VectorToCheck.Y <= FMath::Max(VectorA.Y, VectorB.Y));

	return InRangeX && InRangeY;
}