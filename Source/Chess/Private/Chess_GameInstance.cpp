// Fill out your copyright notice in the Description page of Project Settings.

#include "Chess_GameInstance.h"

/*
 * Singleton pattern implementation
 */ 
UChess_GameInstance* UChess_GameInstance::GetChessGameInstance()
{
	if (!ChessGameInstanceInstance)
	{
		// If the instance doesn't exist, make it
		ChessGameInstanceInstance = NewObject<UChess_GameInstance>();
	}

	return ChessGameInstanceInstance;
}

// Static variable initialization
UChess_GameInstance* UChess_GameInstance::ChessGameInstanceInstance = nullptr;

/*
 * Chess_GameInstance methods implementation
 */
void UChess_GameInstance::IncrementScoreHumanPlayer()
{
	ScoreHumanPlayer += 1;
}

void UChess_GameInstance::IncrementScoreAiPlayer()
{
	ScoreAiPlayer += 1;
}

int32 UChess_GameInstance::GetScoreHumanPlayer()
{
	return ScoreHumanPlayer;
}

int32 UChess_GameInstance::GetScoreAiPlayer()
{
	return ScoreAiPlayer;
}

FString UChess_GameInstance::GetTurnMessage()
{
	return CurrentTurnMessage;
}

void UChess_GameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}
