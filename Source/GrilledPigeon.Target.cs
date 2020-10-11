// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GrilledPigeonTarget : TargetRules
{
    public GrilledPigeonTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("GrilledPigeon");
    }
}