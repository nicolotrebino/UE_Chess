// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimaxPlayer.h"

#include "Chess_GameMode.h"
#include "Chess_Queen.h"
#include "Chess_King.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMinimaxPlayer::AMinimaxPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// Called when the game starts or when spawned
void AMinimaxPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMinimaxPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMinimaxPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMinimaxPlayer::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Minimax) Turn"));

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

			const FNextMove NextMove = FindBestMove();

			if (NextMove.NextTile->GetTileStatus() == ETileStatus::OCCUPIED)
			{
				NextMove.PieceToMove->Kill(NextMove.NextTile->GetPieceOnTile());
			}
		
			NextMove.PieceToMove->MovePiece(NextMove.NextTile);
		
			// Change player
			GameMode->TurnNextPlayer();
		
		}, 3, false);
}

void AMinimaxPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

void AMinimaxPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Loses!"));
	// GameInstance->SetTurnMessage(TEXT("AI Loses!"));
}

int32 AMinimaxPlayer::EvaluateGrid() const
{
	int32 GridValue = 0;
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	// GameMode->UpdateScores();
	// if (GameMode->ScoreBlackTeam > GameMode->ScoreWhiteTeam) GridValue += 30;
	// if (GameMode->ScoreBlackTeam < GameMode->ScoreWhiteTeam) GridValue += -30;
	
	for (AChess_Piece* KilledPiece: GameMode->KilledWhiteTeam)
	{
		GridValue += KilledPiece->GetPieceValue() * 2;
	}
	for (AChess_Piece* KilledPiece: GameMode->KilledBlackTeam)
	{
		GridValue -= KilledPiece->GetPieceValue() * 2;
	}

	if (GameMode->GetTileAtPosition('c', 5)->GetTileTeam() == BLACK)
	{
		GridValue += 5;
	}
	if (GameMode->GetTileAtPosition('d', 5)->GetTileTeam() == BLACK)
	{
		GridValue += 5;
	}
	if (GameMode->GetTileAtPosition('e', 5)->GetTileTeam() == BLACK)
	{
		GridValue += 5;
	}
	if (GameMode->GetTileAtPosition('f', 5)->GetTileTeam() == BLACK)
	{
		GridValue += 5;
	}

	if (GameMode->IsKingInCheck(WHITE)) GridValue += 200;
	if (GameMode->IsKingInCheck(BLACK)) GridValue += -200;
	
	for (AChess_Piece* EnemyPiece: GameMode->WhiteTeam)
	{
		if (EnemyPiece)
		{
			// Get the Possible Moves of that White Piece
			TArray<ATile*> EnemyMoves = EnemyPiece->GetPossibleMoves();
			// If the array of the possible moves of the enemy piece
			// contains the Tile under the King
			if (EnemyMoves.Contains(GameMode->Kings[BLACK]->GetPieceTile()))
			{
				GridValue -= 200;
			}
		}
	}
	for (AChess_Piece* EnemyPiece: GameMode->BlackTeam)
	{
		if (EnemyPiece)
		{
			// Get the Possible Moves of that White Piece
			TArray<ATile*> EnemyMoves = EnemyPiece->GetPossibleMoves();
			// If the array of the possible moves of the enemy piece
			// contains the Tile under the King
			if (EnemyMoves.Contains(GameMode->Kings[WHITE]->GetPieceTile()))
			{
				GridValue += 200;
			}
		}
	}

	return GridValue;
}

int32 AMinimaxPlayer::AlphaBetaMiniMax(int32 Depth, int32 Alpha, int32 Beta, bool IsMax)
{
	if (Depth == 0) return EvaluateGrid();

	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode->bIsDraw) return 0;
	if (GameMode->bIsWhiteKingInCheckMate) return 500;
	if (GameMode->bIsBlackKingInCheckMate) return -500;

	if (IsMax)
	{
		int32 Best = -1000;

		for (AChess_Piece* Piece: GameMode->BlackTeam)
		{
			ATile* PreviousTile = Piece->GetPieceTile();
			for (ATile* Tile: Piece->GetLegitMoves())
			{
				AChess_Piece* Killed = nullptr;
				if (Tile->GetTileStatus() == ETileStatus::OCCUPIED)
				{
					Killed = Tile->GetPieceOnTile();
					// Piece->VirtualKill(Killed);
					// Team.Remove(Killed);
					GameMode->KilledWhiteTeam.Add(Killed);
					GameMode->WhiteTeam.Remove(Killed);
				}
				Piece->VirtualMove(Tile);

				Best = FMath::Max(Best, AlphaBetaMiniMax(Depth - 1, Alpha, Beta, false));

				if (Killed)
				{
					// GameMode->WhiteTeam.Add(Killed);
					GameMode->KilledWhiteTeam.Remove(Killed);
					// Team.Add(Killed);
					GameMode->WhiteTeam.Add(Killed);
					Tile->SetPieceOnTile(Killed);
					Tile->SetTileTeam(Killed->GetTeam());
					Tile->SetTileStatus(ETileStatus::OCCUPIED);
					Killed->SetPieceTile(Tile);
				}
				else
				{
					Tile->SetPieceOnTile(nullptr);
					Tile->SetTileTeam(NONE);
					Tile->SetTileStatus(ETileStatus::EMPTY);
				}
				PreviousTile->SetPieceOnTile(Piece);
				PreviousTile->SetTileTeam(Piece->GetTeam());
				PreviousTile->SetTileStatus(ETileStatus::OCCUPIED);
				Piece->SetPieceTile(PreviousTile);

				if (Best >= Beta) return Best;
				Alpha = FMath::Max(Alpha, Best);
			}
		}
		return Best;
	}
	else
	{
		int32 Best = 1000;

		for (AChess_Piece* Piece: GameMode->BlackTeam)
		{
			ATile* PreviousTile = Piece->GetPieceTile();
			for (ATile* Tile: Piece->GetLegitMoves())
			{
				AChess_Piece* Killed = nullptr;
				if (Tile->GetTileStatus() == ETileStatus::OCCUPIED)
				{
					Killed = Tile->GetPieceOnTile();
					// Piece->VirtualKill(Killed);
					// Team.Remove(Killed);
					GameMode->KilledWhiteTeam.Add(Killed);
					GameMode->WhiteTeam.Remove(Killed);
				}
				Piece->VirtualMove(Tile);

				Best = FMath::Min(Best, AlphaBetaMiniMax(Depth - 1, Alpha, Beta, true));

				if (Killed)
				{
					// GameMode->WhiteTeam.Add(Killed);
					GameMode->KilledWhiteTeam.Remove(Killed);
					GameMode->WhiteTeam.Add(Killed);
					// Team.Add(Killed);
					Tile->SetPieceOnTile(Killed);
					Tile->SetTileTeam(Killed->GetTeam());
					Tile->SetTileStatus(ETileStatus::OCCUPIED);
					Killed->SetPieceTile(Tile);
				}
				else
				{
					Tile->SetPieceOnTile(nullptr);
					Tile->SetTileTeam(NONE);
					Tile->SetTileStatus(ETileStatus::EMPTY);
				}
				PreviousTile->SetPieceOnTile(Piece);
				PreviousTile->SetTileTeam(Piece->GetTeam());
				PreviousTile->SetTileStatus(ETileStatus::OCCUPIED);
				Piece->SetPieceTile(PreviousTile);

				if (Best <= Alpha) return Best;
				Beta = FMath::Min(Beta, Best);
			}
		}
		return Best;
	}
}

FNextMove AMinimaxPlayer::FindBestMove()
{
	int32 BestVal = -1000;

	FNextMove BestMove = {nullptr, nullptr};
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	for (AChess_Piece* Piece: GameMode->BlackTeam)
	{
		ATile* PreviousTile = Piece->GetPieceTile();
		for (ATile* Tile: Piece->GetLegitMoves())
		{
			AChess_Piece* Killed = nullptr;
			if (Tile->GetTileStatus() == ETileStatus::OCCUPIED)
			{
				Killed = Tile->GetPieceOnTile();
				// Piece->VirtualKill(Killed);
				// Team.Remove(Killed);
				GameMode->WhiteTeam.Remove(Killed);
				GameMode->KilledWhiteTeam.Add(Killed);
			}
			Piece->VirtualMove(Tile);

			int32 MoveVal = AlphaBetaMiniMax(3, -1000, 1000, false);

			if (Killed)
			{
				// GameMode->WhiteTeam.Add(Killed);
				GameMode->KilledWhiteTeam.Remove(Killed);
				GameMode->WhiteTeam.Add(Killed);
				Tile->SetPieceOnTile(Killed);
				Tile->SetTileTeam(Killed->GetTeam());
				Tile->SetTileStatus(ETileStatus::OCCUPIED);
				Killed->SetPieceTile(Tile);
			}
			else
			{
				Tile->SetPieceOnTile(nullptr);
				Tile->SetTileTeam(NONE);
				Tile->SetTileStatus(ETileStatus::EMPTY);
			}
			PreviousTile->SetPieceOnTile(Piece);
			PreviousTile->SetTileTeam(Piece->GetTeam());
			PreviousTile->SetTileStatus(ETileStatus::OCCUPIED);
			Piece->SetPieceTile(PreviousTile);

			if (MoveVal > BestVal)
			{
				BestMove.PieceToMove = Piece;
				BestMove.NextTile = Tile;
				BestVal = MoveVal;
			}
		}
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AI (Minimax) bestVal = %d "), BestVal));
	
	return BestMove;
}

