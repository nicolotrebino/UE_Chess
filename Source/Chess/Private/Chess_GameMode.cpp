// Fill out your copyright notice in the Description page of Project Settings.

#include "Chess_GameMode.h"

#include "Chess_PlayerController.h"
#include "EngineUtils.h"
#include "Managers/Manager_Promotion.h"
#include "Kismet/GameplayStatics.h"
#include "Players/HumanPlayer.h"
#include "Players/MinimaxPlayer.h"
#include "Players/RandomPlayer.h"
#include "Pieces/Chess_Piece.h"
#include "Pieces/Chess_King.h"

// Sets default values
AChess_GameMode::AChess_GameMode()
{
	PlayerControllerClass = AChess_PlayerController::StaticClass(); // Associate the Cpc as the Player Controller for to this GameMode
	DefaultPawnClass = AHumanPlayer::StaticClass(); // Associate the HumaPlayer as the default pawn class for this GameMode

	bIsGameOver = false;
	bIsWhiteKingInCheckMate = false;
	bIsBlackKingInCheckMate = false;
	bIsDraw = false;
	bInReplay = false;
	MoveCounter = 0;

	CurrentPlayer = 0;

	CBoard = nullptr;
	TurnManager = nullptr;
	PromotionManager = nullptr;

	Kings.SetNum(2); // Create space for two elements in the array
	Kings[WHITE] = nullptr;
	Kings[BLACK] = nullptr;
	
	MoveSound = nullptr;
	KillSound = nullptr;
	CheckSound = nullptr;
	CastlingSound = nullptr;
	WinSound = nullptr;
	LoseSound = nullptr;
	DrawSound = nullptr;

	ScoreWhiteTeam = 0;
	ScoreBlackTeam = 0;
}

/*
 *	@brief	Called when the game starts or when spawned.
 *			It creates Chessboard, TurnManager, PawnManager, AiPlayer; set camera settings and manage Players.
 *			Then start the match.
 *
 *	@return	Void
 */
void AChess_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (ChessboardClass)
	{
		CBoard = GetWorld()->SpawnActor<AChessboard>(ChessboardClass);
	}
	if (TurnManagerClass)
	{
		TurnManager = GetWorld()->SpawnActor<AManager_Turn>(TurnManagerClass);
	}
	if (PromotionManagerClass)
	{
		PromotionManager = GetWorld()->SpawnActor<AManager_Promotion>(PromotionManagerClass);
	}
	
	AHumanPlayer* HumanPlayer = Cast<AHumanPlayer>(*TActorIterator<AHumanPlayer>(GetWorld()));

	const float CameraPosX = (CBoard->GetTileSize() * CBoard->GetFieldSize() / 2);
	const FVector CameraPos(CameraPosX, 0.0f, 1000.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	Players.Add(HumanPlayer); // Human player = 0
	
	const UChess_GameInstance* GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		if (GameInstance->EnemyPlayer) // Random player
		{
			auto* AI = GetWorld()->SpawnActor<ARandomPlayer>(FVector(), FRotator()); // Random Player
			Players.Add(AI);
		}
		else // MiniMax
		{
			auto* AI = GetWorld()->SpawnActor<AMinimaxPlayer>(FVector(), FRotator()); // MiniMax Player
			Players.Add(AI);
		}
	}

	this->ChoosePlayerAndStartGame();
}

/*
 *	@brief	Get all the legal moves of that particular player in that turn, create the array of the legal
 *			moves for each piece of the current player
 *	@param	Player for whom it is necessary to calculate legal moves
 *
 *	@return	void
 */
void AChess_GameMode::GetAllLegalMoves(const int32 Player)
{
	// For the Human Player
	if (!Player)
	{
		TurnManager->WhiteMoves.Empty();
		for (AChess_Piece* Piece: WhiteTeam)
		{
			Piece->MyLegalMoves.Empty();
			for (ATile* LegalMove: Piece->GetLegitMoves())
			{
				Piece->MyLegalMoves.Add(LegalMove);
				TurnManager->WhiteMoves.AddUnique(LegalMove);
			}
		}
	}

	// For the AI Player
	if (Player)
	{
		TurnManager->BlackMoves.Empty();
		for (AChess_Piece* Piece: BlackTeam)
		{
			Piece->MyLegalMoves.Empty();
			for (ATile* LegalMove: Piece->GetLegitMoves())
			{
				Piece->MyLegalMoves.Add(LegalMove);
				TurnManager->BlackMoves.AddUnique(LegalMove);
			}
		}
	}
}

/*
 *	@brief	Set the team for the two players and choose the Human as the first player of the match
 *
 *	@return Void
 */
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
	UpdateScores();
	
	GetAllLegalMoves(CurrentPlayer);
	
	Players[CurrentPlayer]->OnTurn();
}

/*
 *	@brief	Get the next player to change it during the match
 *
 *	@param	Player: the current player
 *
 *	@return	The next integer corresponding to index of the next player in the array of players
 */
int32 AChess_GameMode::GetNextPlayer(int32 Player) const
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}

/*
 *	@brief	Once a player's turn is over, this method saves the last move in the MoveHistory,
 *			checks the game status (endgame, lose, win, draw), changes the player, finds the current player's legal moves
 *			and starts the new player's turn player
 *
 *	@return	Void
 */
void AChess_GameMode::TurnNextPlayer()
{
	TurnManager->bIsBlackKingInCheck = IsKingInCheck(BLACK);
	TurnManager->bIsWhiteKingInCheck = IsKingInCheck(WHITE);
	
	CurrentPlayer = GetNextPlayer(CurrentPlayer);

	GetAllLegalMoves(!CurrentPlayer);
	GetAllLegalMoves(CurrentPlayer);

	// Check the Draw for only kings on the field
	if (BlackTeam.Num() == 1 && WhiteTeam.Num() == 1)
	{
		bIsGameOver = true;
		bIsDraw = true;
		TurnManager->DisplayMove();
		TurnManager->DisplayEndGame();
		TurnManager->EnableReplay();
		Players[WHITE]->OnDraw();
		return;
	}

	// For Human Player
	if (TurnManager->WhiteMoves.IsEmpty()) // If white has no more moves available
	{
		if (TurnManager->bIsWhiteKingInCheck) // If the white king is in check
		{
			bIsGameOver = true;
			bIsWhiteKingInCheckMate = true;
			TurnManager->DisplayMove();
			TurnManager->DisplayEndGame();
			TurnManager->EnableReplay();
			Players[WHITE]->OnLose(); // White lost
		}
		else // If the white king is not in check
		{
			bIsGameOver = true;
			bIsDraw = true;
			TurnManager->DisplayMove();
			TurnManager->DisplayEndGame();
			TurnManager->EnableReplay();
			Players[WHITE]->OnDraw(); // Draw game
		}
		return;
	}
	// For AI Player
	if (TurnManager->BlackMoves.IsEmpty()) // If black has no more moves available
	{
		if (TurnManager->bIsBlackKingInCheck) // If the black king is in check
		{
			bIsGameOver = true;
			bIsBlackKingInCheckMate = true;
			TurnManager->DisplayMove();
			TurnManager->DisplayEndGame();
			TurnManager->EnableReplay();
			Players[WHITE]->OnWin(); // Black won
		}
		else // If the black king is not in check
		{
			bIsGameOver = true;
			bIsDraw = true;
			TurnManager->DisplayMove();
			TurnManager->DisplayEndGame();
			TurnManager->EnableReplay();
			Players[WHITE]->OnDraw(); // Draw game
		}
		return;
	}

	// Check if the same arrangement of pieces on the board does not occur 3 times during the game,
	// otherwise it is a draw
	TPair<FString, int32> CurrentState = TurnManager->SaveGameState(); // Value = 0
	if (GameStates.Contains(CurrentState))
	{
		CurrentState.Value++; // Value = 1
		if (GameStates.Contains(CurrentState))
		{
			CurrentState.Value++; // Value = 2
			if ((CurrentState.Value + 1) == 3)
			{
				bIsGameOver = true;
				bIsDraw = true;
				TurnManager->DisplayMove();
				TurnManager->DisplayEndGame();
				TurnManager->EnableReplay();
				Players[WHITE]->OnDraw();
				return;
			}
		}
	}
	GameStates.Add(CurrentState);

	// The corresponding sound is played based on the moves made and the state of the game
	if (bEnableSound && (TurnManager->bIsBlackKingInCheck || TurnManager->bIsWhiteKingInCheck) && CheckSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CheckSound, FVector(0, 0, 0));
	}
	else if (bEnableSound && (TurnManager->bIsCastleLong || TurnManager->bIsCastleShort) && CastlingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CastlingSound, FVector(0, 0, 0));
	}
	else if (bEnableSound && TurnManager->bIsKill && KillSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, KillSound, FVector(0, 0, 0));
	}
	else if (bEnableSound && MoveSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, MoveSound, FVector(0, 0, 0));
	}

	TurnManager->DisplayMove(); // The move just made is displayed in the MoveHistory
	
	MoveCounter += 1; // Increment the number of moves

	UpdateScores();

	// Reset turn variables
	TurnManager->ResetVariables();
	bInReplay = false;
	
	Players[CurrentPlayer]->OnTurn(); // Start the turn of the new player
}

/*
 *	@brief	Check whether the desired team's king is or is not in check
 *
 *	@param	KingTeam: king's team for which you want to know if it is in check
 *
 *	@return	True if the current King is in check, false otherwise
 */
bool AChess_GameMode::IsKingInCheck(const int32 KingTeam)
{
	// Get the Tile under the King of the desired team
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
					return true; // The Human King is in Check
				}
			}
		}
	}
	return false; // The King of the CurrentPlayer is not in Check
}

/*
 *	@brief	Binary search in the TArray of all the ChessBoard Tiles to find
 *			the Tile with a particular Letter and Number pair
 *			Notice: TileArray is ordered by numbers: A1-B1-...-A2-B2-...-A8-B8-...-H8
 *
 *	@param	Letter: letter of the desired Tile with AlgebraicPosition
 *	@param	Number: number of the desired Tile with AlgebraicPosition
 *
 *	@return	The desired Tile
 */
ATile* AChess_GameMode::GetTileAtPosition(const TCHAR Letter, const uint8 Number)
{
	int32 StartIndex = 0;
	int32 EndIndex = TileArray.Num() - 1;

	while (StartIndex <= EndIndex)
	{
		const int32 MiddleIndex = (StartIndex + EndIndex) / 2;

		// Obtain the AlgebraicPosition from the Current Tile
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

/*
 *	@brief	Update the score of each team
 *
 *	@return	Void
 */
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

/*
 *	@brief	Calculate the score difference between yourself and the enemy team
 *
 *	@return	String to display in the UI
 */
FString AChess_GameMode::ComputeScoreWhiteTeam()
{
	const int32 NewScore = ScoreWhiteTeam - ScoreBlackTeam;
	if (NewScore > 0)
	{
		return "+" + FString::FromInt(NewScore);
	}
	return "0";
}

/*
 *	@brief	Calculate the score difference between yourself and the enemy team
 *
 *	@return	String to display in the UI
 */
FString AChess_GameMode::ComputeScoreBlackTeam()
{
	const int32 NewScore = ScoreBlackTeam - ScoreWhiteTeam;
	if (NewScore > 0)
	{
		return "+" + FString::FromInt(NewScore);
	}
	return "0";
}

/*
 *	@brief	Returns the field and the board state array to their original state
 *			and starts the match over again
 *
 *	@return	Void
 */
void AChess_GameMode::ResetField()
{
	// Send broadcast event to registered objects 
	OnResetEvent.Broadcast();

	TileArray.Empty();
	WhiteTeam.Empty();
	BlackTeam.Empty();

	Kings[WHITE] = nullptr;
	Kings[BLACK] = nullptr;

	bIsGameOver = false;
	bIsWhiteKingInCheckMate = false;
	bIsBlackKingInCheckMate = false;
	bIsDraw = false;
	MoveCounter = 0;

	TurnManager->ResetVariables();

	TurnManager->DestroyMoveHistory();

	if (ChessboardClass != nullptr)
	{
		CBoard = GetWorld()->SpawnActor<AChessboard>(ChessboardClass);
	}

	GameStates.Empty();
	
	ChoosePlayerAndStartGame();
}

/*
 *	@brief	Destroy PawnManager and TurnManager
 *
 *	@return	Void
 */
void AChess_GameMode::DestroyManagers()
{
	if (TurnManager)
	{
		TurnManager->Destroy();
		TurnManager = nullptr;
	}

	if (PromotionManager)
	{
		PromotionManager->Destroy();
		PromotionManager = nullptr;
	}
}
