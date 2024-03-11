// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_GameInstance.h"
#include "Chess_Piece.h"
#include "PlayerInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "HumanPlayer.generated.h"

UCLASS()
class CHESS_API AHumanPlayer : public APawn, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHumanPlayer();

	UCameraComponent* Camera; // Camera component attached to player pawn
	UChess_GameInstance* GameInstance; // Game instance reference

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsMyTurn = false; // Keeps track of my turn
	AChess_Piece* SelectedPiece; // Current piece selected

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

	// Called on left mouse click (binding)
	UFUNCTION()
	void OnClick();
};
