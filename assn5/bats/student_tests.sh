#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Pipes with header files" {
	run "./dsh" <<EOF                
ls | grep dshlib.h
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dshlib.hdsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Pipe with no output" {
    run "./dsh" <<EOF
echo 'no match' | grep 'notfound'
EOF


    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Simple command" {
	run "./dsh" <<EOF
echo Hello
EOF

	echo "$output"
	stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="Hellodsh3>dsh3>cmdloopreturned0"


	[ "$stripped_output" = "$expected_output" ]
}

@test "Command of file amount" {
    run "./dsh" <<EOF
ls | wc -l
EOF


    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="10dsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "cd does not work on invalid directory" {
    run ./dsh <<EOF
cd fishbowl
EOF
	echo "$output"
	stripped_output=$(echo "$output" | tr -d '[:space:]')
	expected_output="execvp:Nosuchfileordirectorydsh3>dsh3>dsh3>cmdloopreturned0"
	[ "$status" -eq 0 ]
	[ "$stripped_output" = "$expected_output" ]
}



