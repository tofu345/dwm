#!/bin/sh

xset s 1200 5 
xset dpms 1210 1210 1210
xset r rate 250 25

xss-lock -n ~/builds/xss-lock-dim-screen2.sh -- slock &
dunst &
dwmblocks &
gammastep -O 3000 &
picom -b &
xwallpaper --zoom ~/Pictures/Wallpapers/wallhaven-christ.png --daemon &
/usr/libexec/polkit-mate-authentication-agent-1 &
playerctld daemon &
