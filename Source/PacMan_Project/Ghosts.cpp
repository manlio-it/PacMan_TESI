#include "Ghosts.h"


AGhosts::AGhosts()
{

	PrimaryActorTick.bCanEverTick = true;
}


void AGhosts::BeginPlay()
{

	Super::BeginPlay();
	MeshComp = FindComponentByClass<UStaticMeshComponent>();
	BaseGhostsMaterial = MeshComp->GetMaterial(0);
	srand(6260);
}


void AGhosts::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Resetting ghosts (command from GameMap, common default parameters)
	if (Reset)
	{
		srand(6260);
		*CounterGhostsEaten = 0;
		MeshComp->SetMaterial(0, BaseGhostsMaterial);
		Reset = false;
		ShouldMoveElroy = true;
		Base_distance_per_second = Start_Base_distance_per_second;
		SlowApply = false;
		MovementMod = PreviousMovementMod;
		isDead = false;
		GhostsFrightnedTimer = 0;
		GhostsFlashTime = 0;
	}


	//Frightened timer and graphics
	if (PacMan->iFrightnedMod && MovementMod != Utility::FRIGHTENED) {
		if (!isInCage) {
			OppositionMovementFlag = true;
		}
		MovementMod = Utility::FRIGHTENED;
		GhostFlashTimes = LevelGhostFlashTimes[GameMapLevel - 1];
		GhostsFrightnedTimer = LevelGhostsFrightnedTime[GameMapLevel - 1];
		MeshComp->SetMaterial(0, FrightnedMaterial);
	}
	if (GhostsFrightnedTimer < GhostFlashTimes * 0.3 && !isDead) {
		GhostsFlashTime += DeltaTime;
		if (GhostsFlashTime > 0.3) {
			MeshComp->SetMaterial(0, FrightnedMaterialFlash);
			GhostsFlashTime = 0;
		}
		else if (GhostsFlashTime > 0.15) {
			MeshComp->SetMaterial(0, FrightnedMaterial);
		}
	}
	if (GhostsFrightnedTimer >= 0 && !isDead) {
		GhostsFrightnedTimer -= DeltaTime;
	}
	if (GhostsFrightnedTimer < 0) {
		GhostsFrightnedTimer = 0;
		PacMan->iFrightnedMod = false;
		*CounterGhostsEaten = 0;
		MovementMod = PreviousMovementMod;
		MeshComp->SetMaterial(0, BaseGhostsMaterial);
	}


	//Ghosts movement management
	if (!Move)
		return;

	Result_distance_per_second = Base_distance_per_second * LevelGhostSpeed[GameMapLevel - 1];

	if (ModElroy1 && MovementMod != Utility::FRIGHTENED && ShouldMoveElroy == false) {
		Result_distance_per_second = Base_distance_per_second * SpeedElroy1[GameMapLevel - 1];
	}
	if (ModElroy2 && MovementMod != Utility::FRIGHTENED && ShouldMoveElroy == false) {
		Result_distance_per_second = Base_distance_per_second * SpeedElroy2[GameMapLevel - 1];
	}
	if (MovementMod == Utility::FRIGHTENED) {
		Result_distance_per_second = Base_distance_per_second * LevelGhostsFrightnedSpeed[GameMapLevel - 1];
	}
	if (isDead) {
		Result_distance_per_second = Base_distance_per_second * LevelGhostSpeed[GameMapLevel - 1] * 2;
	}


	FVector MovementVector;
	switch (MovDirection)
	{
	case Utility::LEFT: MovementVector = FVector(-1.f, 0.f, 0.f);
		break;
	case Utility::RIGHT: MovementVector = FVector(1.f, 0.f, 0.f);
		break;
	case Utility::UP: MovementVector = FVector(0.f, -1.f, 0.f);
		break;
	case Utility::DOWN: MovementVector = FVector(0.f, 1.f, 0.f);
		break;
	default:
		break;
	}

	FVector NewLocation = GetActorLocation() + (MovementVector * (Result_distance_per_second * DeltaTime));
	SetActorLocation(NewLocation);
	if (CagePosition != -1) {
		Movement_Cage((MovementVector * (Result_distance_per_second * DeltaTime)));
		return;
	}


	//Passage between scatter and chase 
	if (MovementMod != Utility::FRIGHTENED && !isInCage) {
		TimeMods += DeltaTime;

		if (!OppositionMovementFlag && MovementPositionCycle != 7) {

			if (GameMapLevel == 1) {
				GameMapLevelMatrix = 0;
			}
			if (GameMapLevel >= 2 && GameMapLevel <= 4) {
				GameMapLevelMatrix = 1;
			}
			if (GameMapLevel >= 5) {
				GameMapLevelMatrix = 2;
			}

			if ((TimeMods >= LevelMovementModeTimes[GameMapLevelMatrix][MovementPositionCycle]) && (ModElroy1 == false && ModElroy2 == false)) {
				TimeMods = 0;
				MovementPositionCycle++;
				OppositionMovementFlag = true;
				if (MovementPositionCycle % 2 == 0) {
					MovementMod = Utility::SCATTER;
					PreviousMovementMod = Utility::SCATTER;
				}
				else {
					MovementMod = Utility::CHASE;
					PreviousMovementMod = Utility::CHASE;
				}
			}
			else if ((TimeMods >= LevelMovementModeTimes[GameMapLevelMatrix][MovementPositionCycle]) && (ModElroy1 == true || ModElroy2 == true)) {
				TimeMods = 0;
				MovementPositionCycle++;
				OppositionMovementFlag = true;
				MovementMod = Utility::CHASE;
				PreviousMovementMod = Utility::CHASE;
			}
		}
	}


	//Center of new block reached, teleport, controls and operations for calculation of NextMove
	AMapBlock* DestinationBlock = NextBlock(CurrentMapBlock, MovDirection);
	FVector BlockCenterLoc = DestinationBlock->GetActorLocation();
	FVector GhostLocation = GetActorLocation();

	if (UtilityForRange.InRange(GhostLocation, BlockCenterLoc - (MovementVector * (Result_distance_per_second * DeltaTime)), BlockCenterLoc + (MovementVector * (Result_distance_per_second * DeltaTime)))) {

		if (isDead && (DestinationBlock == TargetCageBlocks[0] || DestinationBlock == TargetCageBlocks[1])) {
			isDead = false;
			GhostsFrightnedTimer = 0;
			MeshComp->SetMaterial(0, BaseGhostsMaterial);
		}
		if (DestinationBlock->isTeleport && DestinationBlock->Direction == MovDirection) {
			FVector TempLocation(DestinationBlock->TeleportDestination->GetCenterPoint());
			FVector Location(TempLocation.X, TempLocation.Y, 50.0f);
			SetActorLocation(Location);
			CurrentMapBlock = DestinationBlock->TeleportDestination;
			return;
		}
		if (DestinationBlock->isSlowToApply == true && SlowApply == false) {
			SlowApply = true;
			Base_distance_per_second = Base_distance_per_second * LevelGhostsTunnelSpeed[GameMapLevel - 1];
		}
		if (DestinationBlock->isSlowToApply == false && SlowApply == true) {
			Base_distance_per_second = Start_Base_distance_per_second;
			SlowApply = false;
		}

		FVector BlockLocation = DestinationBlock->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();
		FVector FixedLocation = FVector(BlockLocation.X, BlockLocation.Y, CurrentLocation.Z);
		SetActorLocation(FixedLocation);

		AMapBlock* OldCurrentMapBlockTEMP = CurrentMapBlock;
		CurrentMapBlock = DestinationBlock;
		Utility::MovDirection OldMovDirection = MovDirection;


		//UPDATING BLOCKS
		if (Type == 'B') {
			for (int x = 0; x < 36; x++) {
				for (int y = 0; y < 28; y++) {
					GameMapBlocks[x][y]->Blinky = false;
				}
			}
			GameMapBlocks[CurrentMapBlock->x][CurrentMapBlock->y]->Blinky = true;
		}
		if (Type == 'P') {
			for (int x = 0; x < 36; x++) {
				for (int y = 0; y < 28; y++) {
					GameMapBlocks[x][y]->Pinky = false;
				}
			}
			GameMapBlocks[CurrentMapBlock->x][CurrentMapBlock->y]->Pinky = true;
		}
		if (Type == 'I') {
			for (int x = 0; x < 36; x++) {
				for (int y = 0; y < 28; y++) {
					GameMapBlocks[x][y]->Inky = false;
				}
			}
			GameMapBlocks[CurrentMapBlock->x][CurrentMapBlock->y]->Inky = true;
		}
		if (Type == 'C') {
			for (int x = 0; x < 36; x++) {
				for (int y = 0; y < 28; y++) {
					GameMapBlocks[x][y]->Clyde = false;
				}
			}
			GameMapBlocks[CurrentMapBlock->x][CurrentMapBlock->y]->Clyde = true;
		}


		if (MovementMod == Utility::FRIGHTENED && !isDead) {
			MovDirection = Next_move_FrightnedMod();
		}
		else {
			MovDirection = NextMove;
		}
		if (OppositionMovementFlag) {
			switch (OldMovDirection)
			{
			case Utility::IDLE:
				break;
			case Utility::LEFT:
				MovDirection = Utility::RIGHT;
				break;
			case Utility::RIGHT:
				MovDirection = Utility::LEFT;
				break;
			case Utility::UP:
				MovDirection = Utility::DOWN;
				break;
			case Utility::DOWN:
				MovDirection = Utility::UP;
				break;
			}
			OppositionMovementFlag = false;
		}
		NextMove = Next_move();
	}
}


AMapBlock* AGhosts::NextBlock(AMapBlock* MapBlock, Utility::MovDirection MovDirectionBis) {

	//Next block in one Direction
	switch (MovDirectionBis) {
	case Utility::UP:
		return MapBlock->LateralBlocks[0];
	case Utility::LEFT:
		return MapBlock->LateralBlocks[1];
	case Utility::DOWN:
		return MapBlock->LateralBlocks[2];
	case Utility::RIGHT:
		return MapBlock->LateralBlocks[3];
	default:
		return MapBlock;
	}
}


Utility::MovDirection AGhosts::Next_move() {

	//Thinking a step ahead and calculation for next move (not FRIGHTENED)
	AMapBlock* nextBlock = NextBlock(CurrentMapBlock, MovDirection);
	AMapBlock* left = NextBlock(nextBlock, Utility::LEFT);
	AMapBlock* right = NextBlock(nextBlock, Utility::RIGHT);
	AMapBlock* up = NextBlock(nextBlock, Utility::UP);
	AMapBlock* down = NextBlock(nextBlock, Utility::DOWN);

	switch (MovDirection)
	{
	case Utility::IDLE:
		break;
	case Utility::LEFT:
		right = nullptr;
		break;
	case Utility::RIGHT:
		left = nullptr;
		break;
	case Utility::UP:
		down = nullptr;
		break;
	case Utility::DOWN:
		up = nullptr;
		break;
	}

	AMapBlock* DirBlocks[4] = { right, down, left, up };
	float temp_min_dist = -500;

	FVector Target_pos1;
	FVector Target_pos2;
	if (MovementMod == Utility::CHASE) {
		GetTarget();
		Target_pos1 = Target->GetCenterPoint();
	}
	else {
		Target_pos1 = ScatterModTargetMapBlock->GetCenterPoint();
	}
	if (isDead) {
		Target_pos1 = TargetCageBlocks[0]->GetCenterPoint();;
		Target_pos2 = TargetCageBlocks[1]->GetCenterPoint();
	}

	int FinalDir = 5;
	Utility::MovDirection DirOptions[4] = { Utility::RIGHT, Utility::DOWN, Utility::LEFT, Utility::UP };

	for (int i = 0; i < 4; i++)
	{
		if (DirBlocks[i] == nullptr || !DirBlocks[i]->isWalkable)
			continue;

		if (isDead) {
			FVector pos_block = DirBlocks[i]->GetCenterPoint();
			float temp1 = FVector::Distance(Target_pos1, pos_block);
			float temp2 = FVector::Distance(Target_pos2, pos_block);
			if (temp1 < temp_min_dist || temp_min_dist == -500) {
				temp_min_dist = temp1;
				FinalDir = i;
			}
			if (temp2 < temp_min_dist) {
				temp_min_dist = temp2;
				FinalDir = i;
			}
		}
		else {
			FVector pos_block = DirBlocks[i]->GetCenterPoint();
			float temp = FVector::Distance(Target_pos1, pos_block);
			if (temp < temp_min_dist || temp_min_dist == -500) {
				temp_min_dist = temp;
				FinalDir = i;
			}
		}
	}
	return DirOptions[FinalDir];
}


Utility::MovDirection AGhosts::Next_move_FrightnedMod() {

	//Thinking a step ahead and calculation for next move (FRIGHTENED)
	AMapBlock* left = NextBlock(CurrentMapBlock, Utility::LEFT);
	AMapBlock* right = NextBlock(CurrentMapBlock, Utility::RIGHT);
	AMapBlock* up = NextBlock(CurrentMapBlock, Utility::UP);
	AMapBlock* down = NextBlock(CurrentMapBlock, Utility::DOWN);

	switch (MovDirection)
	{
	case Utility::IDLE:
		break;
	case Utility::LEFT:
		right = nullptr;
		break;
	case Utility::RIGHT:
		left = nullptr;
		break;
	case Utility::UP:
		down = nullptr;
		break;
	case Utility::DOWN:
		up = nullptr;
		break;
	}

	AMapBlock* DirBlocks[4] = { right, down, left, up };
	Utility::MovDirection DirOptions[4] = { Utility::RIGHT, Utility::DOWN, Utility::LEFT, Utility::UP };

	int rand_val = rand();
	int last_two_bits = (rand_val & 0b01);
	int value = (last_two_bits % 4);

	for (int i = value; i < 4; i++)
	{
		if (DirBlocks[i] == nullptr || !DirBlocks[i]->isWalkable)
			continue;

		return DirOptions[i];
	}
	for (int i = 0; i < value; i++)
	{
		if (DirBlocks[i] == nullptr || !DirBlocks[i]->isWalkable)
			continue;

			return DirOptions[i];
	}
	return Utility::IDLE;
}


void AGhosts::Movement_Cage(FVector MovementVector) {

	//Ghosts movement in the cage and for extit cage
	if (isInCage) {
		const FVector CurrentDestination = CagePath[CagePosition];
		const FVector GhostLocation = GetActorLocation();

		if (UtilityForRange.InRange(GhostLocation, CurrentDestination - MovementVector * 3, CurrentDestination + MovementVector * 3)) {
			const FVector FixedLocation(CurrentDestination.X, CurrentDestination.Y, GhostLocation.Z);
			SetActorLocation(FixedLocation);

			CagePosition = (CagePosition + 1) % 4;
			MovDirection = CagePathMovement[CagePosition];
		}
	}
	else {
		if (PreviousIsInCage != isInCage) {
			CagePosition = 0;
		}

		const FVector CurrentDestination = ExtitCagePath[CagePosition];
		const FVector GhostLocation = GetActorLocation();

		if (PreviousIsInCage != isInCage) {
			ExitCageMovement[0] = (GhostLocation.Y <= -100) ? Utility::DOWN : Utility::UP;
			MovDirection = ExitCageMovement[0];
		}

		if (UtilityForRange.InRange(GhostLocation, CurrentDestination - MovementVector * 3, CurrentDestination + MovementVector * 3)) {
			const FVector FixedLocation(CurrentDestination.X, CurrentDestination.Y, GhostLocation.Z);
			SetActorLocation(FixedLocation);

			CagePosition++;
			if (CagePosition == 5) {
				CagePosition = -1;
				MovDirection = Utility::LEFT;
				CurrentMapBlock = DefaultExitCageMapBlock;
				return;
			}
			MovDirection = ExitCageMovement[CagePosition];
		}
	}
	PreviousIsInCage = isInCage;
}


void AGhosts::NotifyActorBeginOverlap(AActor* OtherActor) {

	//PacMan and Ghosts collision events
	if (OtherActor->IsA(APacManPawn::StaticClass()))
	{
		if (MovementMod != Utility::FRIGHTENED && isDead == false) {
			PacMan->isDead = true;
			PacMan->SetActorHiddenInGame(true);
		}

		if (MovementMod == Utility::FRIGHTENED && isDead == false) {
			isDead = true;
			MeshComp->SetMaterial(0, GhostsDeadMaterial);
			*CounterGhostsEaten = *CounterGhostsEaten + 1;
			if (*CounterGhostsEaten == 1) {
				*GameMapPoints = *GameMapPoints + 200;
			}
			if (*CounterGhostsEaten == 2) {
				*GameMapPoints = *GameMapPoints + 400;
			}
			if (*CounterGhostsEaten == 3) {
				*GameMapPoints = *GameMapPoints + 800;
			}
			if (*CounterGhostsEaten == 4) {
				*GameMapPoints = *GameMapPoints + 1600;
			}
		}
	}
}


void AGhosts::GetTarget() {

	//Implementation different for every ghost
}