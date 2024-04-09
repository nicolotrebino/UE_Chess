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

	/* Score of the players during all the game, it counts all the games won */
	UPROPERTY(EditAnywhere) 
	int32 ScoreHumanPlayer = 0; // Score value for human player
	UPROPERTY(EditAnywhere)
	int32 ScoreAiPlayer = 0; // Score value for AI player
	UFUNCTION(BlueprintCallable)
	int32 GetScoreHumanPlayer(); // Get the score for human player
	UFUNCTION(BlueprintCallable)
	int32 GetScoreAiPlayer(); // Get the score for AI player

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 EnemyPlayer;
	
	void IncrementScoreHumanPlayer(); // Increment the score for human player
	void IncrementScoreAiPlayer(); // Increment the score for AI player

	UPROPERTY(EditAnywhere)
	FString CurrentTurnMessage = "Current Player"; // Message to show every turn
	UFUNCTION(BlueprintCallable)
	void SetTurnMessage(FString Message); // Set the turn message
	UFUNCTION(BlueprintCallable)
	FString GetTurnMessage(); // Get the current turn message
};
