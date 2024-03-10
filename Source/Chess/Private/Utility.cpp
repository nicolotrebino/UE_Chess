// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility.h"

// Function to check if a chess position is valid
bool Utility::IsValidPosition(const int32 Letter, const int32 Number)
{
	// Check if the letter and number are within the valid range for a chessboard
	return (Letter >= 'a' && Letter <= 'h' && Number >= 1 && Number <= 8);
}
