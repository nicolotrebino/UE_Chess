// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_GameInstance.h"
#include "Pieces/Chess_Piece.h"
#include "PlayerInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "HumanPlayer.generated.h"

/**
 * Human player class
 */
UCLASS()
class CHESS_API AHumanPlayer : public APawn, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHumanPlayer(); 

	UCameraComponent* Camera; // Camera component attached to player pawn
	UChess_GameInstance* GameInstance; // Chess_GameInstance reference

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	 *	Methods inherited from PlayerInterface and
	 *	overridden for the HumanPlayer
	 */
	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;
	virtual void OnDraw() override;

	// Called on left mouse click (binding)
	UFUNCTION()
	void OnClick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsMyTurn = false; // Keeps track of my turn
	
	AChess_Piece* SelectedPiece; // Current piece selected

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
