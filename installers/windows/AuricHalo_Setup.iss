; Auric Halo - Inno Setup Installer Script (64-bit)
; Version 1.0.0
; Installer otomatis untuk VST3, AAX, dan Standalone

#define MyAppName "Auric Halo"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Auric Audio"
#define MyAppURL "https://www.auricaudio.com"
#define MyAppExeName "AuricHalo.exe"

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
DefaultDirName={autopf64}\Auric Audio\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes

; Output
OutputDir=..\..\Release
OutputBaseFilename=AuricHalo-{#MyAppVersion}-Win64-Setup
; SetupIconFile=..\..\resources\graphics\icon.ico
Compression=lzma2/max
SolidCompression=yes

; Privileges (butuh admin untuk install ke Program Files dan Common Files)
PrivilegesRequired=admin
PrivilegesRequiredOverridesAllowed=dialog

; UI
WizardStyle=modern
; WizardImageFile=..\..\resources\graphics\installer_banner.bmp
; WizardSmallImageFile=..\..\resources\graphics\installer_icon.bmp

; License
LicenseFile=..\..\LICENSE.txt
InfoBeforeFile=..\..\README.txt

ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 6.1; Check: not IsAdminInstallMode

[Files]
; VST3 Plugin (64-bit) - Install otomatis ke folder VST3 standard
Source: "..\..\Builds\VisualStudio2022\x64\Release\VST3\AuricHalo.vst3\*"; DestDir: "{commoncf64}\VST3\AuricHalo.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: vst3

; AAX Plugin (64-bit) - Install otomatis ke Pro Tools
Source: "..\..\Builds\VisualStudio2022\x64\Release\AAX\AuricHalo.aaxplugin\*"; DestDir: "{commoncf64}\Avid\Audio\Plug-Ins\AuricHalo.aaxplugin"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: aax

; Standalone Application (64-bit)
Source: "..\..\Builds\VisualStudio2022\x64\Release\Standalone Plugin\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion; Components: standalone
Source: "..\..\Builds\VisualStudio2022\x64\Release\Standalone Plugin\*.dll"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist; Components: standalone

; Documentation
Source: "..\..\README.txt"; DestDir: "{app}"; Flags: ignoreversion isreadme
Source: "..\..\LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\CHANGELOG.txt"; DestDir: "{app}"; Flags: ignoreversion; DestName: "Version History.txt"

; Presets (if any)
Source: "..\..\resources\presets\*"; DestDir: "{userappdata}\Auric Audio\Auric Halo\Presets"; Flags: ignoreversion recursesubdirs createallsubdirs skipifsourcedoesntexist; Components: presets

[Components]
Name: "vst3"; Description: "VST3 Plugin (64-bit) - Untuk semua DAW modern"; Types: full compact custom; Flags: fixed
Name: "aax"; Description: "AAX Plugin (64-bit) - Untuk Pro Tools 2019+"; Types: full custom
Name: "standalone"; Description: "Standalone Application (64-bit)"; Types: full
Name: "presets"; Description: "Factory Presets"; Types: full

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Components: standalone
Name: "{group}\User Manual"; Filename: "{app}\README.txt"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon; Components: standalone
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon; Components: standalone

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent; Components: standalone

[UninstallDelete]
Type: filesandordirs; Name: "{userappdata}\Auric Audio\Auric Halo"

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
var
  Msg: String;
begin
  if CurStep = ssPostInstall then
  begin
    Msg := '✓ Auric Halo (64-bit) berhasil diinstall!' + #13#10 + #13#10;
    Msg := Msg + 'Plugin sudah otomatis terinstall ke:' + #13#10 + #13#10;
    
    if WizardIsComponentSelected('vst3') then
      Msg := Msg + '• VST3 (64-bit): ' + ExpandConstant('{commoncf64}\VST3\') + #13#10;
    
    if WizardIsComponentSelected('aax') then
      Msg := Msg + '• AAX (64-bit): ' + ExpandConstant('{commoncf64}\Avid\Audio\Plug-Ins\') + #13#10;
    
    if WizardIsComponentSelected('standalone') then
      Msg := Msg + '• Standalone: ' + ExpandConstant('{app}\') + #13#10;
    
    Msg := Msg + #13#10 + '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━' + #13#10;
    Msg := Msg + 'LANGKAH SELANJUTNYA:' + #13#10 + #13#10;
    
    if WizardIsComponentSelected('vst3') then
    begin
      Msg := Msg + '1. Buka DAW kamu (FL Studio, Ableton, Cubase, dll)' + #13#10;
      Msg := Msg + '2. Rescan plugin VST3' + #13#10 + #13#10;
      Msg := Msg + 'CUBASE: Plugin Manager → Rescan' + #13#10;
      Msg := Msg + 'Plugin ada di: Audio FX → Distortion' + #13#10;
    end;
    
    if WizardIsComponentSelected('aax') then
      Msg := Msg + #13#10 + 'PRO TOOLS: Setup → Plug-ins → Rescan' + #13#10;
    
    Msg := Msg + #13#10 + 'Plugin siap digunakan! 🎵';
    
    MsgBox(Msg, mbInformation, MB_OK);
  end;
end;

function InitializeSetup(): Boolean;
var
  ErrorMsg: String;
begin
  Result := True;
  
  // Check Windows 64-bit
  if not IsWin64 then
  begin
    ErrorMsg := 'ERROR: Installer ini hanya untuk Windows 64-bit!' + #13#10 + #13#10;
    ErrorMsg := ErrorMsg + 'Auric Halo membutuhkan:' + #13#10;
    ErrorMsg := ErrorMsg + '• Windows 10/11 (64-bit)' + #13#10;
    ErrorMsg := ErrorMsg + '• DAW 64-bit (FL Studio, Ableton, Pro Tools, dll)' + #13#10 + #13#10;
    ErrorMsg := ErrorMsg + 'Sistem kamu: Windows 32-bit' + #13#10;
    ErrorMsg := ErrorMsg + 'Silakan upgrade ke Windows 64-bit.';
    MsgBox(ErrorMsg, mbError, MB_OK);
    Result := False;
    Exit;
  end;
  
  // Check Windows version (minimum Windows 10)
  if GetWindowsVersion < $0A000000 then
  begin
    ErrorMsg := 'WARNING: Windows versi lama terdeteksi!' + #13#10 + #13#10;
    ErrorMsg := ErrorMsg + 'Auric Halo direkomendasikan untuk:' + #13#10;
    ErrorMsg := ErrorMsg + '• Windows 10 (64-bit) atau lebih baru' + #13#10;
    ErrorMsg := ErrorMsg + '• Windows 11 (64-bit)' + #13#10 + #13#10;
    ErrorMsg := ErrorMsg + 'Lanjutkan install?';
    Result := MsgBox(ErrorMsg, mbConfirmation, MB_YESNO) = IDYES;
  end;
end;

function InitializeUninstall(): Boolean;
var
  Msg: String;
begin
  Msg := 'Uninstall Auric Halo akan menghapus:' + #13#10 + #13#10;
  Msg := Msg + '• Plugin VST3 dan AAX' + #13#10;
  Msg := Msg + '• Aplikasi Standalone' + #13#10;
  Msg := Msg + '• Presets dan settings' + #13#10 + #13#10;
  Msg := Msg + 'Lanjutkan uninstall?';
  Result := MsgBox(Msg, mbConfirmation, MB_YESNO) = IDYES;
end;
