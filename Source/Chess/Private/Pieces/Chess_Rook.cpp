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

TArray<ATile*> AChess_Rook::GetPossibleMoves()
{
	Super::GetPossibleMoves();
	
	TArray<ATile*> PossibleMoves;
	TArray<ATile*> VerticalLine = GetVerticalLine();
	TArray<ATile*> HorizontalLine = GetHorizontalLine();
	
	for (ATile* Tile : VerticalLine)
	{
		// Tile->TargetTile();
		PossibleMoves.Add(Tile);
	}
	
	for (ATile* Tile : HorizontalLine)
	{
		// Tile->TargetTile();
		PossibleMoves.Add(Tile);
	}

	return PossibleMoves;
}

void AChess_Rook::SetMaterial(const int32 Index)
{
	Super::SetMaterial(Index);
	RookMeshComponent->SetMaterial(0, RookMaterials[Index]);
}

void AChess_Rook::BeginPlay()
{
	Super::BeginPlay();
}
