#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility.h"
#include "MapFood.generated.h"

UCLASS()
class PACMAN_PROJECT_API AMapFood : public AActor
{
	GENERATED_BODY()

public:	

	AMapFood();


	Utility::FoodType Type =Utility::BASIC;


protected:	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
};
