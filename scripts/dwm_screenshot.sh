#!/bin/sh

set -eo pipefail

maim -s \
	| tee ~/Pictures/Screenshots/$(date '+%Y-%m-%d_%H-%M-%S')_screenshot.png \
	| xclip -selection clipboard -t image/png

notify-send 'Screenshot Saved' 'Image copied to the Clipboard'
