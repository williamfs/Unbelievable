// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraShake.h"

UMyCameraShake::UMyCameraShake()
{
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(1.0f, 2.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(5.0f, 10.0f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(0.2f, 0.4f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(1.0f, 2.0f);
}