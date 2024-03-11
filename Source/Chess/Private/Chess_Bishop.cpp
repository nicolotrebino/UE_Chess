// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_Bishop.h"

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

TArray<ATile*> AChess_Bishop::GetPossibleMoves()
{
	Super::GetPossibleMoves();

	TArray<ATile*> DiagonalLines = GetDiagonalLine();

	return DiagonalLines;
}

void AChess_Bishop::SetMaterial(const int32 Index)
{
	Super::SetMaterial(Index);

	BishopMeshComponent->SetMaterial(0, BishopMaterials[Index]);
}

void AChess_Bishop::BeginPlay()
{
	Super::BeginPlay();
}
