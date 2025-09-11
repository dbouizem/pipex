#!/bin/bash

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Variables
PIPEX="./pipex"
TMP_DIR="./test_tmp"
mkdir -p $TMP_DIR

# Fonctions
cleanup() {
    rm -rf $TMP_DIR
}

print_result() {
    local test_name=$1
    local status=$2
    if [ $status -eq 0 ]; then
        echo -e "${GREEN}✓ $test_name${NC}"
    else
        echo -e "${RED}✗ $test_name${NC}"
    fi
}

compare_with_shell() {
    local args=("$@")
    local shell_cmd="${args[0]}"
    local pipex_cmd="${args[@]:1}"

    # Exécution shell
    eval "$shell_cmd" > $TMP_DIR/shell_stdout 2> $TMP_DIR/shell_stderr
    shell_exit=$?

    # Exécution pipex
    eval "$pipex_cmd" > $TMP_DIR/pipex_stdout 2> $TMP_DIR/pipex_stderr
    pipex_exit=$?

    # Comparaison stdout et exit code
    diff $TMP_DIR/shell_stdout $TMP_DIR/pipex_stdout > /dev/null
    stdout_diff=$?
    [ $shell_exit -eq $pipex_exit ] && exit_diff=0 || exit_diff=1

    # Comparaison de la présence de stderr
    shell_stderr_size=$(wc -c < $TMP_DIR/shell_stderr)
    pipex_stderr_size=$(wc -c < $TMP_DIR/pipex_stderr)
    if [ $shell_stderr_size -eq 0 ] && [ $pipex_stderr_size -eq 0 ]; then
        stderr_match=0
    elif [ $shell_stderr_size -gt 0 ] && [ $pipex_stderr_size -gt 0 ]; then
        stderr_match=0
    else
        stderr_match=1
    fi

    if [ $stdout_diff -eq 0 ] && [ $exit_diff -eq 0 ] && [ $stderr_match -eq 0 ]; then
        return 0
    else
        echo -e "${YELLOW}Shell exit: $shell_exit | Pipex exit: $pipex_exit${NC}"
        [ $stdout_diff -ne 0 ] && echo -e "${BLUE}Stdout diff:${NC}" && diff --color $TMP_DIR/shell_stdout $TMP_DIR/pipex_stdout
        if [ $stderr_match -ne 0 ]; then
            echo -e "${BLUE}Stderr presence mismatch:${NC}"
            echo "Shell stderr size: $shell_stderr_size"
            echo "Pipex stderr size: $pipex_stderr_size"
        fi
        return 1
    fi
}

# Tests
echo -e "${BLUE}Testing Pipex...${NC}"

# Test 1: Commande normale
echo "Test 1: Normal command"
compare_with_shell "< Makefile grep pipex | wc -l > $TMP_DIR/shell_out" \
                   "$PIPEX Makefile \"grep pipex\" \"wc -l\" $TMP_DIR/pipex_out"
print_result "Normal command" $?

# Test 2: Fichier input inexistant
echo "Test 2: Nonexistent input file"
compare_with_shell "< nonexistent grep pipex | wc -l > $TMP_DIR/shell_out" \
                   "$PIPEX nonexistent \"grep pipex\" \"wc -l\" $TMP_DIR/pipex_out"
print_result "Nonexistent input file" $?

# Test 3: Commande inexistante
echo "Test 3: Nonexistent command"
compare_with_shell "< Makefile nonexistentcmd | wc -l > $TMP_DIR/shell_out" \
                   "$PIPEX Makefile \"nonexistentcmd\" \"wc -l\" $TMP_DIR/pipex_out"
print_result "Nonexistent command" $?

# Test 4: Permission denied sur input
echo "Test 4: Restricted input file"
touch $TMP_DIR/restricted
chmod 000 $TMP_DIR/restricted
compare_with_shell "< $TMP_DIR/restricted grep pipex | wc -l > $TMP_DIR/shell_out" \
                   "$PIPEX $TMP_DIR/restricted \"grep pipex\" \"wc -l\" $TMP_DIR/pipex_out"
print_result "Restricted input file" $?
chmod 644 $TMP_DIR/restricted

# Test 5: Permission denied sur output
echo "Test 5: Restricted output directory"
mkdir -p $TMP_DIR/restricted_dir
touch $TMP_DIR/restricted_dir/outfile
chmod 000 $TMP_DIR/restricted_dir
compare_with_shell "< Makefile grep pipex | wc -l > $TMP_DIR/restricted_dir/outfile" \
                   "$PIPEX Makefile \"grep pipex\" \"wc -l\" $TMP_DIR/restricted_dir/outfile"
print_result "Restricted output directory" $?
chmod 755 $TMP_DIR/restricted_dir

# Nettoyage
cleanup
