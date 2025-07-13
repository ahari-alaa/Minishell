#!/bin/bash

# This script runs test cases against your minishell and checks for leaks using valgrind.
# Usage: ./test_minishell_leaks.sh /path/to/minishell

MINISHELL_BIN="${1:-./minishell}"
VALGRIND_BIN="valgrind"
VALGRIND_OPTS="--leak-check=full --show-leak-kinds=all --track-origins=yes --errors-for-leak-kinds=all --error-exitcode=42"
TMP_OUT="minishell_test_output.txt"
TMP_ERR="minishell_test_valgrind.txt"

if ! [ -x "$MINISHELL_BIN" ]; then
    echo "ERROR: Minishell binary not found or not executable: $MINISHELL_BIN"
    exit 1
fi

test_cases=(
    # Basic
    'echo hello world'
    'export VAR="hello"'
    'echo $VAR'
    'ls | wc -l'
    'cat /dev/null'
    'export a="ls -la"'
    '$a'
    'ls > file1'
    'cat < file1'
    'rm file1'
    'exit'
    # Syntax errors
    'echo >'
    'ls <'
    'cat |'
    '|| echo fail'
    'echo "unclosed'
    'ls ><<<<'
    'echo hello ||| wc -l'
    'echo "" | | cat'
    # Export and expansions
    'export X=foo'
    'export Y="bar baz"'
    'echo $X$Y'
    'export Q="hello" W="world"'
    'echo "$Q $W"'
    'export a="echo test"'
    '$a'
    'export B="abc"'
    'export C="$B"'
    'echo $C'
    # Heredoc
    'cat <<END
hello
END'
    'cat <<EOF | grep foo
foo
bar
EOF'
    'cat <<X
line1
line2
X'
    'cat <<EOF | cat | cat
test
EOF'
    # Heredoc with variable expansion
    'export HER="ok"'
    'cat <<EOF
$HER
EOF'
    # Heredoc with quotes (should not expand)
    'cat <<\'EOF\'
$HER
EOF'
    # Unclosed heredoc
    'cat <<STOP'
)

# Function to test a single command
test_leak() {
    local cmd="$1"
    echo "----------------------------------"
    echo "Testing: $cmd"
    echo "$cmd" | $VALGRIND_BIN $VALGRIND_OPTS "$MINISHELL_BIN" > "$TMP_OUT" 2> "$TMP_ERR"
    LEAKS=$(grep "definitely lost:" "$TMP_ERR" | grep -v " 0 bytes")
    ERR_CODE=$?
    if [[ -n "$LEAKS" ]]; then
        echo "❌ Leak detected for: $cmd"
        grep "definitely lost:" "$TMP_ERR"
        grep "indirectly lost:" "$TMP_ERR"
    else
        echo "✅ No leak detected for: $cmd"
    fi
}

echo "======== Minishell Leak Tests ========"
for tc in "${test_cases[@]}"; do
    test_leak "$tc"
done

rm -f "$TMP_OUT" "$TMP_ERR"
echo "======== All tests finished =========="