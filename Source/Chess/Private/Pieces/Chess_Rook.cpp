// Fill out your copyright notice in the Description page of Project Settings.

#include "Pieces/Chess_Rook.h"

AChess_Rook::AChess_Rook()
{
	// Template function that creates a components
	RookComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RookMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Every actor has a RootComponent that defines the transform in the World
	SetRootComponent(RookComponent);
	RookMeshComponent->SetupAttachment(RookComponent);
	
	Nomenclature = 'R';
	PieceValue = 5;
}

/*
 *	@brief	Method inherited from the parent class to compute the PossibleMoves of the Rook
 *
 *	@return	Array of Tiles where the Rook can go
 */
TArray<ATile*> AChess_Rook::GetPossibleMoves()
{
	Super::GetPossibleMoves();
	
	TArray<ATile*> PossibleMoves;
	TArray<ATile*> VerticalLine = GetVerticalLine();
	TArray<ATile*> HorizontalLine = GetHorizontalLine();
	
	for (ATile* Tile : VerticalLine)
	{
		PossibleMoves.Add(Tile);
	}
	
	for (ATile* Tile : HorizontalLine)
	{
		PossibleMoves.Add(Tile);
	}

	return PossibleMoves;
}

/*
 *	@brief	Method inherited from the parent class to set
 *			the material for this piece based on its team
 *
 *	@param	Index: integer indicates the team of this Rook
 *
 *	@return Void
 */
void AChess_Rook::SetMaterial(const int32 Index)
{
	Super::SetMaterial(Index);
	RookMeshComponent->SetMaterial(0, RookMaterials[Index]);
}

// Called when the game starts or when spawned
void AChess_Rook::BeginPlay()
{
	Super::BeginPlay();
}
