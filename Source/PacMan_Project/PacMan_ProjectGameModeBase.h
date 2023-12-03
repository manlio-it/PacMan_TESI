#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMap.h"
#include "Kismet/GameplayStatics.h"
#include <random>
#include "PacMan_ProjectGameModeBase.generated.h"

UCLASS()
class PACMAN_PROJECT_API APacMan_ProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	APacMan_ProjectGameModeBase();


	UPROPERTY(EditAnywhere)
		TSubclassOf<AGameMap> GameMapActor;


	//COSE DA RANDOMIZZARE E DUNQUE PASSARE
	//int PointsElroy1[21];  OK
	//int PointsElroy2[21];  OK
	//float SpeedElroy1[21]; OK
	//float SpeedElroy2[21]; OK
	//float Base_distance_per_second; OK
	//float LevelGhostSpeed[21]; OK

	float Total_Time_Result = 0.0;
	bool isLevlFinished = false;
	AGameMap* ActualGameMap;

protected:
	void BeginPlay();
	void Tick(float DeltaTime);
};
