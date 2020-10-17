// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GrilledPigeon : ModuleRules
{
    public GrilledPigeon(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[]
            {"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "GameplayTasks"});
    }
}