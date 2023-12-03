#include "PacMan_ProjectGameModeBase.h"

APacMan_ProjectGameModeBase::APacMan_ProjectGameModeBase()
{

	PrimaryActorTick.bCanEverTick = true;
}


void APacMan_ProjectGameModeBase::BeginPlay() {

	//Game start, settig start values and creating map
	FVector Translation = FVector::ZeroVector;
	FQuat Rotation = FQuat::Identity;
	FVector Scale = FVector(1.0f, 1.0f, 1.0f);
	FTransform MyTransform(Rotation, Translation, Scale);
	ActualGameMap = GetWorld()->SpawnActorDeferred<AGameMap>(GameMapActor, MyTransform);

	ActualGameMap->GameMapLevel = 1;
	ActualGameMap->RemainLivesToPlay = 3;
	ActualGameMap->GameMapPoints = 0;
	ActualGameMap->GameMapCompleted = &isLevlFinished;


	//Randomizzazione della variabile PointsElroy1[21] (di default da 20 a 120, int)
	std::random_device rd1;
	std::mt19937 gen1(rd1());
	std::uniform_int_distribution<int> dist1(20, 120);
	for (int i = 0; i < 21; i++) {
		ActualGameMap->PointsElroy1[i] = dist1(gen1);
	}

	//Randomizzazione della variabile PointsElroy2[21] (di default da 10 a 60, int)
	std::random_device rd2;
	std::mt19937 gen2(rd2());
	std::uniform_int_distribution<int> dist2(10, 60);
	for (int i = 0; i < 21; i++) {
		ActualGameMap->PointsElroy2[i] = dist2(gen2);
	}

	//Randomizzazione della variabile SpeedElroy1[21] (di default da 0.8 a 1.0, float)
	std::random_device rd3;
	std::mt19937_64 gen3(rd3());
	std::uniform_real_distribution<float> dist3(0.8f, 1.0f);
	for (int i = 0; i < 21; i++) {
		ActualGameMap->SpeedElroy1[i] = dist3(gen3);
	}

	//Randomizzazione della variabile SpeedElroy2[21] (di default da 0.85 a 1.05, float)
	std::random_device rd4;
	std::mt19937_64 gen4(rd4());
	std::uniform_real_distribution<float> dist4(0.85f, 1.05f);
	for (int i = 0; i < 21; i++) {
		ActualGameMap->SpeedElroy2[i] = dist4(gen4);
	}

	//Randomizzazione della variabile Base_distance_per_second (di default da 300 a 325, int)
	std::random_device rd5;
	std::mt19937_64 gen5(rd5());
	std::uniform_real_distribution<float> dist5(325.0f, 325.0f);
	ActualGameMap->Base_distance_per_second = dist5(gen5);

	//Randomizzazione della variabile LevelGhostSpeed[21] (di default da 0.60 a 0.85, float)
	std::random_device rd6;
	std::mt19937_64 gen6(rd6());
	std::uniform_real_distribution<float> dist6(0.65f, 0.85f);
	for (int i = 0; i < 21; i++) {
		ActualGameMap->LevelGhostSpeed[i] = dist6(gen4);
	}

	UGameplayStatics::FinishSpawningActor(ActualGameMap, MyTransform);
}


void APacMan_ProjectGameModeBase::Tick(float DeltaTime)
{
	if (ActualGameMap->PacMan->ModSetted && ActualGameMap->PacMan->ShouldMove) {
		Total_Time_Result = Total_Time_Result + DeltaTime;
		ActualGameMap->Total_Time_Value_Actor->SetText(FText::FromString(FString::FromInt(Total_Time_Result)));
	}

	//Continual checking if game is finished, if yes creating next level map with previus informations
	if (isLevlFinished) {
		int PrecedentGameMapLevel = ActualGameMap->GameMapLevel;
		int GameMapLevelExtra = ActualGameMap->GameMapLevelExtra;
		int PrecedentLivesRemain = ActualGameMap->PacMan->Lives + 1;
		int PointsColletected = ActualGameMap->GameMapPoints;

		ActualGameMap->Destroy();

		FVector Translation = FVector::ZeroVector;
		FQuat Rotation = FQuat::Identity;
		FVector Scale = FVector(1.0f, 1.0f, 1.0f);
		FTransform MyTransform(Rotation, Translation, Scale);

		APawn* PacMan = GetWorld()->GetFirstPlayerController()->GetPawn();
		PacMan->SetActorLocation(FVector(-100, 800, 50));
		//PacMan->SetActorLocation(MyTransform.GetLocation());
		ActualGameMap = GetWorld()->SpawnActorDeferred<AGameMap>(GameMapActor, MyTransform);

		if (PrecedentGameMapLevel == 21) {
			GameMapLevelExtra++;
			ActualGameMap->GameMapLevel = PrecedentGameMapLevel;
			ActualGameMap->GameMapLevelExtra = GameMapLevelExtra;
		}
		else {
			ActualGameMap->GameMapLevel = PrecedentGameMapLevel + 1;
			ActualGameMap->GameMapLevelExtra = 0;
		}
		ActualGameMap->RemainLivesToPlay = PrecedentLivesRemain;
		ActualGameMap->GameMapPoints = PointsColletected;
		ActualGameMap->GameMapCompleted = &isLevlFinished;

		//Randomizzazione della variabile PointsElroy1[21] (di default da 20 a 120, int)
		std::random_device rd1;
		std::mt19937 gen1(rd1());
		std::uniform_int_distribution<int> dist1(20, 120);
		for (int i = 0; i < 21; i++) {
			ActualGameMap->PointsElroy1[i] = dist1(gen1);
		}

		//Randomizzazione della variabile PointsElroy2[21] (di default da 10 a 60, int)
		std::random_device rd2;
		std::mt19937 gen2(rd2());
		std::uniform_int_distribution<int> dist2(10, 60);
		for (int i = 0; i < 21; i++) {
			ActualGameMap->PointsElroy2[i] = dist2(gen2);
		}

		//Randomizzazione della variabile SpeedElroy1[21] (di default da 0.8 a 1.0, float)
		std::random_device rd3;
		std::mt19937_64 gen3(rd3());
		std::uniform_real_distribution<float> dist3(0.8f, 1.0f);
		for (int i = 0; i < 21; i++) {
			ActualGameMap->SpeedElroy1[i] = dist3(gen3);
		}

		//Randomizzazione della variabile SpeedElroy2[21] (di default da 0.85 a 1.05, float)
		std::random_device rd4;
		std::mt19937_64 gen4(rd4());
		std::uniform_real_distribution<float> dist4(0.85f, 1.05f);
		for (int i = 0; i < 21; i++) {
			ActualGameMap->SpeedElroy2[i] = dist4(gen4);
		}

		//Randomizzazione della variabile Base_distance_per_second (di default da 300 a 325, int)
		std::random_device rd5;
		std::mt19937_64 gen5(rd5());
		std::uniform_real_distribution<float> dist5(325.0f, 325.0f);
		ActualGameMap->Base_distance_per_second = dist5(gen5);

		//Randomizzazione della variabile LevelGhostSpeed[21] (di default da 0.60 a 0.85, float)
		std::random_device rd6;
		std::mt19937_64 gen6(rd6());
		std::uniform_real_distribution<float> dist6(0.65f, 0.85f);
		for (int i = 0; i < 21; i++) {
			ActualGameMap->LevelGhostSpeed[i] = dist6(gen4);
		}

		UGameplayStatics::FinishSpawningActor(ActualGameMap, MyTransform);
		isLevlFinished = false;

		if (ActualGameMap->PacMan->ModSelected == 1) {
			ActualGameMap->Random_Actor->SetTextRenderColor(FColor(53, 128, 255, 1));
			ActualGameMap->Conservative_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
			ActualGameMap->Balanced_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
		}
		if (ActualGameMap->PacMan->ModSelected == 2) {   //Scelta 2 - Conservative
			ActualGameMap->Random_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
			ActualGameMap->Conservative_Actor->SetTextRenderColor(FColor(53, 128, 255, 1));
			ActualGameMap->Balanced_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
		}
		if (ActualGameMap->PacMan->ModSelected == 3) {
			ActualGameMap->Random_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
			ActualGameMap->Conservative_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
			ActualGameMap->Balanced_Actor->SetTextRenderColor(FColor(53, 128, 255, 1));
		}
	}
}