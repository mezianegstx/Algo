#!/bin/bash
# Compile chaque corrigé et le teste sur les fichiers tests/<prog>/*.in
# Compare la sortie octet par octet avec le .ans (donc vérifie aussi les \r\n).
# Usage (Linux, Mac, WSL) : depuis le dossier ds_algo_3if :  bash tests/run_tests.sh

cd "$(dirname "$0")/.."
BIN=$(mktemp -d)
ok=0; ko=0

for src in 1_td/*.c 2_annales_ds/*.c; do
    prog=$(basename "$src" .c)
    [ -d "tests/$prog" ] || continue
    if ! gcc -O2 -w -o "$BIN/$prog" "$src" -lm; then
        echo "ERREUR DE COMPILATION : $src"; ko=$((ko+1)); continue
    fi
    for in in tests/$prog/*.in; do
        ans="${in%.in}.ans"
        if "$BIN/$prog" < "$in" | cmp -s - "$ans"; then
            ok=$((ok+1))
        else
            ko=$((ko+1))
            echo "ECHEC : $prog / $(basename "$in")"
            echo "  attendu : $(cat -A "$ans" | tr '\n' ' ')"
            echo "  obtenu  : $("$BIN/$prog" < "$in" | cat -A | tr '\n' ' ')"
        fi
    done
done

rm -rf "$BIN"
echo "Réussis : $ok   Echecs : $ko"
