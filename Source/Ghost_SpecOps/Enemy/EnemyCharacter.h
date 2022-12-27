// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter/BaseCharacter.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GHOST_SPECOPS_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AEnemyCharacter();

private:

	virtual void BeginPlay() override;

	/** Starts the State Tree Logic */
	void StartStateTree() const;

	/** Taking damage */
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	/** Process incoming Perception Stimuli */
	UFUNCTION()
	void ProcessStimuli(AActor* Actor, FAIStimulus Stimulus);

protected:
	
	/** Path for the enemy to follow (or guard location) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APatrolPath* PatrolPath;

	/** Current index in patrol path */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PatrolIndex;

	/** Enemy sees player or remembers seeing them */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanSeePlayer;

	/** Used for death state */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsDead;

	/** Used for flee state */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bShouldRetreat;

	/** Used for investigate state */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAlert;

	/** For chasing player */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector TargetLocation;

	/** Current health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	/** Direction of patrol. Only really useful when patrol doesn't cycle */
	/** True - Forwards, False - Backwards */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PatrolDirection;

	/** Death animation montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* DeathMontage;

	/** State Tree Logic */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStateTreeComponent* StateTreeComponent;

	/** AI Perception Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAIPerceptionComponent* PerceptionComponent;

	/** AI Perception Stimuli Source Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;

public:

	/** Returns the patrol path */
	APatrolPath* GetPatrolPath() const { return PatrolPath; }

	/** Returns the patrol index */
	int32 GetPatrolIndex() const { return PatrolIndex; }

	/** Updates patrol index to the next point in sequence */
	void UpdatePatrolIndex();
	
};
