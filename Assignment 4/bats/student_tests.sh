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

@test "Exit runs wihout errors" {
	run ./dsh <<EOF
exit
EOF
	[ "$status" -eq 0 ]
}

@test "CD with invalid directionary" {
	run ./dsh <<EOF
cd fishbowl
exit
EOF
	echo "$output"
	echo "$status"
	echo "$output" | grep -q "cd: No such file or directory"
}

@test "Multiple white spaces" {
	run ./dsh <<EOF
echo fish  fish
EOF
	stripped_output=$(echo "$output" | tr -d '[:space:]')
	expected_output="fishfishdsh2>dsh2>cmdloopreturned0"
	echo "Captured stdout:" 
	echo "Output: $output"
	echo "Exit Status: $status"
	echo "${stripped_output} -> ${expected_output}"
	[ "$stripped_output" = "$expected_output" ]
}
