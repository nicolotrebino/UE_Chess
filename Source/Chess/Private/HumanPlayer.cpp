// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanPlayer.h"

#include "Chess_GameInstance.h"
#include "Chess_GameMode.h"

// Sets default values
AHumanPlayer::AHumanPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0; // Set this pawn to be controlled by the lowest-numbered player
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); // Create a camera component
	SetRootComponent(Camera); // Set the camera as RootComponent
	
	GameInstance = UChess_GameInstance::GetChessGameInstance(); // Get the game instance reference with the Singleton pattern

	// Default init values
	PlayerNumber = -1;
	Team = WHITE;
}

// Called when the game starts or when spawned
void AHumanPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AHumanPlayer::OnTurn()
{
	IsMyTurn = true;
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Your Turn"));
	GameInstance->SetTurnMessage(TEXT("Human Turn"));
}

void AHumanPlayer::OnWin()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Win!"));
	GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	GameInstance->IncrementScoreHumanPlayer();
}

void AHumanPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose!"));
	GameInstance->SetTurnMessage(TEXT("Human Loses!"));
}

void AHumanPlayer::OnClick()
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()); // Get the Chess_GameMode instance with Singleton pattern
	
	// Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (Hit.bBlockingHit && IsMyTurn)
	{
		// If the human player clicks on a ChessPiece
		if (AChess_Piece* CurrPiece = Cast<AChess_Piece>(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Tile at position: %c, %i"), CurrPiece->GetPieceTile()->GetAlgebraicPosition().TileLetter, CurrPiece->GetPieceTile()->GetAlgebraicPosition().TileNumber);
			UE_LOG(LogTemp, Warning, TEXT("Tile Status: %i"), CurrPiece->GetPieceTile()->GetTileStatus());
			UE_LOG(LogTemp, Warning, TEXT("Tile Team: %i"), CurrPiece->GetPieceTile()->GetTileTeam());
			
			// If the Chess Piece has the color of the Human Player's team (WHITE)
			if (CurrPiece->GetTeam() == Team)
			{
				GameMode->ResetTargetedAndKillableTiles(); // Reset array of both Targeted and Killable Tiles
				GameMode->ResetSelectedPiece(); // Reset Selected Piece's Tile
				GameMode->SelectedPiece = SelectedPiece = CurrPiece; // Set the Chess Piece selected by the Human Player
				CurrPiece->GetPieceTile()->SetSelectedTile(); // Select the Tile and set it to SELECTED

				// For each tile in the array of possible moves for the selected piece,
				// make it a target and add it to the array of target tiles at that time
				for (ATile* Tile : CurrPiece->GetLegalMoves())
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
			else if (CurrPiece->GetPieceTile()->GetTileStatus() == ETileStatus::KILLABLE)
			{
				ATile* NextTile = CurrPiece->GetPieceTile();

				SelectedPiece->Kill(CurrPiece->GetTeam(), CurrPiece);

				// Reset all the Tiles in the "global" arrays
				GameMode->ResetTargetedAndKillableTiles();

				// Move the selected piece
				SelectedPiece->MovePiece(NextTile);
				
				if (GameMode->bIsPromotion)
				{
					return;
				}
					
				// Change the turn to Random Player
				IsMyTurn = false;
				GameMode->TurnNextPlayer();
			}
		}

		// If the human player clicks on a Tile
		if (ATile* NextTile = Cast<ATile>(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Tile at position: %c, %i"), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
			UE_LOG(LogTemp, Warning, TEXT("Tile Status: %i"), NextTile->GetTileStatus());
			UE_LOG(LogTemp, Warning, TEXT("Tile Team: %i"), NextTile->GetTileTeam());
			// If the Tile is a target
			if (NextTile->GetTileStatus() == ETileStatus::TARGET)
			{
				// If SelectedPiece is not NULL, move that ChessPiece
				// and reset the Array of target Tiles
				if (SelectedPiece)
				{
					// Reset all the Tiles in the "global" arrays
					GameMode->ResetTargetedAndKillableTiles();
					
					// Move the selected piece
					SelectedPiece->MovePiece(NextTile);

					if (GameMode->bIsPromotion)
					{
						return;
					}
					
					// Change the turn to Random Player
					IsMyTurn = false;
					GameMode->TurnNextPlayer();
				}
			}

			// If the Tile is empty
			else if (NextTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				UE_LOG(LogTemp, Warning, TEXT("Tile at position: %c, %i"), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
				UE_LOG(LogTemp, Warning, TEXT("Tile Status: %i"), NextTile->GetTileStatus());
				UE_LOG(LogTemp, Warning, TEXT("Tile Team: %i"), NextTile->GetTileTeam());
				GameMode->ResetTargetedAndKillableTiles(); // Reset array of Targeted Tiles
				GameMode->ResetSelectedPiece(); // Reset the Selected Piece
			}
		}
	}
}
