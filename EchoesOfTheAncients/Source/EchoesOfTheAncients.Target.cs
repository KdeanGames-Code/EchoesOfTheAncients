// Copyright © 2025 Kdean Games. All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class EchoesOfTheAncientsTarget : TargetRules
{
	public EchoesOfTheAncientsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "EchoesOfTheAncients" } );
	}
}
