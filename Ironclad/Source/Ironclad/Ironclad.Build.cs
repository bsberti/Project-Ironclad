// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ironclad : ModuleRules
{
	public Ironclad(ReadOnlyTargetRules Target) : base(Target)
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
			"GameplayTags"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
            "Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
            "Ironclad",
			"Ironclad/Variant_Platforming",
			"Ironclad/Variant_Platforming/Animation",
			"Ironclad/Variant_Combat",
			"Ironclad/Variant_Combat/AI",
			"Ironclad/Variant_Combat/Animation",
			"Ironclad/Variant_Combat/Gameplay",
			"Ironclad/Variant_Combat/Interfaces",
			"Ironclad/Variant_Combat/UI",
			"Ironclad/Variant_SideScrolling",
			"Ironclad/Variant_SideScrolling/AI",
			"Ironclad/Variant_SideScrolling/Gameplay",
			"Ironclad/Variant_SideScrolling/Interfaces",
			"Ironclad/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
