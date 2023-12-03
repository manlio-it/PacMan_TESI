#pragma once

#include "CoreMinimal.h"

class PACMAN_PROJECT_API Utility
{

public:

	bool InRange(const FVector& VectorToCheck, const FVector& VectorA, const FVector& VectorB);

	enum MovDirection {IDLE, UP, DOWN, LEFT, RIGHT};

	enum GhostsMod {SCATTER, CHASE, FRIGHTENED};

	enum FoodType {BASIC, SUPER, CHERRIES, STRAWBERRY, PEACH, APPLE, GRAPES, GALAXIAN, BELL, KEY};
};
