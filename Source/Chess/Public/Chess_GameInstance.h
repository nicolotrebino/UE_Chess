// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Chess_GameInstance.generated.h"

/**
 * It keeps track of all the variables from when the game starts to when it ends
 */
UCLASS()
class CHESS_API UChess_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/* Variables to change game's settings */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 EnemyPlayer = 0; // Used to choose the AI player (Random or Minimax)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MusicNumber = 0; // Used to choose the background music
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* Music = nullptr; // Reference to the background audio component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bNoReplay = false; // Used to enable or disable replay (hard mode)

	/* Score of the players during all the game, it counts all the games won */
	UPROPERTY(EditAnywhere) 
	int32 ScoreHumanPlayer = 0; // Score value for human player
	UPROPERTY(EditAnywhere)
	int32 ScoreAiPlayer = 0; // Score value for AI player
	UFUNCTION(BlueprintCallable)
	int32 GetScoreHumanPlayer(); // Get the score for human player
	UFUNCTION(BlueprintCallable)
	int32 GetScoreAiPlayer(); // Get the score for AI player
	
	void IncrementScoreHumanPlayer(); // Increment the score for human player
	void IncrementScoreAiPlayer(); // Increment the score for AI player

	/* Game's turn messages */
	UPROPERTY(EditAnywhere)
	FString CurrentTurnMessage = "Current Player"; // Message to show every turn
	UFUNCTION(BlueprintCallable)
	void SetTurnMessage(FString Message); // Set the turn message
	UFUNCTION(BlueprintCallable)
	FString GetTurnMessage(); // Get the current turn message
};
