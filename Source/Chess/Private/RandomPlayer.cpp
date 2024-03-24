// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomPlayer.h"

#include "Chess_GameInstance.h"
#include "Chess_GameMode.h"
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

void ARandomPlayer::OnTurn()
{
	const AChess_GameMode* ChessGameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	ChessGameMode->TurnManager->DisableReplay();
	
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Random) Turn"));

	// Set a random timer
	FTimerHandle TimerHandle;
	
	// Wait for the timer before making your move
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
		
			if (GameMode->CurrentPlayer)
			{
				// If black still has ChessPieces available
				if (GameMode->BlackTeam.Num() > 0)
				{
					TArray<ATile*> RandPlayerMoves;
					TSet<int32> CheckedIndices;
					int32 RandPieceIdx = -1;
					// As long as the array of possible moves is empty,
					// randomly choose a black piece and check if it has possible moves
					while (RandPlayerMoves.IsEmpty())
					{
						RandPieceIdx = FMath::Rand() % GameMode->BlackTeam.Num();

						// Continua a generare nuovi indici finché non ottieni uno diverso
						while (CheckedIndices.Contains(RandPieceIdx))
						{
							RandPieceIdx = FMath::Rand() % GameMode->BlackTeam.Num();
						}

						// Aggiungi il nuovo indice al set
						CheckedIndices.Add(RandPieceIdx);
									
						// RandPieceIdx = FMath::Rand() % GameMode->BlackTeam.Num();
						RandPlayerMoves = GameMode->BlackTeam[RandPieceIdx]->GetLegitMoves();
					}
					// After finding the possible moves, choose one randomly
					const int32 RandMoveIdx = FMath::Rand() % RandPlayerMoves.Num();
					if (RandPlayerMoves[RandMoveIdx]->GetTileStatus() == ETileStatus::OCCUPIED && RandPlayerMoves[RandMoveIdx]->GetTileTeam() != Team)
					{
						GameMode->BlackTeam[RandPieceIdx]->Kill(RandPlayerMoves[RandMoveIdx]->GetPieceOnTile());
					}
					
					GameMode->BlackTeam[RandPieceIdx]->MovePiece(RandPlayerMoves[RandMoveIdx]);

					/*
					GameMode->bIsWhiteKingInCheck = GameMode->IsKingInCheck(WHITE);
					if (GameMode->bIsWhiteKingInCheck)
					{
						GameMode->IsCheckMate(WHITE, GameMode->WhiteTeam);
					}
					*/

					// Change player
					GameMode->TurnNextPlayer();
				}
			}
		}, 3, false);
}

void ARandomPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

void ARandomPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Loses!"));
	GameInstance->SetTurnMessage(TEXT("AI Loses!"));
}

// Called every frame
void ARandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
