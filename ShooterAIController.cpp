// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if(AIBehavior != nullptr) //행동트리가 있는지 확인
    {
        RunBehaviorTree(AIBehavior); //AIBehavior실행. BT_Enemy 연결
    }
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation()); //AI의 시작위치 저장 

}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

bool AShooterAIController::IsDead() const 
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if(ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead(); //사망함수 호출
    }

    return true;
}
 
