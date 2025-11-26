@echo off
echo ===============================================
echo  Launching Unreal Editor with OpenGL Renderer
echo  (Compatibility fix for older graphics cards)
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

echo Starting Unreal Editor with OpenGL renderer...
echo This works well for older graphics cards.
echo.

start "" "%EDITOR_PATH%" "%PROJECT_PATH%" -opengl

echo Editor launched successfully!
echo.
pause