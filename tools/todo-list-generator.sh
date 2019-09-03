#!/bin/sh
set -e

## Copied from github (https://gist.github.com/frebib/f04b39c73b953ca9623f777029c4b4b9)
## Automatically generates a todo list before every commit

# This script requires ripgrep and sed.
# Save as .git/hooks/pre-commit for automatic TODO list updates when committing to git.

echo "Updating TODO list"

rg \
    --heading \
    --sort-files \
    --line-number \
    -g '!TODO.md' \
    'TODO' \
    | sed  \
        -e '/\(^[0-9]\|^\s*$\)/!s/^\s*\(.*\)\s*$/## [\1]\(\1\)/g' \
        -e '/^\s*##\s*$/s///g' \
        -e 's|\s*//\s*TODO:\s*| |g' \
        -e '/^[0-9]/s/^/  - Line /g' \
        -e '1i# A list of every _todo_ comment in this project\n**Please note:** _This file is automatically generated from all TODO comments found within the code_' \
    > TODO.md

git diff TODO.md | cat

git add TODO.md