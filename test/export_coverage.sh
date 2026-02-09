#!/bin/sh

# Currently just a manual thing, but I will automate it eventually...
if [ -f "./coverage_report/test_run/index.txt" ]; then
  cat coverage_report/test_run/index.txt | sed 's/\x1b\[[0-9;]*[mGKH]//g' | grep -E '^lib/[a-zA-Z_-]+\.c' \
    | sed -E 's/[[:space:]]+/ /g' | cut -d' ' -f2,3 \
    | awk '{ total_sum += $1; missed_sum += $2 } END { printf "%.2f%%\n", (missed_sum / total_sum) * 100 }' \
    | sed -E 's/^/{"coverage": "/' | sed -E 's/$/"}/' > ./test_coverage.json
fi