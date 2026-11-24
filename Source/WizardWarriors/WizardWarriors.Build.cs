// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WizardWarriors : ModuleRules
{
	public WizardWarriors(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
            "Niagara"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"WizardWarriors",
			"WizardWarriors/Variant_Platforming",
			"WizardWarriors/Variant_Platforming/Animation",
			"WizardWarriors/Variant_Combat",
			"WizardWarriors/Variant_Combat/AI",
			"WizardWarriors/Variant_Combat/Animation",
			"WizardWarriors/Variant_Combat/Gameplay",
			"WizardWarriors/Variant_Combat/Interfaces",
			"WizardWarriors/Variant_Combat/UI",
			"WizardWarriors/Variant_SideScrolling",
			"WizardWarriors/Variant_SideScrolling/AI",
			"WizardWarriors/Variant_SideScrolling/Gameplay",
			"WizardWarriors/Variant_SideScrolling/Interfaces",
			"WizardWarriors/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
