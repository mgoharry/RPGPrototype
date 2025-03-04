// Fill out your copyright notice in the Description page of Project Settings.


#include "Killable.h"

// Add default functionality here for any IStealthable functions that are not pure virtual.

void IKillable::StealthKilled(FVector& outREFLocation, FRotator& outRotation)
{
}

void IKillable::ForceChoked(FVector Location, FVector PlayerLocation)
{
}
