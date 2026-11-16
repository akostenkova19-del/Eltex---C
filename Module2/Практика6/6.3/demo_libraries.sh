#!/bin/sh

make

printf '10\n2\n+\n-\n*\n/\nq\n' | ./calc

mv libs/add.so libs/add.so.disabled

printf '\n\nRemove add:\n\n'

printf '10\n2\n+\n-\n*\n/\nq\n' | ./calc

mv libs/add.so.disabled libs/add.so
