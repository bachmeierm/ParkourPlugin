using UnrealBuildTool;
using System.Collections.Generic;

public class ParkourProjectTarget : TargetRules
{
	public ParkourProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "ParkourProject" } );
	}
}
