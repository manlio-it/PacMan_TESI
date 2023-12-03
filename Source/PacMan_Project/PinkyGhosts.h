#pragma once

#include "CoreMinimal.h"
#include "Ghosts.h"
#include "PinkyGhosts.generated.h"

UCLASS()
class PACMAN_PROJECT_API APinkyGhosts : public AGhosts
{
	GENERATED_BODY()
	
public:

	APinkyGhosts();


	virtual void GetTarget() override;

protected:
	AMapBlock* CalculateOffSet(AMapBlock* block, Utility::MovDirection mov);
};