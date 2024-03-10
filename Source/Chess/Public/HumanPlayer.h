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

	// camera component attached to player pawn
	UCameraComponent* Camera;

	// Game instance reference
	UChess_GameInstance* GameInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Keeps track of turn
	bool IsMyTurn = false;

	// Current piece selected
	AChess_Piece* SelectedPiece;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

	// called on left mouse click (binding)
	UFUNCTION()
	void OnClick();
};
