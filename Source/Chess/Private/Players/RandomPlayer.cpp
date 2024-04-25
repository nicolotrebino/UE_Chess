// Fill out your copyright notice in the Description page of Project Settings.

#include "Players/RandomPlayer.h"

#include "Chess_GameInstance.h"
#include "Chess_GameMode.h"
#include "Pieces/Chess_Piece.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARandomPlayer::ARandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); // Get the Chess_GameInstance reference

	Team = BLACK;
}

// Called when the game starts or when spawned
void ARandomPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void ARandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/*
 *	@brief	Implements the Random player turn
 *
 *	@return	Void
 */
void ARandomPlayer::OnTurn()
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->TurnManager->DisableReplay();
	
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Random) Turn"));

	// Set a random timer
	FTimerHandle TimerHandle;
	
	// Wait for the timer before making your move
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&, GameMode]()
		{
			if (GameMode->CurrentPlayer)
			{
				// If black still has ChessPieces available
				if (GameMode->BlackTeam.Num() > 0)
				{
					TArray<ATile*> RandPlayerMoves;
					TSet<int32> CheckedIndices;
					int32 RandPieceIdx = -1;

					while (RandPlayerMoves.IsEmpty())
					{
						RandPieceIdx = FMath::Rand() % GameMode->BlackTeam.Num();
						
						while (CheckedIndices.Contains(RandPieceIdx))
						{
							RandPieceIdx = FMath::Rand() % GameMode->BlackTeam.Num();
						}
						
						CheckedIndices.Add(RandPieceIdx);

						RandPlayerMoves = GameMode->BlackTeam[RandPieceIdx]->MyLegalMoves;
					}
					
					// After finding the possible moves, choose one randomly
					const int32 RandMoveIdx = FMath::Rand() % RandPlayerMoves.Num();
					if (RandPlayerMoves[RandMoveIdx]->GetTileStatus() == ETileStatus::OCCUPIED && RandPlayerMoves[RandMoveIdx]->GetTileTeam() != Team)
					{
						GameMode->BlackTeam[RandPieceIdx]->Kill(RandPlayerMoves[RandMoveIdx]->GetPieceOnTile());
					}
					GameMode->BlackTeam[RandPieceIdx]->MovePiece(RandPlayerMoves[RandMoveIdx]);

					// Change player
					GameMode->TurnNextPlayer();
				}
			}
		}, 3, false);
}

/*
 *	@brief	Implements the Random player victory
 *
 *	@return	Void
 */
void ARandomPlayer::OnWin()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

/*
 *	@brief	Implements the Random player defeat
 *
 *	@return	Void
 */
void ARandomPlayer::OnLose()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Loses!"));
	GameInstance->SetTurnMessage(TEXT("AI Loses!"));
	GameInstance->IncrementScoreHumanPlayer();
}

/*
 *	@brief	Implements the draw game
 *
 *	@return	Void
 */
void ARandomPlayer::OnDraw()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Loses!"));
	GameInstance->SetTurnMessage(TEXT("Draw game!"));
}

// Called every frame
void ARandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
