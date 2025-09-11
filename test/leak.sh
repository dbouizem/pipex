#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test files
TEST_DIR="test_files"
mkdir -p $TEST_DIR
rm -f $TEST_DIR/*

echo -e "${BLUE}=== Pipex Comprehensive Valgrind Tests ===${NC}"

# Function to run valgrind test
run_valgrind_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit="$3"

    echo -e "${YELLOW}Testing: $test_name${NC}"
    echo "Command: $command"

    # Run with valgrind
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
             --errors-for-leak-kinds=all --error-exitcode=1 \
             $command 2> $TEST_DIR/valgrind.log

    local exit_code=$?

    # Check for memory leaks in the main process
    if grep -q "no leaks are possible" $TEST_DIR/valgrind.log &&
       ! grep -q "definitely lost:" $TEST_DIR/valgrind.log &&
       ! grep -q "indirectly lost:" $TEST_DIR/valgrind.log; then
        echo -e "${GREEN}✓ No memory leaks detected${NC}"
        return 0
    else
        echo -e "${RED}✗ Memory leaks detected${NC}"
        echo "Valgrind output:"
        cat $TEST_DIR/valgrind.log
        return 1
    fi
}

# Create test files
echo "Hello World" > $TEST_DIR/infile1.txt
echo -e "Line 1\nLine 2\nLine 3" > $TEST_DIR/infile2.txt
touch $TEST_DIR/protected.txt
chmod 000 $TEST_DIR/protected.txt

# Test 1: Basic functionality
run_valgrind_test "Basic cat to wc" \
    "./pipex $TEST_DIR/infile1.txt \"cat\" \"wc -l\" $TEST_DIR/outfile1.txt" 0

# Test 2: Non-existent input file
run_valgrind_test "Non-existent input file" \
    "./pipex $TEST_DIR/nonexistent.txt \"cat\" \"wc -l\" $TEST_DIR/outfile2.txt" 0

# Test 3: Non-existent command (cmd1)
run_valgrind_test "Non-existent command (cmd1)" \
    "./pipex $TEST_DIR/infile1.txt \"nonexistentcmd\" \"wc -l\" $TEST_DIR/outfile3.txt" 0

# Test 4: Non-existent command (cmd2) - The specific failing test
run_valgrind_test "Non-existent command (cmd2)" \
    "./pipex $TEST_DIR/infile1.txt \"cat\" \"nonexistentcmd\" $TEST_DIR/outfile4.txt" 0

# Test 5: Permission denied (input file)
run_valgrind_test "Permission denied (input file)" \
    "./pipex $TEST_DIR/protected.txt \"cat\" \"wc -l\" $TEST_DIR/outfile5.txt" 0

# Test 6: Complex command with arguments
run_valgrind_test "Complex command with arguments" \
    "./pipex $TEST_DIR/infile2.txt \"grep Line\" \"wc -l\" $TEST_DIR/outfile6.txt" 0

# Test 7: Empty input file
touch $TEST_DIR/empty.txt
run_valgrind_test "Empty input file" \
    "./pipex $TEST_DIR/empty.txt \"cat\" \"wc -l\" $TEST_DIR/outfile7.txt" 0

# Test 8: Large file
seq 1 1000 > $TEST_DIR/large.txt
run_valgrind_test "Large file" \
    "./pipex $TEST_DIR/large.txt \"wc -l\" \"head -n 5\" $TEST_DIR/outfile8.txt" 0

# Test 9: Multiple commands with special characters
run_valgrind_test "Commands with special characters" \
    "./pipex $TEST_DIR/infile2.txt \"grep -v Line\" \"wc -w\" $TEST_DIR/outfile9.txt" 0

# Test 10: Absolute path commands
run_valgrind_test "Absolute path commands" \
    "./pipex $TEST_DIR/infile1.txt \"/bin/cat\" \"/usr/bin/wc -l\" $TEST_DIR/outfile10.txt" 0

# Cleanup
chmod 644 $TEST_DIR/protected.txt
rm -rf $TEST_DIR

echo -e "${BLUE}=== All tests completed ===${NC}"
