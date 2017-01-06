using System.IO;
using UnrealBuildTool;

public class JUCEUnrealBridge : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }
 
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath (Path.Combine (ModulePath, "../../ThirdParty/" ) ); }
    }

	public JUCEUnrealBridge (TargetInfo target)
	{
        bUseRTTI = true;
        UEBuildConfiguration.bForceEnableExceptions = true;
		PrivateIncludePaths.AddRange (new string[] {"JUCEUnrealBridge/Private"});
        PublicIncludePaths.AddRange  (new string[] {"JUCEUnrealBridge/Public"});

		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Engine",
				"InputCore",
				"Core",
				"Slate",
				"SlateCore",
				"EditorStyle",
				"CoreUObject"
			}
		);

        LoadJUCE (target);
	}

    public bool LoadJUCE (TargetInfo Target)
    {
        bool isLibrarySupported = false;
 
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;
 
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine (ThirdPartyPath, "JUCE", "Libraries");
            string BuildType = "Release";

            /*
            test your path with:
            using System; // Console.WriteLine("");
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
            */

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, BuildType, "juce_dll.lib")); 
        }
 
        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add (Path.Combine (ThirdPartyPath, "JUCE", "Includes/modules" ) );
        }
 
        Definitions.Add (string.Format ("WITH_JUCE_BINDING={0}", isLibrarySupported ? 1 : 0 ) );
 
        return isLibrarySupported;
    }
}