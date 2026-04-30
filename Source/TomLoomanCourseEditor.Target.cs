// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TomLoomanCourseEditorTarget : TargetRules
{
	public TomLoomanCourseEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		ExtraModuleNames.Add("TomLoomanCourse");
	}
}
