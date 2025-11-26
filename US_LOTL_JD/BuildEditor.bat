@echo off
echo Building US_LOTL_JD Editor...
"C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" US_LOTL_JDEditor Win64 Development -project="%~dp0US_LOTL_JD.uproject" -rocket -progress
pause