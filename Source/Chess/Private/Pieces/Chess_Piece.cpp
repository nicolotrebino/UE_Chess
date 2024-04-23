// Fill out your copyright notice in the Description page of Project Settings.

#include "Pieces/Chess_Piece.h"

#include "Chess_GameMode.h"
#include "Pieces/Chess_King.h"
#include "Managers/Manager_Turn.h"
#include "Utility.h"

// Sets default values
AChess_Piece::AChess_Piece()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GameMode = nullptr;
	Nomenclature = 'z';
	PieceTeam = ETeam::NONE;
	PieceType = EPieceType::PAWN;
	CurrentTile = nullptr;
	PieceValue = 0;
}

/*
 *	@brief	Getter for the piece notation
 *
 *	@return	Letter indicates the piece in the Algebraic Notation
 */
TCHAR AChess_Piece::GetNomenclature() const
{
	return Nomenclature;
}

/*
 *	@brief	Setter for the piece team
 *
 *	@param	Team: team of piece from the ETeam enumeration
 *
 *	@return	Void
 */
void AChess_Piece::SetTeam(const ETeam Team)
{
	PieceTeam = Team;
}

/*
 *	@brief	Getter for the piece team
 *	
 *	@return	Team of the piece as ETeam enumeration
 */
ETeam AChess_Piece::GetTeam()
{
	return PieceTeam;
}

/*
 *	@brief	Setter for the piece type
 *
 *	@param	Type: type of the piece from the EPieceType enumeration
 *
 *	@return	Void
 */
void AChess_Piece::SetType(const EPieceType Type)
{
	PieceType = Type;
}

/*
 *	@brief	Getter for the piece type
 *
 *	@return	The type of the piece as EPieceType enumeration
 */
EPieceType AChess_Piece::GetType()
{
	return PieceType;
}

/*
 *	@brief	Setter for the Tile under the piece
 *
 *	@param	Tile: reference to the tile under the current piece
 *
 *	@return	Void
 */
void AChess_Piece::SetPieceTile(ATile* Tile)
{
	CurrentTile = Tile;
}

/*
 *	@brief	Getter for the Tile under the piece
 *
 *	@return	Reference to the Tile under the piece
 */
ATile* AChess_Piece::GetPieceTile()
{
	return CurrentTile;
}

/*
 *	@brief	Setter for the piece location in the grid and in the scene
 *
 *	@param	Location: FVector to set the Actor location in the scene
 *
 *	@return	Void
 */
void AChess_Piece::SetPieceLocation(const FVector& Location)
{
	PieceLocation = Location;
}

/*
 *	@brief	Getter for the piece location
 *
 *	@return	FVector as a location of the piece
 */
FVector AChess_Piece::GetPieceLocation()
{
	return PieceLocation;
}

/*
 *	@brief	Getter for the piece value to compute the scores
 *
 *	@return	The value of the piece as integer
 */
int32 AChess_Piece::GetPieceValue() const
{
	return PieceValue;
}

/*
 *	@brief	Check if in the array of possible moves of a Piece
 *			there are illegal moves, if so it removes them
 *
 *	@param	PossibleMoves: array of tiles in which the piece can move
 *
 *	@return	Void
 */
void AChess_Piece::PossibleMovesCheckControl(TArray<ATile*>& PossibleMoves)
{
	// Create a temporary new array same as the PossibleMoves array
	TArray<ATile*> NewArray = PossibleMoves;
	ATile* PreviousTile = CurrentTile;
	for (ATile* NextTile : PossibleMoves)
	{
		AChess_Piece* Killed = NextTile->GetPieceOnTile();

		this->VirtualMove(NextTile, PreviousTile, Killed);
	
		// If the king is now/still in check with the move just made
		if (GameMode->IsKingInCheck(PieceTeam))
		{
			// Avoid doing this move and remove it from the new array of possible (legal) moves created
			NewArray.Remove(NextTile);
		}

		this->VirtualUnMove(NextTile, PreviousTile, Killed);
	}

	// Move, if it is possible, the New Array (modified) into the old PossibleMoves array
	PossibleMoves = MoveTempIfPossible(NewArray);
}

/*
 *	@brief	Check that the piece (King) can move to its possible tiles without being "eaten"
 *
 *	@param	PossibleMoves: array of tiles in which the piece can move (normally the King)
 *
 *	@return	Void
 */
void AChess_Piece::CheckKingMobility(TArray<ATile*> &PossibleMoves)
{
	// Create a temporary new array same as the PossibleMoves array
	TArray<ATile*> NewArray = PossibleMoves;

	TArray<AChess_Piece*> EnemyTeam = (PieceTeam == WHITE) ? GameMode->BlackTeam : GameMode->WhiteTeam;

	ATile* PreviousTile = CurrentTile;
	// For each Tile in the possible moves array
	for (ATile* NextTile : PossibleMoves)
	{
		AChess_Piece* Killed = NextTile->GetPieceOnTile();

		VirtualMove(NextTile, PreviousTile, Killed);

		// For each enemy piece
		for (AChess_Piece* EnemyPiece : EnemyTeam)
		{
			// Get all the possible moves of that Enemy Piece
			TArray<ATile*> EnemyMoves = EnemyPiece->GetPossibleMoves();
			if (EnemyMoves.Contains(NextTile)) // If that array contains the NextTile
			{
				NewArray.Remove(NextTile); // Remove this Tile from  the array of possible moves
			}
		}

		VirtualUnMove(NextTile, PreviousTile, Killed);
		
	}

	// Move, if it is possible, the New Array (modified) into the old PossibleMoves array
	PossibleMoves = MoveTempIfPossible(NewArray);
}

/*
 *	@brief	Applies control to the possible moves of a piece
 *
 *	@return	Array of legal Tiles in which the piece can move
 */
TArray<ATile*> AChess_Piece::GetLegitMoves()
{
	// Make an array of possible Tiles to move through
	TArray<ATile*> LegitMoves = GetPossibleMoves();
	
	if (this->IsA(AChess_King::StaticClass()))
	{
		CheckKingMobility(LegitMoves);
	}
	else
	{
		// Controls and modifies the array of possible Tiles to avoid a check
		PossibleMovesCheckControl(LegitMoves);
	}

	return LegitMoves;
}

/*
 *	@brief	Move the selected piece to the new location
 *
 *	@param	NextTile: Tile where the piece has to go
 *
 *	@return	Void
 */
void AChess_Piece::MovePiece(ATile* NextTile)
{
	GameMode->TurnManager->SetTilesAndPieces(this->GetPieceTile(), NextTile, this, NextTile->GetPieceOnTile());
	
	// UnSelect the Tile under the selected piece
	this->GetPieceTile()->SetTileStatus(ETileStatus::EMPTY);
	this->GetPieceTile()->SetTileTeam(ETeam::NONE);
	this->GetPieceTile()->SetPieceOnTile(nullptr);
	this->GetPieceTile()->UnsetSelectedTile();
	
	// Set the next Location and Rotation of the ChessPiece
	const FVector SetLocation = NextTile->GetTileLocation() + FVector (0,0,0.1);
	const FRotator SetRotation = FRotator(0.0f, 90.0f, 0.0f);

	// Move the ChessPiece and set all the parameter
	this->SetActorLocation(SetLocation); // Set new location
	this->SetActorRotation(SetRotation); // Set new rotation
	this->SetPieceLocation(SetLocation); // Set new location for the Piece
	this->SetPieceTile(NextTile); // Set the Tile under the ChessPiece
	NextTile->SetTileStatus(ETileStatus::OCCUPIED); // Set the new Tile to OCCUPIED
	NextTile->SetTileTeam(PieceTeam);

	// Set the new reference of the Piece above the Tile
	NextTile->SetPieceOnTile(this);
}

/*
 *	@brief	Hide the killed piece
 *
 *	@param	Enemy: reference to the piece to kill
 *
 *	@return	Void
 */
void AChess_Piece::Kill(AChess_Piece* Enemy) const
{
	// I don't change the pointer of the Tile to the piece above it
	// because I already overwrite it with the "MovePiece"
	if (Enemy->GetTeam() == ETeam::WHITE)
	{
		GameMode->WhiteTeam.Remove(Enemy);
	}
	else
	{
		GameMode->BlackTeam.Remove(Enemy);
	}
	GameMode->UpdateScores();
	Enemy->SetActorHiddenInGame(true);
	Enemy->SetActorEnableCollision(false);
	GameMode->TurnManager->bIsKill = true;
}

/*
 *	@brief	Implement the MOVE but just virtually, not actually in the game
 *
 *	@param	NextTile: reference to the Tile where the piece has to go
 *	@param	PreviousTile: reference to the Tile where the piece started from
 *	@param	Killed: reference to the Piece to kill
 *
 *	@return	Void
 */
void AChess_Piece::VirtualMove(ATile* NextTile, ATile* PreviousTile, AChess_Piece* Killed)
{
	// Unlink the previous Tile to the Piece to move
	PreviousTile->SetTileTeam(NONE);
	PreviousTile->SetTileStatus(ETileStatus::EMPTY);
	PreviousTile->SetPieceOnTile(nullptr);

	if (Killed && NextTile->GetTileStatus() == ETileStatus::OCCUPIED && NextTile->GetTileTeam() != PieceTeam)
	{
		if (Killed->GetTeam() == ETeam::WHITE)
		{
			GameMode->WhiteTeam.Remove(Killed);
		}
		if (Killed->GetTeam() == ETeam::BLACK)
		{
			GameMode->BlackTeam.Remove(Killed);
		}
	}

	this->SetPieceTile(NextTile); // Set the Tile under the ChessPiece
	NextTile->SetTileStatus(ETileStatus::OCCUPIED); // Set the new Tile to OCCUPIED
	NextTile->SetTileTeam(PieceTeam);
	NextTile->SetPieceOnTile(this); // Set the new reference of the Piece above the Tile
}

/*
 *	@brief	Implement the MOVE-BACK but just virtually, not actually in the game
 *
 *	@param	NextTile: reference to the Tile where the piece has to go
 *	@param	PreviousTile: reference to the Tile where the piece started from
 *	@param	Killed: reference to the Piece to kill
 *
 *	@return	Void
 */
void AChess_Piece::VirtualUnMove(ATile* NextTile, ATile* PreviousTile, AChess_Piece* Killed)
{
	if (Killed)
	{
		if (Killed->GetTeam() == ETeam::WHITE)
		{
			// GameMode->KilledWhiteTeam.Remove(Killed);
			GameMode->WhiteTeam.Add(Killed);
		}
		if (Killed->GetTeam() == ETeam::BLACK)
		{
			// GameMode->KilledBlackTeam.Remove(Killed);
			GameMode->BlackTeam.Add(Killed);
		}
		NextTile->SetPieceOnTile(Killed);
		NextTile->SetTileTeam(Killed->GetTeam());
		NextTile->SetTileStatus(ETileStatus::OCCUPIED);
		Killed->SetPieceTile(NextTile);
	}
	else
	{
		NextTile->SetPieceOnTile(nullptr);
		NextTile->SetTileTeam(NONE);
		NextTile->SetTileStatus(ETileStatus::EMPTY);
	}
	PreviousTile->SetPieceOnTile(this);
	PreviousTile->SetTileTeam(PieceTeam);
	PreviousTile->SetTileStatus(ETileStatus::OCCUPIED);
	this->SetPieceTile(PreviousTile);
}

/*
 *	@brief	Used by broadcast event to destruct the Chess_Piece during the "reset game field"
 *
 *	@return	Void
 */
void AChess_Piece::SelfDestroy()
{
	Destroy();
}

// Called when the game starts or when spawned
// It Adds the "self destroy" to the broadcast event
void AChess_Piece::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()); // Get the Chess_GameMode reference
	GameMode->OnResetEvent.AddDynamic(this, &AChess_Piece::SelfDestroy);
}

/*
 *	@brief	Returns an array containing all the pointers to the Tiles on the vertical line of that particular piece
 *			However, it returns only the cells into which the piece can go:
 *			if it finds an occupied cell along the way, it will not be able to continue afterward
 *
 *	@return	Array of Tiles where the piece can go in the vertical line
 */
TArray<ATile*> AChess_Piece::GetVerticalLine() const
{
	TArray<ATile*> VerticalTiles;

	// Boolean for vertical directions
	// U (Up) and D (Down)
	bool bU = true;
	bool bD = true;

	for (int i = 1; i < 8; i++)
	{
		// Calculate and save the new positions for each direction
		const int32 NewNumberUp = CurrentTile->GetAlgebraicPosition().TileNumber + i;
		const int32 NewNumberDown = CurrentTile->GetAlgebraicPosition().TileNumber - i;
		const int32 ConstLetter = CurrentTile->GetAlgebraicPosition().TileLetter;

		// Proceed upwards
		// Check if the new positions are valid and add corresponding tiles to the array
		if (bU && Utility::IsValidPosition(ConstLetter, NewNumberUp))
		{
			ATile* VerticalTile = GameMode->GetTileAtPosition(ConstLetter, NewNumberUp);
			if (VerticalTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				VerticalTiles.Add(VerticalTile);
			}
			else if (VerticalTile->GetTileStatus() == ETileStatus::OCCUPIED && (VerticalTile->GetTileTeam() != this->PieceTeam))
			{
				VerticalTiles.Add(VerticalTile);
				bU = false;
			}
			else bU = false;
		}

		// Proceed downwards
		// Check if the new positions are valid and add corresponding tiles to the array
		if (bD && Utility::IsValidPosition(ConstLetter, NewNumberDown))
		{
			ATile* VerticalTile = GameMode->GetTileAtPosition(ConstLetter, NewNumberDown);
			if (VerticalTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				VerticalTiles.Add(VerticalTile);
			}
			else if (VerticalTile->GetTileStatus() == ETileStatus::OCCUPIED && (VerticalTile->GetTileTeam() != this->PieceTeam))
			{
				VerticalTiles.Add(VerticalTile);
				bD = false;
			}
			else bD = false;
		}
	}

	return VerticalTiles;
}

/*
 *	@brief	Returns an array containing all the pointers to the Tiles on the horizontal line of that particular piece
 *			However, it returns only the cells into which the piece can go:
 *			if it finds an occupied cell along the way, it will not be able to continue afterward
 *
 *	@return	Array of Tiles where the piece can go in the horizontal line
 */
TArray<ATile*> AChess_Piece::GetHorizontalLine() const
{
	TArray<ATile*> HorizontalTiles;

	// Boolean for horizontal directions
	// R (right) and L (left)
	bool bR = true;
	bool bL = true;

	for (int i = 1; i < 8; i++)
	{
		// Calculate and save the new positions for each direction
		const int32 NewLetterRight = CurrentTile->GetAlgebraicPosition().TileLetter + i;
		const int32 NewLetterLeft = CurrentTile->GetAlgebraicPosition().TileLetter - i;
		const int32 ConstNumber = CurrentTile->GetAlgebraicPosition().TileNumber;

		// Proceed to the right
		// Check if the new positions are valid and add corresponding tiles to the array
		if (bR && Utility::IsValidPosition(NewLetterRight, ConstNumber))
		{
			ATile* HorizontalTile = GameMode->GetTileAtPosition(NewLetterRight, ConstNumber);
			if (HorizontalTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				HorizontalTiles.Add(HorizontalTile);
			}
			else if (HorizontalTile->GetTileStatus() == ETileStatus::OCCUPIED && (HorizontalTile->GetTileTeam() != this->PieceTeam))
			{
				HorizontalTiles.Add(HorizontalTile);
				bR = false;
			}
			else bR = false;
		}

		// Proceed to the left
		// Check if the new positions are valid and add corresponding tiles to the array
		if (bL && Utility::IsValidPosition(NewLetterLeft, ConstNumber))
		{
			ATile* HorizontalTile = GameMode->GetTileAtPosition(NewLetterLeft, ConstNumber);
			if (HorizontalTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				HorizontalTiles.Add(HorizontalTile);
			}
			else if (HorizontalTile->GetTileStatus() == ETileStatus::OCCUPIED && (HorizontalTile->GetTileTeam() != this->PieceTeam))
			{
				HorizontalTiles.Add(HorizontalTile);
				bL = false;
			}
			else bL = false;
		}
	}
	
	return HorizontalTiles;
}

/*
 *	@brief	Returns an array containing all the pointers to the Tiles on the diagonal lines of that particular piece
 *			However, it returns only the cells into which the piece can go:
 *			if it finds an occupied cell along the way, it will not be able to continue afterward
 *
 *	@return	Array of Tiles where the piece can go in the diagonals line
 */
TArray<ATile*> AChess_Piece::GetDiagonalLine() const
{
	TArray<ATile*> DiagonalTiles;

	// Boolean for diagonal directions
	bool bUpR = true;
	bool bUpL = true;
	bool bDownR = true;
	bool bDownL = true;
	
	for (int i = 1; i < 8; i++)
	{
		// Calculate and save the new positions for each direction
		const int32 NewLetterRight = CurrentTile->GetAlgebraicPosition().TileLetter + i;
		const int32 NewNumberUp = CurrentTile->GetAlgebraicPosition().TileNumber + i;
		const int32 NewLetterLeft = CurrentTile->GetAlgebraicPosition().TileLetter - i;
		const int32 NewNumberDown = CurrentTile->GetAlgebraicPosition().TileNumber - i;

		// Proceed towards the diagonal at the top right
		// Check if the new positions are valid and add corresponding tiles to the array
		if (bUpR && Utility::IsValidPosition(NewLetterRight, NewNumberUp))
		{
			ATile* DiagonalTile = GameMode->GetTileAtPosition(NewLetterRight, NewNumberUp);
			if (DiagonalTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				DiagonalTiles.Add(DiagonalTile);
			}
			else if (DiagonalTile->GetTileStatus() == ETileStatus::OCCUPIED && (DiagonalTile->GetTileTeam() != this->PieceTeam))
			{
				DiagonalTiles.Add(DiagonalTile);
				bUpR = false;
			}
			else bUpR = false;
		}

		// Proceed towards the diagonal at the top left
		// Check if the new positions are valid and add corresponding tiles to the array
		if (bUpL && Utility::IsValidPosition(NewLetterLeft, NewNumberUp))
		{
			ATile* DiagonalTile = GameMode->GetTileAtPosition(NewLetterLeft, NewNumberUp);
			if (DiagonalTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				DiagonalTiles.Add(DiagonalTile);
			}
			else if (DiagonalTile->GetTileStatus() == ETileStatus::OCCUPIED && (DiagonalTile->GetTileTeam() != this->PieceTeam))
			{
				DiagonalTiles.Add(DiagonalTile);
				bUpL = false;
			}
			else bUpL = false;
		}

		// Proceed towards the diagonal at the bottom right
		// Check if the new positions are valid and add corresponding tiles to the array
		if (bDownR && Utility::IsValidPosition(NewLetterRight, NewNumberDown))
		{
			ATile* DiagonalTile = GameMode->GetTileAtPosition(NewLetterRight, NewNumberDown);
			if (DiagonalTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				DiagonalTiles.Add(DiagonalTile);
			}
			else if (DiagonalTile->GetTileStatus() == ETileStatus::OCCUPIED && (DiagonalTile->GetTileTeam() != this->PieceTeam))
			{
				DiagonalTiles.Add(DiagonalTile);
				bDownR = false;
			}
			else bDownR = false;
		}

		// Proceed towards the diagonal at the bottom left
		// Check if the new positions are valid and add corresponding tiles to the array
		if (bDownL && Utility::IsValidPosition(NewLetterLeft, NewNumberDown))
		{
			ATile* DiagonalTile = GameMode->GetTileAtPosition(NewLetterLeft, NewNumberDown);
			if (DiagonalTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				DiagonalTiles.Add(DiagonalTile);
			}
			else if (DiagonalTile->GetTileStatus() == ETileStatus::OCCUPIED && (DiagonalTile->GetTileTeam() != this->PieceTeam))
			{
				DiagonalTiles.Add(DiagonalTile);
				bDownL = false;
			}
			else bDownL = false;
		}
	}
	
	return DiagonalTiles;
}

// Called every frame
void AChess_Piece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

