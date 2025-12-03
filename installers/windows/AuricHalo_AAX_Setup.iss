; Auric Halo - AAX (Pro Tools) Installer
; Version 1.0.0

#define MyAppName "Auric Halo"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Auric Audio"
#define MyAppURL "https://www.auricaudio.com"

[Setup]
; Basic Info
AppId={{8F9A2B3C-4D5E-6F7A-8B9C-0D1E2F3A4B5C}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}

; Installation Paths
DefaultDirName={autopf}\Auric Audio\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes

; Output
OutputDir=..\..\Release
OutputBaseFilename=AuricHalo-{#MyAppVersion}-AAX-Setup
SetupIconFile=..\..\resources\graphics\icon.ico
Compression=lzma2/max
SolidCompression=yes

; Privileges (REQUIRED for AAX installation)
PrivilegesRequired=admin
PrivilegesRequiredOverridesAllowed=none

; UI
WizardStyle=modern
WizardImageFile=..\..\resources\graphics\installer_banner.bmp
WizardSmallImageFile=..\..\resources\graphics\installer_icon.bmp

; License
LicenseFile=..\..\LICENSE.txt
InfoBeforeFile=..\..\README.txt

; Misc
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "protools"; Description: "Install AAX plugin for Pro Tools"; GroupDescription: "Plugin Formats:"; Flags: fixed

[Files]
; AAX Plugin for Pro Tools
Source: "..\..\Builds\VisualStudio2022\x64\Release\AAX\AuricHalo.aaxplugin\*"; DestDir: "{commoncf64}\Avid\Audio\Plug-Ins\AuricHalo.aaxplugin"; Flags: ignoreversion recursesubdirs createallsubdirs; Tasks: protools

; Documentation
Source: "..\..\README.txt"; DestDir: "{app}"; Flags: ignoreversion isreadme
Source: "..\..\LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\CHANGELOG.txt"; DestDir: "{app}"; Flags: ignoreversion; DestName: "Version History.txt"

; Presets
Source: "..\..\resources\presets\*"; DestDir: "{userappdata}\Auric Audio\Auric Halo\Presets"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\User Manual"; Filename: "{app}\README.txt"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[UninstallDelete]
Type: filesandordirs; Name: "{userappdata}\Auric Audio\Auric Halo"

[Code]
var
  ProToolsFoundPage: TOutputMsgWizardPage;
  ProToolsInstalled: Boolean;

function CheckProToolsInstalled(): Boolean;
var
  ProToolsPath: String;
begin
  Result := False;
  
  // Check common Pro Tools installation paths
  if DirExists('C:\Program Files\Avid\Pro Tools') then
    Result := True
  else if DirExists('C:\Program Files (x86)\Avid\Pro Tools') then
    Result := True
  else if DirExists(ExpandConstant('{commoncf64}\Avid\Audio\Plug-Ins')) then
    Result := True;
end;

procedure InitializeWizard();
begin
  ProToolsInstalled := CheckProToolsInstalled();
  
  if not ProToolsInstalled then
  begin
    ProToolsFoundPage := CreateOutputMsgPage(wpWelcome,
      'Pro Tools Not Detected',
      'Pro Tools installation not found',
      'This installer will install the AAX plugin for Pro Tools.' + #13#10 + #13#10 +
      'Pro Tools was not detected on your system. You can still continue with the installation, ' +
      'but you will need to install Pro Tools to use this plugin.' + #13#10 + #13#10 +
      'The plugin will be installed to:' + #13#10 +
      ExpandConstant('{commoncf64}\Avid\Audio\Plug-Ins\'));
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  AAXPath: String;
begin
  if CurStep = ssPostInstall then
  begin
    AAXPath := ExpandConstant('{commoncf64}\Avid\Audio\Plug-Ins\AuricHalo.aaxplugin');
    
    if ProToolsInstalled then
    begin
      MsgBox('Auric Halo AAX plugin has been installed successfully!' + #13#10 + #13#10 + 
             'Plugin location:' + #13#10 +
             AAXPath + #13#10 + #13#10 +
             'Please launch Pro Tools and rescan plugins.' + #13#10 + #13#10 +
             'In Pro Tools:' + #13#10 +
             '1. Setup → Plug-ins' + #13#10 +
             '2. Click "Rescan"' + #13#10 +
             '3. Find "Auric Halo" in your plugin list', 
             mbInformation, MB_OK);
    end
    else
    begin
      MsgBox('Auric Halo AAX plugin has been installed.' + #13#10 + #13#10 + 
             'Plugin location:' + #13#10 +
             AAXPath + #13#10 + #13#10 +
             'NOTE: Pro Tools was not detected on your system.' + #13#10 +
             'Please install Pro Tools to use this plugin.' + #13#10 + #13#10 +
             'After installing Pro Tools, the plugin will be automatically detected.', 
             mbInformation, MB_OK);
    end;
  end;
end;

function InitializeSetup(): Boolean;
begin
  Result := True;
  
  // Check Windows version
  if not IsWin64 then
  begin
    MsgBox('This plugin requires 64-bit Windows.', mbError, MB_OK);
    Result := False;
    Exit;
  end;
  
  // Check if AAX build exists
  if not FileExists(ExpandConstant('{src}\..\..\Builds\VisualStudio2022\x64\Release\AAX\AuricHalo.aaxplugin\Contents\x64\AuricHalo.aaxplugin')) then
  begin
    MsgBox('AAX plugin files not found!' + #13#10 + #13#10 +
           'Please build the AAX version first:' + #13#10 +
           '1. Download AAX SDK from Avid' + #13#10 +
           '2. Configure in Projucer' + #13#10 +
           '3. Build AAX target in Visual Studio', 
           mbError, MB_OK);
    Result := False;
  end;
end;

function NextButtonClick(CurPageID: Integer): Boolean;
begin
  Result := True;
  
  if (CurPageID = wpSelectTasks) and not ProToolsInstalled then
  begin
    if MsgBox('Pro Tools was not detected on your system.' + #13#10 + #13#10 +
              'Do you want to continue with the installation?', 
              mbConfirmation, MB_YESNO) = IDNO then
      Result := False;
  end;
end;
