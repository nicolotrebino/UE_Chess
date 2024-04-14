// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/Chess_Knight.h"

#include "Chess_GameMode.h"
#include "Utility.h"

AChess_Knight::AChess_Knight()
{
	// Template function that creates a components
	KnightComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	KnightMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Every actor has a RootComponent that defines the transform in the World
	SetRootComponent(KnightComponent);
	KnightMeshComponent->SetupAttachment(KnightComponent);

	Nomenclature = 'N';
	PieceValue = 3;
}

TArray<ATile*> AChess_Knight::GetPossibleMoves()
{
	Super::GetPossibleMoves();

	TArray<ATile*> PossibleMoves;
	
	const TCHAR CurrLetter = CurrentTile->GetAlgebraicPosition().TileLetter;
	const uint8 CurrNumber = CurrentTile->GetAlgebraicPosition().TileNumber;
	
	ATile* PossibleMove;
	
	if (Utility::IsValidPosition(CurrLetter + 1, CurrNumber + 2))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber + 2);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			// PossibleMove->TargetTile();
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter + 1, CurrNumber - 2))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber - 2);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			// PossibleMove->TargetTile();
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter - 1, CurrNumber + 2))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber + 2);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			// PossibleMove->TargetTile();
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter - 1, CurrNumber - 2))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber - 2);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			// PossibleMove->TargetTile();
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter + 2, CurrNumber + 1))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter + 2, CurrNumber +1);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			// PossibleMove->TargetTile();
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter + 2, CurrNumber - 1))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter + 2, CurrNumber - 1);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			// PossibleMove->TargetTile();
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter - 2, CurrNumber + 1))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter - 2, CurrNumber + 1);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			// PossibleMove->TargetTile();
			PossibleMoves.Add(PossibleMove);
		}
	}

	if (Utility::IsValidPosition(CurrLetter - 2, CurrNumber - 1))
	{
		PossibleMove = GameMode->GetTileAtPosition(CurrLetter - 2, CurrNumber - 1);
		if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY ||
			(PossibleMove->GetTileStatus() == ETileStatus::OCCUPIED && PossibleMove->GetTileTeam() != this->PieceTeam))
		{
			// PossibleMove->TargetTile();
			PossibleMoves.Add(PossibleMove);
		}
	}

	return PossibleMoves;
}

void AChess_Knight::SetMaterial(const int32 Index)
{
	Super::SetMaterial(Index);

	KnightMeshComponent->SetMaterial(0, KnightMaterials[Index]);
}

void AChess_Knight::BeginPlay()
{
	Super::BeginPlay();
}
