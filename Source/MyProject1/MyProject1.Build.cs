// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProject1 : ModuleRules
{
	public MyProject1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" , "UMG","OnlineSubsystem", "OnlinesubsystemSteam"});		//UMG : �𸮾� �׷��� ���̺귯��
	}
}
