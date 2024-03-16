// Fill out your copyright notice in the Description page of Project Settings.

#include "Chess_GameInstance.h"

int32 UChess_GameInstance::GetScoreHumanPlayer()
{
	return ScoreHumanPlayer;
}

int32 UChess_GameInstance::GetScoreAiPlayer()
{
	return ScoreAiPlayer;
}

void UChess_GameInstance::IncrementScoreHumanPlayer()
{
	ScoreHumanPlayer += 1;
}

void UChess_GameInstance::IncrementScoreAiPlayer()
{
	ScoreAiPlayer += 1;
}

void UChess_GameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}

FString UChess_GameInstance::GetTurnMessage()
{
	return CurrentTurnMessage;
}
