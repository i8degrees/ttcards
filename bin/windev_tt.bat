@echo off

REM Automated Windows build script for TTcards -- windev.local

REM NOTE: This script is intended to be ran from the project's root
REM directory, i.e.: %HOMEPATH%\Projects\ttcards.git

REM ...Engine setup...
set NOMLIB_DEPS_PREFIX=%HOMEPATH%\Projects\nomlib.git\third-party\windows

REM virgo.local source repository
unison ttcards

IF NOT EXIST build mkdir build
REM pushd build
cd build

if errorlevel 0 call ..\bin\msbuild_gen.bat Debug
if errorlevel 0 call ..\bin\msbuild_build.bat Debug
if errorlevel 0 call ..\bin\msbuild_install.bat Debug

REM popd
