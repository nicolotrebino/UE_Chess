// Fill out your copyright notice in the Description page of Project Settings.

#include "Chess_GameMode.h"

#include "Chess_King.h"
#include "Chess_PlayerController.h"
#include "HumanPlayer.h"
#include "RandomPlayer.h"
#include "EngineUtils.h"

/*
 * Singleton pattern implementation
 */ 
AChess_GameMode* AChess_GameMode::GetChessGameMode()
{
	if (!ChessGameModeInstance)
	{
		// If the instance doesn't exist, make it
		ChessGameModeInstance = NewObject<AChess_GameMode>();
	}

	return ChessGameModeInstance;
}

// Static variable initialization
AChess_GameMode* AChess_GameMode::ChessGameModeInstance = nullptr;

/*
 * Chess_GameMode methods implementation
 */
AChess_GameMode::AChess_GameMode()
{
	PlayerControllerClass = AChess_PlayerController::StaticClass(); // Associate the Cpc as the Player Controller for to this GameMode
	DefaultPawnClass = AHumanPlayer::StaticClass(); // Associate the HumaPlayer as the default pawn class for this GameMode

	// Set default values
	bIsGameOver = false; // Tracks if the game is over
	MoveCounter = 0; // Tracks the number of moves in order to signal a drawn game
	Checker = nullptr; // Piece that is in check
	bIsBlackKingInCheck = false;
	bIsWhiteKingInCheck = false;
	bIsWhiteKingInCheckMate = false;
	bIsBlackKingInCheckMate = false;
	bIsDraw = false;
	bIsKill = false;
	bIsPromotion = false;

	ScoreWhiteTeam = 0;
	ScoreBlackTeam = 0;

	CurrentPlayer = 0;

	CBoard = nullptr;

	SelectedPiece = nullptr;

	Kings.SetNum(2); // Create space for two elements in the array
	Kings[WHITE] = nullptr;
	Kings[BLACK] = nullptr;
}

void AChess_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	AHumanPlayer* HumanPlayer = Cast<AHumanPlayer>(*TActorIterator<AHumanPlayer>(GetWorld()));
	
	if (ChessboardClass != nullptr)
	{
		CBoard = GetWorld()->SpawnActor<AChessboard>(ChessboardClass);
	}

	const float CameraPosX = (CBoard->GetTileSize() * CBoard->GetFieldSize() / 2);
	const FVector CameraPos(CameraPosX, 0.0f, 1000.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	Players.Add(HumanPlayer); // Human player = 0
	auto* AI = GetWorld()->SpawnActor<ARandomPlayer>(FVector(), FRotator()); // Random Player
	// auto* AI = GetWorld()->SpawnActor<AChessMinimaxPlayer>(FVector(), FRotator()); // MiniMax Player
	
	Players.Add(AI); // AI player = 1

	this->ChoosePlayerAndStartGame();
}

void AChess_GameMode::ChoosePlayerAndStartGame()
{
	CurrentPlayer = 0; // Set the first player to the Human Player
	
	// Set the Human Player to WHITE and the AI Player to BLACK
	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
		Players[i]->Team = i == CurrentPlayer ? WHITE : BLACK;
	}
	MoveCounter += 1;
	Players[CurrentPlayer]->OnTurn();
}

int32 AChess_GameMode::GetNextPlayer(int32 Player) const
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}

void AChess_GameMode::TurnNextPlayer()
{
	MoveCounter += 1;
	CurrentPlayer = GetNextPlayer(CurrentPlayer);
	Players[CurrentPlayer]->OnTurn();
	
	// Reset all the game variables for this new turn
	bIsKill = false;
	bIsWhiteKingInCheck = false;
	bIsBlackKingInCheck = false;
	Checker = nullptr;
}

/*
 * Bring back to the default color all the targeted Tiles
 * and empty the TArray with all the pointers to the TargetedTiles
 */
void AChess_GameMode::ResetTargetedAndKillableTiles()
{
	// Reset the targeted tiles and remove them from the "global" array
	for (ATile* Tile : TargetedTiles)
	{
		Tile->UnsetTargetTile(); // Make it EMPTY and with the default material
	}
	TargetedTiles.Empty();

	// Reset the killable tiles and remove them from the "global" array
	for (ATile* Tile : KillableTiles)
	{
		Tile->UnsetKillableTile(); // Make it OCCUPIED and with the default material
	}
	KillableTiles.Empty();
}

/*
 * Bring back to the default color the Tile
 * of the SelectedPiece
 */
void AChess_GameMode::ResetSelectedPiece()
{
	if (SelectedPiece)
	{
		SelectedPiece->GetPieceTile()->UnsetSelectedTile();
	}
	SelectedPiece = nullptr;
}

bool AChess_GameMode::IsKingInCheck(const int32 KingTeam)
{
	// Get the Tile under the King of the particular team
	const ATile* KingTile = Kings[KingTeam]->GetPieceTile();

	// If the Current Player is the AI (1)
	if (KingTeam)
	{
		// For each Piece of the White Team (the enemy team)
		for (AChess_Piece* EnemyPiece: WhiteTeam)
		{
			if (EnemyPiece)
			{
				// Get the Possible Moves of that White Piece
				TArray<ATile*> EnemyMoves = EnemyPiece->GetPossibleMoves();
				// If the array of the possible moves of the enemy piece
				// contains the Tile under the King
				if (EnemyMoves.Contains(KingTile))
				{
					Checker = EnemyPiece; // Set that Piece as the Checker
					// bIsCheck = true; // Set to true the CheckSituation
					return true; // The AI King is in Check
				}
			}
		}
	}
	// If the Current Player is the Human (0)
	else
	{
		// For each Piece of the Black Team (the enemy team)
		for (AChess_Piece* EnemyPiece: BlackTeam)
		{
			if (EnemyPiece)
			{
				// Get the Possible Moves of that Black Piece
				TArray<ATile*> EnemyMoves = EnemyPiece->GetPossibleMoves();
				// If the array of possible moves of the enemy piece
				// contains the Tile under the King
				if (EnemyMoves.Contains(KingTile))
				{
					Checker = EnemyPiece; // Set that Piece as the Checker
					// bIsCheck = true; // Set to true the CheckSituation
					return true; // The Human King is in Check
				}
			}
		}
	}
	// Checker = nullptr;
	// bIsCheck = false;
	return false; // The King of the CurrentPlayer is not in Check
}

bool AChess_GameMode::IsCheckMate(const uint8 KingTeam, const TArray<AChess_Piece*>& Team)
{
	// For each Chess Piece of the team
	for (AChess_Piece* Piece : Team)
	{
		ATile* CurrentTile = Piece->GetPieceTile();
		
		// Get the legal moves of the piece
		TArray<ATile*> PossibleMoves = Piece->GetLegalMoves();

		for (ATile* NextTile : PossibleMoves)
		{
			if (Piece->IsA(AChess_King::StaticClass()))
			{
				ATile* CurrentKingTile = Piece->GetPieceTile();
				Piece->SetPieceTile(NextTile);
				if (!IsKingInCheck(KingTeam) || NextTile->GetPieceOnTile() == Checker)
				{
					Piece->SetPieceTile(CurrentKingTile);
					return false;
				}
				Piece->SetPieceTile(CurrentKingTile);
			}
			const ETileStatus NextStatus = NextTile->GetTileStatus();
			const ETeam NextTeam = NextTile->GetTileTeam();
			NextTile->SetTileStatus(ETileStatus::OCCUPIED);
			NextTile->SetTileTeam(Piece->GetTeam());
			CurrentTile->SetTileStatus(ETileStatus::EMPTY);
			CurrentTile->SetTileTeam(NONE);
			if (!IsKingInCheck(KingTeam) || NextTile->GetPieceOnTile() == Checker)
			{
				NextTile->SetTileStatus(NextStatus);
				NextTile->SetTileTeam(NextTeam);
				CurrentTile->SetTileStatus(ETileStatus::OCCUPIED);
				CurrentTile->SetTileTeam(Piece->GetTeam());
				return false;
			}
			NextTile->SetTileStatus(NextStatus);
			NextTile->SetTileTeam(NextTeam);
			CurrentTile->SetTileStatus(ETileStatus::OCCUPIED);
			CurrentTile->SetTileTeam(Piece->GetTeam());
		}
	}
	
	KingTeam ? bIsBlackKingInCheckMate = true : bIsWhiteKingInCheckMate = true;
	bIsGameOver = true;
	return true;
}

/*
 * Binary search int the TArray of all the ChessBoard Tiles to find
 * the Tile with a particular Letter and Number pair
 * Notice: TileArray is ordered by numbers: A1-B1-...-A2-B2-...-A8-B8-...-H8
 */
ATile* AChess_GameMode::GetTileAtPosition(const TCHAR Letter, const uint8 Number)
{
	int32 StartIndex = 0;
	int32 EndIndex = TileArray.Num() - 1;

	while (StartIndex <= EndIndex)
	{
		const int32 MiddleIndex = (StartIndex + EndIndex) / 2;

		// Obtain the ChessPosition from the Current Tile
		const FAlgebraicPosition& CurrentPosition = TileArray[MiddleIndex]->GetAlgebraicPosition();

		if (CurrentPosition.TileLetter == Letter && CurrentPosition.TileNumber == Number)
		{
			// Tile found
			return TileArray[MiddleIndex];
		}
		if (CurrentPosition.TileNumber < Number || (CurrentPosition.TileNumber == Number && CurrentPosition.TileLetter < Letter))
		{
			// Tile is in the upper half of the array
			StartIndex = MiddleIndex + 1;
		}
		else
		{
			// Tile is in the lower half of the array
			EndIndex = MiddleIndex - 1;
		}
	}

	// Return nullptr if the Tile isn't in the array
	return nullptr;
}

void AChess_GameMode::UpdateScores()
{
	ScoreWhiteTeam = 0;
	ScoreBlackTeam = 0;
	for (const AChess_Piece* WhitePiece: WhiteTeam)
	{
		ScoreWhiteTeam += WhitePiece->GetPieceValue();
	}
	for (const AChess_Piece* BlackPiece: BlackTeam)
	{
		ScoreBlackTeam += BlackPiece->GetPieceValue();
	}
}

FString AChess_GameMode::GetScoreWhiteTeam() const
{
	const int32 NewScore = ScoreWhiteTeam - ScoreBlackTeam;
	if (NewScore > 0)
	{
		return "+" + FString::FromInt(NewScore);
	}
	return "0";
}

FString AChess_GameMode::GetScoreBlackTeam() const
{
	const int32 NewScore = ScoreBlackTeam - ScoreWhiteTeam;
	if (NewScore > 0)
	{
		return "+" + FString::FromInt(NewScore);
	}
	return "0";
}

void AChess_GameMode::ResetField()
{
	// send broadcast event to registered objects 
	OnResetEvent.Broadcast();

	TileArray.Empty();
	WhiteTeam.Empty();
	BlackTeam.Empty();
	ResetSelectedPiece();
	ResetSelectedPiece();
	Kings[WHITE] = nullptr;
	Kings[BLACK] = nullptr;

	bIsGameOver = false;
	MoveCounter = 0;
	
	AChess_PlayerController* Cpc = AChess_PlayerController::GetChessPlayerController();
	Cpc->UserInterfaceWidget->DestroyMoveHistory();

	if (ChessboardClass != nullptr)
	{
		CBoard = GetWorld()->SpawnActor<AChessboard>(ChessboardClass);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}
	
	ChoosePlayerAndStartGame();
}
