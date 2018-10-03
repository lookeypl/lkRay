@echo off

REM TODO:
REM  * Discover CPU count

echo | set /p=Checking for cmake...
for %%X in (cmake.exe) do (set FOUND=%%~$PATH:X)
if defined FOUND (
    echo FOUND
) else (
    echo NOT FOUND
    echo     Make sure you download CMake before using this script.
    pause
    exit /b
)

echo | set /p=Checking for msbuild...
for %%X in (msbuild.exe) do (set FOUND=%%~$PATH:X)
if defined FOUND (
    echo FOUND
) else (
    echo NOT FOUND
    echo     Make sure you set MSBuild on your PATH variable before using this script.
    pause
    exit /b
)

set arg1=%1

if "%arg1%" == "rebuild" (
    echo REBUILDING
    set target=":Rebuild"
) else (
    echo BUILDING
    set target=""
)

pushd .

cd %~dp0
echo Current directory is %cd%

REM ========== lkCommon build ===========

cd lkCommon

msbuild lkCommon.sln /t:lkCommon%target% /p:Configuration=Debug;Platform=x86
msbuild lkCommon.sln /t:lkCommon%target% /p:Configuration=Release;Platform=x86
msbuild lkCommon.sln /t:lkCommon%target% /p:Configuration=Debug;Platform=x64
msbuild lkCommon.sln /t:lkCommon%target% /p:Configuration=Release;Platform=x64

echo.
echo Script is done
echo.

popd
