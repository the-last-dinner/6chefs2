
[Setup]
AppName=6人の料理人と隻眼の少女2
AppVerName=6人の料理人と隻眼の少女2_1.2
VersionInfoVersion=1.2.0.0
AppCopyright=最後の晩餐
AppPublisher=最後の晩餐
AppPublisherURL=http://the-last-dinner.club
AppVersion=1.2
UsePreviousAppDir=no
DefaultDirName={pf}\6chefs2_1.2
DefaultGroupName=6人の料理人と隻眼の少女2
UninstallDisplayIcon={app}\6chefs2_1.2.exe
Compression=lzma
SolidCompression=yes

;ウィザードページに表示されるグラフィック（*.bmp: 164 x 314）
WizardImageFile=C:\Users\LastDinner\Desktop\6chefs2_1.2\installer.bmp
;ウィザードページに表示されるグラフィックが拡大されない
WizardImageStretch=no
;その隙間色
WizardImageBackColor=$ffffff

[Files]Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\src\*"; DestDir: "{app}"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\BGM\*"; DestDir: "{app}\BGM"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\character\*"; DestDir: "{app}\character"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\csv\*"; DestDir: "{app}\csv"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\character\plist\*"; DestDir: "{app}\character\plist"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\event\*"; DestDir: "{app}\event"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\img\disp\*"; DestDir: "{app}\img\disp"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\img\character\*"; DestDir: "{app}\img\character"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\img\*"; DestDir: "{app}\img"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\map\*"; DestDir: "{app}\map"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\save\*"; DestDir: "{app}\save"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\se\*"; DestDir: "{app}\se"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\config\*"; DestDir: "{app}\config"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\effect\*"; DestDir: "{app}\effect"Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\fonts\*"; DestDir: "{app}\fonts"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\shaders\*"; DestDir: "{app}\shaders"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\voice\*"; DestDir: "{app}\voice"
Source: "C:\Users\LastDinner\Desktop\6chefs2_1.2\Resources\video\*"; DestDir: "{app}\video"
;Source: "C:\Users\Ishiwata Lab\Desktop\6chefs1.2\shaders\*"; DestDir: "{app}\shaders";Source: "C:\Users\Ishiwata Lab\Desktop\6chefs1.2\ReadMeWin.txt"; DestDir: "{app}"; Flags: isreadme

[Dirs]
Name: {app}\save;Permissions: everyone-modify

[Tasks]
Name: desktopicon; Description: "デスクトップにショートカットを作成する"

[Icons]
Name: "{group}\6人の料理人と隻眼の少女2"; Filename: "{app}\6chefs2_1.2.exe"
Name: "{commondesktop}\6人の料理人と隻眼の少女2"; Filename: "{app}\6chefs2_1.2.exe"; WorkingDir: "{app}"; Tasks: desktopicon

[Languages]
Name:"jp";MessagesFile:"C:\Program Files (x86)\Inno Setup 5\Languages\Japanese.isl";

[Run]
Filename: "{app}\6chefs2_1.2.exe"; Description: "インストーラ終了時にゲームを起動する"; Flags: postinstall
