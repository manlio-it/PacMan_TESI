#include "BlinkyGhosts.h"


ABlinkyGhosts::ABlinkyGhosts()
{

	PrimaryActorTick.bCanEverTick = true;
}


void ABlinkyGhosts::GetTarget() {

	//Setting the Target variable for Blinky
	Super::Target = Super::PacMan->CurrentMapBlock;
}