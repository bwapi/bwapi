;This is an INNO Setup script
;You should download INNO Setup and INNO Script Studio v2 to modify and compile it

#define BWAPI_VERSION "4.1.2"

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "catalan"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "corsican"; MessagesFile: "compiler:Languages\Corsican.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "danish"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "greek"; MessagesFile: "compiler:Languages\Greek.isl"
Name: "hebrew"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "hungarian"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "norwegian"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "portuguese"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "serbiancyrillic"; MessagesFile: "compiler:Languages\SerbianCyrillic.isl"
Name: "serbianlatin"; MessagesFile: "compiler:Languages\SerbianLatin.isl"
Name: "slovenian"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Files]
Source: "..\Release_Binary\Starcraft\SNP_DirectIP.snp"; DestDir: "{code:BroodwarPath}\"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\BWAPI.dll"; DestDir: "{code:BroodwarPath}\bwapi-data"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\bwapi.ini"; DestDir: "{code:BroodwarPath}\bwapi-data\"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\BWAPId.dll"; DestDir: "{code:BroodwarPath}\bwapi-data\"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\data\Broodwar.map"; DestDir: "{code:BroodwarPath}\bwapi-data\data\"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\LICENSE"; DestDir: "{app}"; DestName: "LICENSE.txt"; Flags: ignoreversion
Source: "..\README.md"; DestDir: "{app}"; DestName: "README.txt"; Flags: ignoreversion isreadme
Source: "..\Release_Binary\vcredist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall
Source: "..\Release_Binary\include\*"; DestDir: "{app}\include\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Headers
Source: "..\Release_Binary\lib\*"; DestDir: "{app}\lib\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Headers
Source: "..\Release_Binary\Chaoslauncher\*"; DestDir: "{app}\Chaoslauncher\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Chaoslauncher
Source: "..\Release_Binary\MPQdraft\*"; DestDir: "{app}\MPQDraft\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: MPQDraft
Source: "..\Release_Binary\AIModuleLoader\*"; DestDir: "{app}\AIModuleLoader\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Examples
Source: "..\Release_Binary\ExampleAIClient\*"; DestDir: "{app}\ExampleAIClient\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Examples
Source: "..\Release_Binary\ExampleAIModule\*"; DestDir: "{app}\ExampleAIModule\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Examples
Source: "..\Release_Binary\ExampleTournamentModule\*"; DestDir: "{app}\ExampleTournamentModule\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Examples
Source: "..\Release_Binary\ExampleProjects.sln"; DestDir: "{app}"; Flags: ignoreversion; Components: Library\Examples
Source: "..\Release_Binary\documentation\*"; DestDir: "{app}\documentation\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Documentation
Source: "..\Release_Binary\BWAPILIB\*"; DestDir: "{app}\BWAPILIB\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Headers
Source: "..\Release_Binary\BWAPIClient\*"; DestDir: "{app}\BWAPIClient\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Headers
Source: "..\Release_Binary\Shared\*"; DestDir: "{app}\Shared\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Headers

[Setup]
AppName=BWAPI
AppId={{5025BB16-9672-4C23-979D-2DC1B276CC5E}
LicenseFile=..\LICENSE
RestartIfNeededByRun=False
AppPublisherURL=http://bwapi.github.io
AppSupportURL=http://bwapi.github.io
AppUpdatesURL=http://bwapi.github.io
VersionInfoDescription=Brood War Application Programming Interface
VersionInfoProductName=BWAPI
MinVersion=0,5.01.2600
DefaultDirName={pf}\BWAPI
UsePreviousSetupType=False
UsePreviousTasks=False
UsePreviousLanguage=False
ShowTasksTreeLines=True
AlwaysShowGroupOnReadyPage=True
AlwaysShowDirOnReadyPage=True
UsePreviousGroup=False
Uninstallable=no
FlatComponentsList=False
WizardSmallImageFile=instSmall.bmp
SetupIconFile=instIcon.ico
WizardImageFile=instLarge.bmp
DisableProgramGroupPage=yes
AppVersion={#BWAPI_VERSION}
ShowLanguageDialog=no
AlwaysShowComponentsList=False
ShowComponentSizes=False
PrivilegesRequired=poweruser
LanguageDetectionMethod=locale
VersionInfoVersion={#BWAPI_VERSION}
VersionInfoTextVersion={#BWAPI_VERSION}
VersionInfoProductVersion={#BWAPI_VERSION}
VersionInfoProductTextVersion={#BWAPI_VERSION}
OutputBaseFilename=BWAPI_Setup
InternalCompressLevel=ultra
SolidCompression=True
Compression=lzma2/ultra

[Components]
Name: "Binaries"; Description: "Binaries"
Name: "Binaries\BWAPI"; Description: "BWAPI Binaries (Requires Starcraft: Broodwar)"; Types: full custom; Check: GetBroodwarPath
Name: "Binaries\vcredist"; Description: "Microsoft Visual C++ 2013 Redistributable (x86) - 12.0.21005"; Types: custom full
Name: "Library"; Description: "Interface"; Types: custom full
Name: "Library\Headers"; Description: "Library & Header Files"; Types: custom full
Name: "Library\Examples"; Description: "Example Projects"; Types: custom full
Name: "Library\Documentation"; Description: "Documentation"; Types: custom full
Name: "Chaoslauncher"; Description: "Chaoslauncher (Plugin Loader) & Plugins"; Types: full custom
Name: "MPQDraft"; Description: "MPQDraft (Mod Manager & Plugin Loader)"; Types: custom full

[Run]
Filename: "{app}\Chaoslauncher\Chaoslauncher.exe"; WorkingDir: "{app}\Chaoslauncher\"; Flags: nowait postinstall skipifdoesntexist; Description: "Run Chaoslauncher"; Components: Chaoslauncher
Filename: "{app}\MPQdraft\MPQDraft.exe"; WorkingDir: "{app}\MPQdraft\"; Flags: nowait postinstall unchecked skipifdoesntexist; Description: "Run MPQDraft"; Components: MPQDraft
Filename: "{app}\vcredist_x86.exe"; Parameters: "-quiet"; WorkingDir: "{app}"; Description: "Visual C++ 2010 Redistributable"; Components: Binaries\vcredist

[ThirdParty]
UseRelativePaths=True

[Types]
Name: "full"; Description: "Full Installation"
Name: "custom"; Description: "Custom Installation"; Flags: iscustom

[Registry]
Root: "HKCU32"; Subkey: "Software\Battle.net\Configuration"; ValueType: multisz; ValueName: "Battle.net gateways"; ValueData: "{code:InstallGateway|sc.theabyss.ru;+7;The Abyss (ICCUP);}"; Tasks: ICCUP
Root: "HKCU32"; Subkey: "Software\Battle.net\Configuration"; ValueType: multisz; ValueName: "Battle.net gateways"; ValueData: "{code:InstallGateway|games.podolsk.ru;+8;Games Podolsk;}"; Tasks: Podolsk

[Tasks]
Name: "ICCUP"; Description: "The Abyss (ICCUP)"; GroupDescription: "Install Additional Battle.net Gateways:"
Name: "Podolsk"; Description: "Games Podolsk"; GroupDescription: "Install Additional Battle.net Gateways:"

[Code]
var sBroodwarPath : String;

// Appends one string to another
procedure StrCat(var dest: String; const src: String);
begin
  Insert(src, dest, Length(dest)+1);
end;

// A function that checks both HKCU and HKLM for 32-bit queries
function StormRegQueryValue(const CompanyName, ProductName, ValueName: String; var ResultStr: String): Boolean;
var
  sSubkeyName : String;
begin
  // Workaround: Determine the correct 32-bit subkey name because of INNO setup bug
  sSubkeyName := 'SOFTWARE\'
  if ( IsWin64() ) then
    StrCat(sSubkeyName, 'Wow6432Node\');

  StrCat(sSubkeyName, CompanyName);
  StrCat(sSubkeyName, '\');
  StrCat(sSubkeyName, ProductName);

  Result := True;
  if ( not RegQueryStringValue( HKCU, sSubkeyName, ValueName, ResultStr ) ) then
    Result := RegQueryStringValue( HKLM, sSubkeyName, ValueName, ResultStr );
end;

// Ask the user to find the Starcraft: Broodwar directory
function PromptBroodwarPath() : Boolean;
var
  sSubkey : String;
begin
  Result := False;

  if ( MsgBox('Setup was unable to locate the installation directory of Starcraft: Broodwar. This indicates that the game was not installed correctly. '#13#10#13#10'Important components of BWAPI will not function if the location is not correct. Do you wish to locate the Starcraft: Broodwar directory?', mbConfirmation, MB_YESNO) = IDYES ) then
  begin
    if ( BrowseForFolder('Please locate the Starcraft: Broodwar installation directory.', sBroodwarPath, False) ) then
    begin
      // Determine subkey due to a bug in INNO setup
      sSubkey := 'SOFTWARE\Blizzard Entertainment\Starcraft';
      if ( IsWin64() ) then
        sSubkey := 'SOFTWARE\Wow6432Node\Blizzard Entertainment\Starcraft';

      // Write the InstallPath, first try HKLM (because Chaoslaunch complains), then HKCU
      Result := True;
      if ( not RegWriteStringValue( HKLM, sSubkey, 'InstallPath', sBroodwarPath) ) then
        Result := RegWriteStringValue( HKCU, sSubkey, 'InstallPath', sBroodwarPath);
    end;
  end;

end;

// Get the location of the Starcraft folder.
function GetBroodwarPath(): Boolean;
begin
  sBroodwarPath := '';
  Result := True;

  // Search for path
  if ( not StormRegQueryValue('Blizzard Entertainment', 'Starcraft', 'InstallPath', sBroodwarPath) ) then
  begin
    // Ask user for the install path if it wasn't found
    if ( not PromptBroodwarPath() ) then
    begin
      MsgBox('Setup was unable to properly locate and/or store the location of the Starcraft: Broodwar installation directory. Key components of BWAPI may not be installed.', mbError, MB_OK);
      Result := False;
    end;
  end;
end;

//  Returns the path
function BroodwarPath(Param: String) : String;
begin
   Result := sBroodwarPath;
end;

// Returns the Nth string (0-based index) given a null-terminated set of strings
function GetStringAt(S: String; N: Integer) : String;
var
  idx : Integer;
  sCurrent : String;
begin
  // init
  idx := 0;
  sCurrent := Copy(S, 1, Length(S));

  // algo
  while ( N > 0 ) do
  begin
    N := N - 1;
    idx := Pos(#0,sCurrent);
    idx := idx + 1;

    sCurrent := Copy(sCurrent, idx, Length(sCurrent));
  end

  idx := Pos(#0,sCurrent);
  Result := Copy(sCurrent, 1, idx);
end;

// Install the ICCUP gateway
function InstallGateway(Param: String) : String;
var
  sDefaultGateways, sGateways : String;
  sTmp : String;
  idx : Integer;
begin
  // Parse parameters
  StringChangeEx(Param, ';', #0, True);
  
  // Assign defaults
  sDefaultGateways := '1000'#0'01'#0'uswest.battle.net'#0'8'#0'U.S. West'#0'useast.battle.net'#0'6'#0'U.S. East'#0'asia.battle.net'#0'-9'#0'Asia'#0'europe.battle.net'#0'-1'#0'Europe'#0;
  sGateways := sDefaultGateways;

  // Retrieve the existing gateways string (if any)
  RegQueryMultiStringValue( HKEY_CURRENT_USER_32, 'SOFTWARE\Battle.net\Configuration', 'Battle.net gateways', sGateways );

  // Check validity of first component
  sTmp := GetStringAt(sGateways, 0);
  if ( Length(sTmp) = 0 ) then  //  (validity not complete, but should be good)
    sGateways := sDefaultGateways;

  // Check validity of second component
  sTmp := GetStringAt(sGateways, 1);
  if ( Length(sTmp) = 0 ) then  //  (validity not complete, but should be good)
    sGateways := sDefaultGateways;

  // Search for existing entry
  idx := 2;
  repeat
    sTmp := GetStringAt(sGateways, idx);
    idx := idx + 3;
    //MsgBox(sTmp, mbInformation, MB_OK);
  until (CompareText(Trim(sTmp), Trim(GetStringAt(Param,0))) = 0) OR (Length(sTmp) = 0);

  // If desired server was not found
  if ( Length(sTmp) = 0 ) then
  begin
    // Append it to the existing gateways
    StrCat(sGateways, Param);
  end

  Result := sGateways;
end;
