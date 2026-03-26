@echo off
title Filza26 Production Builder

set MSYS2=C:\msys64\mingw64\bin
set PATH=%MSYS2%;C:\msys64\usr\bin;%PATH%

cd /d "%~dp0"

echo ========================================
echo   Filza26 Production Builder
echo ========================================

echo [1/2] Compiling...
gcc Filza26Maker.c -o Filza26.exe ^
 -lcurl -lws2_32 -lcrypt32 -lwldap32 -lz

if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed
    pause
    exit /b
)

echo [2/2] Running builder...
Filza26.exe

echo.
echo Done.
pause