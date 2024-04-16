// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Thread.h"

#include "Chess_GameMode.h"

Manager_Thread::Manager_Thread(UWorld* InWorld)
	: World(InWorld)
{
	Thread = FRunnableThread::Create(this, TEXT("EnemyThread"));
	bStop = true;
}

Manager_Thread::~Manager_Thread()
{
	// Imposta il flag di stop
	bStop = true;

	// Attendi che il thread termini naturalmente
	if (Thread)
	{
		Thread->WaitForCompletion();
		delete Thread;
		Thread = nullptr; // Imposta il puntatore a null dopo la cancellazione
	}
}

void Manager_Thread::Exit()
{
	FRunnable::Exit();
}

bool Manager_Thread::Init()
{
	bStop = false;
	return true;
}

uint32 Manager_Thread::Run()
{
	if (!bStop)
	{
		if (World)
		{
			AChess_GameMode* GameMode = Cast<AChess_GameMode>(World->GetAuthGameMode());

			GameMode->Players[BLACK]->ComputeMove();
		}
	}
	return 0;
}

void Manager_Thread::Stop()
{
	bStop = true;
}
