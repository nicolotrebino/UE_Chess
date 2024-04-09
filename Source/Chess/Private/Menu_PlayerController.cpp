// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu_PlayerController.h"

#include "Blueprint/UserWidget.h"

AMenu_PlayerController::AMenu_PlayerController()
{
	bShowMouseCursor = true; // Allows the mouse not to be captured but to be visible during gameplay
}

void AMenu_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	MenuWidget = CreateWidget<UUserWidget>(this, MenuWidgetClass);
	MenuWidget->AddToViewport();
}
