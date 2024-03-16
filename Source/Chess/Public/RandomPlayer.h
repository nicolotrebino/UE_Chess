// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_GameInstance.h"
#include "PlayerInterface.h"
#include "GameFramework/Pawn.h"
#include "RandomPlayer.generated.h"

UCLASS()
class CHESS_API ARandomPlayer : public APawn, public IPlayerInterface
{
	GENERATED_BODY()

public:

	ARandomPlayer(); // Sets default values for this pawn's properties
	
	UChess_GameInstance* GameInstance; // Chess_GameInstance reference
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
