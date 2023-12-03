#include "PinkyGhosts.h"


APinkyGhosts::APinkyGhosts()
{

	PrimaryActorTick.bCanEverTick = true;
}


void APinkyGhosts::GetTarget() {

	//Setting the Target variable for Pinky
	AMapBlock* current = Super::PacMan->CurrentMapBlock;
	AMapBlock* TargetBlock;
	Utility::MovDirection MovementControl = Super::PacMan->MovDirection;

	if (MovementControl == Utility::IDLE) {
		MovementControl = Super::PacMan->MovDirection;
	}

	switch (MovementControl)
	{
	case Utility::UP:
		TargetBlock = CalculateOffSet(CalculateOffSet(current, Utility::UP), Utility::LEFT);
		break;
	case Utility::DOWN:
		TargetBlock = CalculateOffSet(current, Utility::DOWN);
		break;
	case Utility::RIGHT:
		TargetBlock = CalculateOffSet(current, Utility::RIGHT);
		break;
	case Utility::LEFT:
		TargetBlock = CalculateOffSet(current, Utility::LEFT);
		break;
	default:
		TargetBlock = nullptr;
	}

	if (TargetBlock != nullptr) {
		Super::Target = TargetBlock;
	}
	else {
		Super::Target = Super::PacMan->CurrentMapBlock;
	}
}


AMapBlock* APinkyGhosts::CalculateOffSet(AMapBlock* MapBlock, Utility::MovDirection MovDir) {

	//Calculation for off-set in one direction with for cycle
	AMapBlock* ResultMapBlockOffSet = Super::NextBlock(MapBlock, MovDir);
	for (int i = 0; i < 3 && ResultMapBlockOffSet != nullptr; ++i) {
		AMapBlock* TempMapBlock = Super::NextBlock(ResultMapBlockOffSet, MovDir);
		if (TempMapBlock != nullptr) {
			ResultMapBlockOffSet = TempMapBlock;
		}
	}

	if (ResultMapBlockOffSet == nullptr) {
		return MapBlock;
	}

	return ResultMapBlockOffSet;
}