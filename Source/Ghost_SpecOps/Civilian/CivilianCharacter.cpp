// Fill out your copyright notice in the Description page of Project Settings.


#include "CivilianCharacter.h"
#include "Components/StateTreeComponent.h"
#include "Ghost_SpecOps/Enemy/EnemyCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

ACivilianCharacter::ACivilianCharacter() :
	bIsFrightened(false),
	bIsEssential(false),
	bUseComputer(true),
	ScheduleTimerRate(15.f),
	MoveRadius(1000.f),
	SearchRadius(3000.f)
{
	// Components
	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("State Tree"));
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));

	// Delegate Binding
	OnTakeAnyDamage.AddDynamic(this, &ACivilianCharacter::TakeDamage);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACivilianCharacter::ProcessStimuli);
}

void ACivilianCharacter::BeginPlay()
{
	if(HasAuthority())
	{
		FTimerHandle StartLogicHandle;
		GetWorldTimerManager().SetTimer(StartLogicHandle, this, &ACivilianCharacter::StartStateTree, 5.f, false);
		GetWorldTimerManager().SetTimer(ScheduleTimer, this, &ACivilianCharacter::SwitchSchedule, ScheduleTimerRate, true);
	}
	Super::BeginPlay();
}

void ACivilianCharacter::SwitchSchedule()
{
	bUseComputer = !bUseComputer;
}

void ACivilianCharacter::StartStateTree() const
{
	StateTreeComponent->StartLogic();
}

void ACivilianCharacter::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	bIsAlive = false;
	const FGameplayTag Tag = Tag.RequestGameplayTag("Dead");
	StateTreeComponent->SendStateTreeEvent(FStateTreeEvent(Tag));
}

void ACivilianCharacter::ProcessStimuli(AActor* Actor, FAIStimulus Stimulus)
{
	// Handle Vision
	if(Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		// Frighten on seeing corpse
		if(Actor->IsA<AEnemyCharacter>() || Actor->IsA<ACivilianCharacter>())
		{
			if(Actor->IsA<AEnemyCharacter>())
			{
				AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor);
				if(Enemy->IsDead())
				{
					bIsFrightened = true;
					FGameplayTag Tag = Tag.RequestGameplayTag("Flee");
					StateTreeComponent->SendStateTreeEvent(FStateTreeEvent(Tag));
				}
			}
			else
			{
				ACivilianCharacter* Civilian = Cast<ACivilianCharacter>(Actor);
				if(Civilian->IsDead())
				{
					bIsFrightened = true;
					FGameplayTag Tag = Tag.RequestGameplayTag("Flee");
					StateTreeComponent->SendStateTreeEvent(FStateTreeEvent(Tag));
				}
			}
		}
	}
	// Handle Hearing
	else if(Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		// Frighten on hearing gunfire
		bIsFrightened = true;
		FGameplayTag Tag = Tag.RequestGameplayTag("Flee");
		StateTreeComponent->SendStateTreeEvent(FStateTreeEvent(Tag));
	}
}

