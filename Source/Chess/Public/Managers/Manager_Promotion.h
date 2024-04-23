// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager_Promotion.generated.h"

class AChess_Pawn;

/**
 *	Class used as manager for the pawn promotion
 */
UCLASS()
class CHESS_API AManager_Promotion : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AManager_Promotion();
	void SelfDestroy();
	
	/*
	 * Manage pawn promotion
	 */
	void StartPromotion();
	UFUNCTION(BlueprintCallable, Category = "Promotion")
	void HandleButtonClicked(int32 SelectedPieceIndex); // Called when a button in the widget is clicked

	/*
	 * Setter and getter for the pawn to promote
	 */
	void SetCurrentPawn(AChess_Pawn* Pawn);
	UFUNCTION(BlueprintCallable, Category = "Function")
	AChess_Pawn* GetPawn() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	 *	Promotion widget
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Promotion")
	TSubclassOf<UUserWidget> PromotionWidgetClass;
	UPROPERTY(BlueprintReadOnly, Category = "Promotion")
	UUserWidget* PromotionWidget;

	// Pawn to promote
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Current Pawn")
	AChess_Pawn* CurrentPawn = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
