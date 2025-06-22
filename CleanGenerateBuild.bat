@echo off
setlocal

rem プロジェクトのルート（この.batをuprojectと同じ場所に置く前提）
set PROJECT_DIR=%~dp0
set UPROJECT_NAME=ER3Project.uproject

rem UnrealVersionSelector.exe のパス（通常この場所）
set VERSION_SELECTOR="C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"

rem Unreal Engine Editor 実行ファイルのパス（UE5.3.2 の例）
set UE_EDITOR="C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\Win64\UnrealEditor.exe"

rem MSBuild のパス（自動ビルド用）
set MSBUILD_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

rem Visual Studio ソリューションファイル
set SLN_NAME=ER3Project.sln

rem ビルド構成とプラットフォーム
set CONFIGURATION=Development Editor
set PLATFORM=Win64

echo ========================================
echo Unreal Engine キャッシュ削除 + .sln生成 + 自動ビルド + エディタ起動
echo ========================================

set TARGET_DIRS=Binaries Intermediate Saved .vs DerivedDataCache
for %%D in (%TARGET_DIRS%) do (
    if exist "%PROJECT_DIR%%%D" (
        echo 削除中: %%D
        rmdir /s /q "%PROJECT_DIR%%%D"
    ) else (
        echo 存在しない: %%D
    )
)

rem .sln再生成
%VERSION_SELECTOR% /projectfiles "%PROJECT_DIR%%UPROJECT_NAME%"

rem 自動ビルド
if exist %MSBUILD_PATH% if exist "%PROJECT_DIR%%SLN_NAME%" (
    echo ----------------------------------------
    echo ソリューションをビルド中...
    echo ----------------------------------------
    %MSBUILD_PATH% "%PROJECT_DIR%%SLN_NAME%" /p:Configuration="%CONFIGURATION%" /p:Platform="%PLATFORM%" /t:Build /m
) else (
    echo エラー: ソリューションファイルまたは MSBuild が見つかりません。
)

rem Unreal Editor を起動
if exist %UE_EDITOR% (
    echo ----------------------------------------
    echo Unreal Engine Editor を起動します...
    echo ----------------------------------------
    start "" %UE_EDITOR% "%PROJECT_DIR%%UPROJECT_NAME%"
) else (
    echo エラー: UnrealEditor.exe が見つかりません。
)

echo ========================================
echo 完了しました。

rem 「続行するには何かキーを押してください...」の表示
rem pause

endlocal

rem 現在のコマンドプロンプトを明示的に閉じる
exit
