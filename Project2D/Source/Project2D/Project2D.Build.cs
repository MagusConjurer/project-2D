// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project2D : ModuleRules
{
	public Project2D(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
