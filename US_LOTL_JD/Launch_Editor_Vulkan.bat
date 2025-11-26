@echo off
echo ===============================================
echo  Launching Unreal Editor with Vulkan Renderer
echo  (Alternative fix for UI rendering issues)
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

echo Starting Unreal Editor with Vulkan renderer...
echo This is an alternative fix for rendering issues.
echo.

start "" "%EDITOR_PATH%" "%PROJECT_PATH%" -vulkan

echo Editor launched successfully!
echo.
pause