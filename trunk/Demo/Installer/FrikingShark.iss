; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=Friking Shark 3D
AppVerName=Friking Shark 3D v0.9.1.0
AppPublisher=Javier Martin Garcia
AppPublisherURL=http://kyanaprime.blogspot.com/
AppSupportURL=http://kyanaprime.blogspot.com/
AppUpdatesURL=http://kyanaprime.blogspot.com/
DefaultDirName={pf}\Friking Shark 3D
DefaultGroupName=Friking Shark 3D
OutputDir=..\Installer\Output
LicenseFile=..\..\License.txt
OutputBaseFilename=FrikingSharkSetup
SetupIconFile=..\..\GameEngine\res\GameEngine.ico
Compression=lzma
SolidCompression=true
WindowVisible=false

[Languages]
Name: english; MessagesFile: "compiler:Default.isl"; InfoAfterFile:..\Docs\Readme-en.txt
Name: spanish; MessagesFile: "compiler:Languages\Spanish.isl"; InfoAfterFile:..\Docs\Readme-es.txt

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: ..\..\License.txt; DestDir: {app}; Flags: ignoreversion;
Source: ..\Bin\Release\GameEngine.exe; DestDir: {app}\Bin\Release; Flags: ignoreversion;
Source: ..\Bin\Release\*.dll; DestDir: {app}\Bin\Release; Flags: ignoreversion; 
Source: ..\Resources\GUI\*.jpg; DestDir: {app}\Resources\GUI; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Resources\ScenarioEditor\*; DestDir: {app}\Resources\ScenarioEditor; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Resources\FormationEditor\*; DestDir: {app}\Resources\FormationEditor; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Resources\EntityEditor\*; DestDir: {app}\Resources\EntityEditor; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Resources\Scripts\*.cfg; DestDir: {app}\Resources\Scripts; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Resources\Shaders\*.c; DestDir: {app}\Resources\Shaders; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Resources\Sounds\*; DestDir: {app}\Resources\Sounds; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Resources\Textures\*.jpg; DestDir: {app}\Resources\Textures; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Resources\Textures\*.bmp; DestDir: {app}\Resources\Textures; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Resources\Models\*.ase; DestDir: {app}\Resources\Models; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Resources\Models\*.gbs; DestDir: {app}\Resources\Models; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Resources\Level*.ges; DestDir: {app}\Resources; Flags: ignoreversion recursesubdirs createallsubdirs
Source: .\Redistributables\Windows\alut.dll; DestDir: {app}\Bin\Release; Flags: ignoreversion recursesubdirs createallsubdirs; 
Source: .\Redistributables\Windows\oalinst.exe; DestDir: {app}\Redistributables; Flags: ignoreversion recursesubdirs createallsubdirs
Source: .\Redistributables\Windows\msvcp90.dll; DestDir: {sys}; Flags: comparetimestamp onlyifdoesntexist 32bit
Source: .\Redistributables\Windows\msvcr90.dll; DestDir: {sys}; Flags: comparetimestamp onlyifdoesntexist 32bit

[Icons]
Name: {group}\Friking Shark 3D; Filename: {app}\Bin\Release\GameEngine.exe; WorkingDir: {app}; IconIndex: 0
Name: "{group}\Scenario Editor"; Filename: {app}\Bin\Release\GameEngine.exe; WorkingDir: {app}; Parameters: ScenarioEditor; 
Name: "{group}\Formation Editor"; Filename: {app}\Bin\Release\GameEngine.exe; WorkingDir: {app}; Parameters: FormationEditor; 
Name: "{group}\Entity Editor"; Filename: {app}\Bin\Release\GameEngine.exe; WorkingDir: {app}; Parameters: EntityEditor; 
Name: {userdesktop}\Friking Shark 3D; Filename: {app}\Bin\Release\GameEngine.exe; Tasks: desktopicon; WorkingDir: {app}; IconIndex: 0
Name: {group}\Blog; Filename: http://kyanaprime.blogspot.com/
Name: "{group}\Source Code"; Filename: http://code.google.com/p/friking-shark/

[Run]
Filename: {app}\Redistributables\oalinst.exe; Description: "{cm:LaunchProgram,OpenAL Redistributables}"; 
Filename: {app}\Bin\Release\GameEngine.exe; Description: "{cm:LaunchProgram,Friking Shark 3D}"; Flags: nowait postinstall skipifsilent; 

