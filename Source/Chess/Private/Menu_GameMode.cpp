// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu_GameMode.h"

#include "EngineUtils.h"
#include "Menu_Player.h"
#include "Menu_PlayerController.h"

AMenu_GameMode::AMenu_GameMode()
{
	PlayerControllerClass = APlayerController::StaticClass(); // Associate the Cpc as the Player Controller for to this GameMode
	DefaultPawnClass = AMenu_Player::StaticClass(); // Associate the HumaPlayer as the default pawn class for this GameMode
}

void AMenu_GameMode::BeginPlay()
{
	Super::BeginPlay();

	AMenu_Player* HumanPlayer = Cast<AMenu_Player>(*TActorIterator<AMenu_Player>(GetWorld()));
	
	const FVector CameraPos(0.0f, 0.0f, 1000.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());
}
