/* See LICENSE file for copyright and license details. */
#define true 1
#define false 0

#define GTMask(X) 1 << (X - 1)
#define STATUSBAR "dwmblocks"

/* appearance */
static const unsigned int borderpx = 0;    /* border pixel of windows */
static const unsigned int snap     = 16;   /* snap pixel */
static const int default_gaps      = 50;   /* default value of gaps_previous */
static const int showbar           = true; /* 0 means no bar */
static const int topbar            = 0;    /* 0 means bottom bar */
static const char *fonts[]         = { "Noto Sans Mono:size=12" };
static const char dmenufont[]      = "monospace:size=9";
static const char col_black[]      = "#000000";
static const char col_gray1[]      = "#222222";
static const char col_gray2[]      = "#444444";
static const char col_gray3[]      = "#bbbbbb";
static const char col_gray4[]      = "#eeeeee";
static const char col_cyan[]       = "#005577";
static const char col_lblue[]      = "#77aafc";
static const char col_dblue[]      = "#225599";
static const char col_lgreen[]     = "#55f055";
static const char col_dgreen[]     = "#008000";
static const char *colors[][3]     = {
	/* Scheme            fg          bg           border   */
	[SchemeNorm]     = { col_gray3,  col_black,   col_black },
	[SchemeSel]      = { col_gray4,  col_cyan,    "#230142" },
	[SchemeStatus]   = { col_lblue,  col_black,   col_black },
	[SchemeTagsSel]  = { col_gray4,  col_dblue,   col_black },
	[SchemeTagsNorm] = { col_lblue,  col_black,   col_black },
	[SchemeInfoSel]  = { col_gray4,  col_black,   col_black },
	[SchemeInfoNorm] = { col_gray3,  col_black,   col_black },
};

/* tagging */

static const char *tags[] = { "", "", "", "", "", "6", "7", "8", "9" };

/* Custom functions */
static void togglegaps(const Arg *arg);
static void printgaps(const Arg *arg);

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance   title   tags mask     isfloating   monitor */
	{ "Gimp",        NULL,      NULL,   0,            1,           -1 },
	{ "mpv",         NULL,      NULL,   GTMask(3),    0,           -1 },
	{ "Onboard",     NULL,      NULL,   0,            0,           -1 },
	{ "firefox",     NULL,      NULL,   GTMask(2),    0,           -1 },
	{ "Chromium",    NULL,      NULL,   GTMask(2),    0,           -1 },
	{ "Spotify",     NULL,      NULL,   GTMask(3),    0,           -1 },
	{ "Zathura",     NULL,      NULL,   GTMask(4),    0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "./fibonacci.c"
#include "./gaplessgrid.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]=",     tile },    /* first entry is default */
	{ "<F>=",     NULL },    /* no layout function means floating behavior */
	{ "{M}",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "###",      gaplessgrid },
	{ "[D]",      deck },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-e", "tmux", NULL };

static char *statuscmds[] = { "notify-send Mouse$BUTTON" };

static const Key keys[] = {
	/* modifier            key        function        argument */
	{ MODKEY,              XK_b,      togglebar,      {0} },
	{ MODKEY,              XK_grave,  togglescratch,  {.v = scratchpadcmd} },
	{ MODKEY,              XK_j,      focusstack,     {.i = +1} },
	{ MODKEY,              XK_k,      focusstack,     {.i = -1} },
	{ MODKEY,              XK_period, setgaps,        {.i = +5} },
	{ MODKEY,              XK_comma,  setgaps,        {.i = -5} },
	{ MODKEY|ControlMask,  XK_period, setgaps,        {.i = +1} },
	{ MODKEY|ControlMask,  XK_comma,  setgaps,        {.i = -1} },
	{ MODKEY,              XK_slash,  togglegaps,     {0} },
	{ MODKEY|ShiftMask,    XK_slash,  printgaps,      {0} },
	{ MODKEY,              XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,              XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY|ControlMask,  XK_period, incnmaster,     {.i = -1 } },
	{ MODKEY|ControlMask,  XK_comma,  incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_k,      inplacerotate,  {.i = +1} },
	{ MODKEY|ShiftMask,    XK_j,      inplacerotate,  {.i = -1} },
	{ MODKEY|ShiftMask,    XK_h,      inplacerotate,  {.i = -2} },
	{ MODKEY|ShiftMask,    XK_l,      inplacerotate,  {.i = +2} },
	{ MODKEY|ControlMask,  XK_l,      movekeyboard_x, {.i = 32}},
	{ MODKEY|ControlMask,  XK_h,      movekeyboard_x, {.i = -32}},
	{ MODKEY|ControlMask,  XK_j,      movekeyboard_y, {.i = 32}},
	{ MODKEY|ControlMask,  XK_k,      movekeyboard_y, {.i = -32}},
	{ MODKEY|ControlMask,  XK_s,      togglesticky,   {0} },
	{ MODKEY,              XK_c,      zoom,           {0} },
	{ MODKEY|ShiftMask,    XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,    XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,    XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,    XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,    XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,    XK_s,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,    XK_D,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY|ShiftMask,    XK_g,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY|ShiftMask,    XK_d,      setlayout,      {.v = &layouts[8]} },
	{ MODKEY|ShiftMask,    XK_space,  setlayout,      {0} },
	{ MODKEY,              XK_space,  togglefloating, {0} },
	{ MODKEY,              XK_n,      focusmon,       {.i = -1 } },
	{ MODKEY,              XK_m,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,              XK_Tab,    view,           {.ui = 0} },
	{ MODKEY,              XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(               XK_1,                      0)
	TAGKEYS(               XK_2,                      1)
	TAGKEYS(               XK_3,                      2)
	TAGKEYS(               XK_4,                      3)
	TAGKEYS(               XK_5,                      4)
	TAGKEYS(               XK_6,                      5)
	TAGKEYS(               XK_7,                      6)
	TAGKEYS(               XK_8,                      7)
	TAGKEYS(               XK_9,                      8)
	{ MODKEY,              XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,    XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click        event mask  button    function        argument */
	{ ClkLtSymbol,  0,          Button1,  setlayout,      {0} },
	{ ClkLtSymbol,  0,          Button3,  setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,  0,          Button2,  zoom,           {0} },
	{ ClkClientWin, MODKEY,     Button1,  movemouse,      {0} },
	{ ClkClientWin, MODKEY,     Button2,  togglefloating, {0} },
	{ ClkClientWin, MODKEY,     Button3,  resizemouse,    {0} },
	{ ClkTagBar,    0,          Button1,  view,           {0} },
	{ ClkTagBar,    0,          Button3,  toggleview,     {0} },
	{ ClkTagBar,    MODKEY,     Button1,  tag,            {0} },
	{ ClkTagBar,    MODKEY,     Button3,  toggletag,      {0} },
};

void
togglegaps(const Arg *arg)
{
	if (!selmon)
		return;
	int *gaps_current = selmon->pertag->gaps_current + selmon->pertag->curtag;
	int *gaps_previous = selmon->pertag->gaps_previous + selmon->pertag->curtag;
	// Swap the current gaps and the previous gaps
	int temp = *gaps_current;
	*gaps_current = *gaps_previous;
	*gaps_previous = temp;
	arrange(selmon);
}

void
printgaps(const Arg *arg)
{
  if (!selmon)
    return;
  char *cmd = malloc(sizeof(*cmd) * 38);
  sprintf(cmd, "notify-send -u low \"Gaps=%d\"",
					selmon->pertag->gaps_current[selmon->pertag->curtag]);
  system(cmd);
	free(cmd);
}
