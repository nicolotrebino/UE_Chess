// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ETileStatus : uint8
{
	EMPTY			UMETA(DisplayName = "Empty"),
	OCCUPIED		UMETA(DisplayName = "Occupied"),
	TARGET			UMETA(DysplayName = "Target"),
	KILLABLE		UMETA(DisplayName = "Killable"),
};

UENUM()
enum EPieceTeam: uint8
{
	WHITE		UMETA(DisplayName = "White"),
	BLACK		UMETA(DisplayName = "Black"),
	NONE		UMETA(DisplayName = "None"),
};

UENUM()
enum EPieceType: uint8
{
	PAWN		UMETA(DisplayName = "Pawm"),
	KNIGHT		UMETA(DisplayName = "Knight"),
	BISHOP		UMETA(DisplayName = "Bishop"),
	ROOK		UMETA(DisplayName = "Rook"),
	QUEEN		UMETA(DisplayName = "Queen"),
	KING		UMETA(DisplayName = "King"),
};

