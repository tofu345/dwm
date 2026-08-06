/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#include "exitdwm.c"
#include "movestack.c"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 3;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, 0: display systray on the last monitor */
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 0;        /* horizontal padding for statusbar */
static const int vertpadbar         = 10;       /* vertical padding for statusbar */

static const char font[]            = "CaskaydiaMono Nerd Font:style=Bold:size=9";
static const char *fonts[]          = { font };

/* https://github.com/tonybanters/dwm/blob/master/config.h */
static const char col_bg[]     = "#101010";  // background
static const char col_fg[]     = "#a9b1d6";  // foreground
static const char col_sel[]    = "#ffffff";  // foreground selected
static const char col_blk[]    = "#000000";  // black

static const char *colors[][3] = {
    /*                        fg          bg          border (cannot be empty) */
    [SchemeNorm]        = {   col_fg,     col_blk,    "#101014"  }, // Unselected windows
    [SchemeSel]         = {   col_sel,    "#101014",  "#353743"  }, // Selected window
    [SchemeStatus]      = {   col_fg,     col_blk,    col_blk    }, // Statusbar right
    [SchemeTagsSel]     = {   col_sel,    col_blk,    col_blk    }, // Tagbar left selected
    [SchemeTagsNorm]    = {   col_fg,     col_blk,    col_blk    }, // Tagbar left unselected
    [SchemeInfoSel]     = {   col_fg,     col_blk,    col_blk    }, // infobar middle selected
    [SchemeInfoNorm]    = {   col_fg,     col_blk,    col_blk    }, // infobar middle unselected
    [SchemeTabActive]   = {   col_fg,     col_blk,    col_blk    }, // active tab group
    [SchemeTabInactive] = {   col_fg,     col_blk,    col_blk    }  // inactive tab group
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class            instance    title       tags mask     iscentered    isfloating  monitor */
    { "Nemo",           NULL,       NULL,       0,            1,            1,          -1 },
    { "Gcr-prompter",   NULL,       NULL,       0,            1,            1,          -1 },
};

/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 60;   /* refresh rate (per second) for client move/resize */

/* Bartabgroups properties */
#define BARTAB_BORDERS 0       // 0 = off, 1 = on
#define BARTAB_BOTTOMBORDER 0  // 0 = off, 1 = on
#define BARTAB_TAGSINDICATOR 0 // 0 = off, 1 = on if >1 client/view tag, 2 = always on
#define BARTAB_TAGSPX 0        // # pixels for tag grid boxes
#define BARTAB_TAGSROWS 0      // # rows in tag grid (9 tags, e.g. 3x3)
static void (*bartabmonfns[])(Monitor *) = { monocle /* customlayoutfn */ };
static void (*bartabfloatfns[])(Monitor *) = { NULL /* customlayoutfn */ };

static const Layout layouts[] = {
    /* symbol   arrange function */
    { "[]=",    tile }, /* first entry is default */
    { "[M]",    monocle },
    { "[D]",    deck },
    { "><>",    NULL }, /* no layout function means floating behavior */
    { NULL,     NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,             {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,       {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,              {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,        {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", font, "-nb", col_blk, "-nf", col_fg, "-sb", col_blk, "-sf", col_sel, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *rofi[]     = { "rofi", "-modi", "drun,run", "-show", "drun", NULL };
static const char *browser[]  = { "chromium-bin", NULL };
static const char *files[]    = { "nemo", NULL };
static const char *tlp[]      = { "dwm-tlp", NULL };
static const char *screenshot[] = { "dwm-screenshot", NULL };

static const Key keys[] = {
    /* modifier            keycodes (in dvorak :p)  function        argument */
    { MODKEY,          /* Return */ 36,             spawn,          {.v = termcmd } },
    { MODKEY,               /* r */ 32,             spawn,          {.v = dmenucmd } },
    { MODKEY,               /* d */ 43,             spawn,          {.v = rofi } },
    { MODKEY|ShiftMask,     /* b */ 57,             spawn,          {.v = browser } },
    { MODKEY,               /* e */ 40,             spawn,          {.v = files } },
    { 0,                /* Print */ 107,            spawn,          {.v = screenshot } },
    { MODKEY|ControlMask,   /* p */ 27,             spawn,          {.v = tlp} },
    { MODKEY|ShiftMask,     /* w */ 59,             spawn,          SHCMD("alacritty -T 'wiremix' -e wiremix") },
    { MODKEY|ShiftMask,     /* v */ 60,             spawn,          SHCMD("xsel -bc") },
    { MODKEY|ShiftMask,     /* l */ 33,             spawn,          SHCMD("slock & sleep .5; xset dpms force off") },
    { 0,            /* AudioMute */ 121,            spawn,          SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle && dwmblocks-update vol" ) },
    { 0,     /* AudioLowerVolume */ 122,            spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%- && dwmblocks-update vol" ) },
    { 0,     /* AudioRaiseVolume */ 123,            spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%+ --limit 1.0 && dwmblocks-update vol") },
    { 0,      /* MonBrightnessUp */ 233,            spawn,          SHCMD("brightnessctl -q set +3%") },
    { 0,    /* MonBrightnessDown */ 232,            spawn,          SHCMD("brightnessctl -q --min-value=100 set 3-%") },

    { MODKEY,             /* Tab */ 23,             view,           {0} }, // previous tag
    { MODKEY,               /* z */ 61,             zoom,           {0} },
    { MODKEY|ShiftMask,     /* q */ 53,             killclient,     {0} },
    { MODKEY,               /* t */ 45,             setlayout,      {.v = &layouts[0]} }, // tiled
    { MODKEY,               /* m */ 58,             setlayout,      {.v = &layouts[1]} }, // monocle
    { MODKEY|ShiftMask,     /* d */ 43,             setlayout,      {.v = &layouts[2]} }, // deck
    { MODKEY,           /* space */ 65,             setlayout,      {.v = &layouts[3]} }, // floating
    { MODKEY,               /* f */ 29,             togglefullscr,  {0} },
    { MODKEY|ShiftMask, /* space */ 65,             togglefloating, {0} },
    { MODKEY,               /* s */ 47,             togglesticky,   {0} },
    { MODKEY,               /* b */ 57,             togglebar,      {0} },

    { MODKEY,               /* j */ 54,             focusstack,     {.i = +1 } },
    { MODKEY,               /* k */ 55,             focusstack,     {.i = -1 } },
    { MODKEY,               /* p */ 27,             incnmaster,     {.i = +1 } },
    { MODKEY|ShiftMask,     /* p */ 27,             incnmaster,     {.i = -1 } },
    { MODKEY,               /* x */ 56,             transfer,       {0} },
    { MODKEY,               /* h */ 44,             setmfact,       {.f = -0.05 } },
    { MODKEY,               /* l */ 33,             setmfact,       {.f = +0.05 } },
    { MODKEY|ShiftMask,     /* j */ 54,             setcfact,       {.f = -0.25} },
    { MODKEY|ShiftMask,     /* k */ 55,             setcfact,       {.f = +0.25} },
    { MODKEY|ShiftMask,     /* o */ 39,             setcfact,       {.f =  0.00} },
    { MODKEY|ControlMask,   /* j */ 54,             movestack,      {.i = +1 } },
    { MODKEY|ControlMask,   /* k */ 55,             movestack,      {.i = -1 } },

    { MODKEY|ControlMask|ShiftMask,
                            /* q */ 53,             exitdwm,       {0} },

    { MODKEY,           /* comma */ 25,             focusmon,       {.i = -1 } },
    { MODKEY,          /* period */ 26,             focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask, /* comma */ 25,             tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,/* period */ 26,             tagmon,         {.i = +1 } },

    // { MODKEY,                /* 0 */ ,            view,           {.ui = ~0 } },
    // { MODKEY|ShiftMask,      /* 0 */ ,            tag,            {.ui = ~0 } },

    //       key  code  tag
    TAGKEYS(/* 1 */ 10, 0),
    TAGKEYS(/* 2 */ 11, 1),
    TAGKEYS(/* 3 */ 12, 2),
    TAGKEYS(/* 4 */ 13, 3),
    TAGKEYS(/* 5 */ 14, 4),
    TAGKEYS(/* 6 */ 15, 5),
    TAGKEYS(/* 7 */ 16, 6),
    TAGKEYS(/* 8 */ 17, 7),
    TAGKEYS(/* 9 */ 18, 8),
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask          button          function        argument */
    { ClkTagBar,            MODKEY,             Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,             Button3,        toggletag,      {0} },
    { ClkLtSymbol,          0,                  Button1,        layoutmenu,     {0} },
    // { ClkWinTitle,          0,                  Button2,        zoom,           {0} },
    { ClkStatusText,        0,                  Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,             Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,             Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY|ShiftMask,   Button1,        resizemouse,    {0} },
    { ClkTagBar,            0,                  Button1,        view,           {0} },
    { ClkTagBar,            0,                  Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,             Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,             Button3,        toggletag,      {0} },
};

static const char layoutmenu_cmd[] = "dwm-layoutmenu";

// vim:expandtab
