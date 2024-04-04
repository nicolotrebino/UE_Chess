// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_Pawn.h"

#include "Utility.h"
#include "Chess_GameMode.h"
#include "Manager_Promotion.h"
#include "Manager_Turn.h"
#include "MinimaxPlayer.h"
#include "RandomPlayer.h"

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
				if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
				{
					PossibleMoves.Add(PossibleMove);
				if (Utility::IsValidPosition(CurrLetter, CurrNumber + 2))
				{
					PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber + 2);
					if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
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
				if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
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
			ATile* PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber - 1);
			if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
			{
				PossibleMoves.Add(PossibleMove);
				PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber - 2);
				if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
				{
					PossibleMoves.Add(PossibleMove);
				}
			}
		}
		else
		{
			ATile* PossibleMove = GameMode->GetTileAtPosition(CurrLetter, CurrNumber - 1);
			if (PossibleMove->GetTileStatus() == ETileStatus::EMPTY)
			{
				// PossibleMove->TargetTile();
				PossibleMoves.Add(PossibleMove);
			}
		}

		// Kill
		ATile* DownRight = GameMode->GetTileAtPosition(CurrLetter + 1, CurrNumber - 1);
		ATile* DownLeft = GameMode->GetTileAtPosition(CurrLetter - 1, CurrNumber - 1);
		
		if (DownRight && DownRight->GetTileStatus() == ETileStatus::OCCUPIED && (DownRight->GetTileTeam() != PieceTeam))
		{
			PossibleMoves.Add(DownRight);
		}

		if (DownLeft && DownLeft->GetTileStatus() == ETileStatus::OCCUPIED && (DownLeft->GetTileTeam() != PieceTeam))
		{
			PossibleMoves.Add(DownLeft);
		}
	}

	return PossibleMoves;
}

void AChess_Pawn::SetMaterial(const int32 Index)
{
	Super::SetMaterial(Index);
	
	PawnMeshComponent->SetMaterial(0, PawnMaterials[Index]);
}

// Override to implement the promotion
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
			PromotionManager->StartPromotion(this);
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
				PromotionManager->HandleButtonClicked(2); // Always for the Queen
			}
			if (ARandomPlayer* Player = Cast<ARandomPlayer>(GameMode->Players[GameMode->CurrentPlayer]))
			{
				const int32 RandomNumber = FMath::RandRange(0, 3);
				PromotionManager->HandleButtonClicked(RandomNumber);
			}
		}
	}
}

void AChess_Pawn::BeginPlay()
{
	Super::BeginPlay();
}