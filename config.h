/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 3;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, 0: display systray on the last monitor */
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 0;        /* horizontal padding for statusbar */
static const int vertpadbar         = 6;		/* vertical padding for statusbar */

static const char font[]			= "CaskaydiaMono Nerd Font:style=Bold:size=10";
static const char *fonts[]          = { font };

/* https://github.com/tonybanters/dwm/blob/master/config.h */
static const char col_bg[]     = "#101010";  // background
static const char col_fg[]     = "#a9b1d6";  // foreground
static const char col_wht[]    = "#ffffff";  // foreground
static const char col_blk[]    = "#000000";  // black (normal)
static const char col_red[]    = "#f7768e";  // red
static const char col_grn[]    = "#9ece6a";  // green
static const char col_ylw[]    = "#e0af68";  // yellow
static const char col_blu[]    = "#7aa2f7";  // blue
static const char col_mag[]    = "#ad8ee6";  // magenta
static const char col_cyn[]    = "#0db9d7";  // cyan (highlight)
static const char col_brblk[]  = "#444b6a";  // bright black

static const char *colors[][3] = {
    /*                      fg          bg          border */
    [SchemeNorm]        = { col_fg,     col_bg,     col_blk },
    [SchemeSel]         = { col_wht,    col_bg,     "#3b3b3b"  },
    [SchemeStatus]      = { col_wht,    col_bg,     "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
    [SchemeTagsSel]     = { col_wht,    col_bg,     "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]    = { col_fg,     col_bg,     "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]     = { col_fg,     col_bg,     "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]    = { col_fg,     col_bg,     "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class					instance    title       tags mask     iscentered	isfloating   monitor */
	// { "Gimp",				NULL,       NULL,       0,            0,           1,           -1 },
	// { "Firefox",				NULL,       NULL,       1 << 8,       0,           0,           -1 },
    { "org.gnome.Nautilus",     NULL,       NULL,       0,            1,			1,           -1 },
    { "Gcr-prompter",			NULL,       NULL,       0,            1,			1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "[M]",      monocle },
    { "><>",      NULL },    /* no layout function means floating behavior */
    { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      comboview,        {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,       {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      combotag,         {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,        {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", font, "-nb", col_bg, "-nf", col_fg, "-sb", col_bg, "-sf", col_wht, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *rofi[]     = { "rofi", "-modi", "drun,run", "-show", "drun", NULL };
static const char *helium[]   = { "helium-browser", NULL };
static const char *filebrowser[] = { "nautilus", NULL };

static const Key keys[] = {
    /* modifier             key                         function        argument */
    { MODKEY,               XK_Return,                  spawn,          {.v = termcmd } },
    { MODKEY,               XK_r,                       spawn,          {.v = dmenucmd } },
    { MODKEY,               XK_d,                       spawn,          {.v = rofi } },
    { MODKEY|ShiftMask,     XK_h,                       spawn,          {.v = helium } },
    { MODKEY,               XK_e,                       spawn,          {.v = filebrowser } },
    { 0,                    XK_Print,                   spawn,          SHCMD("maim -s | tee ~/Pictures/Screenshots/$(date '+%Y-%m-%d_%H-%M-%S')_screenshot.png | xclip -selection clipboard -t image/png") },
    { MODKEY|ShiftMask,     XK_v,                       spawn,          SHCMD("xsel -bc") },
    { MODKEY|ShiftMask,     XK_l,                       spawn,          SHCMD("xset s activate") },
    { 0,                    XF86XK_AudioMute,           spawn,          SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle && pkill -RTMIN+1 dwmblocks") },
    { 0,                    XF86XK_AudioLowerVolume,    spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%- && pkill -RTMIN+1 dwmblocks") },
    { 0,                    XF86XK_AudioRaiseVolume,    spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+ --limit 1.0 && pkill -RTMIN+1 dwmblocks") },
    { 0,					XF86XK_MonBrightnessUp,		spawn,	        SHCMD("xbacklight -steps 25 +10") },
    { 0,					XF86XK_MonBrightnessDown,	spawn,	        SHCMD("[ `xbacklight -get` -gt 10 ] && xbacklight -steps 25 -10") },

    { MODKEY,               XK_Tab,                     view,           {0} }, // previous tag
    { MODKEY,               XK_z,                       zoom,           {0} },
    { MODKEY|ShiftMask,     XK_q,                       killclient,     {0} },
    { MODKEY,               XK_t,                       setlayout,      {.v = &layouts[0]} }, // tiled
    { MODKEY,               XK_m,                       setlayout,      {.v = &layouts[1]} }, // monocle
    { MODKEY,               XK_space,                   setlayout,      {.v = &layouts[2]} }, // floating
	{ MODKEY,               XK_f,						togglefullscr,  {0} },
    { MODKEY|ShiftMask,     XK_space,                   togglefloating, {0} },
    { MODKEY,               XK_s,                       togglesticky,   {0} },
    { MODKEY,               XK_b,                       togglebar,      {0} },
    { MODKEY,               XK_j,                       focusstack,     {.i = +1 } },
    { MODKEY,               XK_k,                       focusstack,     {.i = -1 } },
    { MODKEY,               XK_p,                       incnmaster,     {.i = +1 } },
    { MODKEY|ShiftMask,     XK_p,                       incnmaster,     {.i = -1 } },
    { MODKEY,               XK_x,                       transfer,       {0} },
    { MODKEY,               XK_h,                       setmfact,       {.f = -0.05 } },
    { MODKEY,               XK_l,                       setmfact,       {.f = +0.05 } },
    // { MODKEY,               XK_comma,					focusmon,       {.i = -1 } },
    // { MODKEY,               XK_period,					focusmon,       {.i = +1 } },
    // { MODKEY|ShiftMask,     XK_comma,					tagmon,         {.i = -1 } },
    // { MODKEY|ShiftMask,     XK_period,					tagmon,         {.i = +1 } },
    // { MODKEY,               XK_0,						view,           {.ui = ~0 } },
    // { MODKEY|ShiftMask,     XK_0,						tag,            {.ui = ~0 } },

    { Mod1Mask|ControlMask, XK_Delete,                  quit,           {0} }, // Control-Alt-Delete

    // real prog dvorak L https://github.com/ThePrimeagen/keyboards
    TAGKEYS(XK_plus,         0),
    TAGKEYS(XK_bracketleft,  1),
    TAGKEYS(XK_braceleft,    2),
    TAGKEYS(XK_parenleft,    3),
    TAGKEYS(XK_ampersand,    4),
    TAGKEYS(XK_equal,        5),
    TAGKEYS(XK_parenright,   6),
    TAGKEYS(XK_braceright,   7),
    TAGKEYS(XK_bracketright, 8),
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask          button          function        argument */
    { ClkTagBar,            MODKEY,             Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,             Button3,        toggletag,      {0} },
	{ ClkLtSymbol,          0,					Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkStatusText,        0,                  Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,             Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,             Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY|ShiftMask,   Button1,        resizemouse,    {0} },
    { ClkTagBar,            0,                  Button1,        view,           {0} },
    { ClkTagBar,            0,                  Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,             Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,             Button3,        toggletag,      {0} },
};
