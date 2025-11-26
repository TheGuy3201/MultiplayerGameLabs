@echo off
echo ===============================================
echo  Launching Unreal Editor with D3D11 Renderer
echo  (Fixes UI rendering issues)
echo ===============================================
echo.

set PROJECT_PATH=%~dp0US_LOTL_JD.uproject
set EDITOR_PATH=C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\Win64\UnrealEditor.exe

if not exist "%PROJECT_PATH%" (
    echo ERROR: Project file not found at %PROJECT_PATH%
    pause
    exit /b 1
)

if not exist "%EDITOR_PATH%" (
    echo ERROR: Unreal Editor not found at %EDITOR_PATH%
    echo Please check your UE5.5 installation path
    pause
    exit /b 1
)

echo Starting Unreal Editor with DirectX 11 renderer...
echo This should fix menu and UI rendering issues.
echo.

start "" "%EDITOR_PATH%" "%PROJECT_PATH%" -d3d11

echo Editor launched successfully!
echo If you still experience rendering issues, try the other launch options.
echo.
pause