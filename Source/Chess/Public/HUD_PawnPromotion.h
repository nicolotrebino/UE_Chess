// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_Pawn.h"
#include "Blueprint/UserWidget.h"
#include "HUD_PawnPromotion.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UHUD_PawnPromotion : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetCurrentPawn(AChess_Pawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = "Function")
	AChess_Pawn* GetPawn() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Current Pawn")
	AChess_Pawn* CurrentPawn = nullptr;
};
