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

set script_path="%~dp0"
set target=""
set config=Release

:argparse_loop
if NOT "%1" == "" (
    if "%1" == "Rebuild" (
        set target=":Rebuild"
    ) else if "%1" == "Debug"  (
        set config=Debug
    )

    shift
    goto :argparse_loop
)

pushd .

cd "%script_path%"
echo Current directory is %cd%

REM ========== lkCommon build ===========

cd lkCommon

echo Building lkCommon in %config%
msbuild lkCommon.sln /t:lkCommon%target% /p:Configuration=%config%;Platform=x64
if %ERRORLEVEL% GEQ 1 (
    echo.
    echo Build failed
    echo.
    exit /b
)

echo.
echo Script is done
echo.

popd
