// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_Piece.h"

#include "BlueprintEditor.h"
#include "Chess_GameMode.h"
#include "Chess_King.h"
// #include "Manager_Turn.h"
#include "Manager_Turn.h"
#include "Utility.h"

// Sets default values
AChess_Piece::AChess_Piece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set defaults value
	// GameMode = nullptr;
	// Nomenclature = '';
	// PieceTeam = ETeam::NONE;
	// PieceType = EPieceType::NONE;
	// CurrentTile = nullptr;
}

TCHAR AChess_Piece::GetNomenclature()
{
	return Nomenclature;
}

void AChess_Piece::SetTeam(const ETeam Team)
{
	PieceTeam = Team;
}

ETeam AChess_Piece::GetTeam()
{
	return PieceTeam;
}

void AChess_Piece::SetType(const EPieceType Type)
{
	PieceType = Type;
}

EPieceType AChess_Piece::GetType()
{
	return PieceType;
}

void AChess_Piece::SetPieceTile(ATile* Tile)
{
	CurrentTile = Tile;
}

ATile* AChess_Piece::GetPieceTile()
{
	return CurrentTile;
}

void AChess_Piece::SetPieceLocation(const FVector& Location)
{
	PieceLocation = Location;
}

FVector AChess_Piece::GetPieceLocation()
{
	return PieceLocation;
}

int32 AChess_Piece::GetPieceValue() const
{
	return PieceValue;
}

/*
 * Check if in the array of possible moves of a Piece
 * there are illegal moves, if so it removes them
 */
void AChess_Piece::PossibleMovesCheckControl(TArray<ATile*>& PossibleMoves)
{
	// Create a temporary new array same as the PossibleMoves array
	const AManager_Turn* TurnManager = GameMode->TurnManager;
	TArray<ATile*> NewArray = PossibleMoves;
	for (ATile* NextTile : PossibleMoves)
	{
		// Save the current Status and Team of the Next Tile
		const ETileStatus Status = NextTile->GetTileStatus();
		const ETeam Team = NextTile->GetTileTeam();
		
		// If the next tile is the one under the Checker piece it means I can kill the Checker
		if (!(TurnManager->Checker && NextTile == TurnManager->Checker->GetPieceTile()))
		{
			// Pretend you're making the next move
			NextTile->SetTileStatus(ETileStatus::OCCUPIED);
			NextTile->SetTileTeam(PieceTeam);
			CurrentTile->SetTileStatus(ETileStatus::EMPTY);
			CurrentTile->SetTileTeam(ETeam::NONE);
		
			// If the king is now/still in check with the move just made
			if (GameMode->IsKingInCheck(PieceTeam))
			{
				// Avoid doing this move and remove it from the new array of possible (legal) moves created
				NewArray.Remove(NextTile);
			}
		
			// Restores Tiles to how they originally were
			NextTile->SetTileStatus(Status);
			NextTile->SetTileTeam(Team);
			CurrentTile->SetTileStatus(ETileStatus::OCCUPIED);
			CurrentTile->SetTileTeam(PieceTeam);
		}
	}

	// Move, if it is possible, the New Array (modified) into the old PossibleMoves array
	PossibleMoves = MoveTempIfPossible(NewArray);
}

TArray<ATile*> AChess_Piece::GetLegitMoves()
{
	// Make an array of possible Tiles to move through
	TArray<ATile*> LegitMoves = GetPossibleMoves();

	if (this->IsA(AChess_King::StaticClass()))
	{
		return LegitMoves;
	}
	
	// Controls and modifies the array of possible Tiles to avoid a check
	PossibleMovesCheckControl(LegitMoves);

	return LegitMoves;
}

/*
 * Move the selected piece to the new location
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
	this->GetPieceTile()->SetTileStatus(ETileStatus::OCCUPIED); // Set the new Tile to OCCUPIED
	this->GetPieceTile()->SetTileTeam(PieceTeam);

	// Set the new reference of the Piece above the Tile
	NextTile->SetPieceOnTile(this);
}

void AChess_Piece::Kill(AChess_Piece* Enemy) const
{
	// Non dovrebbe esserci bisogno di cambiare il puntatore dalla Tile al Piece e metterlo a nullptr
	// perché intanto lo sovrascrivo con la Move() e faccio puntare la Tile direttamente al nuovo pezzo
	// sopra di essa
	if (Enemy->GetTeam() == ETeam::WHITE)
	{
		GameMode->WhiteTeam.Remove(Enemy);
		// GameMode->KilledWhiteTeam.Add(Enemy);
	}
	else
	{
		GameMode->BlackTeam.Remove(Enemy);
		// GameMode->KilledBlackTeam.Add(Enemy);
	}
	GameMode->UpdateScores();
	Enemy->SetActorHiddenInGame(true);
	Enemy->SetActorEnableCollision(false);
	GameMode->TurnManager->bIsKill = true;
}

void AChess_Piece::VirtualMove(ATile* NextTile)
{
	this->GetPieceTile()->SetTileTeam(NONE);
	this->GetPieceTile()->SetTileStatus(ETileStatus::EMPTY);
	this->GetPieceTile()->SetPieceOnTile(nullptr);

	this->SetPieceTile(NextTile); // Set the Tile under the ChessPiece
	NextTile->SetTileStatus(ETileStatus::OCCUPIED); // Set the new Tile to OCCUPIED
	NextTile->SetTileTeam(this->GetTeam());
	NextTile->SetPieceOnTile(this); // Set the new reference of the Piece above the Tile
}

void AChess_Piece::VirtualKill(AChess_Piece* Enemy)
{
	// Enemy->SetActorHiddenInGame(true);
	// Enemy->SetActorEnableCollision(false);
	// GameMode->WhiteTeam.Remove(Enemy);
	GameMode->KilledWhiteTeam.Add(Enemy);
}

void AChess_Piece::SelfDestroy()
{
	Destroy();
}

// Called when the game starts or when spawned
void AChess_Piece::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()); // Get the Chess_GameMode reference
	GameMode->OnResetEvent.AddDynamic(this, &AChess_Piece::SelfDestroy);
}

/*
 * Returns an array containing all the pointers to the Tiles on the vertical line of that particular piece
 * However, it returns only the cells into which the piece can go:
 * if it finds an occupied cell along the way, it will not be able to continue afterwards
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
 * Returns an array containing all the pointers to the Tiles on the horizontal line of that particular piece
 * However, it returns only the cells into which the piece can go:
 * if it finds an occupied cell along the way, it will not be able to continue afterwards
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
 * Returns an array containing all the pointers to the Tiles on the diagonal lines of that particular piece
 * However, it returns only the cells into which the piece can go:
 * if it finds an occupied cell along the way, it will not be able to continue afterwards
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

