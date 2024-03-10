// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Chess_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UChess_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Static method to obtain singleton instance
	UFUNCTION(BlueprintCallable, Category = "Chess_GameInstance")
	static UChess_GameInstance* GetChessGameInstance();

protected:
	// Static variable for the singleton instance
	static UChess_GameInstance* ChessGameInstanceInstance;

public:
	// Score value for human player
	UPROPERTY(EditAnywhere)
	int32 ScoreHumanPlayer = 0;

	// Score value for AI player
	UPROPERTY(EditAnywhere)
	int32 ScoreAiPlayer = 0;

	// Message to show every turn
	UPROPERTY(EditAnywhere)
	FString CurrentTurnMessage = "Current Player";

	// Increment the score for human player
	void IncrementScoreHumanPlayer();

	// Increment the score for AI player
	void IncrementScoreAiPlayer();

	// Get the score for human player
	UFUNCTION(BlueprintCallable)
	int32 GetScoreHumanPlayer();

	// Get the score for AI player
	UFUNCTION(BlueprintCallable)
	int32 GetScoreAiPlayer();

	// Get the current turn message
	UFUNCTION(BlueprintCallable)
	FString GetTurnMessage();

	// Set the turn message
	UFUNCTION(BlueprintCallable)
	void SetTurnMessage(FString Message);
};
