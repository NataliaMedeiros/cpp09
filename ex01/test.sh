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
test_rpn "Multiplication" "7 7 *" "49"
test_rpn "Division" "8 2 /" "4"
test_rpn "Invalid expression" "2 +" "Error: Invalid expression"
test_rpn "Division by zero" "8 0 /" "Error: Division by zero"
test_rpn "Empty expression" "" "Error: invalid argument"
test_rpn "Starting by operator" "+ 2 2" "Error: Invalid expression"
test_rpn "Invalid expression" "2 2 + *" "Error: Invalid expression"
test_rpn "Incomplet expression" "2 +" "Error: Invalid expression"
test_rpn "No operator" "2 2" "Error: Invalid expression"
test_rpn "Only spaces" "   " "Error: Invalid expression"
test_rpn "Invalid char inside arg" "2 a +" "Error: invalid argument"
test_rpn "invalid parentesis" "(1 1 +)" "Error: invalid argument"
test_rpn "Number bigger than 9" "10 2 +" "Error: Number bigger than 9"
test_rpn "Negative number" "-2 3 +" "Error: Invalid expression"
test_rpn "Multiple spaces" "2     3      +" "5"
test_rpn "Negative result" "2 5 -" "-3"
test_rpn "Multiple operations" "2 3 + 4 *" "20"
test_rpn "Multiple operations" "7 7 * 7 -" "42"
test_rpn "No space" "2+2" "Error: Invalid expression"
test_rpn "Valid" "8 9 * 9 - 9 - 9 - 4 - 1 +" "42"
test_rpn "Valid" "1 2 * 2 / 2 * 2 4 - +" "0"
