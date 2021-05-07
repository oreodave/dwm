/* See LICENSE file for copyright and license details. */

#define true 1
#define false 0

#define GTMask(X) 1 << (X - 1)

/* appearance */
static const unsigned int borderpx  = 2;       /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const int showbar            = true;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "IBM Plex Sans:size=11" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_black[]       = "#161616";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_blue[]       = "#0217FC";
static const char col_blue2[]      = "#6e95ff";
static const char col_red[]         = "#910d01";
static const char *colors[][3]      = {
	/* Scheme            fg         bg         border   */
	[SchemeNorm]     = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]      = { col_gray4, col_cyan,  col_blue  },
	[SchemeStatus]   = { col_blue2, "#222222",  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { col_blue2, col_black,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm] = { col_gray3, col_gray1,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]  = { col_gray4, col_black,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm] = { col_gray3, col_gray1,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "6", "7", "8", "9" };

/* Custom functions */
static void resetgaps(const Arg *arg);
static void printgaps(const Arg *arg);

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title       tags mask     isfloating   monitor */
	{ "Gimp",           NULL,       NULL,       0,            1,           -1 },
	{ "qutebrowser",    NULL,       NULL,       GTMask(2),    0,           -1 },
	{ "mpv",            NULL,       NULL,       GTMask(3),    0,           -1 },
	{ "media-term",     NULL,       NULL,       GTMask(3),    0,           -1 },
	{ "Zathura",        NULL,       NULL,       GTMask(4),    0,           -1 },
	{ "Dev",            NULL,       NULL,       GTMask(5),    0,           -1 },
	{ "firefox",        NULL,       NULL,       GTMask(2),    0,           -1 },
	{ "chromium",       NULL,       NULL,       GTMask(2),    0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55;    /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]=",      tile },    /* first entry is default */
	{ "<F>=",      NULL },    /* no layout function means floating behavior */
	{ "{M}",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-T", scratchpadname, "-e", "tmux",
  NULL };

static char *statuscmds[] = { "notify-send Mouse$BUTTON" };
static char *statuscmd[] = { "/bin/sh", "-c", NULL, NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_comma,  setgaps,        {.i = -1} },
	{ MODKEY,                       XK_period, setgaps,        {.i = +1 } },
	{ MODKEY,                       XK_slash,  resetgaps,      {0} },
	{ MODKEY|ShiftMask,             XK_slash,  printgaps,      {0} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY|ControlMask,           XK_period, incnmaster,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_comma,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_c,      zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_i,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_d,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

void
resetgaps(const Arg *arg)
{
  if (!selmon)
    return;
  selmon->gappx = gappx;
  arrange(selmon);
}

void
printgaps(const Arg *arg)
{
  if (!selmon)
    return;
  char *cmd = malloc(sizeof(*cmd) * 24);
  sprintf(cmd, "notify-send -u low \"Gaps=%d\"", selmon->gappx);
  system(cmd);
}
