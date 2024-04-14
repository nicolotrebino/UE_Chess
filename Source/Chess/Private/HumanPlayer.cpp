// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanPlayer.h"

#include "Chess_GameInstance.h"
#include "Chess_GameMode.h"
#include "Manager_Turn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHumanPlayer::AHumanPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0; // Set this pawn to be controlled by the lowest-numbered player
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); // Create a camera component
	SetRootComponent(Camera); // Set the camera as RootComponent
	
	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); // Get the game instance reference

	// Default init values
	PlayerNumber = -1;
	Team = WHITE;
}

// Called when the game starts or when spawned
void AHumanPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void AHumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AHumanPlayer::OnTurn()
{
	bIsMyTurn = true;
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Your Turn"));
	GameInstance->SetTurnMessage(TEXT("Human Turn"));
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()); // Get the reference to the Chess_GameMode
	GameMode->TurnManager->EnableReplay();
}

void AHumanPlayer::OnWin()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Win!"));
	GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	GameInstance->IncrementScoreHumanPlayer();
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()); // Get the reference to the Chess_GameMode
	if (GameMode->WinSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GameMode->WinSound, FVector(0, 0, 0));
	}
}

void AHumanPlayer::OnLose()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose!"));
	GameInstance->SetTurnMessage(TEXT("Human Loses!"));
	GameInstance->IncrementScoreAiPlayer();
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()); // Get the reference to the Chess_GameMode
	if (GameMode->LoseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GameMode->LoseSound, FVector(0, 0, 0));
	}
}

void AHumanPlayer::OnDraw()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose!"));
	GameInstance->SetTurnMessage(TEXT("Draw game!"));
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()); // Get the reference to the Chess_GameMode
	if (GameMode->DrawSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GameMode->DrawSound, FVector(0, 0, 0));
	}
}

void AHumanPlayer::OnClick()
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()); // Get the reference to the Chess_GameMode
	
	// Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (Hit.bBlockingHit && bIsMyTurn && !GameMode->bIsGameOver)
	{
		// If the human player clicks on a ChessPiece
		if (AChess_Piece* CurrPiece = Cast<AChess_Piece>(Hit.GetActor()))
		{
			// If the Chess Piece has the color of the Human Player's team (WHITE)
			if (CurrPiece->GetTeam() == Team)
			{
				GameMode->TurnManager->ResetTargetedAndKillableTiles(); // Reset array of both Targeted and Killable Tiles
				GameMode->TurnManager->ResetSelectedPiece(); // Reset Selected Piece's Tile
				GameMode->TurnManager->SelectedPiece = SelectedPiece = CurrPiece; // Set the Chess Piece selected by the Human Player
				CurrPiece->GetPieceTile()->SetSelectedTile(); // Select the Tile and set it to SELECTED

				// For each tile in the array of possible moves for the selected piece,
				// make it a target and add it to the array of target tiles at that time
				/*
				for (ATile* Tile : CurrPiece->GetLegitMoves())
				{
					if (Tile->GetTileStatus() == ETileStatus::OCCUPIED)
					{
						Tile->SetKillableTile();
						GameMode->KillableTiles.Add(Tile);
					}
					else
					{
						Tile->SetTargetTile();
						GameMode->TargetedTiles.Add(Tile);
					}
				}
				*/

				/*
				for (ATile* Tile: CurrPiece->GetPossibleMoves())
				{
					if (GameMode->TurnManager->LegalMoves.Contains(Tile))
					{
						if (Tile->GetTileStatus() == ETileStatus::OCCUPIED)
						{
							Tile->SetKillableTile();
							GameMode->KillableTiles.Add(Tile);
						}
						else
						{
							Tile->SetTargetTile();
							GameMode->TargetedTiles.Add(Tile);
						}
					}
				}
				*/

				for (ATile* Tile: CurrPiece->MyLegalMoves)
				{
					if (Tile->GetTileStatus() == ETileStatus::OCCUPIED)
					{
						Tile->SetKillableTile();
						GameMode->TurnManager->KillableTiles.Add(Tile);
					}
					else
					{
						Tile->SetTargetTile();
						GameMode->TurnManager->TargetedTiles.Add(Tile);
					}
				}
			}
			else if (CurrPiece->GetPieceTile()->GetTileStatus() == ETileStatus::KILLABLE)
			{
				ATile* NextTile = CurrPiece->GetPieceTile();

				SelectedPiece->Kill(CurrPiece);

				// Reset all the Tiles in the "global" arrays
				GameMode->TurnManager->ResetTargetedAndKillableTiles();

				// Move the selected piece
				SelectedPiece->MovePiece(NextTile);
				
				if (GameMode->TurnManager->bIsPromotion)
				{
					return;
				}
					
				// Change the turn to Random Player
				bIsMyTurn = false;
				GameMode->TurnNextPlayer();
			}
		}

		// If the human player clicks on a Tile
		if (ATile* NextTile = Cast<ATile>(Hit.GetActor()))
		{
			// If the Tile is a target
			if (NextTile->GetTileStatus() == ETileStatus::TARGET)
			{
				// If SelectedPiece is not NULL, move that ChessPiece
				// and reset the Array of target Tiles
				if (SelectedPiece)
				{
					// Reset all the Tiles in the "global" arrays
					GameMode->TurnManager->ResetTargetedAndKillableTiles();
					
					// Move the selected piece
					SelectedPiece->MovePiece(NextTile);
					
					if (GameMode->TurnManager->bIsPromotion)
					{
						return;
					}
					
					// Change the turn to Random Player
					bIsMyTurn = false;
					GameMode->TurnNextPlayer();
				}
			}

			// If the Tile is empty
			else if (NextTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				GameMode->TurnManager->ResetTargetedAndKillableTiles(); // Reset array of Targeted Tiles
				GameMode->TurnManager->ResetSelectedPiece(); // Reset the Selected Piece
			}
		}
	}
}

// Called every frame
void AHumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

