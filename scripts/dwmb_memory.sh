#!/bin/sh

free -h | awk '/^Mem/{
	gsub("i", ""); # Gi -> G

	# a bit of a hack to avoid comparing usage in Mb to Gb
	if (index($3, "G") != 0) {
		used = gensub(/G/, "", 1, $3)
		avail = gensub(/G/, "", 1, $2)
		if (used > (0.75 * avail)) {
			printf "^c#e0af68^"
		} else if (used > (0.85 * avail)) {
			printf "^c#f7768e^"
		}
	}
	print " " $3 "/" $2 "^d^"
}'
