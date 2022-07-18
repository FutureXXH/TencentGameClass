// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Lesson_1 : ModuleRules
{
	public Lesson_1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
