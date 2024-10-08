// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chessboard.h"
#include "Managers/Manager_Turn.h"
#include "Players/PlayerInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Chess_GameMode.generated.h"

class AManager_Promotion;
class AChess_King;

// Macro declaration for a dynamic multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);

/**
 *	It keeps track of all the variables needed for every chess game you play
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
	 *	Game variables
	 */
	bool bIsGameOver; // Tracks if the game is over
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsWhiteKingInCheckMate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsBlackKingInCheckMate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsDraw;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bInReplay; // If the human player is using the MoveHistory
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MoveCounter; // Tracks the number of moves
	
	// Array of player interfaces
	TArray<IPlayerInterface*> Players;
	int32 CurrentPlayer;

	/*
	 *	Managers & Chessboard
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessboard> ChessboardClass;
	UPROPERTY(VisibleAnywhere)
	AChessboard* CBoard; // Reference to the Chessboard

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AManager_Turn> TurnManagerClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AManager_Turn* TurnManager; // Reference to the TurnManager

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AManager_Promotion> PromotionManagerClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AManager_Promotion* PromotionManager; // Reference to the PromotionManager

	/*
	 *	Arrays to store important stuff for the game
	 */
	TArray<TPair<FString, int32>> GameStates; // It stores each state of the game as a pair (string, int)

	UPROPERTY(Transient)
	TArray<ATile*> TileArray; // Keeps track of Tiles
	UPROPERTY(Transient)
	TArray<AChess_Piece*> WhiteTeam; // Keeps track of White Pieces
	UPROPERTY(Transient)
	TArray<AChess_Piece*> BlackTeam; // Keeps track of Black Pieces

	// Array with references to the two kings on the Chessboard
	// 0 (WHITE) --> White King
	// 1 (BLACK) --> Black King
	TArray<AChess_King*> Kings;

	/*
	 *	Audio Components
	 */
	UPROPERTY(EditDefaultsOnly)
	USoundBase* MoveSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* KillSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* CheckSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* CastlingSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* WinSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* LoseSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DrawSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bEnableSound = true; // Used to enable or disable sounds during the match

	/*
	 *	Class methods
	 */
	void GetAllLegalMoves(int32 Player);
	void ChoosePlayerAndStartGame(); // Called at the start of the game
	int32 GetNextPlayer(int32 Player) const; // Get the next player index
	UFUNCTION(BlueprintCallable)
	void TurnNextPlayer(); // Called at the end of the game turn
	bool IsKingInCheck(const int32 KingTeam);
	ATile* GetTileAtPosition(const TCHAR Letter, const uint8 Number);

	/*
	 *	Score manager
	 */
	int32 ScoreWhiteTeam;
	int32 ScoreBlackTeam;
	
	UFUNCTION(BlueprintCallable)
	FString ComputeScoreWhiteTeam();
	UFUNCTION(BlueprintCallable)
	FString ComputeScoreBlackTeam();
	void UpdateScores();

	/*
	 *	Reset field & turn to the menu
	 */
	// BlueprintAssignable usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	UPROPERTY(BlueprintAssignable)
	FOnReset OnResetEvent;
	UFUNCTION(BlueprintCallable)
	void ResetField(); // Destroy Chessboard and pieces and create a new game field
	UFUNCTION(BlueprintCallable)
	void DestroyManagers(); // Destroy PawnManager and TurnManager
};
