# JUCEUnrealBridge
---------------------------------------------
#Overview
---------------------------------------------
A plugin for the Unreal engine that links the JUCE library and facilitates the use of JUCE calls from within your Unreal project. The plugin also includes some Unreal wrappers for JUCE functionality, including a simple synthesiser and metronome that can be integrated into the game world.

---------------------------------------------
#Requirements
---------------------------------------------
The JUCE C++ framework https://github.com/WeAreROLI/JUCE and the Unreal engine https://www.unrealengine.com/what-is-unreal-engine-4.

---------------------------------------------
#Setup
---------------------------------------------
In your Unreal project folder, create a subdirectory called "Plugins".

Copy the top root folder of this repository and paste it into the newly created Plugins folder.

Right-click on your .uproject file in your Unreal project directory, and select "Generate Visual Studio Project files".

Open your visual studio project.

Build your solution.

You can now either run your solution from visual studio, or open your .uproject file and the editor will open with the JUCE plugin loaded.

Maintaining the latest version of JUCE
--------------------------------------
The plugin works by linking the JUCE library as an external library to the Ureal project. In the ThirdParty/ directory you will find the JUCE static library files. To maintain the latest build of JUCE you should build JUCE as a static library and copy it, along with the JUCE source files, to the relevant locations in the ThirdParty/ directory. This involves the following steps:

In your JUCE folder, browse to the windows .dll example at JUCE/extras/windows dll. Open the project in the Projucer, and build in release mode.

Find the juce_dll.lib file that is created. For example, if using Visual Studio 2015, this will be at JUCE/extras/windows dll/Builds/VisualStudio2015/x64/Release/juce_dll.lib.

Copy this file to your Unreal project plugins directory at the following location: _YourUnrealProject_/Plugins/_JUCEUnrealPluginName_/ThirdParty/JUCE/Libraries/Release. 
(_YourUnrealProject_ should be the name of some existing Unreal project. _JUCEUnrealPluginName_ is whatever name you have given to this JUCE plugin, within your Unreal project.)

Go back to your JUCE folder and copy the modules folder from JUCE/modules to ThirdParty/JUCE/Includes.

