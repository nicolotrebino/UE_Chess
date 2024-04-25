// Fill out your copyright notice in the Description page of Project Settings.

#include "Pieces/Chess_Pawn.h"

#include "Utility.h"
#include "Chess_GameMode.h"
#include "Managers/Manager_Promotion.h"
#include "Managers/Manager_Turn.h"
#include "Pieces/Chess_Queen.h"
#include "Players/MinimaxPlayer.h"
#include "Players/RandomPlayer.h"

AChess_Pawn::AChess_Pawn()
{
	// Template function that creates a components
	PawnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	PawnMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Every actor has a RootComponent that defines the transform in the World
	SetRootComponent(PawnComponent);
	PawnMeshComponent->SetupAttachment(PawnComponent);

	Nomenclature = 'P';
	PieceValue = 1;
}

/*
 *	@brief	Method inherited from the parent class to compute the PossibleMoves of the Pawn
 *
 *	@return	Array of Tiles where the Pawn can go
 */
TArray<ATile*> AChess_Pawn::GetPossibleMoves()
{
	Super::GetPossibleMoves();
	TArray<ATile*> PossibleMoves;
	
	const TCHAR CurrLetter = CurrentTile->GetAlgebraicPosition().TileLetter;
	const uint8 CurrNumber = CurrentTile->GetAlgebraicPosition().TileNumber;

	// For white pawns
	if (GetTeam() == WHITE)
	{
		// Advance
		if (CurrNumber == 2)
		{
			if (Utility::IsValidPosition(CurrLetter, CurrNumber + 1))
			{
				ATile* PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber + 1);
				if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY || PossibleMove->GetTileStatus() == ETileStatus::CASTLE)
				{
					PossibleMoves.Add(PossibleMove);
					if (Utility::IsValidPosition(CurrLetter, CurrNumber + 2))
					{
						PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber + 2);
						if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY || PossibleMove->GetTileStatus() == ETileStatus::CASTLE)
						{
							PossibleMoves.Add(PossibleMove);
						}
					}
				}
			}
		}
		else
		{
			if (Utility::IsValidPosition(CurrLetter, CurrNumber + 1))
			{
				ATile* PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber + 1);
				if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY || PossibleMove->GetTileStatus() == ETileStatus::CASTLE)
				{
					PossibleMoves.Add(PossibleMove);
				}
			}
		}

		// Kill
		if (Utility::IsValidPosition(CurrLetter + 1, CurrNumber + 1))
		{
			ATile* UpRight = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber + 1);

			if (UpRight && UpRight->GetTileStatus() == ETileStatus::OCCUPIED && (UpRight->GetTileTeam() != PieceTeam))
			{
				PossibleMoves.Add(UpRight);
			}
		}
		if (Utility::IsValidPosition(CurrLetter - 1, CurrNumber + 1))
		{
			ATile* UpLeft = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber + 1);

			if (UpLeft && UpLeft->GetTileStatus() == ETileStatus::OCCUPIED && (UpLeft->GetTileTeam() != PieceTeam))
			{
				PossibleMoves.Add(UpLeft);
			}
		}
	}
	
	// For black pawns
	else
	{
		// Recede
		if (CurrNumber == 7)
		{
			if (Utility::IsValidPosition(CurrLetter, CurrNumber - 1))
			{
				ATile* PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber - 1);
				if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY || PossibleMove->GetTileStatus() == ETileStatus::CASTLE)
				{
					PossibleMoves.Add(PossibleMove);
					if (Utility::IsValidPosition(CurrLetter, CurrNumber - 2))
					{
						PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber - 2);
						if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY || PossibleMove->GetTileStatus() == ETileStatus::CASTLE)
						{
							PossibleMoves.Add(PossibleMove);
						}
					}
				}
			}
		}
		else
		{
			if (Utility::IsValidPosition(CurrLetter, CurrNumber - 1))
			{
				ATile* PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber - 1);
				if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY || PossibleMove->GetTileStatus() == ETileStatus::CASTLE)
				{
					// PossibleMove->TargetTile();
					PossibleMoves.Add(PossibleMove);
				}
			}
		}

		// Kill
		if (Utility::IsValidPosition(CurrLetter + 1, CurrNumber - 1))
		{
			ATile* DownRight = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber - 1);

			if (DownRight && DownRight->GetTileStatus() == ETileStatus::OCCUPIED && (DownRight->GetTileTeam() != PieceTeam))
			{
				PossibleMoves.Add(DownRight);
			}
		}
		if (Utility::IsValidPosition(CurrLetter - 1, CurrNumber - 1))
		{
			ATile* DownLeft = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber - 1);

			if (DownLeft && DownLeft->GetTileStatus() == ETileStatus::OCCUPIED && (DownLeft->GetTileTeam() != PieceTeam))
			{
				PossibleMoves.Add(DownLeft);
			}
		}
	}

	return PossibleMoves;
}

/*
 *	@brief	Method inherited from the parent class to set
 *			the material for this piece based on its team
 *
 *	@param	Index: integer indicates the team of this Pawn
 *
 *	@return Void
 */
void AChess_Pawn::SetMaterial(const int32 Index)
{
	Super::SetMaterial(Index);
	
	PawnMeshComponent->SetMaterial(0, PawnMaterials[Index]);
}

/*
 *	@brief	Method inherited from the parent class to Move
 *			the pawn and, in particular conditions, promote it
 *
 *	@param	NextTile: reference to the Tile where the piece as to go
 *
 *	@return Void
 */
void AChess_Pawn::MovePiece(ATile* NextTile)
{
	Super::MovePiece(NextTile);

	if (!this->IsHidden())
	{
		if (NextTile->GetAlgebraicPosition().TileNumber == 8)
		{
			AManager_Turn* TurnManager = GameMode->TurnManager;
			AManager_Promotion* PromotionManager = GameMode->PromotionManager;
		
			// Implement promotion for Human player
			TurnManager->bIsPromotion = true;

			GameMode->WhiteTeam.Remove(this);

			PromotionManager->SetCurrentPawn(this);
			PromotionManager->StartPromotion();
		}
		else if (NextTile->GetAlgebraicPosition().TileNumber == 1)
		{
			AManager_Turn* TurnManager = GameMode->TurnManager;
			AManager_Promotion* PromotionManager = GameMode->PromotionManager;
		
			// Implement promotion for AI player
			TurnManager->bIsPromotion = true;

			GameMode->BlackTeam.Remove(this);

			PromotionManager->SetCurrentPawn(this);

			if (AMinimaxPlayer* Player = Cast<AMinimaxPlayer>(GameMode->Players[GameMode->CurrentPlayer]))
			{
				PromotionManager->HandleButtonClicked(1); // Always for the Queen
			}
			if (ARandomPlayer* Player = Cast<ARandomPlayer>(GameMode->Players[GameMode->CurrentPlayer]))
			{
				const int32 RandomNumber = FMath::RandRange(0, 3);
				PromotionManager->HandleButtonClicked(RandomNumber);
			}
		}
	}
}

// Called when the game starts or when spawned
void AChess_Pawn::BeginPlay()
{
	Super::BeginPlay();
}
