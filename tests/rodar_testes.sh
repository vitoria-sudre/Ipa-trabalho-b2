
#!/bin/bash

echo "=================================="
echo " TESTES DO SISTEMA DE ESTACIONAMENTO"
echo "=================================="

gcc -std=c11 -Wall -Wextra -pedantic \
    tests/testes.c src/estacionamento.c \
    -o testes.exe

if [ $? -ne 0 ]; then
    echo "ERRO NA COMPILACAO!"
    exit 1
fi

./testes.exe

resultado=$?

rm -f testes.exe

exit $resultado
