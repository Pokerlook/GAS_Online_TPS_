// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class GT : ModuleRules
{
	public GT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", //"GameplayAbilities", "UMG", "NetCore"
            "AdvancedSessions",
            "AdvancedSteamSessions"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "MotionTrajectory", "PoseSearch", "AIModule", "AnimGraphRuntime", "Chooser", "AnimationWarpingRuntime", "BlendStack", "MotionWarping"
        , "AnimationLocomotionLibraryRuntime"

    //        "GameplayTags", "GameplayTasks", "NavigationSystem", "Niagara"
        });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
