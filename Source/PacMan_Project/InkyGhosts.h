#pragma once

#include "CoreMinimal.h"
#include "Ghosts.h"
#include "BlinkyGhosts.h"
#include "InkyGhosts.generated.h"

UCLASS()
class PACMAN_PROJECT_API AInkyGhosts : public AGhosts
{
	GENERATED_BODY()
	

public:

	AInkyGhosts();


	ABlinkyGhosts* blinky;

	virtual void GetTarget() override;

protected:
	AMapBlock* CalculateTarget(AMapBlock* pacmanOffsetBlock, AMapBlock* blinkyBlock);
	AMapBlock* CalculateOffSet(AMapBlock* block, Utility::MovDirection mov);
};
