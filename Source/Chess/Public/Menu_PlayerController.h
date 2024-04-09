// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Menu_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API AMenu_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMenu_PlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Menu")
	UUserWidget* MenuWidget;

protected:
	virtual void BeginPlay() override;
	
};
