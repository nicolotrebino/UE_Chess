// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager_Turn.h"

#include "Chess_GameMode.h"
#include "Chess_PlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
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

void AManager_Turn::DisplayMove()
{
	const AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Creazione del widget del pulsante
	UUserWidget* ButtonWidget = Cpc->UserInterfaceWidget->WidgetTree->ConstructWidget<UUserWidget>(ButtonClass);
	FHistoryButton Move = {ButtonWidget, MovedPiece, KilledPiece, PreviousTile, NextTile};
	// GameMode->MhButtons.Add(Move);
	
	// If it is the Human
	if (!GameMode->CurrentPlayer)
	{
		UScrollBox* Container = Cast<UScrollBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget("MH_Scroll"));
		
		if (Container)
		{
			// Creazione dell'Horizontal Box
			UHorizontalBox* HorizontalBox = Cpc->UserInterfaceWidget->WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

			// Assegna un nome all'Horizontal Box per trovarla
			HorizontalBox->Rename(*FString::Printf(TEXT("MH_%d"), FMath::CeilToInt(GameMode->MoveCounter / 2.0f)));
			
			HorizontalBox->AddChild(ButtonWidget);
			Container->AddChild(HorizontalBox);
		}
	}
	else
	{
		UHorizontalBox* Container = Cast<UHorizontalBox>(Cpc->UserInterfaceWidget->WidgetTree->FindWidget(*FString::Printf(TEXT("MH_%d"), FMath::CeilToInt(GameMode->MoveCounter / 2.0f))));

		if (Container)
		{
			Container->AddChild(ButtonWidget);
		}
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

