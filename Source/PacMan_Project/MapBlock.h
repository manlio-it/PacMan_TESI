#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility.h"
#include "MapBlock.generated.h"

UCLASS()
class PACMAN_PROJECT_API AMapBlock : public AActor
{
	GENERATED_BODY()

public:

	AMapBlock();


	UPROPERTY(EditAnywhere, Category = "Walkable")
		bool isWalkable = false;
	UPROPERTY(EditAnywhere, Category = "Telport")
		bool isTeleport = false;
	UPROPERTY(EditAnywhere, Category = "Telport")
		AMapBlock* TeleportDestination;
	UPROPERTY(EditAnywhere, Category = "Telport")
		bool isSlowToApply = false;


	//Block informations
	Utility::MovDirection Direction = Utility::IDLE;
	AMapBlock* LateralBlocks[4]; // (0:UP, 1:LEFT, 2:DOWN, 3:RIGHT)
	int x = -1; //OK
	int y = -1; //OK
	int valore_rischio = 0.0; //OK
	bool Blinky = false; //OK
	bool Pinky = false; //OK
	bool Inky = false; //OK
	bool Clyde = false; //OK
	bool PacMan = false; //OK
	bool Food = false; //OK


	//Public function to use
	FVector GetCenterPoint();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
};
