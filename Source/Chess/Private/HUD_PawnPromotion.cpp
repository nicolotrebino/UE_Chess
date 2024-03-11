// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_PawnPromotion.h"

void UHUD_PawnPromotion::SetCurrentPawn(AChess_Pawn* Pawn)
{
	CurrentPawn = Pawn;
}

AChess_Pawn* UHUD_PawnPromotion::GetPawn() const
{
	return CurrentPawn;
}