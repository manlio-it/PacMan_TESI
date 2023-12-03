#pragma once

#include "CoreMinimal.h"
#include "Ghosts.h"
#include "BlinkyGhosts.generated.h"

UCLASS()
class PACMAN_PROJECT_API ABlinkyGhosts : public AGhosts
{
	GENERATED_BODY()

public:

	ABlinkyGhosts();
	virtual void GetTarget() override;
};
