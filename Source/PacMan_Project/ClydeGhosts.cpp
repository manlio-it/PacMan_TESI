#include "ClydeGhosts.h"


AClydeGhosts::AClydeGhosts()
{

	PrimaryActorTick.bCanEverTick = true;
}


void AClydeGhosts::GetTarget() {

	//Setting the Target variable for Clyde
	AMapBlock* CurrentPacManMapBlock = Super::PacMan->CurrentMapBlock;
	float TempDist = FVector::Distance(CurrentPacManMapBlock->GetActorLocation(), CurrentMapBlock->GetActorLocation());
	
	if (TempDist >= 800) {
		Super::Target = Super::PacMan->CurrentMapBlock;
	}
	else {
		Super::Target = Super::ScatterModTargetMapBlock;
	}
}