// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_Queen.h"

AChess_Queen::AChess_Queen()
{
	// Template function that creates a components
	QueenComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	QueenMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Every actor has a RootComponent that defines the transform in the World
	SetRootComponent(QueenComponent);
	QueenMeshComponent->SetupAttachment(QueenComponent);

	Nomenclature = 'Q';
	PieceValue = 9;
}

TArray<ATile*> AChess_Queen::GetPossibleMoves()
{
	Super::GetPossibleMoves();

	TArray<ATile*> PossibleMoves;
	TArray<ATile*> VerticalLines = GetVerticalLine();
	TArray<ATile*> HorizontalLines = GetHorizontalLine();
	TArray<ATile*> DiagonalLines = GetDiagonalLine();
	
	for (ATile* Tile : VerticalLines)
	{
		PossibleMoves.Add(Tile);
	}
	
	for (ATile* Tile : HorizontalLines)
	{
		PossibleMoves.Add(Tile);
	}

	for (ATile* Tile : DiagonalLines)
	{
		PossibleMoves.Add(Tile);
	}

	return PossibleMoves;
}

void AChess_Queen::SetMaterial(const int32 Index)
{
	Super::SetMaterial(Index);

	QueenMeshComponent->SetMaterial(0, QueenMaterials[Index]);
}

void AChess_Queen::BeginPlay()
{
	Super::BeginPlay();
}
