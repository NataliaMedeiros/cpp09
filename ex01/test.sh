#!/bin/bash

make re >/dev/null 2>&1 || {
    echo "❌ Compilation failed!"
    exit 1
}

test_RPN()
{
    name="$1"
    expression="$2"
    expected="$3"

    result=$(./RPN "$expression" 2>&1)

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

test_RPN "Addition" "2 2 +" "4"
test_RPN "Subtraction" "8 2 -" "6"
test_RPN "Multiplication" "7 7 *" "49"
test_RPN "Division" "8 2 /" "4"
test_RPN "Invalid expression" "2 +" "Error: Invalid expression"
test_RPN "Division by zero" "8 0 /" "Error: Division by zero"
test_RPN "Empty expression" "" "Error: invalid argument"
test_RPN "Starting by operator" "+ 2 2" "Error: Invalid expression"
test_RPN "Invalid expression" "2 2 + *" "Error: Invalid expression"
test_RPN "Incomplet expression" "2 +" "Error: Invalid expression"
test_RPN "No operator" "2 2" "Error: Invalid expression"
test_RPN "Only spaces" "   " "Error: Invalid expression"
test_RPN "Invalid char inside arg" "2 a +" "Error: invalid argument"
test_RPN "invalid parentesis" "(1 1 +)" "Error: invalid argument"
test_RPN "Number bigger than 9" "10 2 +" "Error: Number bigger than 9"
test_RPN "Negative number" "-2 3 +" "Error: Invalid expression"
test_RPN "Multiple spaces" "2     3      +" "5"
test_RPN "Negative result" "2 5 -" "-3"
test_RPN "Multiple operations" "2 3 + 4 *" "20"
test_RPN "Multiple operations" "7 7 * 7 -" "42"
test_RPN "No space" "2+2" "Error: Invalid expression"
test_RPN "Valid" "8 9 * 9 - 9 - 9 - 4 - 1 +" "42"
test_RPN "Valid" "1 2 * 2 / 2 * 2 4 - +" "0"
