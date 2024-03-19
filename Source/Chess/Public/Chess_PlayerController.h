// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Chess_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AChess_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AChess_PlayerController();

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* ChessContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ClickAction;

	void ClickOnGrid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> UserInterfaceWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* UserInterfaceWidget;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
};
