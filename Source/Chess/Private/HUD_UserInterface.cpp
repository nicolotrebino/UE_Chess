// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_UserInterface.h"

#include "Chess_GameMode.h"
#include "Chess_Pawn.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UHUD_UserInterface::SetNextTile(ATile* Tile)
{
	NextTile = Tile;
}

ATile* UHUD_UserInterface::GetNextTile() const
{
	return NextTile;
}

void UHUD_UserInterface::SetPreviousTile(ATile* Tile)
{
	PreviousTile = Tile;
}

ATile* UHUD_UserInterface::GetPreviousTile() const
{
	return PreviousTile;
}

void UHUD_UserInterface::SetPieceToMove(AChess_Piece* Piece)
{
	PieceToMove = Piece;
}

AChess_Piece* UHUD_UserInterface::GetPieceToMove() const
{
	return PieceToMove;
}

void UHUD_UserInterface::SetPieceToKill(AChess_Piece* KilledPiece)
{
	if (KilledPiece)
	{
		KilledPieceTeam = KilledPiece->GetTeam();
		KilledPieceType = KilledPiece->GetType();
	}
}

ETeam UHUD_UserInterface::GetPieceToKillTeam() const
{
	return KilledPieceTeam;
}

EPieceType UHUD_UserInterface::GetPieceToKillType() const
{
	return KilledPieceType;
}

void UHUD_UserInterface::SaveMove(const FString& Nomenclature)
{
	const AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	
	// If it is the Human
	if (!GameMode->CurrentPlayer)
	{
		UScrollBox* Container = Cast<UScrollBox>(WidgetTree->FindWidget("MH_Scroll"));

		if (Container)
		{
			UTextBlock* TextTurnNumber = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			const FString TurnString = FString::FromInt(FMath::CeilToInt(GameMode->MoveCounter / 2.0f));
			TextTurnNumber->SetText(FText::FromString(TurnString));
			
			// Creazione del widget di testo
			UTextBlock* TextBlockWidget = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			TextBlockWidget->SetText(FText::FromString(Nomenclature));

			// Creazione del widget del pulsante
			UButton* ButtonWidget = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
			MakeHistoryArray(ButtonWidget);
			// ButtonWidget->OnClicked.AddDynamic(this, &UHUD_UserInterface::Replay);

			// Creazione dell'Horizontal Box
			UHorizontalBox* HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

			// Assegna un nome all'Horizontal Box
			HorizontalBox->Rename(*FString::Printf(TEXT("MH_%d"), FMath::CeilToInt(GameMode->MoveCounter / 2.0f)));

			ButtonWidget->AddChild(TextBlockWidget);
			HorizontalBox->AddChild(TextTurnNumber);
			HorizontalBox->AddChild(ButtonWidget);
			Container->AddChild(HorizontalBox);
		}
	}
	else
	{
		UHorizontalBox* Container = Cast<UHorizontalBox>(WidgetTree->FindWidget(*FString::Printf(TEXT("MH_%d"), FMath::CeilToInt(GameMode->MoveCounter / 2.0f))));

		if (Container)
		{
			// Creazione del widget di testo
			UTextBlock* TextBlockWidget = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			TextBlockWidget->SetText(FText::FromString(Nomenclature)); // Sostituisci con il tuo testo desiderato

			// Creazione del widget del pulsante
			UButton* ButtonWidget = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
			MakeHistoryArray(ButtonWidget);
			// ButtonWidget->OnClicked.AddDynamic(this, &UHUD_UserInterface::Replay);

			ButtonWidget->AddChild(TextBlockWidget);
			Container->AddChild(ButtonWidget);
		}
	}
}

void UHUD_UserInterface::SaveEndGame(const FString& EndGame) const
{
	UScrollBox* Container = Cast<UScrollBox>(WidgetTree->FindWidget("MH_Scroll"));

	// Creazione del widget di testo
	UTextBlock* TextBlockWidget = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	TextBlockWidget->SetText(FText::FromString(EndGame));

	// Creazione del widget del pulsante
	UButton* ButtonWidget = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());

	ButtonWidget->AddChild(TextBlockWidget);
	Container->AddChild(ButtonWidget);
}

FString UHUD_UserInterface::ComputeNomenclature() const
{
	const AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	if (!(NextTile || PieceToMove))
	{
		return ""; // Gestione errore
	}

	FString MoveNomenclature = "";
	if (GameMode->bIsPromotion)
	{
		MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c/%c%d"), PieceToMove->GetNomenclature(), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
	}
	else if (PieceToMove->IsA(AChess_Pawn::StaticClass()))
	{
		if (PreviousTile && GameMode->bIsKill)
		{
			MoveNomenclature = FString::Printf(TEXT("%cx"), PreviousTile->GetAlgebraicPosition().TileLetter);
		}
		MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c%d"), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
	}
	else
	{
		MoveNomenclature = FString::Printf(TEXT("%c"), PieceToMove->GetNomenclature());
		if (GameMode->bIsKill)
		{
			MoveNomenclature = MoveNomenclature + "x";
		}
		MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c%d"), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
	}

	if (GameMode->bIsWhiteKingInCheckMate || GameMode->bIsBlackKingInCheckMate)
	{
		MoveNomenclature = MoveNomenclature + "#";
	}
	else if (GameMode->bIsWhiteKingInCheck || GameMode->bIsBlackKingInCheck)
	{
		MoveNomenclature = MoveNomenclature + "+";
	}
	
	return MoveNomenclature;
}


void UHUD_UserInterface::MakeHistoryArray(UButton* Button)
{
	const FHistoryButton Move = {Button, PieceToMove, KilledPieceTeam, KilledPieceType, PreviousTile, NextTile};
	MhButtons.Add(Move);
}

void UHUD_UserInterface::Backtrack()
{
	const UButton* ClickedButton = Cast<UButton>(this);
	int32 i = MhButtons.Num() - 1;
	while (i >= 0 && MhButtons[i].Button == ClickedButton)
	{
		MhButtons[i].MovedPiece->MovePiece(NextTile);
		if(MhButtons[i].KilledPieceTeam && MhButtons[i].KilledPieceType)
		{
			const AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
			GameMode->CBoard->SpawnSinglePiece(NextTile, MhButtons[i].KilledPieceTeam, MhButtons[i].KilledPieceType);
		}
		i--;
	}
}

void UHUD_UserInterface::DestroyMoveHistory() const
{
	// Trova la ScrollBox
	UScrollBox* Container = Cast<UScrollBox>(WidgetTree->FindWidget("MH_Scroll"));

	if (Container)
	{
		while(Container->HasAnyChildren())
		{
			Container->RemoveChildAt(0);
		}
	}
}

