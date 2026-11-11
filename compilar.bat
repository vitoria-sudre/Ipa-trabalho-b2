
@echo off
cd /d "%~dp0"

gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/estacionamento.c -o estacionamento.exe

if errorlevel 1 (
    echo.
    echo ERRO NA COMPILACAO!
    pause
    exit /b 1
)

echo.
echo PROGRAMA COMPILADO COM SUCESSO!
echo.

estacionamento.exe
pause
