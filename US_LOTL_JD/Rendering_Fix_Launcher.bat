@echo off
title Unreal Engine Rendering Fix Launcher
color 0A
echo.
echo ===============================================================================
echo                    UNREAL ENGINE RENDERING ISSUE FIX LAUNCHER
echo ===============================================================================
echo.
echo This tool helps fix common UI rendering issues in Unreal Engine Editor
echo where menus appear as duplicated menu bars or garbled interface elements.
echo.
echo Choose a rendering backend:
echo.
echo [1] DirectX 11 (Recommended - fixes most rendering issues)
echo [2] Vulkan (Alternative option)
echo [3] OpenGL (Compatibility mode for older hardware)
echo [4] Normal Launch (Default DirectX 12)
echo [5] Exit
echo.
set /p choice="Enter your choice (1-5): "

set PROJECT_PATH=%~dp0US_LOTL_JD.uproject
set EDITOR_PATH=C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\Win64\UnrealEditor.exe

if not exist "%PROJECT_PATH%" (
    echo.
    echo ERROR: Project file not found at %PROJECT_PATH%
    pause
    exit /b 1
)

if not exist "%EDITOR_PATH%" (
    echo.
    echo ERROR: Unreal Editor not found at %EDITOR_PATH%
    echo Please check your UE5.5 installation path
    pause
    exit /b 1
)

if "%choice%"=="1" (
    echo.
    echo Launching with DirectX 11...
    start "" "%EDITOR_PATH%" "%PROJECT_PATH%" -d3d11
    echo Editor launched with D3D11 renderer!
    goto end
)

if "%choice%"=="2" (
    echo.
    echo Launching with Vulkan...
    start "" "%EDITOR_PATH%" "%PROJECT_PATH%" -vulkan
    echo Editor launched with Vulkan renderer!
    goto end
)

if "%choice%"=="3" (
    echo.
    echo Launching with OpenGL...
    start "" "%EDITOR_PATH%" "%PROJECT_PATH%" -opengl
    echo Editor launched with OpenGL renderer!
    goto end
)

if "%choice%"=="4" (
    echo.
    echo Launching normally (DirectX 12)...
    start "" "%EDITOR_PATH%" "%PROJECT_PATH%"
    echo Editor launched normally!
    goto end
)

if "%choice%"=="5" (
    echo.
    echo Goodbye!
    exit /b 0
)

echo.
echo Invalid choice. Please try again.
pause
goto :choice

:end
echo.
echo If you still experience rendering issues:
echo 1. Try a different rendering option
echo 2. Update your graphics drivers
echo 3. Check Windows Display settings (try 100%% scaling)
echo.
pause