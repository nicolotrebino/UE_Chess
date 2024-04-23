// Fill out your copyright notice in the Description page of Project Settings.


#include "Pieces/Chess_Bishop.h"

AChess_Bishop::AChess_Bishop()
{
	// Template function that creates a components
	BishopComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	BishopMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Every actor has a RootComponent that defines the transform in the World
	SetRootComponent(BishopComponent);
	BishopMeshComponent->SetupAttachment(BishopComponent);

	Nomenclature = 'B';
	PieceValue = 3;
}

/*
 *	@brief	Method inherited from the parent class to compute the PossibleMoves of the Bishop
 *
 *	@return	Array of Tiles where the Bishop can go
 */
TArray<ATile*> AChess_Bishop::GetPossibleMoves()
{
	Super::GetPossibleMoves();

	TArray<ATile*> DiagonalLines = GetDiagonalLine();

	return DiagonalLines;
}

/*
 *	@brief	Method inherited from the parent class to set
 *			the material for this piece based on its team
 *
 *	@param	Index: integer indicates the team of this Bishop
 *
 *	@return Void
 */
void AChess_Bishop::SetMaterial(const int32 Index)
{
	Super::SetMaterial(Index);

	BishopMeshComponent->SetMaterial(0, BishopMaterials[Index]);
}

// Called when the game starts or when spawned
void AChess_Bishop::BeginPlay()
{
	Super::BeginPlay();
}
