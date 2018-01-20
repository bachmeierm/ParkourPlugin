using UnrealBuildTool;
using System.Collections.Generic;

public class ParkourProjectEditorTarget : TargetRules
{
	public ParkourProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "ParkourProject" } );
	}
}
