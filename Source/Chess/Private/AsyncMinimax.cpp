// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncMinimax.h"

#include "Chess_GameMode.h"

AsyncMinimax::AsyncMinimax(UWorld* InWorld)
	: World(InWorld)
{
}

AsyncMinimax::~AsyncMinimax()
{
}

void AsyncMinimax::Exit()
{
	FRunnable::Exit();
}

bool AsyncMinimax::Init()
{
	bStop = false;
	return true;
}

uint32 AsyncMinimax::Run()
{
	if (!bStop)
	{
		AChess_GameMode* GameMode = Cast<AChess_GameMode>(World->GetAuthGameMode());

		GameMode->Players[BLACK]->OnTurn();
		Stop();
	}
	return 0;
}

void AsyncMinimax::Stop()
{
	bStop = true;
}
