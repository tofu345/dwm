#!/bin/sh

# Originally By: Jake@Linux https://gitlab.com/jped/scripts/-/blob/main/volume?ref_type=heads

vol="$(pamixer --get-volume)"
[[ $(pamixer --get-mute) = true ]] && echo -n "^c#f7768e^"

if [[ "$vol" -gt 66 ]]; then
    icon="󰕾"
elif [[ "$vol" -gt 33 ]]; then
    icon="󰖀"
else
    icon="󰕿"
fi

echo "$icon $vol^d^"
