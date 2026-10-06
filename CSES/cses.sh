#!/usr/bin/env bash
set -euo pipefail

TEMPLATE="$HOME/.config/cses/template.c"
COOKIEPATH="$HOME/.config/cses/cookie"
usage() { echo "Usage: cses <init|test> <ID>"; }

check_id() {
	id=${1:-}
	if ! [[ "$id" =~ ^[0-9]{4}$ ]]; then
		echo "4-digits ID invalid : $id" >&2
		exit 1
	fi
}

cmd_init() {
	check_id "${1:-}"
	if ! page=$(curl -sf "https://cses.fi/problemset/task/$id"); then
		echo "Wrong ID : $id" >&2
		exit 1
	fi
	title=$(sed -n 's/.*<title>CSES - \(.*\)<\/title>.*/\1/p' <<< "$page" )
	if [[ -z "$title" ]]; then
		echo "Cannot find title, check the regex" >&2
		exit 1
	fi
	name=$(tr -cd 'A-Za-z0-9' <<< "$title")
	dir="$id-$name"
	mkdir -p "$dir"
	cd "$dir"
	if ! [[ -f "./$name.c" ]]; then
		if [[ -f "$TEMPLATE" ]]; then	
			cat "$TEMPLATE" > "./$name.c"
		else
			touch "./$name.c"
			echo "No template found, $name.c created empty"
		fi
	else
		echo "File $name.c already exists"
	fi
	if [[ -f "$COOKIEPATH" ]]; then
		COOKIE=$(< "$COOKIEPATH")
		csrf_token=$(curl -s -b "PHPSESSID=$COOKIE" "https://cses.fi/problemset/tests/$id/" | sed -n 's/.*<input type="hidden" name="csrf_token" value="\([^"]*\)".*/\1/p')
		if [[ -z $csrf_token ]]; then
			echo "Cookie expired" >&2
			exit 1
		fi
	else 
		echo "Cookie file not found" >&2
		exit 1
	fi
	mkdir -p tests
	cd tests
	if ! curl -sf -d "csrf_token=$csrf_token" -d "download=true" -b "PHPSESSID=$COOKIE" -o tests.zip "https://cses.fi/problemset/tests/$id/"; then
		echo "Unable to download zip file" >&2
		exit 1
	fi
	if unzip -qq -t tests.zip 2> /dev/null; then
		unzip -qq -o tests.zip &> /dev/null
		rm tests.zip
	else
		rm tests.zip
		echo "Bad zip">&2
		exit 1
	fi
	
	

}

cmd_test() {
	check_id "${1:-}"
	rep=( "$id"-* )
	if ! [[ -d "$rep" ]]; then
		echo "Init the problem first" >&2
		exit 1
	fi
	if [[ ${#rep[@]} -gt 1 ]]; then
		echo "Several directories found, choosing 1st : ${rep[0]}" >&2
	fi
	cd "${rep[0]}"
	cfile=( *.c )
	if ! [[ -f "$cfile" ]]; then
		echo "No c file found" >&2
		exit 1
	fi
	if [[ ${#cfile[@]} -gt 1 ]]; then
		echo "Several c files found, choosing 1st : ${cfile[0]}" >&2
	fi
	if ! gcc -Wall -O2 "${cfile[0]}" -o "$id"; then
		echo "Compilation failed" >&2
		exit 1
	fi
	out=$(mktemp)
	ok=0
	total=0
	for f in tests/*.in; do
		expected="${f%.in}.out"
		total=$((total + 1))
		code=0
		timeout 1 "./$id" < "$f" > "$out" || code=$?
		if [[ $code -eq 124 ]]; then
			echo "$f : TLE"
		elif [[ $code -ne 0 ]]; then
			echo "$f : runtime error (code $code)"
		elif diff -bq "$out" "$expected" > /dev/null; then
			ok=$((ok + 1))
		else
			echo "$f : wrong answer"
		fi
	done
	rm -f "$out"
	echo "$ok/$total tests passed"
	if [[ $ok -ne $total ]]; then
		exit 1
	fi
}

cmd=${1:-}

shift || true

case $cmd in
	init) cmd_init "$@" ;;
	test) cmd_test "$@" ;;
	""|-h|--help|help) usage ;;
	*) echo "Unknown command : $cmd" >&2; usage; exit 1 ;;
esac


