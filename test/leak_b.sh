#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test files
TEST_DIR="test_files_bonus"
mkdir -p $TEST_DIR
rm -f $TEST_DIR/*

echo -e "${BLUE}=== Pipex Bonus Comprehensive Valgrind Tests ===${NC}"

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
echo -e "Line 1\nLine 2\nLine 3\ntest\ndata" > $TEST_DIR/infile2.txt
echo -e "apple\nbanana\napricot\nblueberry\navocado" > $TEST_DIR/fruits.txt
touch $TEST_DIR/protected.txt
chmod 000 $TEST_DIR/protected.txt

echo -e "${BLUE}=== BONUS 1: Multiple Pipes Tests ===${NC}"

# Test 1: 3 commands (2 pipes)
run_valgrind_test "3 commands pipeline" \
    "./pipex_bonus $TEST_DIR/infile2.txt \"cat\" \"grep Line\" \"wc -l\" $TEST_DIR/outfile1.txt" 0

# Test 2: 4 commands (3 pipes)
run_valgrind_test "4 commands pipeline" \
    "./pipex_bonus $TEST_DIR/fruits.txt \"cat\" \"grep a\" \"wc -l\" \"cat\" $TEST_DIR/outfile2.txt" 0

# Test 3: 5 commands (4 pipes)
run_valgrind_test "5 commands pipeline" \
    "./pipex_bonus $TEST_DIR/fruits.txt \"cat\" \"sort\" \"head -n 3\" \"wc -l\" \"cat\" $TEST_DIR/outfile3.txt" 0

# Test 4: Single command (no pipes)
run_valgrind_test "Single command (no pipes)" \
    "./pipex_bonus $TEST_DIR/infile1.txt \"cat\" $TEST_DIR/outfile4.txt" 0

# Test 5: Multiple pipes with non-existent command in middle
run_valgrind_test "Multiple pipes with invalid middle command" \
    "./pipex_bonus $TEST_DIR/infile2.txt \"cat\" \"invalidcmd\" \"wc -l\" $TEST_DIR/outfile5.txt" 0

# Test 6: Multiple pipes with non-existent first command
run_valgrind_test "Multiple pipes with invalid first command" \
    "./pipex_bonus $TEST_DIR/infile2.txt \"invalidcmd\" \"cat\" \"wc -l\" $TEST_DIR/outfile6.txt" 0

# Test 7: Multiple pipes with non-existent last command
run_valgrind_test "Multiple pipes with invalid last command" \
    "./pipex_bonus $TEST_DIR/infile2.txt \"cat\" \"grep Line\" \"invalidcmd\" $TEST_DIR/outfile7.txt" 0

# Test 8: Long pipeline (6 commands)
seq 1 100 > $TEST_DIR/numbers.txt
run_valgrind_test "Long pipeline (6 commands)" \
    "./pipex_bonus $TEST_DIR/numbers.txt \"cat\" \"head -n 50\" \"tail -n 10\" \"sort -n\" \"head -n 5\" \"wc -l\" $TEST_DIR/outfile8.txt" 0

echo -e "${BLUE}=== BONUS 2: Here_doc Tests ===${NC}"

# Test 9: Basic here_doc
run_valgrind_test "Basic here_doc" \
    "echo -e \"line1\nline2\nEOF\" | ./pipex_bonus here_doc EOF \"cat\" \"wc -l\" $TEST_DIR/outfile9.txt" 0

# Test 10: Here_doc with multiple commands
run_valgrind_test "Here_doc with multiple commands" \
    "echo -e \"apple\nbanana\nSTOP\" | ./pipex_bonus here_doc STOP \"cat\" \"sort\" \"head -n 1\" $TEST_DIR/outfile10.txt" 0

# Test 11: Here_doc with grep and multiple pipes
run_valgrind_test "Here_doc with grep and multiple pipes" \
    "echo -e \"test line\nanother test\nno match\nEND\" | ./pipex_bonus here_doc END \"cat\" \"grep test\" \"wc -l\" $TEST_DIR/outfile11.txt" 0

# Test 12: Here_doc with empty input
run_valgrind_test "Here_doc with immediate limiter" \
    "echo \"LIMIT\" | ./pipex_bonus here_doc LIMIT \"cat\" \"wc -l\" $TEST_DIR/outfile12.txt" 0

# Test 13: Here_doc with complex limiter
run_valgrind_test "Here_doc with complex limiter" \
    "echo -e \"data1\ndata2\nCOMPLEX_LIMIT_123\" | ./pipex_bonus here_doc COMPLEX_LIMIT_123 \"cat\" \"wc -w\" $TEST_DIR/outfile13.txt" 0

echo -e "${BLUE}=== Error Handling Tests ===${NC}"

# Test 14: Invalid arguments for bonus
run_valgrind_test "Invalid arguments (too few)" \
    "./pipex_bonus $TEST_DIR/infile1.txt \"cat\" $TEST_DIR/outfile14.txt" 1

# Test 15: Invalid here_doc arguments
run_valgrind_test "Invalid here_doc arguments" \
    "./pipex_bonus here_doc LIMIT \"cat\" $TEST_DIR/outfile15.txt" 1

# Test 16: Non-existent input file with multiple commands
run_valgrind_test "Non-existent input file with multiple commands" \
    "./pipex_bonus $TEST_DIR/nonexistent.txt \"cat\" \"grep test\" \"wc -l\" $TEST_DIR/outfile16.txt" 0

# Test 17: Permission denied input with multiple commands
run_valgrind_test "Permission denied input with multiple commands" \
    "./pipex_bonus $TEST_DIR/protected.txt \"cat\" \"head -n 1\" \"wc -l\" $TEST_DIR/outfile17.txt" 0

echo -e "${BLUE}=== Stress Tests ===${NC}"

# Test 18: Many commands (8 commands = 7 pipes)
run_valgrind_test "Stress test: 8 commands pipeline" \
    "./pipex_bonus $TEST_DIR/fruits.txt \"cat\" \"sort\" \"uniq\" \"head -n 3\" \"tail -n 2\" \"wc -l\" \"cat\" \"cat\" $TEST_DIR/outfile18.txt" 0

# Test 19: Large here_doc input
run_valgrind_test "Large here_doc input" \
    "seq 1 1000 | sed 's/$//' | (cat; echo \"STOP\") | ./pipex_bonus here_doc STOP \"head -n 100\" \"tail -n 10\" \"wc -l\" $TEST_DIR/outfile19.txt" 0

# Test 20: Mixed absolute and relative paths with multiple commands
run_valgrind_test "Mixed absolute/relative paths with multiple commands" \
    "./pipex_bonus $TEST_DIR/infile2.txt \"/bin/cat\" \"grep Line\" \"/usr/bin/wc -l\" $TEST_DIR/outfile20.txt" 0

echo -e "${BLUE}=== Edge Cases ===${NC}"

# Test 21: Empty commands in pipeline
run_valgrind_test "Empty command in pipeline" \
    "./pipex_bonus $TEST_DIR/infile1.txt \"\" \"cat\" \"wc -l\" $TEST_DIR/outfile21.txt" 127

# Test 22: Whitespace-only commands
run_valgrind_test "Whitespace-only command" \
    "./pipex_bonus $TEST_DIR/infile1.txt \"cat\" \"   \" \"wc -l\" $TEST_DIR/outfile22.txt" 127

# Test 23: Commands with multiple spaces
run_valgrind_test "Commands with multiple spaces" \
    "./pipex_bonus $TEST_DIR/fruits.txt \"cat\" \"grep  -v  apple\" \"wc -l\" $TEST_DIR/outfile23.txt" 0

# Test 24: Very long pipeline (10 commands)
run_valgrind_test "Very long pipeline (10 commands)" \
    "./pipex_bonus $TEST_DIR/fruits.txt \"cat\" \"cat\" \"cat\" \"cat\" \"cat\" \"cat\" \"cat\" \"cat\" \"cat\" \"wc -l\" $TEST_DIR/outfile24.txt" 0

# Test 25: Here_doc with append mode
run_valgrind_test "Here_doc append mode" \
    "echo -e \"first line\nEOF\" | ./pipex_bonus here_doc EOF \"cat\" $TEST_DIR/append_test.txt && echo -e \"second line\nEOF\" | ./pipex_bonus here_doc EOF \"cat\" $TEST_DIR/append_test.txt" 0

# Cleanup
chmod 644 $TEST_DIR/protected.txt
rm -rf $TEST_DIR

echo -e "${BLUE}=== All bonus tests completed ===${NC}"

# Summary
echo -e "${BLUE}=== Test Summary ===${NC}"
echo "Tested features:"
echo "- Multiple pipes (2-9 pipes)"
echo "- Here_doc functionality"
echo "- Error handling for invalid commands"
echo "- Memory leak detection for complex pipelines"
echo "- Edge cases and stress tests"
echo ""
echo "If all tests pass without memory leaks, your bonus implementation is robust!"
