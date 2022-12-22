#pragma once
#include "CoreMinimal.h"
#include "Ghost_SpecOps/Weapon/Weapon.h"
#include "GameFramework/Character.h"
#include "Ghost_SpecOps/Types/TurningInPlace.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UPlayerCombatComponent;
class UAIPerceptionStimuliSourceComponent;

UCLASS()
class GHOST_SPECOPS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void PlayFireMontage(bool bInAiming) const;

	UAIPerceptionStimuliSourceComponent* AIPerception;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DEFAULTS, Replicated)
	bool bIsProne;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DEFAULTS, Replicated)
	bool bIsAiming;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DEFAULTS, Replicated)
	bool bIsADS;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DEFAULTS, Replicated)
	bool bIsStanding;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DEFAULTS, Replicated)
	bool bIsRunning;

	// UPROPERTY(BlueprintReadWrite, Category = DEFAULTS, Replicated, VisibleAnywhere)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DEFAULTS, Replicated)
	bool bIsAlive;

protected:
	virtual void BeginPlay() override;

	//---------------------------------- Character Weapon settings ---------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TSubclassOf<AWeapon> StartingWeaponClass;
	
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	FName WeaponSocketName;

	UPROPERTY(BlueprintReadOnly)
	AWeapon* CurrentWeapon;
	
	UPROPERTY(VisibleAnywhere)
	UPlayerCombatComponent* CombatComponent;

	//---------------------------------------------------------------------------------------------------------

	float Speed;
	
	float BaseWalkSpeed;
	float AimWalkSpeed;
	float RunSpeed;
	float ProneSpeed;

	UPROPERTY(BlueprintReadOnly)
	float AO_Yaw;
	
	UPROPERTY(BlueprintReadOnly)
	float AO_Pitch;

	float InterpAO_Yaw;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* FireWeaponMontage;

public:
	FORCEINLINE UPlayerCombatComponent* GetCombatComponent() const { return CombatComponent; }
	FORCEINLINE AWeapon* GetWeapon() const { return CurrentWeapon; }
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace;}
	FVector GetHitTarget() const; 

};
