// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Chess_GameMode.h"

ATile::ATile()
{
	// Template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	// Set default values
	Status = ETileStatus::EMPTY;
	Team = ETeam::NONE;
	AlgebraicPosition = {'A', 0};
	CurrentPieceOnTile = nullptr; // Nullptr as a default value and used also for the empty Tiles
	TileLocation = FVector(0, 0, 0);
}

/*
 *	@brief	Setter for the Tile location
 *
 *	@params	FVector as a location in the game scene
 *
 *	@return	Void
 */
void ATile::SetTileLocation(const FVector& Location)
{
	TileLocation = Location;
}

/*
 *	@brief	Getter for the Tile location
 *
 *	@return	FVector as the Tile location in the game scene
 */
FVector ATile::GetTileLocation() const
{
	return TileLocation;
}

/*
 * Set the Tile position based on the chess Algebraic notation,
 * with a letter and a number
 */

/*
 *	@brief	Set the Tile position based on the chess Algebraic notation,
 *			with a letter and a number
 *
 *	@params	X and Y to compute the algebraic position and as coordinates in the grid
 *
 *	@return	Void
 */
void ATile::SetAlgebraicPosition(const int X, const int Y)
{
	if ((X >= 0 && X < 8) && (Y >= 0 && Y < 8))
	{
		// Convert the number to the corresponding letter
		const TCHAR Letter = TEXT('a' + X);
		AlgebraicPosition.TileLetter = Letter;
		AlgebraicPosition.TileNumber = Y+1;
	}
}

/*
 *	@brief	Get the Tile position based on the chess Algebraic notation,
 *			with a letter and a number
 *
 *	@return	The Algebraic position of the Tile (letter, number)
 */
FAlgebraicPosition ATile::GetAlgebraicPosition() const
{
	return AlgebraicPosition;
}

/*
 *	@brief	Set the Tile status
 *
 *	@params	Status from the structure TileStatus
 *
 *	@return	Void
 */
void ATile::SetTileStatus(const ETileStatus TileStatus)
{
	Status = TileStatus;
}

/*
 *	@brief	Get the Tile status
 *
 *	@return	Tile status from the structure TileStatus
 */
ETileStatus ATile::GetTileStatus() const
{
	return Status;
}

/*
 *	@brief	Setter for the Tile team
 *
 *	@params	A team from the structure of team
 *
 *	@return	Void
 */
void ATile::SetTileTeam(const ETeam TileTeam)
{
	Team = TileTeam;
}

/*
 *	@brief	Getter for the Tile team
 *
 *	@return	The team of the Tile (that is, the team to which the piece above the tile itself belongs)
 */
ETeam ATile::GetTileTeam() const
{
	return Team;
}

/*
 *	@brief	Set the Piece above the Tile
 *
 *	@params Reference to the Piece above this Tile
 *
 *	@return	Void
 */
void ATile::SetPieceOnTile(AChess_Piece* ChessPiece)
{
	CurrentPieceOnTile = ChessPiece;
}

/*
 *	@brief	Get the Piece above the Tile
 *
 *	@return	Reference to that Piece
 */
AChess_Piece* ATile::GetPieceOnTile() const
{
	return CurrentPieceOnTile;
}


/*
 *	@brief	Set the proper Tile material
 *
 *	@params An integer for the index of the proper material in the array of materials
 *
 *	@return	Void
 */
void ATile::SetMaterial(const uint8 Index)
{
	if (DefaultMaterials[Index])
	{
		TileMaterial = Index;
		StaticMeshComponent->SetMaterial(0, DefaultMaterials[Index]);
	}
}

/*
 *	@brief	Set the Tile to the status TARGET and set the correct material
 *
 *	@return	Void
 */
void ATile::SetTargetTile()
{
	Status = ETileStatus::TARGET;
	if (TargetMaterials[TileMaterial])
	{
		StaticMeshComponent->SetMaterial(0, TargetMaterials[TileMaterial]);
	}
}

/*
 *	@brief	Set the Tile to the status EMPTY and set the proper material
 *
 *	@return	Void
 */
void ATile::UnsetTargetTile()
{
	Status = ETileStatus::EMPTY;
	StaticMeshComponent->SetMaterial(0, DefaultMaterials[TileMaterial]);
}

/*
 *	@brief	Set the right material for the Tile selected by the user
 *
 *	@return	Void
 */
void ATile::SetSelectedTile() const
{
	if (SelectMaterial && Status == ETileStatus::OCCUPIED)
	{
		StaticMeshComponent->SetMaterial(0, SelectMaterial);
	}
}

/*
 *	@brief	Returns the material to the default one (reset the material)
 *
 *	@return	Void
 */
void ATile::UnsetSelectedTile()
{
	StaticMeshComponent->SetMaterial(0, DefaultMaterials[TileMaterial]);
}

/*
 *	@brief	Set the Tile status to KILLABLE and set the proper material
 *
 *	@return	Void
 */
void ATile::SetKillableTile()
{
	Status = ETileStatus::KILLABLE;
	if (KillMaterials[TileMaterial])
	{
		StaticMeshComponent->SetMaterial(0, KillMaterials[TileMaterial]);
	} 
}

/*
 *	@brief	Set the Tile status to OCCUPIED and set the proper material
 *
 *	@return	Void
 */
void ATile::UnsetKillableTile()
{
	Status = ETileStatus::OCCUPIED;
	StaticMeshComponent->SetMaterial(0, DefaultMaterials[TileMaterial]);
}

// Called when the game starts or when spawned
/*
 *	@brief	Called when the game starts or when spawned.
 *			It Adds the "self destroy" to the broadcast event
 *
 *	@return	Void
 */
void ATile::BeginPlay()
{
	Super::BeginPlay();

	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->OnResetEvent.AddDynamic(this, &ATile::SelfDestroy);
}

/*
 *	@brief	Used by broadcast event to destruct the Tile during the "reset game field"
 *
 *	@return	Void
 */
void ATile::SelfDestroy()
{
	Destroy();
}

