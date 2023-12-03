#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "Components/TextRenderComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MapBlock.h"
#include "PacManPawn.h"
#include "MapFood.h"
#include "Ghosts.h"
#include "BlinkyGhosts.h"
#include "PinkyGhosts.h"
#include "InkyGhosts.h"
#include "ClydeGhosts.h"
#include "Utility.h"
#include "GameMap.generated.h"

UCLASS()
class PACMAN_PROJECT_API AGameMap : public AActor
{
	GENERATED_BODY()

public:

	AGameMap();


	UPROPERTY(EditAnywhere, Category = "Map")
		TSubclassOf<AActor> Wall;
	UPROPERTY(EditAnywhere, Category = "Map")
		TSubclassOf<AActor> Street;
	UPROPERTY(EditAnywhere, Category = "Map")
		TSubclassOf<AActor> Slowed;
	UPROPERTY(EditAnywhere, Category = "Map")
		TSubclassOf<AActor> Teleport;
	UPROPERTY(EditAnywhere, Category = "Map")
		TSubclassOf<AActor> Gate;
	
	UPROPERTY(EditAnywhere, Category = "Food")
		TSubclassOf<AActor> BasicFood;
	UPROPERTY(EditAnywhere, Category = "Food")
		TSubclassOf<AActor> SuperFood;

	UPROPERTY(EditAnywhere, Category = "Ghosts")
		TSubclassOf<AActor> BlinkyGhost;
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		TSubclassOf<AActor> PinkyGhost;
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		TSubclassOf<AActor> InkyGhost;
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		TSubclassOf<AActor> ClydeGhost;

	UPROPERTY(EditAnywhere, Category = "PacManLives")
		TSubclassOf<AActor> PacManLives;

	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Cherries;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Strawberry;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Peach;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Apple;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Grapes;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Galaxian;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Bell;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Key;


	//VARIABILI RANDOMIZZATE, gestite attraverso la GameModeBase
	//int PointsElroy1[21] = { 20, 30, 40, 40, 40, 50, 50, 50, 60, 60, 60, 80, 80, 80, 100, 100, 100, 100, 120, 120, 120 };
	int PointsElroy1[21];
	//int PointsElroy2[21] = { 10, 15, 20, 20, 20, 25, 25, 25, 30, 30, 30, 40, 40, 40, 50, 50, 50, 50, 60, 60, 60 };
	int PointsElroy2[21];


	//VARIABILI RANDOMIZZATE, passate dalla GameModeBase e da passare alle rispettive classi
	float SpeedElroy1[21]; //passata
	float SpeedElroy2[21]; //passata
	float Base_distance_per_second; //passata
	float LevelGhostSpeed[21]; //passata


	//Graphics

	FActorSpawnParameters spawnParams;
	UDirectionalLightComponent* Light_Actor;
	UDirectionalLightComponent* Light_Actor2;
	UTextRenderComponent* Level_Text_Actor;
	UTextRenderComponent* Level_Actor;
	UTextRenderComponent* Achieved_Points_Actor;
	UTextRenderComponent* Points_Actor;
	UTextRenderComponent* Random_Actor;
	UTextRenderComponent* Conservative_Actor;
	UTextRenderComponent* Balanced_Actor;
	UTextRenderComponent* Total_Time_Actor;
	UTextRenderComponent* Total_Time_Value_Actor;
	AActor* PacManLivesVisual[3];
	AActor* RemainFruitsVisual[2];
	

	//Levels
	int GameMapLevel;
	int GameMapLevelExtra;
	int RemainLivesToPlay;
	int GameMapPoints;
	bool* GameMapCompleted;
	TSubclassOf<AActor> LevelFruits[21];
	Utility::FoodType LevelFruitsType[21] = { Utility::CHERRIES,Utility::STRAWBERRY,Utility::PEACH,Utility::PEACH ,Utility::APPLE,Utility::APPLE,Utility::GRAPES,Utility::GRAPES,Utility::GALAXIAN,Utility::GALAXIAN,Utility::BELL,Utility::BELL,Utility::KEY,Utility::KEY,Utility::KEY ,Utility::KEY ,Utility::KEY ,Utility::KEY ,Utility::KEY ,Utility::KEY ,Utility::KEY };
	AMapFood* AttualFruit = nullptr;
	float FruitTimer;


	//PacMan Ghostsand Ghosts
	APacManPawn* PacMan = nullptr;
	float TimerPacManPointEaten = 0;
	int CounterGhostsEaten = 0;
	int GlobalGhostPoints = 0;
	AGhosts* Ghosts[4];
	ABlinkyGhosts* BlinkyGhostSpawn = nullptr;
	APinkyGhosts* PinkyGhostSpawn = nullptr;
	AInkyGhosts* InkyGhostsSpawn = nullptr;
	AClydeGhosts* ClydeGhostsSpawn = nullptr;
	AMapBlock* TargetCageBlocks[2];
	AMapBlock* GhostsCustomBlocks[4];
	bool FlagGhosts = true;


	//Map
	bool FlagGlobalCounter = false;
	bool SecondFruit = false;
	AMapBlock* GameMapBlocks[36][28];
	int TotalPoints = 244;
	float Total_Time_Result = 0.0;

		//M = Wall
		//N = Street
		//B = Street and Basic Food
		//R = Slowed
		//T = Teleport
		//V = Gate

		//X = PacMan
		//R = Blinky
		//O = Pinky
		//I = Inky
		//P = Clyde

	char arrayMap[36][28] = {
		{ 'N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N'},
		{ 'N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N' },
		{ 'N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N' },
		{ 'M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M' },
		{ 'M','B','B','B','B','B','B','B','B','B','B','B','B','M','M','B','B','B','B','B','B','B','B','B','B','B','B','M' },
		{ 'M','B','M','M','M','M','B','M','M','M','M','M','B','M','M','B','M','M','M','M','M','B','M','M','M','M','B','M' },
		{ 'M','s','M','M','M','M','B','M','M','M','M','M','B','M','M','B','M','M','M','M','M','B','M','M','M','M','s','M' },
		{ 'M','B','M','M','M','M','B','M','M','M','M','M','B','M','M','B','M','M','M','M','M','B','M','M','M','M','B','M' },
		{ 'M','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','M' },
		{ 'M','B','M','M','M','M','B','M','M','B','M','M','M','M','M','M','M','M','B','M','M','B','M','M','M','M','B','M' },
		{ 'M','B','M','M','M','M','B','M','M','B','M','M','M','M','M','M','M','M','B','M','M','B','M','M','M','M','B','M' },
		{ 'M','B','B','B','B','B','B','M','M','B','B','B','B','M','M','B','B','B','B','M','M','B','B','B','B','B','B','M' },
		{ 'M','M','M','M','M','M','B','M','M','M','M','M','N','M','M','N','M','M','M','M','M','B','M','M','M','M','M','M' },
		{ 'N','N','N','N','N','M','B','M','M','M','M','M','N','M','M','N','M','M','M','M','M','B','M','N','N','N','N','N' },
		{ 'N','N','N','N','N','M','B','M','M','N','N','N','N','R','Z','N','N','N','N','M','M','B','M','N','N','N','N','N' },
		{ 'N','N','N','N','N','M','B','M','M','N','M','M','M','V','V','M','M','M','N','M','M','B','M','N','N','N','N','N' },
		{ 'M','M','M','M','M','M','B','M','M','N','M','N','I','N','O','N','P','M','N','M','M','B','M','M','M','M','M','M' },
		{ 'T','K','K','K','K','K','B','N','N','N','M','N','N','N','N','N','N','M','N','N','N','B','K','K','K','K','K','T' },
		{ 'M','M','M','M','M','M','B','M','M','N','M','N','N','N','N','N','N','M','N','M','M','B','M','M','M','M','M','M' },
		{ 'N','N','N','N','N','M','B','M','M','N','M','M','M','M','M','M','M','M','N','M','M','B','M','N','N','N','N','N' },
		{ 'N','N','N','N','N','M','B','M','M','N','N','N','N','N','N','N','N','N','N','M','M','B','M','N','N','N','N','N' },
		{ 'N','N','N','N','N','M','B','M','M','N','M','M','M','M','M','M','M','M','N','M','M','B','M','N','N','N','N','N' },
		{ 'M','M','M','M','M','M','B','M','M','N','M','M','M','M','M','M','M','M','N','M','M','B','M','M','M','M','M','M' },
		{ 'M','B','B','B','B','B','B','B','B','B','B','B','B','M','M','B','B','B','B','B','B','B','B','B','B','B','B','M' },
		{ 'M','B','M','M','M','M','B','M','M','M','M','M','B','M','M','B','M','M','M','M','M','B','M','M','M','M','B','M' },
		{ 'M','B','M','M','M','M','B','M','M','M','M','M','B','M','M','B','M','M','M','M','M','B','M','M','M','M','B','M' },
		{ 'M','s','B','B','M','M','B','B','B','B','B','B','B','X','N','B','B','B','B','B','B','B','M','M','B','B','s','M' },
		{ 'M','M','M','B','M','M','B','M','M','B','M','M','M','M','M','M','M','M','B','M','M','B','M','M','B','M','M','M' },
		{ 'M','M','M','B','M','M','B','M','M','B','M','M','M','M','M','M','M','M','B','M','M','B','M','M','B','M','M','M' },
		{ 'M','B','B','B','B','B','B','M','M','B','B','B','B','M','M','B','B','B','B','M','M','B','B','B','B','B','B','M' },
		{ 'M','B','M','M','M','M','M','M','M','M','M','M','B','M','M','B','M','M','M','M','M','M','M','M','M','M','B','M' },
		{ 'M','B','M','M','M','M','M','M','M','M','M','M','B','M','M','B','M','M','M','M','M','M','M','M','M','M','B','M' },
		{ 'M','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','M' },
		{ 'M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M','M' },
		{ 'N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N' },
		{ 'N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N','N' },
	};


	//Public function
	void Destroy();

protected:
	void SpawnPacMan(FTransform transform, AActor* block);
	virtual void BeginPlay() override;
	void Map();
	void Graphics();
	void SpawnGhosts();
	void ResetGhosts();
	void ResetPacMan(bool raw);
	virtual void Tick(float DeltaTime) override;
	AGhosts* NextGhostToExit();
};