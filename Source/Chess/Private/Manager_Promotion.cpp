// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager_Promotion.h"

#include "Chess_GameMode.h"
#include "Pieces/Chess_Pawn.h"
#include "Chess_PlayerController.h"
#include "Manager_Turn.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AManager_Promotion::AManager_Promotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PromotionWidget = nullptr;
}

void AManager_Promotion::SelfDestroy()
{
	Destroy();
}

void AManager_Promotion::StartPromotion(AChess_Pawn* Pawn)
{
	AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (Cpc)
	{
		PromotionWidget = CreateWidget<UUserWidget>(Cpc, PromotionWidgetClass);
		PromotionWidget->AddToViewport();
	}
}

void AManager_Promotion::HandleButtonClicked(const int32 SelectedPieceIndex)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()); // Get the Chess_GameMode reference
	const AChess_PlayerController* Cpc = Cast<AChess_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AManager_Turn* TurnManager = GameMode->TurnManager;
	
	AChess_Piece* NewPiece;
	
	switch (SelectedPieceIndex)
	{
	case 0:
		NewPiece = GameMode->CBoard->SpawnSinglePiece(CurrentPawn->GetPieceTile(), GameMode->Players[GameMode->CurrentPlayer]->Team, ROOK);
		break;
	case 1:
		NewPiece = GameMode->CBoard->SpawnSinglePiece(CurrentPawn->GetPieceTile(), GameMode->Players[GameMode->CurrentPlayer]->Team, QUEEN);
		break;
	case 2:
		NewPiece = GameMode->CBoard->SpawnSinglePiece(CurrentPawn->GetPieceTile(), GameMode->Players[GameMode->CurrentPlayer]->Team, BISHOP);
		break;
	case 3:
		NewPiece = GameMode->CBoard->SpawnSinglePiece(CurrentPawn->GetPieceTile(), GameMode->Players[GameMode->CurrentPlayer]->Team, KNIGHT);
		break;
	default:
		NewPiece = nullptr;
		break;
	}
	
	// Cpc->UserInterfaceWidget->SetPieceToMove(NewPiece);
	// TurnManager->MovedPiece = NewPiece;
	TurnManager->PromotedPiece = NewPiece;
	GameMode->UpdateScores();
	// CurrentPawn->SelfDestroy();
	CurrentPawn->SetActorHiddenInGame(true);
	CurrentPawn->SetActorEnableCollision(false);

	if (GameMode->Players[GameMode->CurrentPlayer]->Team == WHITE)
	{
		GameMode->WhiteTeam.Add(NewPiece);
	} else
	{
		GameMode->BlackTeam.Add(NewPiece);
	}
	
	if (PromotionWidget)
	{
		PromotionWidget->RemoveFromParent();
		// Probably I can't destroy the widget directly
		// Hoping the garbage collector take care of it
		PromotionWidget = nullptr;
	}
}

void AManager_Promotion::SetCurrentPawn(AChess_Pawn* Pawn)
{
	CurrentPawn = Pawn;
}

AChess_Pawn* AManager_Promotion::GetPawn() const
{
	return CurrentPawn;
}

// Called when the game starts or when spawned
void AManager_Promotion::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AManager_Promotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

