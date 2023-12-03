#include "MapBlock.h"


AMapBlock::AMapBlock()
{

	PrimaryActorTick.bCanEverTick = true;
}


void AMapBlock::BeginPlay()
{

	Super::BeginPlay();
}


void AMapBlock::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
}


FVector AMapBlock::GetCenterPoint() {

	//Return the center point of the block
	FBox ActorBounds = GetComponentsBoundingBox();
	return ActorBounds.GetCenter();
}