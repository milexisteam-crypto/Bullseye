@echo off
setlocal

REM Pobierz katalog, w którym znajduje się .bat
set SCRIPT_DIR=%~dp0

REM Uruchom WSL Arch i wykonaj run.sh w tym katalogu
wsl.exe -d archlinux  ./run.sh
