#!/bin/bash
BIN=./PmergeMe
pass=0; fail=0
ERR=$(mktemp)

ok()  { pass=$((pass+1)); }
bad() { fail=$((fail+1)); echo "FAIL: $1"; }

# gen COUNT MAX -> COUNT random ints in 1..MAX (duplicates possible, portable)
gen() { awk -v n="$1" -v m="$2" 'BEGIN{srand(); for(i=0;i<n;i++) printf "%d ", int(rand()*m)+1}'; }

# valid input: exit 0, empty stderr, 4 lines, After == sort -n (first 20), timing lines
check_ok() {
	local desc="$1"; shift
	local n=$# out code got want
	out=$("$BIN" "$@" 2>"$ERR"); code=$?
	[ $code -eq 0 ]  || { bad "$desc: exit code $code"; return; }
	[ ! -s "$ERR" ]  || { bad "$desc: stderr not empty: $(head -c 100 "$ERR")"; return; }
	[ "$(echo "$out" | wc -l | tr -d ' ')" -eq 4 ] || { bad "$desc: expected 4 lines"; return; }
	echo "$out" | sed -n 1p | grep -q '^Before:' || { bad "$desc: line 1 format"; return; }
	got=$(echo "$out" | sed -n 2p | sed 's/^After: *//; s/ *\[\.\.\.\] *$//; s/ *$//')
	want=$(printf '%s\n' "$@" | sort -n | head -20 | tr '\n' ' ' | sed 's/ *$//')
	[ "$got" = "$want" ] || { bad "$desc: After line is wrong"; return; }
	echo "$out" | sed -n 3p | grep -q "range of $n elements with std::vector" || { bad "$desc: vector time line"; return; }
	echo "$out" | sed -n 4p | grep -q "range of $n elements with std::deque"  || { bad "$desc: deque time line"; return; }
	ok
}

# invalid input: exit != 0, empty stdout, message on stderr
check_err() {
	local desc="$1"; shift
	local out code
	out=$("$BIN" "$@" 2>"$ERR"); code=$?
	[ $code -ne 0 ] || { bad "$desc: exit code was 0"; return; }
	[ -z "$out" ]   || { bad "$desc: stdout not empty"; return; }
	[ -s "$ERR" ]   || { bad "$desc: nothing on stderr"; return; }
	ok
}

echo "== basic =="
check_ok "single"        7
check_ok "two"           2 1
check_ok "sorted"        1 2 3 4 5 6 7 8 9 10
check_ok "reversed"      10 9 8 7 6 5 4 3 2 1
check_ok "subject"       3 5 9 7 4

echo "== duplicates =="
check_ok "dups mixed"    3 3 1 1 2
check_ok "all equal"     5 5 5 5 5
check_ok "all equal odd" 2 2 2

echo "== every size 1..60 (covers Jacobsthal groups and leftover) =="
for n in $(seq 1 60); do
	read -ra A <<< "$(gen "$n" 1000)"
	check_ok "random size $n" "${A[@]}"
done

echo "== large =="
for n in 3000 2999 5000; do
	read -ra A <<< "$(gen "$n" 100000)"
	check_ok "random $n" "${A[@]}"
done
read -ra A <<< "$(gen 3000 10)";      check_ok "3000 with many dups" "${A[@]}"
read -ra A <<< "$(gen 1000 1)";       check_ok "1000 equal values"   "${A[@]}"
read -ra A <<< "$(seq 1 3000)";       check_ok "3000 sorted"         "${A[@]}"
read -ra A <<< "$(seq 3000 -1 1)";    check_ok "3000 reversed"       "${A[@]}"
check_ok "INT_MAX valid" 2147483647 1

echo "== errors =="
check_err "no args"
check_err "zero"          0
check_err "negative"      -1 2
check_err "text"          abc
check_err "trailing junk" 12abc
check_err "float"         3.5
check_err "empty string"  ""
check_err "overflow"      99999999999
check_err "INT_MAX + 1"   2147483648
check_err "one bad of 3"  1 2 x

rm -f "$ERR"
echo
echo "passed: $pass   failed: $fail"
[ $fail -eq 0 ]
