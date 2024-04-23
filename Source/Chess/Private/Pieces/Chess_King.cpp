// Fill out your copyright notice in the Description page of Project Settings.

#include "Pieces/Chess_King.h"

#include "Chess_GameMode.h"
#include "Utility.h"

AChess_King::AChess_King()
{
	// Template function that creates a components
	KingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	KingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Every actor has a RootComponent that defines the transform in the World
	SetRootComponent(KingComponent);
	KingMeshComponent->SetupAttachment(KingComponent);

	Nomenclature = 'K';
	PieceValue = 0;
}

/*
 *	@brief	Method inherited from the parent class to compute the PossibleMoves of the King
 *
 *	@return	Array of Tiles where the King can go
 */
TArray<ATile*> AChess_King::GetPossibleMoves()
{
	Super::GetPossibleMoves();

	// Make an array to save King's possible moves
	TArray<ATile*> PossibleMoves;

	// Save temporary current letter and number
	const TCHAR CurrLetter = CurrentTile->GetAlgebraicPosition().TileLetter;
	const uint8 CurrNumber = CurrentTile->GetAlgebraicPosition().TileNumber;
	
	ATile* PossibleMove;
	
	if (Utility::IsValidPosition(CurrLetter + 1, CurrNumber))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter - 1, CurrNumber))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter, CurrNumber + 1))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber + 1);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter, CurrNumber - 1))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber - 1);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter + 1, CurrNumber + 1))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber + 1);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter + 1, CurrNumber - 1))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber - 1);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			PossibleMoves.Add(PossibleMove);
		}
	}
	
	if (Utility::IsValidPosition(CurrLetter - 1, CurrNumber + 1))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber + 1);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter - 1, CurrNumber - 1))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber - 1);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			PossibleMoves.Add(PossibleMove);
		}
	}

	return PossibleMoves;
}

/*
 *	@brief	Method inherited from the parent class to set
 *			the material for this piece based on its team
 *
 *	@param	Index: integer indicates the team of this King
 *
 *	@return Void
 */
void AChess_King::SetMaterial(const int32 Index)
{
	Super::SetMaterial(Index);

	KingMeshComponent->SetMaterial(0, KingMaterials[Index]);
}

// Called when the game starts or when spawned
void AChess_King::BeginPlay()
{
	Super::BeginPlay();
}

