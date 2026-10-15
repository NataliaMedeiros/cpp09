#!/bin/bash

make re >/dev/null 2>&1 || {
    echo "❌ Compilation failed!"
    exit 1
}

test_rpn()
{
    name="$1"
    expression="$2"
    expected="$3"

    result=$(./rpn "$expression" 2>&1)

    if [ "$result" = "$expected" ]; then
        echo "✅ $name"
    else
        echo "❌ $name"
    fi

    echo "   Expression: $expression"
    echo "   Expected:   $expected"
    echo "   Got:        $result"
    echo ""
}

test_rpn "Addition" "2 2 +" "4"
test_rpn "Subtraction" "8 2 -" "6"
test_rpn "Multiplication" "7 7 * 7 -" "42"
test_rpn "Division" "8 2 /" "4"
test_rpn "Invalid expression" "2 +" "Error: Invalid expression"
test_rpn "Division by zero" "8 0 /" "Error: Division by zero"
test_rpn "Empty expression" "" "Error: invalid argument"
test_rpn "Starting by operator" "+ 2 2" "Error: Invalid expression"
test_rpn "Invalid expression" "2 2 + *" "Error: Invalid expression"
test_rpn "Incomplet expression" "2 +" "Error: Invalid expression"
test_rpn "No operator" "2 2" "Error: Invalid expression"

#Caso	Expressão	O que estás a testar
#Ordem errada	+ 2 2	Operador antes dos operandos
#Expressão incompleta	2 +	Falta um operando
#Sem operador	2 2	Sobram dois números no stack
#Operador a mais	2 2 + +	Falta de operandos para o segundo operador
#Expressão vazia	""	Argumento vazio
#Apenas espaços	"   "	Ausência de tokens
#Divisão por zero	8 0 /	Divisor igual a zero
#Operadores consecutivos	2 3 + *	Falta de operandos para *
#Carácter inválido	2 a +	Token que não é permitido
#Parênteses	(1 + 1)	Sintaxe não suportada
#Número com dois dígitos	10 2 +	Números individuais devem ser inferiores a 10
#Número negativo na entrada	-2 3 +	O sinal - é interpretado como operador
#Espaços múltiplos	2   3   +	Separação entre tokens
#Resultado negativo	2 5 -	Resultado pode ser negativo
#Operação encadeada	2 3 + 4 *	Resultado intermédio usado na operação seguinte
