// Fill out your copyright notice in the Description page of Project Settings.

#include "Chess_PlayerController.h"

#include "HumanPlayer.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"

/*
 * Chess_GameMode methods implementation
 */
AChess_PlayerController::AChess_PlayerController()
{
	bShowMouseCursor = true; // Allows the mouse not to be captured but to be visible during gameplay
	bEnableClickEvents = true;
}

void AChess_PlayerController::ClickOnGrid()
{
	const auto HumanPlayer = Cast<AHumanPlayer>(GetPawn());
	if (IsValid(HumanPlayer))
	{
		HumanPlayer->OnClick();
	}
}

void AChess_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Subsystem manages the Player Controller
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(ChessContext, 0);
	}

	// UserInterfaceWidget = CreateWidget<UHUD_UserInterface>(this, UserInterfaceWidgetClass);
	// UserInterfaceWidget->AddToViewport();
}

void AChess_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AChess_PlayerController::ClickOnGrid);
	}
}