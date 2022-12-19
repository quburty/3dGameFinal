// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Portfolio_C : ModuleRules
{
	public Portfolio_C(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayTasks" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayTasks", "ApexDestruction" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PrivateIncludePaths.Add(ModuleDirectory);
        PublicIncludePaths.Add(ModuleDirectory);

        MinFilesUsingPrecompiledHeaderOverride = 1;
        //bFasterWithoutUnity = true;
        //To Use this option, go to AppData/Roaming/Unreal Engine/UnrealBuildTool/BuildConfiguration.xml
        //and set bUseUnityBuild to false.

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
