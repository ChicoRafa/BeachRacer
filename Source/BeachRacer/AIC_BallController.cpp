// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_BallController.h"

#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void AAIC_BallController::SortWayPoints()
{
	WayPoints.Sort([this](const ATargetPoint& A, const ATargetPoint& B)
	{
		return A.GetName() < B.GetName();
	});
}

void AAIC_BallController::AdjustSpeedBasedOnCurve(ACharacter* AICharacter, FVector CurrentDirection, FVector NextWaypointDirection)
{
	if (!AICharacter)
		return;
	
	UCharacterMovementComponent* CharacterMovementComponent = AICharacter->GetCharacterMovement();
	if (!CharacterMovementComponent)
		return;

	if (CurrentDirection.IsNearlyZero() || NextWaypointDirection.IsNearlyZero())
		return;
	
	float DotProduct = FVector::DotProduct(CurrentDirection.GetSafeNormal(), NextWaypointDirection.GetSafeNormal());
	DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);
	float Angle = FMath::Acos(DotProduct) * (180.0f / PI);

	if (Angle < 20.0f)
	{
		CharacterMovementComponent->MaxWalkSpeed = MaxSpeed;
	}
	else
	{
 		CharacterMovementComponent->MaxWalkSpeed = MinSpeed;
	}
	
}

FVector AAIC_BallController::GetAdjustedDirection(FVector TargetDirection)
{
	if (TargetDirection.IsNearlyZero())
		return TargetDirection;

	float RandomOffsetX = FMath::RandRange(-DeviationStrenght, DeviationStrenght);
	float RandomOffsetY = FMath::RandRange(-DeviationStrenght, DeviationStrenght);

	FVector Offset(RandomOffsetX, RandomOffsetY, 0.0f);

	FVector AdjustedDirection = (TargetDirection + Offset);

	return AdjustedDirection;
}


