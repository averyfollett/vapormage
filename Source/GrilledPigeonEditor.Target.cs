// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GrilledPigeonEditorTarget : TargetRules
{
    public GrilledPigeonEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("GrilledPigeon");
    }
}