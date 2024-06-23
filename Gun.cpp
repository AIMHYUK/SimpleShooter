// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"




// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;
	//SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalComponent"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket")); //총구에서 임팩트
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket")); //총구에서 사운드
	
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);

	if(bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFlash, Hit.Location, ShotDirection.Rotation()); //히트임팩트
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location); //히트사운드

		AActor* HitActor = Hit.GetActor(); 
		if(HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr); //데미지 이벤트 발생
			AController* OwnerController = GetOwnerController();
			Hit.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{	
	AController* OwnerController = GetOwnerController();
	if(OwnerController == nullptr)
	{
		return false;
	}

	FVector Location;
	FRotator Rotation;

	OwnerController->GetPlayerViewPoint(Location,Rotation);
	ShotDirection = -Rotation.Vector(); //총알이 어디에서 왔는지, 변수로 사용

	FVector End = Location + Rotation.Vector() * MaxRange; //회전 방향을 가리키는 벡터
	
	//라인트레이스
	Hit; //변수로 사용
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); //총알이 총에 안맞게
	Params.AddIgnoredActor(GetOwner()); //총 주인이 총에 안맞게
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner()); //총으로부터 ShooterCharacter 가져오기, 범위 크게 Pawn으로 캐스팅
	if(OwnerPawn == nullptr) 
		return nullptr;
	return OwnerPawn->GetController();
}

