#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "MapBlock.h"
#include "Components/CapsuleComponent.h"
#include "PacManPawn.h"
#include "Utility.h"
#include "Ghosts.generated.h"

UCLASS()
class PACMAN_PROJECT_API AGhosts : public AActor
{
	GENERATED_BODY()
	
public:	

	AGhosts();


	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* FrightnedMaterial;
	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* FrightnedMaterialFlash;
	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* GhostsDeadMaterial;

	UMaterialInterface* BaseGhostsMaterial;
	UStaticMeshComponent* MeshComp;


	//VARIABILI RANDOMIZZATE, gestite attraverso la GameMap
	//float SpeedElroy1[21] = { 0.80, 0.90, 0.90, 0.90, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	float SpeedElroy1[21];
	//float SpeedElroy2[21] = { 0.85, 0.95, 0.95, 0.95, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05 };
	float SpeedElroy2[21];
	//float Base_distance_per_second = 400.0f;
	float Base_distance_per_second;
	float Start_Base_distance_per_second;
	//float LevelGhostSpeed[21] = { 0.75, 0.85, 0.85, 0.85, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95, 0.95 };
	float LevelGhostSpeed[21];



	//escluse dalla inizializzazione
	float LevelMovementModeTimes[3][7] = { {7,20,7,20,5,20,5}, {7,20,7,20,5,1033,0.017}, {7,20,7,20,5,1037,0.017} };
	int LevelGhostFlashTimes[21] = { 5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 3, 3, 5, 3, 3, 0, 3, 0, 0, 0 };
	int LevelGhostsFrightnedTime[21] = { 6, 5, 4, 3, 2, 5, 2, 2, 1, 5, 2, 1, 1, 3, 1, 1, 0, 1, 0, 0, 0 };
	float LevelGhostsFrightnedSpeed[21] = { 0.50, 0.55, 0.55, 0.55, 0.60, 0.60, 0.60, 0.60, 0.60, 0.60, 0.60, 0.60, 0.60, 0.60, 0.60, 0.60, 1, 0.60, 1, 1, 1 };
	float LevelGhostsTunnelSpeed[21] = { 0.40, 0.45, 0.45, 0.45, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50 };


	//Levels
	int GameMapLevel;
	int GameMapLevelMatrix;


	//PacMan
	APacManPawn* PacMan;
	

	//Ghosts
	bool isDead = false;
	bool Reset = false;
	bool Move = false;
	char Type = 'a';
	bool isInCage = true;
	bool PreviousIsInCage = true;
	bool SlowApply = false;
	int GhostsFirstExitPoints;
	int GlobalGhostsLimitPoint = 0;
	int GhostFlashTimes;
	float Result_distance_per_second;
	Utility::GhostsMod MovementMod = Utility::SCATTER;
	int MovementPositionCycle = 0;
	float TimeMods = 0;
	bool OppositionMovementFlag = false;
	float GhostsFrightnedTimer = 0;
	float GhostsFlashTime = 0;
	int* CounterGhostsEaten;
	Utility::MovDirection MovDirection = Utility::LEFT;
	Utility::MovDirection NextMove = Utility::LEFT;
	Utility::GhostsMod PreviousMovementMod = Utility::SCATTER;
	FVector CagePath[4];
	Utility::MovDirection CagePathMovement[4] = { Utility::UP, Utility::DOWN,Utility::DOWN,Utility::UP };
	FVector ExtitCagePath[6];
	Utility::MovDirection ExitCageMovement[6] = { Utility::UP,Utility::RIGHT, Utility::UP,Utility::UP, Utility::UP,Utility::LEFT };
	int CagePosition = 0;

	
	//Map
	int* GameMapPoints;
	int PointsCounter = 0;
	AMapBlock* TargetCageBlocks[2];
	AMapBlock* Target = nullptr;
	AMapBlock* CurrentMapBlock = nullptr;
	AMapBlock* DefaultExitCageMapBlock = nullptr;
	AMapBlock* ScatterModTargetMapBlock = nullptr;
	AMapBlock* GameMapBlocks[36][28];

	//Elroy
	bool ModElroy1;
	bool ModElroy2;
	bool ShouldMoveElroy;


	Utility UtilityForRange;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void Movement_Cage(FVector MovementVector);
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void GetTarget();
	Utility::MovDirection Next_move();
	Utility::MovDirection Next_move_FrightnedMod();
	AMapBlock* NextBlock(AMapBlock* MapBlock, Utility::MovDirection MovDirection);
};