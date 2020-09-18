using UnrealBuildTool;
using System.IO;

public class GPA : ModuleRules
{
	public GPA(ReadOnlyTargetRules Target) : base(Target)
	{
        bool bApplyUE4EngineVersoin = false;

        BuildVersion Version = BuildVersion.ReadDefault();
        bApplyUE4EngineVersoin = (Version.MajorVersion == 4 &&
                                     (Version.MinorVersion >= 21 && Version.MinorVersion <= 25));

        if( bApplyUE4EngineVersoin == false )
        {
            throw new BuildException("GPA - Not supported UE4 version.");
        }

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
		new string[]
		{
            "Core",
            "CoreUObject",
            "Engine",
            "Projects",
            "RHI"
        });

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDependencyModuleNames.Add("SlateCore");

            string libPath = Path.Combine(ModuleDirectory, "lib", "x86_64");
            PublicIncludePaths.Add(libPath);

            PublicAdditionalLibraries.Add(Path.Combine(libPath, "NCGPA.lib"));
            PublicDelayLoadDLLs.Add("NCGPA.dll");

            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(libPath, "NCGPA.dll")));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            PublicDependencyModuleNames.Add("SlateCore");
            
            string libPath = Path.Combine(ModuleDirectory, "lib", "x86");
            PublicIncludePaths.Add(libPath);

            PublicAdditionalLibraries.Add(Path.Combine(libPath, "NCGPA.lib"));
            PublicDelayLoadDLLs.Add("NCGPA.dll");

            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(libPath, "NCGPA.dll")));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "GPALibrary_APL.xml"));
        }
    }
}
