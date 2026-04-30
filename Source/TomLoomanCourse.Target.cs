// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TomLoomanCourseTarget : TargetRules
{
	public TomLoomanCourseTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		ExtraModuleNames.Add("TomLoomanCourse");
	}
}
