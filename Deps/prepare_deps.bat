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
set rebuild=0

:argparse_loop
if NOT "%1" == "" (
    if "%1" == "Rebuild" (
        set target=":Rebuild"
        set rebuild=1
    ) else if "%1" == "Debug" (
        set config=Debug
    ) else if "%1" == "Clean" (
        set target=":Clean"
    )

    shift
    goto :argparse_loop
)

pushd .

cd "%script_path%"
echo Current directory is %cd%

REM ========== lkCommon build ===========

cd lkCommon

echo Building lkCommon
call :ms_build lkCommon

cd ..

echo.
echo Script is done
echo.

popd

exit /b 0



REM ========== Functions ==========

:ms_build
set project=%1
set solution=%project%.sln

msbuild %solution% /t:%project%%target% /p:Configuration=%config%;Platform=x64
if %ERRORLEVEL% GEQ 1 (
    echo.
    echo Build failed
    echo.
    exit /b
)

exit /b 0



:cmake_build
set project=%1
set build_dir=build-%config%
mkdir %build_dir%
cd %build_dir%

cmake .. -DCMAKE_BUILD_TYPE=%config%
call :ms_build %project%

exit /b %ERRORLEVEL%
