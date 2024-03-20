// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager_Turn.h"

#include "Chess_GameMode.h"
#include "Chess_PlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AManager_Turn::AManager_Turn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Checker = nullptr;

	MovedPiece = nullptr;
	KilledPiece = nullptr;
	
	bIsKill = false;
	bIsPromotion = false;
	bIsBlackKingInCheck = false;
	bIsWhiteKingInCheck = false;

	PreviousTile = nullptr;
	NextTile = nullptr;
}

void AManager_Turn::SelfDestroy()
{
	Destroy();
}

void AManager_Turn::ResetVariables()
{
	Checker = nullptr;
	
	bIsKill = false;
	bIsPromotion = false;
	bIsBlackKingInCheck = false;
	bIsWhiteKingInCheck = false;

	PreviousTile = nullptr;
	NextTile = nullptr;
}

void AManager_Turn::SetTilesAndPieces(ATile* PTile, ATile* NTile, AChess_Piece* PieceToMove, AChess_Piece* PieceToKill)
{
	PreviousTile = PTile;
	NextTile = NTile;
	MovedPiece = PieceToMove;
	KilledPiece = PieceToKill;
}

void AManager_Turn::DisplayMove() const
{
	const AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Creazione del widget del pulsante
	UUserWidget* ButtonWidget = Cpc->UserInterfaceWidget->WidgetTree->ConstructWidget<UUserWidget>(MhButtonClass);
	FHistoryButton Move = {ButtonWidget, MovedPiece, KilledPiece, PreviousTile, NextTile};
	// GameMode->MhButtons.Add(Move);
	
	// If it is the Human
	if (!GameMode->CurrentPlayer)
	{
		UVerticalBox* Container = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_White"));
		
		if (Container)
		{
			Container->AddChild(ButtonWidget);
		}
	}
	else
	{
		UVerticalBox* Container = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_Red"));

		if (Container)
		{
			Container->AddChild(ButtonWidget);
		}
	}
}

void AManager_Turn::DisplayEndGame() const
{
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UScrollBox* Container = Cast<UScrollBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_Scroll"));

	if (Container)
	{
		UUserWidget* LastButton = Cpc->UserInterfaceWidget->WidgetTree->ConstructWidget<UUserWidget>(EndButtonClass);
		Container->AddChild(LastButton);
	}
}

FString AManager_Turn::ComputeNotation() const
{
	const AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	const AManager_Turn* TurnManager = GameMode->TurnManager;
	if (!(NextTile || MovedPiece))
	{
		return ""; // Gestione errore
	}

	FString MoveNomenclature = "";
	if (TurnManager->bIsPromotion)
	{
		MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c/%c%d"), MovedPiece->GetNomenclature(), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
	}
	else if (MovedPiece->GetType() == EPieceType::PAWN)
	{
		if (PreviousTile && TurnManager->bIsKill)
		{
			MoveNomenclature = FString::Printf(TEXT("%cx"), PreviousTile->GetAlgebraicPosition().TileLetter);
		}
		MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c%d"), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
	}
	else
	{
		MoveNomenclature = FString::Printf(TEXT("%c"), MovedPiece->GetNomenclature());
		if (TurnManager->bIsKill)
		{
			MoveNomenclature = MoveNomenclature + "x";
		}
		MoveNomenclature = MoveNomenclature + FString::Printf(TEXT("%c%d"), NextTile->GetAlgebraicPosition().TileLetter, NextTile->GetAlgebraicPosition().TileNumber);
	}

	if (GameMode->bIsWhiteKingInCheckMate || GameMode->bIsBlackKingInCheckMate)
	{
		MoveNomenclature = MoveNomenclature + "#";
	}
	else if (TurnManager->bIsWhiteKingInCheck || TurnManager->bIsBlackKingInCheck)
	{
		MoveNomenclature = MoveNomenclature + "+";
	} 
	return MoveNomenclature;
}

void AManager_Turn::DestroyMoveHistory() const
{
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UVerticalBox* WhiteHistory = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_White"));
	UVerticalBox* RedHistory = Cast<UVerticalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_Red"));

	WhiteHistory->ClearChildren();
	RedHistory->ClearChildren();
}

// Called when the game starts or when spawned
void AManager_Turn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AManager_Turn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

