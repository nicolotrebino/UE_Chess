// Fill out your copyright notice in the Description page of Project Settings.

#include "Chess_PlayerController.h"

#include "Players/HumanPlayer.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"

AChess_PlayerController::AChess_PlayerController()
{
	bShowMouseCursor = true; // Allows the mouse not to be captured but to be visible during gameplay
	bEnableClickEvents = true;
	ChessContext = nullptr;
	ClickAction = nullptr;
	UserInterfaceWidget = nullptr;
}

/*
 *	@brief Method bonded to the user's left click, calls the Human Player method that manages the click
 *
 *	@return Void
 */
void AChess_PlayerController::ClickOnGrid()
{
	const auto HumanPlayer = Cast<AHumanPlayer>(GetPawn());
	if (IsValid(HumanPlayer))
	{
		HumanPlayer->OnClick();
	}
}

// Called when the game starts or when spawned.
// It maps the ChessContext and create the Widget (HUD)
void AChess_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Subsystem manages the Player Controller
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(ChessContext, 0);
	}

	UserInterfaceWidget = CreateWidget<UUserWidget>(this, UserInterfaceWidgetClass);
	UserInterfaceWidget->AddToViewport();
}

/*
 *	@brief Bind the click action to the method to be performed
 *
 *	@return Void
 */
void AChess_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AChess_PlayerController::ClickOnGrid);
	}
}
