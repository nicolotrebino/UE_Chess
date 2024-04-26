// Fill out your copyright notice in the Description page of Project Settings.

#include "Chess_GameInstance.h"

/*
 *	@brief	Getter for the Human Player's score (number of games won by the human)
 *
 *	@return	The score of the human as an integer
 */
int32 UChess_GameInstance::GetScoreHumanPlayer()
{
	return ScoreHumanPlayer;
}

/*
 *	@brief	Getter for the AI Player's score (number of games won by the Ai)
 *
 *	@return	The score of the Ai as an integer
 */
int32 UChess_GameInstance::GetScoreAiPlayer()
{
	return ScoreAiPlayer;
}

/*
 *	@brief	Increments the Human Player's score by one
 *
 *	@return	Void
 */
void UChess_GameInstance::IncrementScoreHumanPlayer()
{
	ScoreHumanPlayer += 1;
}

/*
 *	@brief	Increments the AI Player's score by one
 *
 *	@return	Void
 */
void UChess_GameInstance::IncrementScoreAiPlayer()
{
	ScoreAiPlayer += 1;
}

/*
 * @brief Sets the current turn message to display in the UI
 *
 * @param  Message: string to show in the UI
 *
 * @return Void
 */
void UChess_GameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}

/*
 * @brief Getter for the current turn message
 *
 * @return The current turn message as a string
 */
FString UChess_GameInstance::GetTurnMessage()
{
	return CurrentTurnMessage;
}
