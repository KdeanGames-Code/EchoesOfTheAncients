// Copyright © 2025 Kdean Games. All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class EchoesOfTheAncientsEditorTarget : TargetRules
{
	public EchoesOfTheAncientsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "EchoesOfTheAncients" } );
	}
}
