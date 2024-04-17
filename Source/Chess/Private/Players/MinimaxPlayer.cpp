// Fill out your copyright notice in the Description page of Project Settings.

#include "Players/MinimaxPlayer.h"

#include "Chess_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Pieces/Chess_Pawn.h"
#include "Pieces/Chess_Queen.h"

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
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->TurnManager->DisableReplay();
	
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Minimax) Turn"));

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&, GameMode]()
		{
			FNextMove NextMove = FindBestMove();

			if (NextMove.NextTile->GetTileStatus() == ETileStatus::OCCUPIED)
			{
				NextMove.PieceToMove->Kill(NextMove.NextTile->GetPieceOnTile());
			}
			
			NextMove.PieceToMove->MovePiece(NextMove.NextTile);
			
			// Change player
			GameMode->TurnNextPlayer();
		}, 1, false);
}

void AMinimaxPlayer::OnWin()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

void AMinimaxPlayer::OnLose()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Loses!"));
	GameInstance->SetTurnMessage(TEXT("AI Loses!"));
	GameInstance->IncrementScoreHumanPlayer();
}

void AMinimaxPlayer::OnDraw()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Loses!"));
	GameInstance->SetTurnMessage(TEXT("Draw game!"));
}

void AMinimaxPlayer::Destroy()
{
	delete this;
}

int32 AMinimaxPlayer::EvaluateGrid() const
{
	int32 GridValue = 0;
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	int32 BlackValue = 0;
	int32 WhiteValue = 0;

	GameMode->GetAllLegalMoves(0);
	GameMode->GetAllLegalMoves(1);

	// Mobility
	WhiteValue += 10 * GameMode->TurnManager->WhiteMoves.Num(); // Human
	BlackValue += 10 * GameMode->TurnManager->BlackMoves.Num(); // AI
	
	GameMode->TurnManager->bIsBlackKingInCheck = GameMode->IsKingInCheck(WHITE);
	GameMode->TurnManager->bIsWhiteKingInCheck = GameMode->IsKingInCheck(BLACK);

	if (GameMode->TurnManager->BlackMoves.IsEmpty())
	{
		if (GameMode->TurnManager->bIsBlackKingInCheck)
		{
			GameMode->bIsGameOver = true;
			return -30000;
		}
		return 0;
	}
	if (GameMode->TurnManager->WhiteMoves.IsEmpty())
	{
		if (GameMode->TurnManager->bIsWhiteKingInCheck)
		{
			GameMode->bIsGameOver = true;
			return 30000;
		}
		return 0;
	}

	// Position and material
	for (int32 i = 0; i < GameMode->TileArray.Num(); i++)
	{
		if (AChess_Piece* Piece = GameMode->TileArray[i]->GetPieceOnTile())
		{
			if (Piece->GetTeam() == WHITE)
			{
				switch (Piece->GetType())
				{
				case PAWN:
					WhiteValue += 100;
					WhiteValue += Pawns[i];
					if (Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber == 8)
					{
						WhiteValue += 900;
					}
					break;
				case KNIGHT:
					WhiteValue += 320;
					WhiteValue += Knight[i];
					break;
				case BISHOP:
					WhiteValue += 330;
					WhiteValue += Bishops[i];
					break;
				case ROOK:
					WhiteValue += 500;
					WhiteValue += Rooks[i];
					break;
				case QUEEN:
					WhiteValue += 900;
					break;
				case KING:
					WhiteValue += 10000;
					WhiteValue += Kings[i];
					break;
				}
			}
			else if (Piece->GetTeam() == BLACK)
			{
				switch (Piece->GetType())
				{
				case PAWN:
					BlackValue += 100;
					BlackValue += Pawns[Flip[i]];
					if (Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber == 1)
					{
						BlackValue += 900;
					}
					break;
				case KNIGHT:
					BlackValue += 320;
					BlackValue += Knight[Flip[i]];
					break;
				case BISHOP:
					BlackValue += 330;
					BlackValue += Bishops[Flip[i]];
					break;
				case ROOK:
					BlackValue += 500;
					BlackValue += Rooks[Flip[i]];
					break;
				case QUEEN:
					BlackValue += 900;
					break;
				case KING:
					BlackValue += 10000;
					BlackValue += Kings[Flip[i]];
					break;
				}
			}
		}
	}

	GridValue = BlackValue - WhiteValue;
	return GridValue;
}

int32 AMinimaxPlayer::AlphaBetaMiniMax(int32 Depth, int32 Alpha, int32 Beta, bool IsMax)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	if (Depth == 0 || GameMode->bIsGameOver) return EvaluateGrid();

	if (IsMax)
	{
		int32 Best = -50000;

		TArray<AChess_Piece*> AiTeam = GameMode->BlackTeam;
		for (AChess_Piece* Piece: AiTeam)
		{
			ATile* PreviousTile = Piece->GetPieceTile();
			for (ATile* Tile: Piece->GetLegitMoves())
			{
				AChess_Piece* Killed = Tile->GetPieceOnTile();

				if (Piece->IsA(AChess_Pawn::StaticClass()) && Tile->GetAlgebraicPosition().TileNumber == 1)
				{
					PiecePromoted = Piece;
					NewQueen = NewObject<AChess_Queen>();
					GameMode->BlackTeam.Add(NewQueen);
					GameMode->BlackTeam.Remove(Piece);
					NewQueen->VirtualMove(Tile, PreviousTile, Killed);
					bIsVirtualPromotion = true;
				}
				else
				{
					Piece->VirtualMove(Tile, PreviousTile, Killed);
				}
				
				Best = FMath::Max(Best, AlphaBetaMiniMax(Depth - 1, Alpha, Beta, false));

				if (bIsVirtualPromotion && NewQueen && Piece == PiecePromoted)
				{
					GameMode->BlackTeam.Add(PiecePromoted);
					GameMode->BlackTeam.Remove(NewQueen);
					bIsVirtualPromotion = false;
					PiecePromoted = nullptr;
					NewQueen->Destroy();
				}
				
				Piece->VirtualUnMove(Tile, PreviousTile, Killed);

				if (Best >= Beta) return Best;
				Alpha = FMath::Max(Alpha, Best);
			}
		}
		return Best;
	}
	else
	{
		int32 Best = 50000;

		// Human player
		TArray<AChess_Piece*> HumanTeam = GameMode->WhiteTeam;
		for (AChess_Piece* Piece: HumanTeam)
		{
			ATile* PreviousTile = Piece->GetPieceTile();

			for (ATile* Tile: Piece->GetLegitMoves())
			{
				AChess_Piece* Killed = Tile->GetPieceOnTile();

				if (Piece->IsA(AChess_Pawn::StaticClass()) && Tile->GetAlgebraicPosition().TileNumber == 8)
				{
					PiecePromoted = Piece;
					NewQueen = NewObject<AChess_Queen>();
					GameMode->WhiteTeam.Add(NewQueen);
					GameMode->WhiteTeam.Remove(Piece);
					NewQueen->VirtualMove(Tile, PreviousTile, Killed);
					bIsVirtualPromotion = true;
				}
				else
				{
					Piece->VirtualMove(Tile, PreviousTile, Killed);
				}
				
				Best = FMath::Min(Best, AlphaBetaMiniMax(Depth - 1, Alpha, Beta, true));

				if (bIsVirtualPromotion && NewQueen && Piece == PiecePromoted)
				{
					GameMode->WhiteTeam.Add(PiecePromoted);
					GameMode->WhiteTeam.Remove(NewQueen);
					bIsVirtualPromotion = false;
					PiecePromoted = nullptr;
					NewQueen->Destroy();
				}

				Piece->VirtualUnMove(Tile, PreviousTile, Killed);

				if (Best <= Alpha) return Best;
				Beta = FMath::Min(Beta, Best);
			}
		}
		return Best;
	}
}

FNextMove AMinimaxPlayer::FindBestMove()
{
	int32 BestVal = -50000;
	int32 Alpha = -50000;
	int32 Beta = 50000;
	int32 Depth = 2;

	FNextMove BestMove = {nullptr, nullptr};
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode->BlackTeam.Num() + GameMode->WhiteTeam.Num() >= 5 && GameMode->BlackTeam.Num() + GameMode->WhiteTeam.Num() <= 15)
	{
		Depth = 3;
	}
	else if (GameMode->BlackTeam.Num() + GameMode->WhiteTeam.Num() >= 0 && GameMode->BlackTeam.Num() + GameMode->WhiteTeam.Num() < 5)
	{
		Depth = 4;
	}

	TArray<AChess_Piece*> AiTeam = GameMode->BlackTeam;
	for (AChess_Piece* Piece: AiTeam)
	{
		ATile* PreviousTile = Piece->GetPieceTile();
		for (ATile* Tile: Piece->GetLegitMoves())
		{
			AChess_Piece* Killed = Tile->GetPieceOnTile();

			if (Piece->IsA(AChess_Pawn::StaticClass()) && Tile->GetAlgebraicPosition().TileNumber == 1)
			{
				PiecePromoted = Piece;
				NewQueen = NewObject<AChess_Queen>();
				GameMode->BlackTeam.Add(NewQueen);
				GameMode->BlackTeam.Remove(Piece);
				NewQueen->VirtualMove(Tile, PreviousTile, Killed);
				bIsVirtualPromotion = true;
			}
			else
			{
				Piece->VirtualMove(Tile, PreviousTile, Killed);
			}
			
			Piece->VirtualMove(Tile, PreviousTile, Killed);
			
			int32 MoveVal = AlphaBetaMiniMax(Depth, Alpha, Beta, false);

			if (bIsVirtualPromotion && NewQueen && Piece == PiecePromoted)
			{
				GameMode->BlackTeam.Add(PiecePromoted);
				GameMode->BlackTeam.Remove(NewQueen);
				bIsVirtualPromotion = false;
				PiecePromoted = nullptr;
				NewQueen->Destroy();
			}

			Piece->VirtualUnMove(Tile, PreviousTile, Killed);

			if (MoveVal > BestVal || (MoveVal == BestVal && FMath::Rand() % 2 == 1))
			{
				BestMove.PieceToMove = Piece;
				BestMove.NextTile = Tile;
				BestVal = MoveVal;
			}
		}
	}
	
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AI (Minimax) bestVal = %d "), BestVal));
	
	return BestMove;
}

