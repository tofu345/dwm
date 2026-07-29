/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
    /*                      fg         bg         border   */
    [SchemeNorm]        = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]         = { col_gray4, col_cyan,  col_cyan  },
    [SchemeTabActive]   = { col_gray2, col_gray3,  col_gray2 },
    [SchemeTabInactive] = { col_gray1, col_gray3,  col_gray1 }
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask  iscentered  isfloating  isterminal  noswallow  monitor */
    { "Gimp",     NULL,       NULL,       0,         0           1,          0,           0,        -1 },
    { "Firefox",  NULL,       NULL,       1 << 8,    0           0,          0,          -1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

/* Bartabgroups properties */
#define BARTAB_BORDERS 1       // 0 = off, 1 = on
#define BARTAB_BOTTOMBORDER 1  // 0 = off, 1 = on
#define BARTAB_TAGSINDICATOR 1 // 0 = off, 1 = on if >1 client/view tag, 2 = always on
#define BARTAB_TAGSPX 5        // # pixels for tag grid boxes
#define BARTAB_TAGSROWS 3      // # rows in tag grid (9 tags, e.g. 3x3)
static void (*bartabmonfns[])(Monitor *) = { monocle /* , customlayoutfn */ };
static void (*bartabfloatfns[])(Monitor *) = { NULL /* , customlayoutfn */ };

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
    { "[D]",      deck },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *layoutmenu_cmd = "layoutmenu.sh";

#include "movestack.c"
static const Key keys[] = {
    /* modifier             keycode (in qwerty)     function        argument */
    { MODKEY,                       /* p */ 33,     spawn,          {.v = dmenucmd } },
    { MODKEY|ShiftMask,        /* Return */ 36,     spawn,          {.v = termcmd } },
    { MODKEY,                       /* b */ 56,     togglebar,      {0} },
    { MODKEY,                       /* j */ 44,     focusstack,     {.i = +1 } },
    { MODKEY,                       /* k */ 45,     focusstack,     {.i = -1 } },
    { MODKEY,                       /* i */ 31,     incnmaster,     {.i = +1 } },
    { MODKEY,                       /* d */ 40,     incnmaster,     {.i = -1 } },
    { MODKEY,                       /* h */ 43,     setmfact,       {.f = -0.05} },
    { MODKEY,                       /* l */ 46,     setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             /* h */ 43,     setcfact,       {.f = +0.25} },
    { MODKEY|ShiftMask,             /* l */ 46,     setcfact,       {.f = -0.25} },
    { MODKEY|ControlMask,           /* j */ 44,     movestack,      {.i = +1 } },
    { MODKEY|ControlMask,           /* k */ 45,     movestack,      {.i = -1 } },
    { MODKEY|ShiftMask,             /* o */ 32,     setcfact,       {.f =  0.00} },
    { MODKEY,                  /* Return */ 36,     zoom,           {0} },
    { MODKEY,                     /* Tab */ 23,     view,           {0} },
    { MODKEY|ShiftMask,             /* c */ 54,     killclient,     {0} },
    { MODKEY,                       /* t */ 28,     setlayout,      {.v = &layouts[0]} },
    { MODKEY,                       /* f */ 41,     setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       /* m */ 58,     setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       /* c */ 54,     setlayout,      {.v = &layouts[3]} },
    { MODKEY,                   /* space */ 65,     setlayout,      {0} },
    { MODKEY|ShiftMask,         /* space */ 65,     togglefloating, {0} },
    { MODKEY,                       /* 0 */ 19,     view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             /* 0 */ 19,     tag,            {.ui = ~0 } },
    { MODKEY,                   /* comma */ 59,     focusmon,       {.i = -1 } },
    { MODKEY,                  /* period */ 60,     focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,         /* comma */ 59,     tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,        /* period */ 60,     tagmon,         {.i = +1 } },
    TAGKEYS(                        /* 1 */ 10,                      0)
    TAGKEYS(                        /* 2 */ 11,                      1)
    TAGKEYS(                        /* 3 */ 12,                      2)
    TAGKEYS(                        /* 4 */ 13,                      3)
    TAGKEYS(                        /* 5 */ 14,                      4)
    TAGKEYS(                        /* 6 */ 15,                      5)
    TAGKEYS(                        /* 7 */ 16,                      6)
    TAGKEYS(                        /* 8 */ 17,                      7)
    TAGKEYS(                        /* 9 */ 18,                      8)
    { MODKEY|ShiftMask,             /* e */ 26,      exitdwm,        {0} },
    { MODKEY|ControlMask|ShiftMask, /* q */ 24,      quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        layoutmenu,     {0} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

// vim:expandtab
