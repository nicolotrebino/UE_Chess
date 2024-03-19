// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager_Turn.generated.h"

class AChess_Piece;
class ATile;

USTRUCT()
struct FHistoryButton
{
	GENERATED_BODY()

	UUserWidget* Button;
	AChess_Piece* MovedPiece;
	AChess_Piece* KilledPiece;
	ATile* PreviousTile;
	ATile* NextTile;
};

UCLASS()
class CHESS_API AManager_Turn : public AActor
{
	GENERATED_BODY()

	/**
	* Class methods and attributes
	*/
public:
	// Sets default values for this actor's properties
	AManager_Turn();
	void SelfDestroy();

	/* Methods */
	void ResetVariables();

	/* Game variable for a turn */
	AChess_Piece* Checker; // Piece that is in check

	AChess_Piece* MovedPiece;
	AChess_Piece* KilledPiece;
	
	bool bIsKill; // If in that turn there was a kill
	bool bIsPromotion; // If in that turn there was a promotion
	bool bIsBlackKingInCheck;
	bool bIsWhiteKingInCheck;

	ATile* PreviousTile;
	ATile* NextTile;

	/* Move History managing */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ButtonClass;

	void DisplayMove();
	UFUNCTION(BlueprintCallable)
	FString ComputeNotation() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
