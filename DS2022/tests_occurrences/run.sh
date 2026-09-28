#!/bin/bash
# Usage : ./run.sh <exécutable>   (ex : ./run.sh ../r1)
cd "$(dirname "$0")"
EXE="$(realpath "${1:-../r1}")"
ok=0; ko=0
for f in *.in; do
  t="${f%.in}"
  if cmp -s <("$EXE" < "$f") "$t.out"; then ok=$((ok+1)); echo "OK   $t"
  else ko=$((ko+1)); echo "FAIL $t : attendu $(tr -d '\r' < "$t.out"), obtenu $("$EXE" < "$f" | cat -A)"; fi
done
echo "== $ok OK, $ko FAIL"
