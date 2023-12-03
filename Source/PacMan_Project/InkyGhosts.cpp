#include "InkyGhosts.h"


AInkyGhosts::AInkyGhosts()
{

	PrimaryActorTick.bCanEverTick = true;
}


void AInkyGhosts::GetTarget() {

	//Setting the Target variable for Inky
	AMapBlock* CurrentMapBlockBis = Super::PacMan->CurrentMapBlock;
	AMapBlock* MapBlockOffSet = nullptr;

	Utility::MovDirection MovementToCheck = Super::PacMan->MovDirection;

	if (MovementToCheck == Utility::IDLE) {
		MovementToCheck = Super::PacMan->MovDirection;
	}

	switch (MovementToCheck) {
	case Utility::UP:
		MapBlockOffSet = CalculateOffSet(CalculateOffSet(CurrentMapBlockBis, Utility::UP), Utility::LEFT);
		break;
	case Utility::LEFT:
		MapBlockOffSet = CalculateOffSet(CurrentMapBlockBis, Utility::LEFT);
		break;
	case Utility::DOWN:
		MapBlockOffSet = CalculateOffSet(CurrentMapBlockBis, Utility::DOWN);
		break;
	case Utility::RIGHT:
		MapBlockOffSet = CalculateOffSet(CurrentMapBlockBis, Utility::RIGHT);
		break;
	}

	Super::Target = CalculateTarget(PacMan->CurrentMapBlock, blinky->CurrentMapBlock);
}


AMapBlock* AInkyGhosts::CalculateOffSet(AMapBlock* MapBlock, Utility::MovDirection MovDir) {
	
	//Calculation for off-set in one direction
	AMapBlock* InitialOffSet = Super::NextBlock(MapBlock, MovDir);
	if (InitialOffSet == nullptr) {
		return MapBlock;
	}

	AMapBlock* FinalOffSet = Super::NextBlock(InitialOffSet, MovDir);
	if (FinalOffSet == nullptr) {
		return InitialOffSet;
	}
	
	return FinalOffSet;
}


AMapBlock* AInkyGhosts::CalculateTarget(AMapBlock* MapBlockOffSet, AMapBlock* BlinkyMapBlock) {

	//Calculation of target block in map limits
	int xResult = BlinkyMapBlock->x - 2 * (BlinkyMapBlock->x - MapBlockOffSet->x);
	int yResult = BlinkyMapBlock->y - 2 * (BlinkyMapBlock->y - MapBlockOffSet->y);

	xResult = FMath::Clamp(xResult, 0, 27);
	yResult = FMath::Clamp(yResult, 0, 35);

	return GameMapBlocks[yResult][xResult];
}