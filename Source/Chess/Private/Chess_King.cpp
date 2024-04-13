// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_King.h"

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

	// Check if the Possible Moves founded are legit or not
	// CheckMobility(PossibleMoves);

	return PossibleMoves;
}

void AChess_King::SetMaterial(const int32 Index)
{
	Super::SetMaterial(Index);

	KingMeshComponent->SetMaterial(0, KingMaterials[Index]);
}

void AChess_King::BeginPlay()
{
	Super::BeginPlay();
}

TArray<ATile*> AChess_King::GetNeighbors() const
{
	const TCHAR CurrLetter = CurrentTile->GetAlgebraicPosition().TileLetter;
	const uint8 CurrNumber = CurrentTile->GetAlgebraicPosition().TileNumber;

	TArray<ATile*> NeighbourTiles;
	ATile* Neighbor;
	
	if (Utility::IsValidPosition(CurrLetter + 1, CurrNumber))
	{
		Neighbor = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber);
		NeighbourTiles.Add(Neighbor);
	}

	if (Utility::IsValidPosition(CurrLetter - 1, CurrNumber))
	{
		Neighbor = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber);
		NeighbourTiles.Add(Neighbor);
	}

	if (Utility::IsValidPosition(CurrLetter, CurrNumber + 1))
	{
		Neighbor = GameMode->GetTileAtPosition(CurrLetter, CurrNumber + 1);
		NeighbourTiles.Add(Neighbor);
	}

	if (Utility::IsValidPosition(CurrLetter, CurrNumber - 1))
	{
		Neighbor = GameMode->GetTileAtPosition(CurrLetter, CurrNumber - 1);
		NeighbourTiles.Add(Neighbor);
	}

	if (Utility::IsValidPosition(CurrLetter + 1, CurrNumber + 1))
	{
		Neighbor = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber + 1);
		NeighbourTiles.Add(Neighbor);
	}

	if (Utility::IsValidPosition(CurrLetter + 1, CurrNumber - 1))
	{
		Neighbor = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber - 1);
		NeighbourTiles.Add(Neighbor);
	}
	
	if (Utility::IsValidPosition(CurrLetter - 1, CurrNumber + 1))
	{
		Neighbor = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber + 1);
		NeighbourTiles.Add(Neighbor);
	}

	if (Utility::IsValidPosition(CurrLetter - 1, CurrNumber - 1))
	{
		Neighbor = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber - 1);
		NeighbourTiles.Add(Neighbor);
	}

	return NeighbourTiles;
}

