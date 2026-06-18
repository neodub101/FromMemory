// Fill out your copyright notice in the Description page of Project Settings.



#include "CoreMinimal.h"
#include "FromMemoryAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "PlayerCharacter.h"



void UFromMemoryAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	CharacterRef = Cast<APlayerCharacter>(TryGetPawnOwner());
	OwnerPawn = TryGetPawnOwner();
	 
	if (OwnerPawn)
	{
		// Cache the Movement Component so we don't have to find it every frame
		MovementComponent = OwnerPawn->FindComponentByClass<UCharacterMovementComponent>();
	}
}

void UFromMemoryAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	
	if (!OwnerPawn || !MovementComponent)
	{
		OwnerPawn = TryGetPawnOwner();
		if (OwnerPawn)
		{
			MovementComponent = OwnerPawn->FindComponentByClass<UCharacterMovementComponent>();
		}
		return;
	}
	
	if (!OwnerPawn || !MovementComponent) return;
	
	if (!CharacterRef)
	{
		CharacterRef = Cast<APlayerCharacter>(TryGetPawnOwner());
	}
	
	FVector Velocity = CharacterRef->GetVelocity();
	FRotator Rotation = CharacterRef->GetActorRotation();
	
 	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Rotation);
	
	// Also update Speed for the BlendSpace
	Speed = Velocity.Size();
	
	// Get walking value for animation - calling Walking State
	bIsWalking = CharacterRef->GetIsWalking();
	
	
	
	bIsCrouching = CharacterRef->GetIsCrouching();
	/*if (bIsCrouching == true)
	{
		CrouchSpeed = Velocity.Size();
	}*/
	
}
