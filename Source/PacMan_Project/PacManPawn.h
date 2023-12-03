#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include <random>
#include "MapBlock.h"
#include "DirectionGraph.h"
#include "MapFood.h"
#include "Utility.h"
#include "PacManPawn.generated.h"

UCLASS()
class PACMAN_PROJECT_API APacManPawn : public APawn
{
	GENERATED_BODY()

public:

	APacManPawn();


	//PacMan movement
	bool PacManFirstMoveFlag = false;
	bool PacManSkipMovement = false;
	double PacManSkipMovementTimer = 0;
	float Result_distance_per_second;
	bool ShouldMove = true;
	bool ModSetted = false;
	int ModSelected = 0;
	Utility::MovDirection MovDirection = Utility::LEFT; //
	Utility::MovDirection PreviusMove = Utility::IDLE;
	Utility::MovDirection MovDirections[4] = { Utility::LEFT, Utility::UP, Utility::RIGHT, Utility::DOWN }; //
	int contatore = 0;
	float LevelPacManSpeed[21] = { 0.80, 0.90, 0.90, 0.90, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.90 };
	float LevelPacManFrightnedSpeed[21] = { 0.90, 0.95, 0.95, 0.95, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	float Base_distance_per_second = 400.0f;
	void ModAlgo(float MovementCode);


	//PacMan informations
	AMapBlock* PacManDefaultMapBlock = nullptr;
	int Lives;
	bool isDead = false;
	bool iFrightnedMod = false;
	AMapBlock* CurrentMapBlock = nullptr;
	AMapBlock* NextMapBlock = nullptr;


	//Map informations
	int GameMapLevel;
	int GameMapPoints = 0;
	int RawPoints = 0;
	int* TotalPoints;
	bool* GameMapCompleted;
	UTextRenderComponent* Random_Actor;
	UTextRenderComponent* Conservative_Actor;
	UTextRenderComponent* Balanced_Actor;
	AMapBlock* GameMapBlocks[36][28];
	bool* Ghosts[4];

	UCapsuleComponent* CollisionComponent;
	Utility UtilityForRange;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	void UpDate_BlocksRisk();
	AMapBlock* NextBlock();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool WakableDirection(Utility::MovDirection movement);
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};