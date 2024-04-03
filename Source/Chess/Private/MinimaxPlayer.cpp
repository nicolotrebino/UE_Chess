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

	/*
	for (AChess_Piece* KilledPiece: GameMode->KilledWhiteTeam)
	{
		GridValue += KilledPiece->GetPieceValue() * 2;
	}
	for (AChess_Piece* KilledPiece: GameMode->KilledBlackTeam)
	{
		GridValue -= KilledPiece->GetPieceValue() * 2;
	}
	
	GameMode->UpdateScores();
	GridValue += GameMode->ScoreBlackTeam - GameMode->ScoreWhiteTeam;
	*/

	int32 BlackValue = 0;
	for (AChess_Piece* Piece: GameMode->BlackTeam)
	{
		if (Piece->IsA(AChess_Pawn::StaticClass()))
		{
			BlackValue += BPawns[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			if (GameMode->TurnManager->WhiteMoves.Contains(Piece->GetPieceTile())) BlackValue -= 100;
			else BlackValue += 100;
		}
		if (Piece->IsA(AChess_Rook::StaticClass()))
		{
			BlackValue += BRooks[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			if (GameMode->TurnManager->WhiteMoves.Contains(Piece->GetPieceTile())) BlackValue -= 500;
			else BlackValue += 500;
		}
		if (Piece->IsA(AChess_Knight::StaticClass()))
		{
			BlackValue += BKnights[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			if (GameMode->TurnManager->WhiteMoves.Contains(Piece->GetPieceTile())) BlackValue -= 320;
			else BlackValue += 320;
		}
		if (Piece->IsA(AChess_Bishop::StaticClass()))
		{
			BlackValue += BBishops[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			if (GameMode->TurnManager->WhiteMoves.Contains(Piece->GetPieceTile())) BlackValue -= 330;
			else BlackValue += 330;
		}
		if (Piece->IsA(AChess_Queen::StaticClass()))
		{
			BlackValue += BQueens[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			if (GameMode->TurnManager->WhiteMoves.Contains(Piece->GetPieceTile())) BlackValue -= 900;
			else BlackValue += 900;
		}
	}
	
	int32 WhiteValue = 0;
	for (AChess_Piece* Piece: GameMode->WhiteTeam)
	{
		if (Piece->IsA(AChess_Pawn::StaticClass()))
		{
			WhiteValue += WPawns[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			if (GameMode->TurnManager->WhiteMoves.Contains(Piece->GetPieceTile())) WhiteValue -= 100;
			else WhiteValue += 100;
		}
		if (Piece->IsA(AChess_Rook::StaticClass()))
		{
			WhiteValue += WRooks[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			if (GameMode->TurnManager->WhiteMoves.Contains(Piece->GetPieceTile())) WhiteValue -= 500;
			else WhiteValue += 500;
		}
		if (Piece->IsA(AChess_Knight::StaticClass()))
		{
			WhiteValue += WKnights[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			if (GameMode->TurnManager->WhiteMoves.Contains(Piece->GetPieceTile())) WhiteValue -= 320;
			else WhiteValue += 320;
		}
		if (Piece->IsA(AChess_Bishop::StaticClass()))
		{
			WhiteValue += WBishops[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			if (GameMode->TurnManager->WhiteMoves.Contains(Piece->GetPieceTile())) WhiteValue -= 330;
			else WhiteValue += 330;
		}
		if (Piece->IsA(AChess_Queen::StaticClass()))
		{
			WhiteValue += WQueens[Piece->GetPieceTile()->GetAlgebraicPosition().TileNumber - 1][Piece->GetPieceTile()->GetAlgebraicPosition().TileLetter - 'a'];
			if (GameMode->TurnManager->WhiteMoves.Contains(Piece->GetPieceTile())) WhiteValue -= 900;
			else WhiteValue += 900;
		}
	}

	GameMode->TurnManager->bIsBlackKingInCheck = GameMode->IsKingInCheck(WHITE);
	GameMode->TurnManager->bIsWhiteKingInCheck = GameMode->IsKingInCheck(BLACK);
	if (GameMode->TurnManager->bIsBlackKingInCheck) BlackValue += 450;
	if (GameMode->TurnManager->bIsWhiteKingInCheck) WhiteValue += 450;

	GridValue = BlackValue - WhiteValue;
	return GridValue;
}

int32 AMinimaxPlayer::AlphaBetaMiniMax(int32 Depth, int32 Alpha, int32 Beta, bool IsMax)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->GetAllLegalMoves(); // Creo le legal moves per tutti i pezzi dopo la virtual move
	
	// GameMode->TurnManager->bIsBlackKingInCheck = GameMode->IsKingInCheck(BLACK);
	// GameMode->TurnManager->bIsWhiteKingInCheck = GameMode->IsKingInCheck(WHITE);

	if (GameMode->TurnManager->BlackMoves.IsEmpty())
	{
		if (GameMode->TurnManager->bIsBlackKingInCheck)
		{
			return -20000;
		}
		return 0;
	}
	if (GameMode->TurnManager->WhiteMoves.IsEmpty())
	{
		if (GameMode->TurnManager->bIsWhiteKingInCheck)
		{
			return -20000;
		}
		return 0;
	}

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

	if (Depth == 0) return EvaluateGrid();

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

