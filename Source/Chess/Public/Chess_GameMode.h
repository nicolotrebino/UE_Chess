// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chessboard.h"
#include "Manager_Turn.h"
#include "PlayerInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Chess_GameMode.generated.h"

class AManager_Promotion;
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

	/* Game variable */
	bool bIsGameOver; // Tracks if the game is over
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsWhiteKingInCheckMate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsBlackKingInCheckMate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsDraw;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MoveCounter; // Tracks the number of moves

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bInReplay;
	
	// Array of player interfaces
	TArray<IPlayerInterface*> Players;
	int32 CurrentPlayer;

	// Chessboard
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessboard> ChessboardClass;
	UPROPERTY(VisibleAnywhere)
	AChessboard* CBoard;

	// Turn Manager
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AManager_Turn> TurnManagerClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AManager_Turn* TurnManager;

	// Promotion Manager
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AManager_Promotion> PromotionManagerClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AManager_Promotion* PromotionManager;

	/*
	 * Array to store important stuff for the game
	 */
	
	// Array with references to the two kings on the Chessboard
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
	TArray<AChess_Piece*> KilledWhiteTeam; // Keeps track of Black Pieces
	UPROPERTY(Transient)
	TArray<AChess_Piece*> KilledBlackTeam; // Keeps track of Black Pieces

	// Audio Components
	UPROPERTY(EditDefaultsOnly)
	USoundBase* MoveSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* KillSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* WinSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* LoseSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DrawSound;

	/*
	 * Class methods
	 */

	TArray<ATile*> GetAllLegalMoves(int32 Player);
	
	void ChoosePlayerAndStartGame(); // Called at the start of the game
	int32 GetNextPlayer(int32 Player) const; // Get the next player index
	UFUNCTION(BlueprintCallable)
	void TurnNextPlayer(); // Called at the end of the game turn

	bool IsKingInCheck(const int32 KingTeam);
	bool IsCheckMate(const uint8 KingTeam, const TArray<AChess_Piece*>& Team);
	ATile* GetTileAtPosition(const TCHAR Letter, const uint8 Number);

	/* Score manager */
	UFUNCTION(BlueprintCallable)
	FString ComputeScoreWhiteTeam();
	UFUNCTION(BlueprintCallable)
	FString ComputeScoreBlackTeam();
	
	int32 ScoreWhiteTeam;
	int32 ScoreBlackTeam;
	
	void UpdateScores();

	// BlueprintAssignable usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	UPROPERTY(BlueprintAssignable)
	FOnReset OnResetEvent;

	// Destroy the Chessboard and the pieces and create a new game field
	UFUNCTION(BlueprintCallable)
	void ResetField();
};
