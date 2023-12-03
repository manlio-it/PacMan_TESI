#pragma once

#include "CoreMinimal.h"
#include "Ghosts.h"
#include "ClydeGhosts.generated.h"

UCLASS()
class PACMAN_PROJECT_API AClydeGhosts : public AGhosts
{
	GENERATED_BODY()
	
public:

	AClydeGhosts();
	virtual void GetTarget() override;
};
