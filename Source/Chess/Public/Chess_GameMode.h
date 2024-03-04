// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chessboard.h"
#include "GameFramework/GameModeBase.h"
#include "Chess_GameMode.generated.h"

class AChess_King;

// Macro declaration for a dynamic multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);

/**
 * 
 */
UCLASS()
class CHESS_API AChess_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AChess_GameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	 * Game variable
	 */
	bool IsGameOver; // Tracks if the game is over
	int32 MoveCounter; // Tracks the number of moves in order to signal a drawn game
	bool bIsCheckMate;
	bool bIsDraw;

	/*
	 * Game variable for a turn
	 */
	bool bIsKill; // If in that turn there was a kill
	AChess_Piece* Checker; // Piece that is in check
	bool bIsBlackKingInCheck;
	bool bIsWhiteKingInCheck;
	
	// Array of player interfaces
	TArray<IPlayerInterface*> Players;
	int32 CurrentPlayer;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessboard> ChessboardClass;
	UPROPERTY(VisibleAnywhere)
	AChessboard* CBoard;

	/*
	 * Array to store important stuff
	 */
	
	// Array with the two kings on the Chessboard
	// 0 (WHITE) --> White King
	// 1 (BLACK) --> Black King
	TArray<AChess_King*> Kings;

	UPROPERTY(Transient)
	TArray<ATile*> TileArray; // Keeps track of Tiles
	UPROPERTY(Transient)
	TArray<AChess_Piece*> WhiteTeam; // Keeps track of White Pieces
	UPROPERTY(Transient)
	TArray<AChess_Piece*> BlackTeam; // Keeps track of Black Pieces
	
	UPROPERTY(Transient)
	TArray<ATile*> TargetedTiles; // Keeps track of Targeted Tiles
	UPROPERTY(Transient)
	TArray<ATile*> KillableTiles; // Keeps track of Killable Tiles
	UPROPERTY(Transient)
	AChess_Piece* SelectedPiece; // Keeps track of Selected Piece

	/*
	 * Class methods
	 */
	void ChoosePlayerAndStartGame(); // Called at the start of the game
	int32 GetNextPlayer(int32 Player) const; // Get the next player index
	void TurnNextPlayer(); // Called at the end of the game turn
	void ResetTargetedAndKillableTiles();
	void ResetSelectedPiece() const;
	void SetGamePaused(bool bPaused) const;
	bool IsKingInCheck(const int32 KingTeam);
	bool IsCheckMate(const TArray<AChess_Piece*>& Team);
	ATile* GetTileAtPosition(const TCHAR Letter, const uint8 Number);

	// UFUNCTION(BlueprintPure, Category="CurrentPlayer")
	// EPieceTeam PrintCurrentPlayer();

	// BlueprintAssignable Usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	UPROPERTY(BlueprintAssignable)
	FOnReset OnResetEvent;

	// Destroy the Chessboard and the pieces and create a new game field
	UFUNCTION(BlueprintCallable)
	void ResetField();
};
