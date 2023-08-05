// Fill out your copyright notice in the Description page of Project Settings.

//若打包插件，此处需详细指定引用文件；可以从UE官方手册查询。
#include "PushCubeLogic.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UPushCubeLogic::UPushCubeLogic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Force = 500.0f;
}


// Called when the game starts
void UPushCubeLogic::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPushCubeLogic::StartPushing(bool IsSpawnCube)
{
	// 检查此组件是否有效
	if (GetOwner())
	{
		//射线的起点和终点
		FVector Start = GetOwner()->GetActorLocation();
		FVector End = Start + (GetOwner()->GetActorForwardVector() * 300.0f);

		if (IsSpawnCube && SpawnedCube)
		{
			// 生成Cube
			GetWorld()->SpawnActor<AActor>(SpawnedCube, End, FRotator(0, 0, 0));
		}

		//发出射线，施加力
		FHitResult Hit;
		FCollisionQueryParams QParams;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QParams);

		//获取射线检测到的物体，获取其Static Mesh
		if (bHit)
		{
			//获取碰撞到的物体静态组件并获取该组件是否有移动属性
			UStaticMeshComponent* SMComp = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());//此处只检测根组件
			bool CubeMovable = Hit.GetActor()->IsRootComponentMovable();
			
			if (CubeMovable && SMComp)
			{
				//显示测试线
				DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.5f);

				//施加力
				SMComp->AddImpulse(GetOwner()->GetActorForwardVector() * SMComp->GetMass() * Force);
			}

		}
	}
}


