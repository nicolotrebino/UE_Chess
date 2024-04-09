// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Menu_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AMenu_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AMenu_GameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
