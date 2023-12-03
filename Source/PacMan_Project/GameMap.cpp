#include "GameMap.h"


AGameMap::AGameMap()
{

	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}


void AGameMap::BeginPlay() 
{

	Super::BeginPlay();

	Graphics();

	Map();

	SpawnGhosts();

	for (int i = 0; i < 21; i++) {
		Ghosts[0]->SpeedElroy1[i] = SpeedElroy1[i];
		Ghosts[0]->SpeedElroy2[i] = SpeedElroy2[i];
		Ghosts[0]->LevelGhostSpeed[i] = LevelGhostSpeed[i];
	}
	for (int i = 0; i < 21; i++) {
		Ghosts[1]->SpeedElroy1[i] = SpeedElroy1[i];
		Ghosts[1]->SpeedElroy2[i] = SpeedElroy2[i];
		Ghosts[1]->LevelGhostSpeed[i] = LevelGhostSpeed[i];
	}
	for (int i = 0; i < 21; i++) {
		Ghosts[2]->SpeedElroy1[i] = SpeedElroy1[i];
		Ghosts[2]->SpeedElroy2[i] = SpeedElroy2[i];
		Ghosts[2]->LevelGhostSpeed[i] = LevelGhostSpeed[i];
	}
	for (int i = 0; i < 21; i++) {
		Ghosts[3]->SpeedElroy1[i] = SpeedElroy1[i];
		Ghosts[3]->SpeedElroy2[i] = SpeedElroy2[i];
		Ghosts[3]->LevelGhostSpeed[i] = LevelGhostSpeed[i];
	}
	for (int i = 0; i < 4; i++) {
		Ghosts[i]->Base_distance_per_second = Base_distance_per_second;
		Ghosts[i]->Start_Base_distance_per_second = Base_distance_per_second;
	}

	for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 28; y++) {
			PacMan->GameMapBlocks[x][y] = GameMapBlocks[x][y];
		}
	}
	PacMan->Ghosts[0] = &(Ghosts[0]->isInCage);
	PacMan->Ghosts[1] = &(Ghosts[1]->isInCage);
	PacMan->Ghosts[2] = &(Ghosts[2]->isInCage);
	PacMan->Ghosts[3] = &(Ghosts[3]->isInCage);

}


void AGameMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Stop ghosts move when PacMan eaten
	if (PacMan->isDead && BlinkyGhostSpawn->Move) {
		BlinkyGhostSpawn->Move = false;
		PinkyGhostSpawn->Move = false;
		InkyGhostsSpawn->Move = false;
		ClydeGhostsSpawn->Move = false;
	}


	//Elroy for Blinky
	if (TotalPoints <= PointsElroy1[GameMapLevel - 1]) {
		BlinkyGhostSpawn->ModElroy1 = true;
	}
	if (TotalPoints <= PointsElroy2[GameMapLevel - 1]) {
		BlinkyGhostSpawn->ModElroy2 = true;
	}
	if (!ClydeGhostsSpawn->isInCage) {
		BlinkyGhostSpawn->ShouldMoveElroy = false;
	}


	//PacMan events management, ghosts active-inactive, points update, fruits spawn and cage exit
	if (PacMan != nullptr) {
		if (PacMan->isDead) {
			PacMan->Lives--;
			if (PacMan->Lives == -1) {
				SetActorTickEnabled(false);
				PacMan->ShouldMove = false;
				return;
			}
			else {
				ResetGhosts();
				ResetPacMan(false);
				PacMan->isDead = false;
				FlagGlobalCounter = true;
			}
		}

		if (PacMan->PacManFirstMoveFlag && FlagGhosts) {
			PacManLivesVisual[PacMan->Lives]->Destroy();
			PacManLivesVisual[PacMan->Lives] = nullptr;
			BlinkyGhostSpawn->Move = true;
			PinkyGhostSpawn->Move = true;
			InkyGhostsSpawn->Move = true;
			ClydeGhostsSpawn->Move = true;
			FlagGhosts = false;
		}

		if (GameMapPoints != PacMan->GameMapPoints) {
			TimerPacManPointEaten = 0;
			GameMapPoints = FMath::Max(GameMapPoints, PacMan->GameMapPoints);
			PacMan->GameMapPoints = GameMapPoints;
			Points_Actor->SetText(FText::FromString(FString::FromInt(GameMapPoints)));
			if (PacMan->RawPoints == 70 || PacMan->RawPoints == 171) {
				PacMan->RawPoints++;
				FTransform Tranform = GetTransform();
				Tranform.AddToTranslation(FVector(-50, 800, 0));
				AActor* RawFruit = GetWorld()->SpawnActor<AActor>(LevelFruits[GameMapLevel - 1], Tranform, spawnParams);
				AttualFruit = static_cast<AMapFood*>(RawFruit);
				if (SecondFruit == false) {
					RemainFruitsVisual[1]->Destroy();
					SecondFruit = true;
				}
				else {
					RemainFruitsVisual[0]->Destroy();
				}
				AttualFruit->Type = LevelFruitsType[GameMapLevel - 1];
				FruitTimer = 9.5;
			}

			if (FlagGlobalCounter) {
				GlobalGhostPoints++;
				if (GlobalGhostPoints == 7) {
					AGhosts* NextGhost = NextGhostToExit();
					if (NextGhost && NextGhost->IsA(APinkyGhosts::StaticClass())) {
						NextGhost->isInCage = false;
					}
				}
				if (GlobalGhostPoints == 17) {
					AGhosts* NextGhost = NextGhostToExit();
					if (NextGhost && NextGhost->IsA(AInkyGhosts::StaticClass())) {
						NextGhost->isInCage = false;
					}
				}
				if (GlobalGhostPoints == 32) {
					GlobalGhostPoints = 0;
					FlagGlobalCounter = false;
				}
			}
			else {
				AGhosts* NextGhost = NextGhostToExit();
				if (NextGhost && NextGhost->PointsCounter != NextGhost->GhostsFirstExitPoints) {
					NextGhost->PointsCounter++;
				}
			}
		}

		if (FlagGlobalCounter && PacMan->PacManFirstMoveFlag) {
			TimerPacManPointEaten += DeltaTime;

			int ExitLimitTime = (GameMapLevel >= 5) ? 3 : 4;
			if (TimerPacManPointEaten >= ExitLimitTime) {
				AGhosts* NextGhost = NextGhostToExit();
				if (NextGhost != nullptr) {
					NextGhost->isInCage = false;
				}
				TimerPacManPointEaten = 0;
			}
		}

		if (!FlagGlobalCounter) {
			AGhosts* NextGhost = NextGhostToExit();
			if (NextGhost != nullptr && NextGhost->PointsCounter == NextGhost->GhostsFirstExitPoints) {
				NextGhost->isInCage = false;
			}
		}
	}


	//Spawned fruit countdown
	if (AttualFruit != nullptr) {
		if (FruitTimer < 0) {
			AttualFruit->Destroy();
			AttualFruit = nullptr;
		}
		else {
			FruitTimer -= DeltaTime;
		}
	}
}


void AGameMap::Map() {

	//setting MapBlocks, BasicFood and PacMan
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = -18; i < 18; i++) {
		for (int k = -14; k < 14; k++) {

			char MapPieceType = arrayMap[i + 18][k + 14];

			if (MapPieceType == 'T')
				continue;

			TSubclassOf<AActor> MapPieceToSpawn;
			FVector TranslationOffset(k * 100, i * 100, 0.0f);
			FTransform FinalTransform = GetTransform();
			FinalTransform.AddToTranslation(TranslationOffset);

			switch (MapPieceType)
			{
			case 'M':
				MapPieceToSpawn = Wall;
				break;
			case 'N':
			case 'B':
			case 'X':
				MapPieceToSpawn = Street;
				break;
			case 'K':
				MapPieceToSpawn = Slowed;
				break;
			case 'V':
				MapPieceToSpawn = Gate;
				break;
			default:
				MapPieceToSpawn = Street;
				break;
			}

			AActor* MapPieceSpawned = GetWorld()->SpawnActor<AActor>(MapPieceToSpawn, FinalTransform, SpawnParams);
			AMapBlock* ParsedMapPiece = Cast<AMapBlock>(MapPieceSpawned);

			if (ParsedMapPiece) {
				ParsedMapPiece->x = i + 18;
				ParsedMapPiece->y = k + 14;
				GameMapBlocks[i + 18][k + 14] = ParsedMapPiece;
			}

			if (MapPieceType == 'X') {
				SpawnPacMan(FinalTransform, MapPieceSpawned);
			}
			else if (MapPieceType == 'R') {
				GhostsCustomBlocks[0] = ParsedMapPiece;
				TargetCageBlocks[0] = ParsedMapPiece;
			}
			else if (MapPieceType == 'O') {
				GhostsCustomBlocks[1] = ParsedMapPiece;
			}
			else if (MapPieceType == 'Z') {
				TargetCageBlocks[1] = ParsedMapPiece;
			}
			else if (MapPieceType == 'P') {
				GhostsCustomBlocks[2] = ParsedMapPiece;
			}
			else if (MapPieceType == 'I') {
				GhostsCustomBlocks[3] = ParsedMapPiece;
			}
			else if (MapPieceType == 'B') {
				FVector PointOffset(0, 0, 35);
				FTransform PointTransform = FinalTransform;
				PointTransform.AddToTranslation(PointOffset);
				GetWorld()->SpawnActor<AActor>(BasicFood, PointTransform, SpawnParams);
				GameMapBlocks[i + 18][k + 14]->Food = true;
			}
			else if (MapPieceType == 's') {
				GameMapBlocks[i + 18][k + 14]->Food = true;
			}
		}
	}


	//setting SuperFood
	FVector PointOffSett(0, 0, 30.0f);

	FVector FoodOffSetLocation(-1300, -1200, 40.0f);
	FTransform FoodTransform = GetTransform();
	FoodTransform.AddToTranslation(FoodOffSetLocation);
	AActor* food1 = GetWorld()->SpawnActor<AActor>(SuperFood, FoodTransform, spawnParams);
	AMapFood* parsedFood1 = static_cast<AMapFood*>(food1);
	parsedFood1->Type = Utility::SUPER;

	FoodOffSetLocation = FVector(-1300, 800, 40.0f);
	FoodTransform = GetTransform();
	FoodTransform.AddToTranslation(FoodOffSetLocation);
	AActor* food2 = GetWorld()->SpawnActor<AActor>(SuperFood, FoodTransform, spawnParams);
	AMapFood* parsedFood2 = static_cast<AMapFood*>(food2);
	parsedFood2->Type = Utility::SUPER;

	FoodOffSetLocation = FVector(1200, -1200, 40.0f);
	FoodTransform = GetTransform();
	FoodTransform.AddToTranslation(FoodOffSetLocation);
	AActor* food3 = GetWorld()->SpawnActor<AActor>(SuperFood, FoodTransform, spawnParams);
	AMapFood* parsedFood3 = static_cast<AMapFood*>(food3);
	parsedFood3->Type = Utility::SUPER;

	FoodOffSetLocation = FVector(1200, 800, 40.0f);
	FoodTransform = GetTransform();
	FoodTransform.AddToTranslation(FoodOffSetLocation);
	AActor* food4 = GetWorld()->SpawnActor<AActor>(SuperFood, FoodTransform, spawnParams);
	AMapFood* parsedFood4 = static_cast<AMapFood*>(food4);
	parsedFood4->Type = Utility::SUPER;


	//setting CurrentTeleportBlocks
	AActor* CurrentTeleportBlocks[2] = { nullptr };

	FVector TeleportOffSetLocation(-1400, -100, 00.0f);
	FTransform TelportTransform = GetTransform();
	TelportTransform.AddToTranslation(TeleportOffSetLocation);
	CurrentTeleportBlocks[0] = GetWorld()->SpawnActor<AActor>(Teleport, TelportTransform, spawnParams);

	FVector TeleportOffSetLocation2(1300, -100, 00.0f);
	FTransform TelportTransform2 = GetTransform();
	TelportTransform2.AddToTranslation(TeleportOffSetLocation2);
	CurrentTeleportBlocks[1] = GetWorld()->SpawnActor<AActor>(Teleport, TelportTransform2, spawnParams);

	AMapBlock* MapTeleport1 = dynamic_cast<AMapBlock*>(CurrentTeleportBlocks[0]);
	AMapBlock* MapTeleport2 = dynamic_cast<AMapBlock*>(CurrentTeleportBlocks[1]);
	MapTeleport1->x = 0;
	MapTeleport1->y = 17;
	MapTeleport2->x = 27;
	MapTeleport2->y = 17;
	MapTeleport1->TeleportDestination = MapTeleport2;
	MapTeleport1->Direction = Utility::LEFT;
	MapTeleport2->TeleportDestination = MapTeleport1;
	MapTeleport2->Direction = Utility::RIGHT;

	GameMapBlocks[17][0] = MapTeleport1;
	GameMapBlocks[17][27] = MapTeleport2;


	//saving blocks matrix
	for (int i = 0; i < 36; i++) {
		for (int k = 0; k < 28; k++) {

			AMapBlock* selectedBlock = GameMapBlocks[i][k];

			selectedBlock->LateralBlocks[0] = (i > 0) ? GameMapBlocks[i - 1][k] : nullptr;  // UP
			selectedBlock->LateralBlocks[1] = (k > 0) ? GameMapBlocks[i][k - 1] : nullptr;  // LEFT
			selectedBlock->LateralBlocks[2] = (i < 35) ? GameMapBlocks[i + 1][k] : nullptr; // DOWN
			selectedBlock->LateralBlocks[3] = (k < 27) ? GameMapBlocks[i][k + 1] : nullptr; // RIGHT
		}
	}
	GameMapBlocks[0][17]->LateralBlocks[1] = GameMapBlocks[27][17];
	GameMapBlocks[27][17]->LateralBlocks[3] = GameMapBlocks[0][17];
}


void AGameMap::SpawnPacMan(FTransform Transform, AActor* Block)
{

	//setting PacMan
	FVector TranslationOffset(0, 0, 0);
	Transform.AddToTranslation(TranslationOffset);

	APawn* PacManPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PacManPawn)
	{
		PacManPawn->SetActorTransform(Transform);
		PacMan = Cast<APacManPawn>(PacManPawn);

		if (PacMan)
		{
			PacMan->Lives = RemainLivesToPlay - 1;
			PacMan->TotalPoints = &TotalPoints;
			PacMan->GameMapCompleted = GameMapCompleted;
			PacMan->Random_Actor = Random_Actor;
			PacMan->Conservative_Actor = Conservative_Actor;
			PacMan->Balanced_Actor = Balanced_Actor;

			AMapBlock* MapBlock = Cast<AMapBlock>(Block);
			if (MapBlock)
			{
				PacMan->CurrentMapBlock = MapBlock;
				PacMan->PacManDefaultMapBlock = MapBlock;
			}

			ResetPacMan(true);
		}
	}
}


void AGameMap::Graphics() {

	//setting camera
	FActorSpawnParameters SpawnParams;
	ACameraActor* MyFixedCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), FVector(0.f, -60.f, 3400.f), FRotator(-90.f, -90.f, 0.f), SpawnParams);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetViewTargetWithBlend(MyFixedCamera, 0.f);


	//setting visual Lives and fruits
	TSubclassOf<AActor> LevelFruitsTEMPLATE[] = { Cherries, Strawberry, Peach, Peach, Apple, Apple, Grapes, Grapes, Galaxian, Galaxian, Bell, Bell, Key, Key, Key, Key, Key, Key, Key, Key, Key };
	for (int i = 0; i < 21; i++) {
		LevelFruits[i] = LevelFruitsTEMPLATE[i];
	}
	AActor* Fruit1 = GetWorld()->SpawnActor<AActor>(LevelFruits[GameMapLevel - 1], FVector(1200.f, 1640.f, 0.f), FRotator::ZeroRotator, spawnParams);
	RemainFruitsVisual[0] = Fruit1;
	AActor* Fruit2 = GetWorld()->SpawnActor<AActor>(LevelFruits[GameMapLevel - 1], FVector(1045.f, 1640.f, 0.f), FRotator::ZeroRotator, spawnParams);
	RemainFruitsVisual[1] = Fruit2;

	if (RemainLivesToPlay >= 1) {
		AActor* life1 = GetWorld()->SpawnActor<AActor>(PacManLives, FVector(-1280.f, 1640.f, 0.f), FRotator::ZeroRotator, spawnParams);
		PacManLivesVisual[0] = life1;
	}
	if (RemainLivesToPlay >= 2) {
		AActor* life2 = GetWorld()->SpawnActor<AActor>(PacManLives, FVector(-1125.f, 1640.f, 0.f), FRotator::ZeroRotator, spawnParams);
		PacManLivesVisual[1] = life2;
	}
	if (RemainLivesToPlay == 3) {
		AActor* life3 = GetWorld()->SpawnActor<AActor>(PacManLives, FVector(-970.f, 1640.f, 0.f), FRotator::ZeroRotator, spawnParams);
		PacManLivesVisual[2] = life3;
	}


	//setting info level and points
	Level_Text_Actor = NewObject<UTextRenderComponent>(this, TEXT("LEVEL_MAP_TEXT"));
	Level_Text_Actor->SetText(FText::FromString("LEVEL"));
	Level_Text_Actor->SetWorldSize(125.0f);
	Level_Text_Actor->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Level_Text_Actor->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Level_Text_Actor->SetHorizSpacingAdjust(1.60f);
	Level_Text_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
	Level_Text_Actor->SetWorldLocation(FVector(-1000, -1750, 100));
	Level_Text_Actor->SetWorldRotation(FRotator(90, 90, 0));
	Level_Text_Actor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Level_Text_Actor->RegisterComponent();

	Level_Actor = NewObject<UTextRenderComponent>(this, TEXT("LEVEL_MAP"));
	Level_Actor->SetText(FText::FromString(FString::FromInt(GameMapLevel + GameMapLevelExtra)));
	Level_Actor->SetWorldSize(125.0f);
	Level_Actor->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Level_Actor->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Level_Actor->SetHorizSpacingAdjust(1.60f);
	Level_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
	Level_Actor->SetWorldLocation(FVector(-1000, -1590, 100));
	Level_Actor->SetWorldRotation(FRotator(90, 90, 0));
	Level_Actor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Level_Actor->RegisterComponent();

	Achieved_Points_Actor = NewObject<UTextRenderComponent>(this, TEXT("ACHIEVED_POINTS_TEXT"));
	Achieved_Points_Actor->SetText(FText::FromString("ACHIEVED POINTS"));
	Achieved_Points_Actor->SetWorldSize(125.0f);
	Achieved_Points_Actor->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Achieved_Points_Actor->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Achieved_Points_Actor->SetHorizSpacingAdjust(1.60f);
	Achieved_Points_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
	Achieved_Points_Actor->SetWorldLocation(FVector(-60, -1750, 100));
	Achieved_Points_Actor->SetWorldRotation(FRotator(90, 90, 0));
	Achieved_Points_Actor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Achieved_Points_Actor->RegisterComponent();

	Points_Actor = NewObject<UTextRenderComponent>(this, TEXT("PointTEXT"));
	Points_Actor->SetText(FText::FromString("0"));
	Points_Actor->SetWorldSize(125.0f);
	Points_Actor->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Points_Actor->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Points_Actor->SetHorizSpacingAdjust(1.60f);
	Points_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
	Points_Actor->SetWorldLocation(FVector(-60, -1590, 100));
	Points_Actor->SetWorldRotation(FRotator(90, 90, 0));
	Points_Actor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Points_Actor->RegisterComponent();

	Total_Time_Actor = NewObject<UTextRenderComponent>(this, TEXT("Total_Time"));
	Total_Time_Actor->SetText(FText::FromString("TOTAL TIME"));
	Total_Time_Actor->SetWorldSize(125.0f);
	Total_Time_Actor->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Total_Time_Actor->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Total_Time_Actor->SetHorizSpacingAdjust(1.60f);
	Total_Time_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
	Total_Time_Actor->SetWorldLocation(FVector(2500, -1750, 100));
	Total_Time_Actor->SetWorldRotation(FRotator(90, 90, 0));
	Total_Time_Actor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Total_Time_Actor->RegisterComponent();

	Total_Time_Value_Actor = NewObject<UTextRenderComponent>(this, TEXT("Total_Time_Value"));
	Total_Time_Value_Actor->SetText(FText::FromString("0"));
	Total_Time_Value_Actor->SetWorldSize(125.0f);
	Total_Time_Value_Actor->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Total_Time_Value_Actor->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Total_Time_Value_Actor->SetHorizSpacingAdjust(1.60f);
	Total_Time_Value_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
	Total_Time_Value_Actor->SetWorldLocation(FVector(2500, -1590, 100));
	Total_Time_Value_Actor->SetWorldRotation(FRotator(90, 90, 0));
	Total_Time_Value_Actor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Total_Time_Value_Actor->RegisterComponent();


	//setting Lights
	Light_Actor = NewObject<UDirectionalLightComponent>(this, TEXT("Light"));
	Light_Actor->SetLightColor(FLinearColor(1, 1, 1));
	Light_Actor->SetIntensity(20);
	Light_Actor->SetWorldLocationAndRotation(FVector(-500, 0, 0), FRotator(-90, 0, 0));
	Light_Actor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Light_Actor->RegisterComponent();

	Light_Actor2 = NewObject<UDirectionalLightComponent>(this, TEXT("Light"));
	Light_Actor2->SetLightColor(FLinearColor(1, 1, 1));
	Light_Actor2->SetIntensity(20);
	Light_Actor2->SetWorldLocationAndRotation(FVector(500, 0, 0), FRotator(-90, 0, 0));
	Light_Actor2->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Light_Actor2->RegisterComponent();


	//settings Algo
	Random_Actor = NewObject<UTextRenderComponent>(this, TEXT("Random"));
	Random_Actor->SetText(FText::FromString("(1) Random"));
	Random_Actor->SetWorldSize(221.0f);
	Random_Actor->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
	Random_Actor->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Random_Actor->SetHorizSpacingAdjust(1.00f);
	Random_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
	Random_Actor->SetWorldLocation(FVector(-3400, -625, 0));
	Random_Actor->SetWorldRotation(FRotator(90, 90, 0));
	Random_Actor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Random_Actor->RegisterComponent();

	Conservative_Actor = NewObject<UTextRenderComponent>(this, TEXT("Conservative"));
	Conservative_Actor->SetText(FText::FromString("(2) Conservative"));
	Conservative_Actor->SetWorldSize(221.0f);
	Conservative_Actor->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
	Conservative_Actor->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Conservative_Actor->SetHorizSpacingAdjust(1.00f);
	Conservative_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
	Conservative_Actor->SetWorldLocation(FVector(-3400, -100, 0));
	Conservative_Actor->SetWorldRotation(FRotator(90, 90, 0));
	Conservative_Actor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Conservative_Actor->RegisterComponent();

	Balanced_Actor = NewObject<UTextRenderComponent>(this, TEXT("Balanced"));
	Balanced_Actor->SetText(FText::FromString("(3) Balanced"));
	Balanced_Actor->SetWorldSize(221.0f);
	Balanced_Actor->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
	Balanced_Actor->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Balanced_Actor->SetHorizSpacingAdjust(1.00f);
	Balanced_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
	Balanced_Actor->SetWorldLocation(FVector(-3400, 425, 0));
	Balanced_Actor->SetWorldRotation(FRotator(90, 90, 0));
	Balanced_Actor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Balanced_Actor->RegisterComponent();
}


void AGameMap::SpawnGhosts() {

	//Blinky
	FTransform blinkyTransform = GetTransform();
	blinkyTransform.SetLocation(FVector(-100, -400, 10));
	AActor* BlinkyActor = GetWorld()->SpawnActor<AActor>(BlinkyGhost, blinkyTransform, spawnParams);
	BlinkyGhostSpawn = dynamic_cast<ABlinkyGhosts*>(BlinkyActor);
	
	BlinkyGhostSpawn->GhostsFirstExitPoints = 0;
	BlinkyGhostSpawn->Type = 'B';
	BlinkyGhostSpawn->PacMan = PacMan;
	BlinkyGhostSpawn->GameMapLevel = GameMapLevel;
	BlinkyGhostSpawn->isInCage = false;
	BlinkyGhostSpawn->CurrentMapBlock = GhostsCustomBlocks[0];
	BlinkyGhostSpawn->DefaultExitCageMapBlock = GhostsCustomBlocks[0];
	BlinkyGhostSpawn->TargetCageBlocks[0] = TargetCageBlocks[0];
	BlinkyGhostSpawn->TargetCageBlocks[1] = TargetCageBlocks[1];
	BlinkyGhostSpawn->ScatterModTargetMapBlock = GameMapBlocks[0][25];
	BlinkyGhostSpawn->CounterGhostsEaten = &CounterGhostsEaten;
	BlinkyGhostSpawn->GameMapPoints = &GameMapPoints;
	BlinkyGhostSpawn->ModElroy1 = false;
	BlinkyGhostSpawn->ModElroy2 = false;
	BlinkyGhostSpawn->ShouldMoveElroy = false;
	BlinkyGhostSpawn->CagePosition = -1;
	Ghosts[0] = BlinkyGhostSpawn;

	for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 28; y++) {
			BlinkyGhostSpawn->GameMapBlocks[x][y] = GameMapBlocks[x][y];
		}
	}

	//Pinky
	FTransform pinkyTransform = GetTransform();
	pinkyTransform.SetLocation(FVector(-50, -100, 10));
	AActor* PinkyActor = GetWorld()->SpawnActor<AActor>(PinkyGhost, pinkyTransform, spawnParams);
	PinkyGhostSpawn = dynamic_cast<APinkyGhosts*>(PinkyActor);
	
	PinkyGhostSpawn->GhostsFirstExitPoints = 0;
	PinkyGhostSpawn->Type = 'P';
	PinkyGhostSpawn->GameMapLevel = GameMapLevel;
	PinkyGhostSpawn->PacMan = PacMan;
	PinkyGhostSpawn->CurrentMapBlock = GhostsCustomBlocks[1];
	PinkyGhostSpawn->DefaultExitCageMapBlock = GhostsCustomBlocks[0];
	PinkyGhostSpawn->TargetCageBlocks[0] = TargetCageBlocks[0];
	PinkyGhostSpawn->TargetCageBlocks[1] = TargetCageBlocks[1];
	PinkyGhostSpawn->ScatterModTargetMapBlock = GameMapBlocks[0][2];
	PinkyGhostSpawn->ModElroy1 = false;
	PinkyGhostSpawn->ModElroy2 = false;
	PinkyGhostSpawn->ShouldMoveElroy = false;
	PinkyGhostSpawn->MovDirection = Utility::UP;
	PinkyGhostSpawn->CagePosition = 0;
	PinkyGhostSpawn->CagePath[0] = FVector(-50, -200, 10);
	PinkyGhostSpawn->CagePath[1] = FVector(-50, -100, 10);
	PinkyGhostSpawn->CagePath[2] = FVector(-50, 0, 10);
	PinkyGhostSpawn->CagePath[3] = FVector(-50, -100, 10);
	PinkyGhostSpawn->ExtitCagePath[0] = FVector(-50, -100, 10);
	PinkyGhostSpawn->ExtitCagePath[1] = FVector(-50, -100, 10);
	PinkyGhostSpawn->ExtitCagePath[2] = FVector(-50, -200, 10);
	PinkyGhostSpawn->ExtitCagePath[3] = FVector(-50, -300, 10);
	PinkyGhostSpawn->ExtitCagePath[4] = FVector(-50, -400, 10);
	PinkyGhostSpawn->ExtitCagePath[5] = FVector(-100, -400, 10);
	PinkyGhostSpawn->ExitCageMovement[1] = Utility::IDLE;
	PinkyGhostSpawn->CounterGhostsEaten = &CounterGhostsEaten;
	PinkyGhostSpawn->GameMapPoints = &GameMapPoints;
	Ghosts[1] = PinkyGhostSpawn;

	for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 28; y++) {
			PinkyGhostSpawn->GameMapBlocks[x][y] = GameMapBlocks[x][y];
		}
	}

	//Inky
	FTransform inkyTransform = GetTransform();
	inkyTransform.SetLocation(FVector(-250, -100, 10));
	AActor* InkyActor = GetWorld()->SpawnActor<AActor>(InkyGhost, inkyTransform, spawnParams);
	InkyGhostsSpawn = dynamic_cast<AInkyGhosts*>(InkyActor);
	
	if (GameMapLevel == 1) {
		InkyGhostsSpawn->GhostsFirstExitPoints = 30;
	}
	else if (GameMapLevel >= 2) {
		InkyGhostsSpawn->GhostsFirstExitPoints = 0;
	}
	InkyGhostsSpawn->Type = 'I';
	InkyGhostsSpawn->GameMapLevel = GameMapLevel;
	InkyGhostsSpawn->PacMan = PacMan;
	InkyGhostsSpawn->blinky = BlinkyGhostSpawn;
	InkyGhostsSpawn->ModElroy1 = false;
	InkyGhostsSpawn->ModElroy2 = false;
	InkyGhostsSpawn->ShouldMoveElroy = false;
	InkyGhostsSpawn->CurrentMapBlock = GhostsCustomBlocks[2];
	InkyGhostsSpawn->DefaultExitCageMapBlock = GhostsCustomBlocks[0];
	InkyGhostsSpawn->TargetCageBlocks[0] = TargetCageBlocks[0];
	InkyGhostsSpawn->TargetCageBlocks[1] = TargetCageBlocks[1];
	InkyGhostsSpawn->ScatterModTargetMapBlock = GameMapBlocks[34][27];
	InkyGhostsSpawn->MovDirection = Utility::UP;
	InkyGhostsSpawn->CagePosition = 0;
	InkyGhostsSpawn->CagePath[0] = FVector(-250, -200, 10);
	InkyGhostsSpawn->CagePath[1] = FVector(-250, -100, 10);
	InkyGhostsSpawn->CagePath[2] = FVector(-250, 0, 10);
	InkyGhostsSpawn->CagePath[3] = FVector(-250, -100, 10);
	InkyGhostsSpawn->ExtitCagePath[0] = FVector(-250, -100, 10);
	InkyGhostsSpawn->ExtitCagePath[1] = FVector(-50, -100, 10);
	InkyGhostsSpawn->ExtitCagePath[2] = FVector(-50, -200, 10);
	InkyGhostsSpawn->ExtitCagePath[3] = FVector(-50, -300, 10);
	InkyGhostsSpawn->ExtitCagePath[4] = FVector(-50, -400, 10);
	InkyGhostsSpawn->ExtitCagePath[5] = FVector(-100, -400, 10);
	InkyGhostsSpawn->ExitCageMovement[1] = Utility::RIGHT;
	InkyGhostsSpawn->CounterGhostsEaten = &CounterGhostsEaten;
	InkyGhostsSpawn->GameMapPoints = &GameMapPoints;
	Ghosts[2] = InkyGhostsSpawn;

	for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 28; y++) {
			InkyGhostsSpawn->GameMapBlocks[x][y] = GameMapBlocks[x][y];
		}
	}


	//Clyde
	FTransform clydeTransform = GetTransform();
	clydeTransform.SetLocation(FVector(150, -100, 10));
	AActor* ClydeActor = GetWorld()->SpawnActor<AActor>(ClydeGhost, clydeTransform, spawnParams);
	ClydeGhostsSpawn = dynamic_cast<AClydeGhosts*>(ClydeActor);

	if (GameMapLevel == 1) {
		ClydeGhostsSpawn->GhostsFirstExitPoints = 60;
	}
	else if (GameMapLevel == 2) {
		ClydeGhostsSpawn->GhostsFirstExitPoints = 50;
	}
	else if (GameMapLevel >= 3) {
		ClydeGhostsSpawn->GhostsFirstExitPoints = 0;
	}
	ClydeGhostsSpawn->Type = 'C';
	ClydeGhostsSpawn->GameMapLevel = GameMapLevel;
	ClydeGhostsSpawn->PacMan = PacMan;
	ClydeGhostsSpawn->CurrentMapBlock = GhostsCustomBlocks[3];
	ClydeGhostsSpawn->DefaultExitCageMapBlock = GhostsCustomBlocks[0];
	ClydeGhostsSpawn->TargetCageBlocks[0] = TargetCageBlocks[0];
	ClydeGhostsSpawn->TargetCageBlocks[1] = TargetCageBlocks[1];
	ClydeGhostsSpawn->ScatterModTargetMapBlock = GameMapBlocks[34][0];
	ClydeGhostsSpawn->ModElroy1 = false;
	ClydeGhostsSpawn->ModElroy2 = false;
	ClydeGhostsSpawn->ShouldMoveElroy = false;
	ClydeGhostsSpawn->MovDirection = Utility::UP;
	ClydeGhostsSpawn->CagePosition = 0;
	ClydeGhostsSpawn->CagePath[0] = FVector(150, -200, 10);
	ClydeGhostsSpawn->CagePath[1] = FVector(150, -100, 10);
	ClydeGhostsSpawn->CagePath[2] = FVector(150, 0, 10);
	ClydeGhostsSpawn->CagePath[3] = FVector(150, -100, 10);
	ClydeGhostsSpawn->ExtitCagePath[0] = FVector(150, -100, 10);
	ClydeGhostsSpawn->ExtitCagePath[1] = FVector(-50, -100, 10);
	ClydeGhostsSpawn->ExtitCagePath[2] = FVector(-50, -200, 10);
	ClydeGhostsSpawn->ExtitCagePath[3] = FVector(-50, -300, 10);
	ClydeGhostsSpawn->ExtitCagePath[4] = FVector(-50, -400, 10);
	ClydeGhostsSpawn->ExtitCagePath[5] = FVector(-100, -400, 10);
	ClydeGhostsSpawn->ExitCageMovement[1] = Utility::LEFT;
	ClydeGhostsSpawn->CounterGhostsEaten = &CounterGhostsEaten;
	ClydeGhostsSpawn->GameMapPoints = &GameMapPoints;
	Ghosts[3] = ClydeGhostsSpawn;

	for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 28; y++) {
			ClydeGhostsSpawn->GameMapBlocks[x][y] = GameMapBlocks[x][y];
		}
	}
}


AGhosts* AGameMap::NextGhostToExit()
{

	//Get next ghost to exit cage
	for (int i = 0; i < 4; i++) {
		AGhosts* ghost = Ghosts[i];
		if (ghost->Move && ghost->isInCage) {
			return ghost;
		}
	}
	return nullptr;
}


void AGameMap::ResetPacMan(bool raw) {

	//Reset PacMan deafult values
	if (raw) {
		PacMan->RawPoints = 0;
	}

	PacMan->SetActorLocation(FVector(-100, 800, 50.0f));
	PacMan->SetActorHiddenInGame(false);
	PacMan->MovDirection = Utility::IDLE;
	PacMan->CurrentMapBlock = PacMan->PacManDefaultMapBlock;
	PacMan->NextMapBlock = nullptr;
	PacMan->GameMapLevel = GameMapLevel;
	PacMan->iFrightnedMod = false;
	PacMan->PacManFirstMoveFlag = false;
	FlagGhosts = true;
}


void AGameMap::ResetGhosts() {

	//Reset ghosts default values
	PacMan->iFrightnedMod = false;

	BlinkyGhostSpawn->SetActorLocation(FVector(-100, -400, 10));
	BlinkyGhostSpawn->MovDirection = Utility::LEFT;
	BlinkyGhostSpawn->NextMove = Utility::LEFT;
	BlinkyGhostSpawn->CurrentMapBlock = GhostsCustomBlocks[0];
	BlinkyGhostSpawn->CagePosition = -1;
	BlinkyGhostSpawn->Reset = true;

	PinkyGhostSpawn->SetActorLocation(FVector(-50, -100, 10));
	PinkyGhostSpawn->MovDirection = Utility::UP;
	PinkyGhostSpawn->NextMove = Utility::LEFT;
	PinkyGhostSpawn->CurrentMapBlock = GhostsCustomBlocks[1];
	PinkyGhostSpawn->isInCage = true;
	PinkyGhostSpawn->PreviousIsInCage = true;
	PinkyGhostSpawn->CagePosition = 0;
	PinkyGhostSpawn->Reset = true;

	InkyGhostsSpawn->SetActorLocation(FVector(-250, -100, 10));
	InkyGhostsSpawn->MovDirection = Utility::UP;
	InkyGhostsSpawn->NextMove = Utility::LEFT;
	InkyGhostsSpawn->CurrentMapBlock = GhostsCustomBlocks[2];
	InkyGhostsSpawn->isInCage = true;
	InkyGhostsSpawn->PreviousIsInCage = true;
	InkyGhostsSpawn->CagePosition = 0;
	InkyGhostsSpawn->Reset = true;

	ClydeGhostsSpawn->SetActorLocation(FVector(150, -100, 10));
	ClydeGhostsSpawn->MovDirection = Utility::UP;
	ClydeGhostsSpawn->NextMove = Utility::LEFT;
	ClydeGhostsSpawn->CurrentMapBlock = GhostsCustomBlocks[3];
	ClydeGhostsSpawn->isInCage = true;
	ClydeGhostsSpawn->PreviousIsInCage = true;
	ClydeGhostsSpawn->CagePosition = 0;
	ClydeGhostsSpawn->Reset = true;
}


void AGameMap::Destroy() {

	//Desotry Map
	Super::Destroy();

	for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 28; y++) {
			GameMapBlocks[x][y]->Destroy();
		}
	}

	if (PacManLivesVisual[0] != nullptr)
		PacManLivesVisual[0]->Destroy();
	if (PacManLivesVisual[1] != nullptr)
		PacManLivesVisual[1]->Destroy();
	if (PacManLivesVisual[2] != nullptr)
		PacManLivesVisual[2]->Destroy();

	BlinkyGhostSpawn->Destroy();
	PinkyGhostSpawn->Destroy();
	InkyGhostsSpawn->Destroy();
	ClydeGhostsSpawn->Destroy();
}