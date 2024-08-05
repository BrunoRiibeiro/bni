#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
CURRENT_DIR="$(pwd)"

if [ "$#" -ne 2 ]; then
	echo Usage: "$0" \<domain_file\> \<problem_file\>
	exit 1
fi

DOMAIN="$1"
PROBLEM="$2"
if [[ ! -r "$DOMAIN" ]]; then
	echo "Error: Domain file '$DOMAIN' cannot be read."
	exit 1
fi
if [[ ! -r "$PROBLEM" ]]; then
	echo "Error: Problem file '$PROBLEM' cannot be read."
	exit 1
fi

"$SCRIPT_DIR/parser" "$DOMAIN" "$PROBLEM"
status=$?
if [ $status -ne 0 ]; then
	echo "Error: The parser encountered a problem."
	exit $status
fi

make PREFIX="$CURRENT_DIR" -C "$SCRIPT_DIR" repl
status=$?
if [ $status -ne 0 ]; then
	echo "Error: The 'make repl' command failed."
	exit $status
fi

REPL_EXEC="$CURRENT_DIR/repl"
if [ -f "$REPL_EXEC" ]; then
	"$REPL_EXEC"
	status=$?
	if [ $status -ne 0 ]; then
		echo "Error: The 'repl' command failed."
		exit $status
	fi
else
	echo "Error: The 'repl' executable not found at $REPL_EXEC."
	exit 1
fi
