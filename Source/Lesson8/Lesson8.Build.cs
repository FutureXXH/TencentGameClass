// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Lesson8 : ModuleRules
{
	public Lesson8(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" , "UMG" });
	}
}
