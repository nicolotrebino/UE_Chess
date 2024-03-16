// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Chess_GameMode.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	// Set default values
	Status = ETileStatus::EMPTY;
	Team = ETeam::NONE;
	ChessPosition = {'A', 0};
	CurrentPieceOnTile = nullptr; // Nullptr as a default value and used also for the empty Tiles
	TileLocation = FVector(0, 0, 0);
}

void ATile::SetTileLocation(const FVector& Location)
{
	TileLocation = Location;
}

FVector ATile::GetTileLocation() const
{
	return TileLocation;
}

/*
 * Set the Tile position based on the chess Algebraic notation,
 * with a letter and a number
 */
void ATile::SetAlgebraicPosition(const int X, const int Y)
{
	if ((X >= 0 && X < 8) && (Y >= 0 && Y < 8))
	{
		// Convert the number to the corresponding letter
		const TCHAR Letter = TEXT('a' + X);
		ChessPosition.TileLetter = Letter;
		ChessPosition.TileNumber = Y+1;
	}
}

FAlgebraicPosition ATile::GetAlgebraicPosition() const
{
	return ChessPosition;
}

void ATile::SetTileStatus(const ETileStatus TileStatus)
{
	Status = TileStatus;
}

ETileStatus ATile::GetTileStatus() const
{
	return Status;
}

void ATile::SetTileTeam(const ETeam TileTeam)
{
	Team = TileTeam;
}

ETeam ATile::GetTileTeam() const
{
	return Team;
}

void ATile::SetPieceOnTile(AChess_Piece* ChessPiece)
{
	CurrentPieceOnTile = ChessPiece;
}

AChess_Piece* ATile::GetPieceOnTile() const
{
	return CurrentPieceOnTile;
}


// Set the proper Tile Material
void ATile::SetMaterial(const uint8 Index)
{
	if (DefaultMaterials[Index])
	{
		TileMaterial = Index;
		StaticMeshComponent->SetMaterial(0, DefaultMaterials[Index]);
	}
}

// Set the Tile to TARGET and set the correct Material
void ATile::SetTargetTile()
{
	Status = ETileStatus::TARGET;
	if (TargetMaterials[TileMaterial])
	{
		StaticMeshComponent->SetMaterial(0, TargetMaterials[TileMaterial]);
	}
}

// Set the Tile to EMPTY and set the correct Material
void ATile::UnsetTargetTile()
{
	Status = ETileStatus::EMPTY;
	StaticMeshComponent->SetMaterial(0, DefaultMaterials[TileMaterial]);
}

// Set the Material for selection
void ATile::SetSelectedTile() const
{
	if (SelectMaterial && Status == ETileStatus::OCCUPIED)
	{
		StaticMeshComponent->SetMaterial(0, SelectMaterial);
	}
}

// Reset the default Material of the Tile
void ATile::UnsetSelectedTile()
{
	StaticMeshComponent->SetMaterial(0, DefaultMaterials[TileMaterial]);
}

// Set the Tile to KILLABLE and set the correct Material
void ATile::SetKillableTile()
{
	Status = ETileStatus::KILLABLE;
	if (KillMaterials[TileMaterial])
	{
		StaticMeshComponent->SetMaterial(0, KillMaterials[TileMaterial]);
	} 
}

// Set the Tile to OCCUPIED and set the correct Material
void ATile::UnsetKillableTile()
{
	Status = ETileStatus::OCCUPIED;
	StaticMeshComponent->SetMaterial(0, DefaultMaterials[TileMaterial]);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->OnResetEvent.AddDynamic(this, &ATile::SelfDestroy);
}

void ATile::SelfDestroy()
{
	Destroy();
}

// Called every frame
// void ATile::Tick(float DeltaTime)
// {
	// Super::Tick(DeltaTime);
// }

