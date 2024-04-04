// Fill out your copyright notice in the Description page of Project Settings.

#include "MinimaxPlayer.h"

#include "Chess_Bishop.h"
#include "Chess_GameMode.h"
#include "Chess_Queen.h"
#include "Chess_King.h"
#include "Chess_Knight.h"
#include "Chess_Pawn.h"
#include "Chess_Rook.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMinimaxPlayer::AMinimaxPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	MovedPiece = nullptr;
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

			FNextMove NextMove = FindBestMove();

			if (NextMove.NextTile->GetTileStatus() == ETileStatus::OCCUPIED)
			{
				NextMove.PieceToMove->Kill(NextMove.NextTile->GetPieceOnTile());
			}
		
			NextMove.PieceToMove->MovePiece(NextMove.NextTile);
			MovedPiece = NextMove.PieceToMove;
		
			// Change player
			GameMode->TurnNextPlayer();
		
		}, 1, false);
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

	int32 BlackValue = 0;
	int32 WhiteValue = 0;

	/*
	for (AChess_Piece* Piece: GameMode->BlackTeam)
	{
		if (Piece->IsA(AChess_Pawn::StaticClass()))
		{
			// BlackValue += BPawns[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			BlackValue += 100;
		}
		if (Piece->IsA(AChess_Rook::StaticClass()))
		{
			// BlackValue += BRooks[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			BlackValue += 500;
		}
		if (Piece->IsA(AChess_Knight::StaticClass()))
		{
			// BlackValue += BKnights[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			BlackValue += 320;
		}
		if (Piece->IsA(AChess_Bishop::StaticClass()))
		{
			// BlackValue += BBishops[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			BlackValue += 330;
		}
		if (Piece->IsA(AChess_Queen::StaticClass()))
		{
			// BlackValue += BQueens[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			BlackValue += 900;
		}
		if (MovedPiece && Piece->GetType() != MovedPiece->GetType())
		{
			BlackValue += 450;
		}
	}
	
	for (AChess_Piece* Piece: GameMode->WhiteTeam)
	{
		if (Piece->IsA(AChess_Pawn::StaticClass()))
		{
			// WhiteValue += WPawns[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			WhiteValue += 100;
		}
		if (Piece->IsA(AChess_Rook::StaticClass()))
		{
			// WhiteValue += WRooks[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			WhiteValue += 500;
		}
		if (Piece->IsA(AChess_Knight::StaticClass()))
		{
			// WhiteValue += WKnights[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			WhiteValue += 320;
		}
		if (Piece->IsA(AChess_Bishop::StaticClass()))
		{
			// WhiteValue += WBishops[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			WhiteValue += 330;
		}
		if (Piece->IsA(AChess_Queen::StaticClass()))
		{
			// WhiteValue += WQueens[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			WhiteValue += 900;
		}
	}
	*/

	// Mobility
	BlackValue += 10 * GameMode->TurnManager->BlackMoves.Num();
	WhiteValue += 10 * GameMode->TurnManager->WhiteMoves.Num();

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

	GameMode->TurnManager->bIsBlackKingInCheck = GameMode->IsKingInCheck(WHITE);
	GameMode->TurnManager->bIsWhiteKingInCheck = GameMode->IsKingInCheck(BLACK);

	if (GameMode->TurnManager->BlackMoves.IsEmpty())
	{
		if (GameMode->TurnManager->bIsBlackKingInCheck)
		{
			GameMode->bIsGameOver = true;
			WhiteValue += 10000; // Mate
		}
	}
	if (GameMode->TurnManager->WhiteMoves.IsEmpty())
	{
		if (GameMode->TurnManager->bIsWhiteKingInCheck)
		{
			GameMode->bIsGameOver = true;
			BlackValue += 10000;
		}
	}

	GridValue = BlackValue - WhiteValue;
	return GridValue;
}

int32 AMinimaxPlayer::AlphaBetaMiniMax(int32 Depth, int32 Alpha, int32 Beta, bool IsMax)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->GetAllLegalMoves(); // Creo le legal moves per tutti i pezzi dopo la virtual move
	
	// GameMode->TurnManager->bIsBlackKingInCheck = GameMode->IsKingInCheck(BLACK);
	// GameMode->TurnManager->bIsWhiteKingInCheck = GameMode->IsKingInCheck(WHITE);

	/*
	if (GameMode->TurnManager->LegalMoves.IsEmpty())
	{
		if (GameMode->TurnManager->bIsWhiteKingInCheck)
		{
			return 20000;
		}
		if (GameMode->TurnManager->bIsBlackKingInCheck)
		{
			return -20000;
		}
		return 0;
	}
	*/

	if (Depth == 0 || GameMode->bIsGameOver) return EvaluateGrid();

	if (IsMax)
	{
		int32 Best = -50000;

		TArray<AChess_Piece*> AiTeam = GameMode->BlackTeam;
		for (AChess_Piece* Piece: AiTeam)
		{
			ATile* PreviousTile = Piece->GetPieceTile();
			// TArray<ATile*> LegalMoves = Piece->MyLegalMoves;
			for (ATile* Tile: Piece->GetLegitMoves())
			{
				AChess_Piece* Killed = Tile->GetPieceOnTile();
				Piece->VirtualMove(Tile, PreviousTile, Killed);
				
				Best = FMath::Max(Best, AlphaBetaMiniMax(Depth - 1, Alpha, Beta, false));

				Piece->VirtualUnMove(Tile, PreviousTile, Killed);
				// UE_LOG(LogTemp, Error, TEXT("%i, %i, %i"), Tile, PreviousTile, Killed);

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
			// TArray<ATile*> LegalMoves = Piece->MyLegalMoves;
			for (ATile* Tile: Piece->GetLegitMoves())
			{
				AChess_Piece* Killed = Tile->GetPieceOnTile();
				Piece->VirtualMove(Tile, PreviousTile, Killed);
				
				Best = FMath::Min(Best, AlphaBetaMiniMax(Depth - 1, Alpha, Beta, true));

				Piece->VirtualUnMove(Tile, PreviousTile, Killed);
				// UE_LOG(LogTemp, Error, TEXT("%i, %i, %i"), Tile, PreviousTile, Killed);

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

	FNextMove BestMove = {nullptr, nullptr};
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	TArray<AChess_Piece*> AiTeam = GameMode->BlackTeam;
	for (AChess_Piece* Piece: AiTeam)
	{
		ATile* PreviousTile = Piece->GetPieceTile();
		// TArray<ATile*> LegalMoves = Piece->MyLegalMoves;
		for (ATile* Tile: Piece->GetLegitMoves())
		{
			AChess_Piece* Killed = Tile->GetPieceOnTile();
			Piece->VirtualMove(Tile, PreviousTile, Killed);
			
			int32 MoveVal = AlphaBetaMiniMax(2, Alpha, Beta, false);

			if (MoveVal > BestVal)
			{
				BestMove.PieceToMove = Piece;
				BestMove.NextTile = Tile;
				BestVal = MoveVal;
			}
			
			Piece->VirtualUnMove(Tile, PreviousTile, Killed);
		}
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AI (Minimax) bestVal = %d "), BestVal));
	
	return BestMove;
}

