#include "PacManPawn.h"


APacManPawn::APacManPawn()
{

	PrimaryActorTick.bCanEverTick = true;
}


void APacManPawn::BeginPlay()
{

	Super::BeginPlay();
}


void APacManPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Read keyboard inputs
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Mod"), this, &APacManPawn::ModAlgo);
}
void APacManPawn::ModAlgo(float MovementCode) {

	if (MovementCode == 1 && ModSetted == false) {   //Scelta 1 - Random
		ModSelected = 1;
		Random_Actor->SetTextRenderColor(FColor(53, 128, 255, 1));
		Conservative_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
		Balanced_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
	}
	if (MovementCode == 2 && ModSetted == false) {   //Scelta 2 - Conservative
		ModSelected = 2;
		Random_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
		Conservative_Actor->SetTextRenderColor(FColor(53, 128, 255, 1));
		Balanced_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
	}
	if (MovementCode == 3 && ModSetted == false) {   //Scelta 3 - Balanced
		ModSelected = 3;
		Random_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
		Conservative_Actor->SetTextRenderColor(FColor(255, 165, 0, 1));
		Balanced_Actor->SetTextRenderColor(FColor(53, 128, 255, 1));
	}
	if (MovementCode == 4 && ModSelected != 0) {   //Conferma
		ModSetted = true;
	}
}


void APacManPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Checking should me moving
	if (ShouldMove == false) {
		return;
	}
	if (ModSetted == false) {
		return;
	}


	//Skipping the movement if eaten a food
	if (PacManSkipMovement) {
		if (PacManSkipMovementTimer <= 0) {
			PacManSkipMovement = false;
			PacManSkipMovementTimer = 0;
		}
		else {
			PacManSkipMovementTimer -= DeltaTime;
			return;
		}
	}


	//Checking if PacMan is IDLE, PacManFirstMoveFlag for Ghosts
	if (MovDirection != Utility::IDLE && PacManFirstMoveFlag == false) {
		PacManFirstMoveFlag = true;
	}

	if (CurrentMapBlock == NextMapBlock || NextMapBlock == nullptr)
	{
		NextMapBlock = NextBlock();

		for (int x = 0; x < 36; x++) {
			for (int y = 0; y < 28; y++) {
				GameMapBlocks[x][y]->PacMan = false;
			}
		}
		GameMapBlocks[CurrentMapBlock->x][CurrentMapBlock->y]->PacMan = true;
	}


	//PacMan movement management
	if (iFrightnedMod) {
		Result_distance_per_second = Base_distance_per_second * LevelPacManFrightnedSpeed[GameMapLevel - 1];
	}
	else {
		Result_distance_per_second = Base_distance_per_second * LevelPacManSpeed[GameMapLevel - 1];
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

	FVector underBlockCenterLoc = NextMapBlock->GetCenterPoint();
	FVector pacmanLoc = GetActorLocation();


	if (UtilityForRange.InRange(pacmanLoc, underBlockCenterLoc - (MovementVector * (Result_distance_per_second * DeltaTime)) * 3, underBlockCenterLoc + (MovementVector * (Result_distance_per_second * DeltaTime)) * 3)) {
		
		FVector BlockLocation = NextMapBlock->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();
		FVector FixedLocation = FVector(BlockLocation.X, BlockLocation.Y, 50.0f);
		SetActorLocation(FixedLocation);

		if (CurrentMapBlock->isTeleport && CurrentMapBlock->Direction == MovDirection) {
			FVector TempLocation(CurrentMapBlock->TeleportDestination->GetCenterPoint());
			FVector Location(TempLocation.X, TempLocation.Y, 50.0f);
			SetActorLocation(Location);
			NextMapBlock = CurrentMapBlock->TeleportDestination;
			CurrentMapBlock = NextMapBlock;
			return;
		}

		GameMapBlocks[CurrentMapBlock->x][CurrentMapBlock->y]->Food = false;
		CurrentMapBlock = NextMapBlock;

		//upading blocks
		for (int x = 0; x < 36; x++) {
			for (int y = 0; y < 28; y++) {
				GameMapBlocks[x][y]->PacMan = false;
			}
		}
		GameMapBlocks[CurrentMapBlock->x][CurrentMapBlock->y]->PacMan = true;


		//ALGORITHMS FOR MOVEMENT DIRECTION CALCULATION

		if (ModSelected == 1) {  //Random
			std::random_device rand;
			std::mt19937 GenRandom(rand());
			std::uniform_int_distribution<int> distribution(0, 3);

			do
			{
				contatore = distribution(GenRandom);
			} while (WakableDirection(MovDirections[contatore]) == false);

			MovDirection = MovDirections[contatore];
		}


		if (ModSelected == 2) {  //Conservative
			UpDate_BlocksRisk();
			DirectionGraph GraphMap(GameMapBlocks);
			MovDirection = GraphMap.ConservativeDirection(PreviusMove);
			PreviusMove = MovDirection;
		}
		
		
		if (ModSelected == 3) {  //Balanced
			UpDate_BlocksRisk();
			DirectionGraph GraphMap(GameMapBlocks);
			MovDirection = GraphMap.BalancedDirection(PreviusMove);
			PreviusMove = MovDirection;
		}

		NextMapBlock = NextBlock();
	}
}


AMapBlock* APacManPawn::NextBlock() {

	//Next block for MoveDirection

	switch (MovDirection) {
	case Utility::UP:
		if (CurrentMapBlock->LateralBlocks[0] != nullptr) {
			return CurrentMapBlock->LateralBlocks[0];
		}
		return CurrentMapBlock;
	case Utility::LEFT:
		if (CurrentMapBlock->LateralBlocks[1] != nullptr) {
			return CurrentMapBlock->LateralBlocks[1];
		}
		return CurrentMapBlock;
	case Utility::DOWN:
		if (CurrentMapBlock->LateralBlocks[2] != nullptr) {
			return CurrentMapBlock->LateralBlocks[2];
		}
		return CurrentMapBlock;
	case Utility::RIGHT:
		if (CurrentMapBlock->LateralBlocks[3] != nullptr) {
			return CurrentMapBlock->LateralBlocks[3];
		}
		return CurrentMapBlock;
	default:
		return CurrentMapBlock;
	}
}


bool APacManPawn::WakableDirection(Utility::MovDirection BufferedMoveBis) {

	//Check if the block in one Direction is walkable

	switch (BufferedMoveBis) {
	case Utility::UP:
		if (CurrentMapBlock->LateralBlocks[0] != nullptr) {
			return CurrentMapBlock->LateralBlocks[0]->isWalkable;
		}
		return false;
	case Utility::LEFT:
		if (CurrentMapBlock->LateralBlocks[1] != nullptr) {
			return CurrentMapBlock->LateralBlocks[1]->isWalkable;
		}
		return false;
	case Utility::DOWN:
		if (CurrentMapBlock->LateralBlocks[2] != nullptr) {
			return CurrentMapBlock->LateralBlocks[2]->isWalkable;
		}
		return false;
	case Utility::RIGHT:
		if (CurrentMapBlock->LateralBlocks[3] != nullptr) {
			return CurrentMapBlock->LateralBlocks[3]->isWalkable;
		}
		return false;
	default:
		return false;
	}

}


void APacManPawn::NotifyActorBeginOverlap(AActor* OtherActor) {

	//PacMan hits something, checking what and operations
	if (OtherActor->IsA(AMapFood::StaticClass())) {
		AMapFood* DynamicFood = dynamic_cast<AMapFood*>(OtherActor);

		if (DynamicFood) {
			PacManSkipMovement = true;
			PacManSkipMovementTimer = 0.017;
		}

		OtherActor->Destroy();

		switch (DynamicFood->Type) {
		case Utility::BASIC:
			GameMapPoints += 10;
			RawPoints++;
			(*TotalPoints)--;
			if (*TotalPoints == 0) {
				*GameMapCompleted = true;
			}
			PacManSkipMovement = true;
			PacManSkipMovementTimer = 0.0167;
			break;
		case Utility::SUPER:
			iFrightnedMod = true;
			GameMapPoints += 100;
			(*TotalPoints)--;
			if (*TotalPoints == 0) {
				*GameMapCompleted = true;
			}
			PacManSkipMovement = true;
			PacManSkipMovementTimer = 0.05;
			break;
		case Utility::CHERRIES:
			GameMapPoints += 100;
			break;
		case Utility::STRAWBERRY:
			GameMapPoints += 300;
			break;
		case Utility::PEACH:
			GameMapPoints += 500;
			break;
		case Utility::APPLE:
			GameMapPoints += 700;
			break;
		case Utility::GRAPES:
			GameMapPoints += 1000;
			break;
		case Utility::GALAXIAN:
			GameMapPoints += 2000;
			break;
		case Utility::BELL:
			GameMapPoints += 3000;
			break;
		case Utility::KEY:
			GameMapPoints += 5000;
			break;
		}
	}
}


void APacManPawn::UpDate_BlocksRisk() {

	//Operations for setting valore rischio blocks

	for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 28; y++) {
			GameMapBlocks[x][y]->valore_rischio = 1;
		}
	}

	for (int x = 0; x < 36; x++) {
		for (int y = 0; y < 28; y++) {
			
			if (GameMapBlocks[x][y]->Blinky == true && *Ghosts[0] == false) {

				for (int a = -7; a < 8; a++) {
					for (int b = -7; b < 8; b++) {
						if (a == 0 && b == 0) {
							GameMapBlocks[x][y]->valore_rischio = GameMapBlocks[x][y]->valore_rischio + 7000;
							continue;
						}
						if ((x + a >= 0 && x + a < 36) && (y + b >= 0 && y + b < 28)) {
							int value1 = abs(a);
							int value2 = abs(b);
							int ris = 0;
							if (value1 > value2) {
								ris = value1;
							}
							else {
								ris = value2;
							}
							if (ris == 1) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 2047;
							}
							if (ris == 2) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 1023;
							}
							if (ris == 3) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 511;
							}
							if (ris == 4) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 255;
							}
							if (ris == 5) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 127;
							}
							if (ris == 6) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 63;
							}
							if (ris == 7) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 31;
							}
						}
					}
				}
			}

			if (GameMapBlocks[x][y]->Pinky == true && *Ghosts[1] == false) {

				for (int a = -7; a < 8; a++) {
					for (int b = -7; b < 8; b++) {
						if (a == 0 && b == 0) {
							GameMapBlocks[x][y]->valore_rischio = GameMapBlocks[x][y]->valore_rischio + 7000;
							continue;
						}
						if ((x + a >= 0 && x + a < 36) && (y + b >= 0 && y + b < 28)) {
							int value1 = abs(a);
							int value2 = abs(b);
							int ris = 0;
							if (value1 > value2) {
								ris = value1;
							}
							else {
								ris = value2;
							}
							if (ris == 1) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 2047;
							}
							if (ris == 2) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 1023;
							}
							if (ris == 3) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 511;
							}
							if (ris == 4) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 255;
							}
							if (ris == 5) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 127;
							}
							if (ris == 6) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 63;
							}
							if (ris == 7) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 31;
							}
						}
					}
				}
			}

			if (GameMapBlocks[x][y]->Inky == true && *Ghosts[2] == false) {

				for (int a = -7; a < 8; a++) {
					for (int b = -7; b < 8; b++) {
						if (a == 0 && b == 0) {
							GameMapBlocks[x][y]->valore_rischio = GameMapBlocks[x][y]->valore_rischio + 7000;
							continue;
						}
						if ((x + a >= 0 && x + a < 36) && (y + b >= 0 && y + b < 28)) {
							int value1 = abs(a);
							int value2 = abs(b);
							int ris = 0;
							if (value1 > value2) {
								ris = value1;
							}
							else {
								ris = value2;
							}
							if (ris == 1) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 2047;
							}
							if (ris == 2) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 1023;
							}
							if (ris == 3) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 511;
							}
							if (ris == 4) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 255;
							}
							if (ris == 5) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 127;
							}
							if (ris == 6) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 63;
							}
							if (ris == 7) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 31;
							}
						}
					}
				}
			}

			if (GameMapBlocks[x][y]->Clyde == true && *Ghosts[3] == false) {

				for (int a = -7; a < 8; a++) {
					for (int b = -7; b < 8; b++) {
						if (a == 0 && b == 0) {
							GameMapBlocks[x][y]->valore_rischio = GameMapBlocks[x][y]->valore_rischio + 7000;
							continue;
						}
						if ((x + a >= 0 && x + a < 36) && (y + b >= 0 && y + b < 28)) {
							int value1 = abs(a);
							int value2 = abs(b);
							int ris = 0;
							if (value1 > value2) {
								ris = value1;
							}
							else {
								ris = value2;
							}
							if (ris == 1) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 2047;
							}
							if (ris == 2) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 1023;
							}
							if (ris == 3) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 511;
							}
							if (ris == 4) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 255;
							}
							if (ris == 5) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 127;
							}
							if (ris == 6) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 63;
							}
							if (ris == 7) {
								GameMapBlocks[x + a][y + b]->valore_rischio = GameMapBlocks[x + a][y + b]->valore_rischio + 31;
							}
						}
					}
				}
			}
		}
	}
}