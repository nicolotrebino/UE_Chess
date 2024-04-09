// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu_Player.h"

// Sets default values
AMenu_Player::AMenu_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); // Create a camera component
	SetRootComponent(Camera); // Set the camera as RootComponent
}

// Called when the game starts or when spawned
void AMenu_Player::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMenu_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMenu_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

