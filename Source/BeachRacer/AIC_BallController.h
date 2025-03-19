// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"
#include "AIC_BallController.generated.h"

/**
 * 
 */
UCLASS()
class BEACHRACER_API AAIC_BallController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "AI")
	TArray<ATargetPoint*> WayPoints;
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SortWayPoints();
	UFUNCTION(BlueprintCallable, Category = "AI")
	void AdjustSpeedBasedOnCurve(ACharacter* AICharacter, FVector CurrentDirection, FVector NextWaypointDirection);
	UFUNCTION(BlueprintCallable, Category = "AI")
	FVector GetAdjustedDirection(FVector TargetDirection);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void CheckNavMeshProximity();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxSpeed = 700.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MinSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DeviationStrenght= 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SafetyMargin = 100.0f;
};
