#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
CURRENT_DIR="$(pwd)"
exec_repl=false

function show_help {
	echo "Usage: $0 [options] <domain_file> <problem_file>"
	echo
	echo "Options:"
	echo "  -o <file>        Specify the output file name (default: pddl.c/pddl.h)"
	echo "  -r, --run-repl   Run the REPL executable after parsing PDD to C"
	echo "  -h, --help       Show this help message and exit"
	exit 0
}

while getopts "hr-:" opt; do
	case "$opt" in
		h) show_help;;
		r) exec_repl=true;;
		-)	case "${OPTARG}" in
				help) show_help;; 
				run-repl) exec_repl=true;;
				*) echo "Invalid option: --${OPTARG}" >&2; exit 1;;
			esac
			;;
		\?)	echo "Invalid option: -${OPTARG}" >&2; exit 1;;
	esac
done
shift $((OPTIND-1))

if [[ "$#" -ne 2 ]]; then
	echo Usage: "$0" \<domain_file\> \<problem_file\>
	exit 1
fi

DOMAIN="$1"
PROBLEM="$2"
if [[ ! -r "$DOMAIN" ]]; then
	echo "Error: Domain file '$DOMAIN' cannot be read." >&2
	exit 1
fi
if [[ ! -r "$PROBLEM" ]]; then
	echo "Error: Problem file '$PROBLEM' cannot be read." >&2
	exit 1
fi

"$SCRIPT_DIR/parser" "$DOMAIN" "$PROBLEM"
status=$?
if [[ $status -ne 0 ]]; then
	echo "Error: The parser encountered a problem." >&2
	exit $status
fi

if $exec_repl; then
	make PREFIX="$CURRENT_DIR" -C "$SCRIPT_DIR" repl
	status=$?
	if [[ $status -ne 0 ]]; then
		echo "Error: The 'make repl' command failed." >&2
		exit $status
	fi

	REPL_EXEC="$CURRENT_DIR/repl"
	if [[ -f "$REPL_EXEC" ]]; then
		"$REPL_EXEC"
		status=$?
		if [[ $status -ne 0 ]]; then
			echo "Error: The 'repl' command failed." >&2
			exit $status
		fi
	else
		echo "Error: The 'repl' executable not found at $REPL_EXEC." >&2
		exit 1
	fi
fi
