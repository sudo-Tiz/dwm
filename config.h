/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"
#define SESSION_FILE "/tmp/dwm-session"

/* Appearance */
static unsigned int borderpx = 2; /* border pixel of windows */
static unsigned int snap = 10;    /* snap pixel */
static const unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray = 1; /* 0 means no systray */
static unsigned int gappih = 10;  /* horiz inner gap between windows */
static unsigned int gappiv = 10;   /* vert inner gap between windows */
static unsigned int gappoh = 10; /* horiz outer gap between windows and screen edge */
static unsigned int gappov = 10; /* vert outer gap between windows and screen edge */
static int swallowfloating = 0; /* 1 means swallow floating windows by default */
static int smartgaps = 1; /* 1 means no outer gap when there is only one window */
static int showbar = 1; /* 0 means no bar */
static int topbar = 1;  /* 0 means bottom bar */
static const char slopspawnstyle[] = "-t 0 -c 0.92,0.85,0.69,0.3 -o"; /* do NOT define -f (format) here */
static const char slopresizestyle[] = "-t 0 -c 0.92,0.85,0.69,0.3"; /* do NOT define -f (format) here */
static const int riodraw_borders = 0; /* 0 or 1, indicates whether the area drawn using slop includes the window borders */
static const int riodraw_matchpid = 1; /* 0 or 1, indicates whether to match the PID of the client that was spawned with riospawn */
static const int riodraw_spawnasync = 0; /* 0 means that the application is only spawned after a successful selection while 1 means that the application is being initialised in the background while the selection is made */

static char *fonts[] = {"monospace:size=10:antialias=true:autohint=true",
                        "JoyPixels:pixelsize=10:antialias=true:autohint=true"};

/* Bar background color */
static char normbgcolor[] = "#222222";
/* Bar foreground color */
static char normbordercolor[] = "#444444";
/* Selected foreground color */
static char normfgcolor[] = "#bbbbbb";
/* Light Border color used for focused windows */
static char selbordercolor[] = "#770000"; //"#8C9CA8";
/* Dark Border color used for unfocused windows */
static char selfgcolor[] = "#eeeeee";
/* Bar foreground color */
static char selbgcolor[] = "#005577";
static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor, selbgcolor, selbordercolor},
};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd0[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL};
const char *spcmd1[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL};
const char *spcmd2[] = {TERMINAL, "-n", "spvim", "-g", "120x34", "-e", "vim", NULL};
const char *spcmd3[] = {TERMINAL, "-n", "sptime", "-f", "monospace:size=26", "-g", "9x1", "-e", "watch", "-t", "-n", "1", "date", "+%H:%M:%S", NULL};
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm", spcmd0},
    {"spcalc", spcmd1},
    {"spvim", spcmd2},
    {"sptime", spcmd3},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class    instance      title                   tags mask 	isfloating  isterminal  noswallow   monitor */
    {"Gimp",    NULL,         NULL,                   1 << 8,     0,          0,          0,          -1},
    {NULL,      NULL,         "PlayOnLinux",          0,          1,          0,          0,          -1},
    {TERMCLASS, NULL,         NULL,                   0,          0,          1,          0,          -1},
    {NULL,      NULL,         "Event Tester",         0,          0,          0,          1,          -1},
    {TERMCLASS, "bg",         NULL,                   1 << 7,     0,          1,          0,          -1},
    {NULL,      NULL,         "zbar barcode reader",  0,          0,          0,          1,          -1},
    {NULL,      "spterm",     NULL,                   SPTAG(0),   1,          1,          0,          -1},
    {NULL,      "spcalc",     NULL,                   SPTAG(1),   1,          1,          0,          -1},
    {NULL,      "spvim",      NULL,                   SPTAG(2),   1,          1,          0,          -1},
    {NULL,      "sptime",     NULL,                   SPTAG(3),   1,          1,          0,          -1},
};

/* layout(s) */
static float mfact = 0.5;  /* factor of master area size [0.05..0.95] */
static int nmaster = 1;     /* number of clients in master area */
static int resizehints = 0; /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT                                                           \
  1 /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile},   /* Default: Master on left, slaves on right */
    {"TTT", bstack}, /* Master on top, slaves on bottom */

    {"[@]", spiral},   /* Fibonacci spiral */
    {"[\\]", dwindle}, /* Decreasing in size right and leftward */

    {"[D]", deck},    /* Master on left, slaves in monocle-like mode on right */
    {"[M]", monocle}, /* All windows on top of eachother */

    {"|M|", centeredmaster},         /* Master in middle, slaves on sides */
    {">M>", centeredfloatingmaster}, /* Same but master floats */

    {"><>", NULL}, /* no layout function means floating behavior */
    {NULL, NULL},
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
  {MODKEY|ControlMask, KEY, toggleview, {.ui = 1 << TAG}},                   \
  {MODKEY|ShiftMask, KEY, tag, {.ui = 1 << TAG}},                            \
  {MODKEY|ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},
#define STACKKEYS(MOD, ACTION)                                                 \
  {MOD,        XK_j, ACTION##stack, {.i = INC(+1)}},                                  \
  {MOD,        XK_k, ACTION##stack, {.i = INC(-1)}},                                  \
/* { MOD,         XK_v,   ACTION##stack,  {.i = 0 } }, \ */ // TODO To ADD ?
/* { MOD,        XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
/* { MOD,        XK_a,     ACTION##stack, {.i = 1 } }, \ */
/* { MOD,        XK_z,     ACTION##stack, {.i = 2 } }, \ */
/* { MOD,        XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/bash", "-c", cmd, NULL }                     \
  }

/* commands */
static const char *termcmd[] = {TERMINAL, NULL};

/* Xresources preferences to load at startup */
ResourcePref resources[] = {
    {"dwm.color0", STRING, &normbordercolor},
    {"dwm.color9", STRING, &selbordercolor},
    {"dwm.color0", STRING, &normbgcolor},
    {"dwm.color7", STRING, &normfgcolor},
    {"dwm.color0", STRING, &selfgcolor},
    {"dwm.color9", STRING, &selbgcolor},

    {"borderpx", INTEGER, &borderpx},
    {"snap", INTEGER, &snap},
    {"showbar", INTEGER, &showbar},
    {"topbar", INTEGER, &topbar},
    {"nmaster", INTEGER, &nmaster},
    {"resizehints", INTEGER, &resizehints},
    {"mfact", FLOAT, &mfact},
    {"gappih", INTEGER, &gappih},
    {"gappiv", INTEGER, &gappiv},
    {"gappoh", INTEGER, &gappoh},
    {"gappov", INTEGER, &gappov},
    {"swallowfloating", INTEGER, &swallowfloating},
    {"smartgaps", INTEGER, &smartgaps},
};

#include "shiftview.c"
#include <X11/XF86keysym.h>

static Key
    keys[] =
        {
    STACKKEYS(MODKEY, focus) STACKKEYS(MODKEY|ShiftMask, push)
    /* J and K are automatically bound above in STACKEYS */

    /* Following comment is used to parse the file for the documentation */
/* BEGINNING OF THE DOC */

    /* [LAYOUTS] */
    {MODKEY,                  XK_t, setlayout, {.v = &layouts[0]}},                          /* tile */
    {MODKEY|ShiftMask,        XK_t, setlayout, {.v = &layouts[1]}},                          /* bstack */
    {MODKEY,                  XK_y, setlayout, {.v = &layouts[2]}},                          /* spiral */
    {MODKEY|ShiftMask,        XK_y, setlayout, {.v = &layouts[3]}},                          /* dwindle */
    {MODKEY,                  XK_u, setlayout, {.v = &layouts[4]}},                          /* deck */
    {MODKEY|ShiftMask,        XK_u, setlayout, {.v = &layouts[5]}},                          /* monocle */
    {MODKEY,                  XK_i, setlayout, {.v = &layouts[6]}},                          /* centeredmaster */
    {MODKEY|ShiftMask,        XK_i, setlayout, {.v = &layouts[7]}},                          /* centeredfloatingmaster */

    /* [TAGS] */
    /* Assign keys to tags ("containers") */
    /* AZERTY */
    TAGKEYS(XK_ampersand, 0)
    TAGKEYS(XK_eacute, 1)
    TAGKEYS(XK_quotedbl, 2)
    TAGKEYS(XK_apostrophe, 3)
    TAGKEYS(XK_parenleft, 4)
    TAGKEYS(XK_minus, 5)
    TAGKEYS(XK_egrave, 6)
    TAGKEYS(XK_underscore, 7)
    TAGKEYS(XK_ccedilla,8)
    {MODKEY,                  XK_agrave, view, {.ui = ~0}},

    /* QUERTY */
    TAGKEYS(XK_1, 0)
    TAGKEYS(XK_2, 1)
    TAGKEYS(XK_3, 2)
    TAGKEYS(XK_4, 3)
    TAGKEYS(XK_5, 4)
    TAGKEYS(XK_6, 5)
    TAGKEYS(XK_7, 6)
    TAGKEYS(XK_8, 7)
    TAGKEYS(XK_9,8)
    {MODKEY,                  XK_0, view, {.ui = ~0}},



    /* {MODKEY|ShiftMask,        XK_agrave, tag, {.ui = ~0}}, */
    /* Switch between active tag and last opened tag */
    {MODKEY,                  XK_Tab, view, {0}},
    /* Cycle through tags */
    {MODKEY|ShiftMask,        XK_Tab, shiftview, {.i = -1}},
    /* Go a tag left */
    {MODKEY,                  XK_Page_Up, shiftview, {.i = -1}},
    /* Go a tag right */
    {MODKEY,                  XK_Page_Down, shiftview, {.i = +1}},

    /* [MONITOR] */
    /* Go to left Monitor */
    {MODKEY,                  XK_Left, focusmon, {.i = -1}},
    /* Move windows to left Monitor */
    {MODKEY|ShiftMask,        XK_Left, tagmon, {.i = -1}},
    /* Go to right Monitor */
    {MODKEY,                  XK_Right, focusmon, {.i = +1}},
    /* Move windows to right Monitor */
    {MODKEY|ShiftMask,        XK_Right, tagmon, {.i = +1}},

    /* [WINDOWS] */
    /* Move window to tag on the left */
    {MODKEY|ShiftMask,        XK_Page_Up, shifttag, {.i = -1}},
    /* Move window to tag on the right */
    {MODKEY|ShiftMask,        XK_Page_Down, shifttag, {.i = +1}},
    /* Sticky windows */
    {MODKEY,                  XK_s, togglesticky, {0}},
    /* Increase the number of master windows */
    {MODKEY,                  XK_m, incnmaster, {.i = +1}},
    /* Decrease the number of master windows */
    {MODKEY|ShiftMask,        XK_m, incnmaster, {.i = -1}},
    /* Kill a window */
    {MODKEY,                  XK_q, killclient, {0}},
    /* Xkill a window */
    {MODKEY|ShiftMask,        XK_q, spawn, SHCMD("xkill")},
    /* Resize window left (make it smaller)*/
    {MODKEY,                  XK_h, setmfact, {.f = -0.05}},
    /* Resize window right (make it bigger) */
    {MODKEY,                  XK_l, setmfact, {.f = +0.05}},
    /* Resize window like Plan9's rio (Rio-Resize patch) */
    {MODKEY|ShiftMask,        XK_r, rioresize, {0}},
    /* Promote selected window to master */
    {MODKEY,                  XK_space, zoom, {0}},
    /* Toggle floating mode for a window */
    {MODKEY|ShiftMask,        XK_space, togglefloating, {0}},

    /* [GAPS] */
    /* Toggle Gaps */
    {MODKEY|ControlMask,      XK_a, togglegaps, {0}},
    /* Increase gaps */
    {MODKEY,                  XK_a, incrgaps,	{.i = +3 } },
    /* Decrease gaps */
    {MODKEY|ShiftMask,        XK_a, incrgaps,	{.i = -3 } },
    /* Reset gaps to default size */
    /* { MODKEY|ShiftMask,		    XK_z,		defaultgaps,	{0} }, */

    /* [DWM] */
    /* Toggle fullscreen */
    {MODKEY,                  XK_f, togglefullscr, {0}},
    /* Show/hide bar */
    {MODKEY|ShiftMask,        XK_f, togglebar, {0}},
    /* Restart dwm */
    {MODKEY,                  XK_asterisk, quit, {1}},
    /* Restart dwmblocks */
    {MODKEY|ShiftMask,        XK_asterisk, spawn, SHCMD("killall -q dwmblocks;setsid -f dwmblocks")},
    /* Close dwm */
    /* { MODKEY|ShiftMask,		XK_q,		quit,		{0}}, */
    /* Lock the screen */
    {MODKEY|ShiftMask,        XK_BackSpace, spawn, SHCMD("slock")},
    /* Hibernate */
    {0,                       XF86XK_Sleep, spawn, SHCMD("systemctl hibernate -i")},

    /* [SCRATCHPADS] */
    /* Open/close terminal in scratchpad */
    {MODKEY|ShiftMask,        XK_Return, togglescratch, {.ui = 0}},
    /* Open/close calc in scratchpad */
    {MODKEY,                  XK_KP_Multiply, togglescratch, {.ui = 1}},
    /* Open/close vim in scratchpad */
    {MODKEY|ShiftMask,        XK_v, togglescratch, {.ui = 2}},
    /* Open/close timer in scratchpad */
    {MODKEY,                  XK_KP_Divide, togglescratch, {.ui = 3}},


    /* [TERMINAL] */
    /* Launch a terminal */
    {MODKEY,                  XK_Return, spawn, {.v = termcmd}},
    /* Spawn a new floating terminal, like Plan9's rio (Rio-Resize patch) */
    {MODKEY|ControlMask,      XK_Return, riospawn, {.v = termcmd}},

    /* [MUSIC] */ 
    /* Play/Pause*/
    {MODKEY,                  XK_p, spawn, SHCMD("mpc toggle")},
    /* Pause all mpv players */
    {MODKEY|ShiftMask,        XK_p, spawn, SHCMD("mpc pause; pauseallmpv")},
    /* Previous track */
    {MODKEY,                  XK_comma, spawn, SHCMD("mpc prev")},
    /* Next track */
    {MODKEY|ShiftMask,        XK_comma, spawn, SHCMD("mpc next")},
    /* Toggle volume */
    {0,                       XF86XK_AudioMute, spawn, SHCMD( "wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof " "dwmblocks)")}, // amixer -qD pulse sset Master toggle; kill -44 $(pidof dwmblocks)
    /* Increase volume */
    {0,                       XF86XK_AudioRaiseVolume, spawn, SHCMD( "wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%+; kill -44 $(pidof " "dwmblocks)")}, // amixer -qD pulse sset Master 5%+; kill -44 $(pidof dwmblocks)
    /* Decrease volume */
    {0,                       XF86XK_AudioLowerVolume, spawn, SHCMD( "wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%-; kill -44 $(pidof " "dwmblocks)")}, // amixer -qD pulse sset Master 5%-; kill -44 $(pidof dwmblocks)


    /* [VIDEO] */
    /* Take a screenshot */
    {0,                       XK_Print, spawn, SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png")},
    /* Launch Maim menu for screenshot */
    {ShiftMask,               XK_Print, spawn, SHCMD("maimpick")},
    /* Open dmenu for screenshot and record */
    {MODKEY,                  XK_Print, spawn, SHCMD("dmenurecord")},
    /* Kill curent record  */
    {MODKEY|ShiftMask,        XK_Print, spawn, SHCMD("dmenurecord kill")},
    {MODKEY,                  XK_Delete, spawn, SHCMD("dmenurecord kill")},

    /* [SCREEN]  */
    /* Landscape mode */
    {MODKEY,                  XK_KP_Add, spawn, SHCMD("xrandr --output $(xrandr | grep 'connected primary' | awk " "'{print " "$1}') --rotate normal")},
    /* Rotate screen to the left */
    {MODKEY|ShiftMask,        XK_KP_Add, spawn, SHCMD("xrandr --output $(xrandr | grep 'connected primary' | awk " "'{print " "$1}') --rotate left")},
    /* Night mode */
    {MODKEY,                  XK_KP_Enter, spawn, SHCMD("redshift -P -O 4500 && xbacklight -set 1")},
    /* Day mode */
    {MODKEY|ShiftMask,        XK_KP_Enter, spawn, SHCMD("redshift -P -O 5500 && xbacklight -set 100")},
    /* Increase brightness */
    {0,                       XF86XK_MonBrightnessUp, spawn, SHCMD("xbacklight -inc 10")},
    /* Decrease brightness */
    {0,                       XF86XK_MonBrightnessDown, spawn, SHCMD("xbacklight -dec 10")},

    /* Toggle cooler booster*/
    {MODKEY,                  XK_KP_Subtract, spawn, SHCMD("sudo isw -b on")},
    {MODKEY|ShiftMask,        XK_KP_Subtract, spawn, SHCMD("sudo isw -b off")},
    /* Toggle Bluetooth service */
    {MODKEY|ShiftMask,        XK_b, spawn, SHCMD("toggle-bluetooth")},

    /* [LAUNCHER] */
    /* Launch dmenu */
    {MODKEY,                  XK_d, spawn, SHCMD("dmenu_run")},
    /* Launch System action dmenu */
    {MODKEY,                  XK_BackSpace, spawn, SHCMD("sysact")},
    /* Launch Monitoring dmenu */
    {MODKEY,                  XK_x, spawn, SHCMD("dmenumsi")},
    /* Launch Password dmenu */
    {MODKEY|ShiftMask,        XK_d, spawn, SHCMD("neopassmenu")},
    /* Launch Clipmenu  */
    {MODKEY,                  XK_c, spawn, SHCMD("clipmenu")},
    /* Launch Bookmarks dmenu */
    {MODKEY,                  XK_Insert, spawn, SHCMD("xdotool type $(grep -v '^#' ~/notes/bookmarks.md | dmenu " "-i -l 50 " "| cut -d' ' -f1)")},
    /* launch Notes dmenu */
    {MODKEY,                  XK_n, spawn, SHCMD(TERMINAL " -e dmenuvim ~/notes")},
    /* Launch Emoji dmenu */
    {MODKEY|ShiftMask,        XK_Insert, spawn, SHCMD("dmenuunicode")},
    /* launch Bluetooth dmenu */
    {MODKEY,                  XK_b, spawn, SHCMD("dmenu-bluetooth")},
    /* Launch NetworkManager dmenu */
    /* {MODKEY|ShiftMask,        XK_w, spawn, SHCMD(TERMINAL "-e sudo nmtui")}, */
    /* {MODKEY|ShiftMask,              XK_w,   spawn, SHCMD("networkmanager_dmenu") }, */

    /* [APP] */
    {MODKEY,                  XK_g, spawn, SHCMD("steam")},
    {MODKEY,                  XK_e, spawn, SHCMD("thunderbird")},
    {MODKEY|ShiftMask,        XK_e, spawn, SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook")},
    {MODKEY,                  XK_w, spawn, SHCMD(BROWSER)},
    {MODKEY|ShiftMask,        XK_w, spawn, SHCMD("chromium")},
    {MODKEY|ControlMask,      XK_w, spawn, SHCMD("brave")},
    {MODKEY,                  XK_o, spawn, SHCMD("signal-desktop")},
    {MODKEY,                  XK_v, spawn, SHCMD(TERMINAL "-e nvim")},
    {MODKEY|ShiftMask,        XK_x, spawn, SHCMD(TERMINAL "-e htop")},
    {MODKEY,                  XK_r, spawn, SHCMD(TERMINAL "-e lfub")},
    {MODKEY|ShiftMask,        XK_n, spawn, SHCMD(TERMINAL " -e newsboat ; pkill -RTMIN+6 dwmblocks")},

/* [FUNCTION KEY] */
    {MODKEY,                  XK_F1, spawn, SHCMD("dmenukeys")},
    {MODKEY,                  XK_F2, spawn, SHCMD("switcheme")}, 
    {MODKEY,                  XK_F3, spawn, SHCMD("displayselect")},
    {MODKEY,                  XK_F4, spawn, SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)")},
    {MODKEY,                  XK_F5,	xrdb,	 {.v = NULL } },
    {MODKEY,                  XK_F6, spawn, SHCMD("torwrap")},
    {MODKEY,                  XK_F7, spawn, SHCMD("td-toggle")},
    {MODKEY,                  XK_F8, spawn, SHCMD("mw -Y")},
    {MODKEY,                  XK_F9, spawn, SHCMD("mounter")},
    {MODKEY,                  XK_F10, spawn, SHCMD("unmounter")},
    {MODKEY,                  XK_F11, spawn, SHCMD("mpv --untimed --no-cache --no-osc " "--no-input-default-bindings " "--profile=low-latency --input-conf=/dev/null " "--title=webcam $(ls " "/dev/video[0,2,4,6,8] | tail -n 1) || notify-send -u critical 'Webcam' 'Could not open webcam!'")},
    {MODKEY,                  XK_F12, spawn, SHCMD("remaps")}, // amixer -qD pulse sset Master 5%-; kill -44 $(pidof dwmblocks)
    {MODKEY|ShiftMask,        XK_F12, spawn, SHCMD("switchlayout")}, // amixer -qD pulse sset Master 5%-; kill -44 $(pidof dwmblocks)

    /* [UNUSED] */
    {MODKEY|ControlMask,      XK_z, spawn, SHCMD("notify-send 'Control z'")},
    {MODKEY|ControlMask,      XK_e, spawn, SHCMD("notify-send 'Control e'")},
    {MODKEY|ControlMask,      XK_r, spawn, SHCMD("notify-send 'Control r'")},
    {MODKEY|ControlMask,      XK_t, spawn, SHCMD("notify-send 'Control t'")},
    {MODKEY|ControlMask,      XK_y, spawn, SHCMD("notify-send 'Control y'")},
    {MODKEY|ControlMask,      XK_u, spawn, SHCMD("notify-send 'Control u'")},
    {MODKEY|ControlMask,      XK_i, spawn, SHCMD("notify-send 'Control i'")},
    {MODKEY|ShiftMask,        XK_o, spawn, SHCMD("notify-send 'Control o'")},
    {MODKEY|ControlMask,      XK_o, spawn, SHCMD("notify-send 'Control o'")},
    {MODKEY|ControlMask,      XK_p, spawn, SHCMD("notify-send 'Control p'")},
    {MODKEY|ControlMask,      XK_s, spawn, SHCMD("notify-send 'Control s'")},
    {MODKEY|ControlMask,      XK_d, spawn, SHCMD("notify-send 'Control d'")},
    {MODKEY|ControlMask,      XK_f, spawn, SHCMD("notify-send 'Control f'")},
    {MODKEY|ShiftMask,        XK_g, spawn, SHCMD("notify-send 'Shift g'")},
    {MODKEY|ControlMask,      XK_g, spawn, SHCMD("notify-send 'Control g'")},
    {MODKEY|ControlMask,      XK_h, spawn, SHCMD("notify-send 'Control h'")},
    {MODKEY|ControlMask,      XK_j, spawn, SHCMD("notify-send 'Control j'")},
    {MODKEY|ControlMask,      XK_k, spawn, SHCMD("notify-send 'Control k'")},
    {MODKEY|ControlMask,      XK_l, spawn, SHCMD("notify-send 'Control l'")},
    {MODKEY|ControlMask,      XK_m, spawn, SHCMD("notify-send 'Control m'")},
    {MODKEY|ControlMask,      XK_x, spawn, SHCMD("notify-send 'Control x'")},
    {MODKEY|ControlMask,      XK_c, spawn, SHCMD("notify-send 'Control c'")},
    {MODKEY|ControlMask,      XK_v, spawn, SHCMD("notify-send 'Control v'")},
    {MODKEY|ControlMask,      XK_b, spawn, SHCMD("notify-send 'Control b'")},
    {MODKEY|ControlMask,      XK_n, spawn, SHCMD("notify-send 'Control n'")},
    {MODKEY|ControlMask,      XK_KP_Add, spawn, SHCMD("notify-send 'Control KP_Add'")},
    {MODKEY|ControlMask,      XK_KP_Subtract, spawn, SHCMD("notify-send 'Control KP_Substract'")},
    {MODKEY|ControlMask,      XK_KP_Enter, spawn, SHCMD("notify-send 'Control KP_Enter'")},
    {MODKEY|ControlMask,      XK_Insert, spawn, SHCMD("notify-send 'Control Insert'")},
    {MODKEY|ControlMask,      XK_asterisk, spawn, SHCMD("notify-send 'Control asterisk'")},
    {MODKEY|ControlMask,      XK_comma, spawn, SHCMD("notify-send 'Control comma'")},
    {MODKEY|ShiftMask,        XK_KP_Multiply, spawn, SHCMD("notify-send 'Shift KP_Multiply'")},
    {MODKEY|ControlMask,      XK_KP_Multiply, spawn, SHCMD("notify-send 'Control KP_Multiply'")},
    {MODKEY|ShiftMask,        XK_KP_Divide, spawn, SHCMD("notify-send 'Shift KP_Divide'")},
    {MODKEY|ControlMask,      XK_KP_Divide, spawn, SHCMD("notify-send 'Control KP_Divide'")},
    {MODKEY,                  XK_parenright, spawn, SHCMD("notify-send ')'")},
    {MODKEY|ShiftMask,        XK_parenright, spawn, SHCMD("notify-send '°'")},
    {MODKEY|ControlMask,      XK_parenright, spawn, SHCMD("notify-send 'Control parenright'")},
    {MODKEY,                  XK_equal, spawn, SHCMD("notify-send '='")},
    {MODKEY|ShiftMask,        XK_equal, spawn, SHCMD("notify-send '+'")},
    {MODKEY|ControlMask,      XK_equal, spawn, SHCMD("notify-send 'Control equal'")},
    {MODKEY,                  XK_dead_circumflex, spawn, SHCMD("notify-send '^'")},
    {MODKEY|ShiftMask,        XK_dead_circumflex, spawn, SHCMD("notify-send '¨'")},
    {MODKEY|ControlMask,      XK_dead_circumflex, spawn, SHCMD("notify-send 'Control dead_circumflex'")},
    {MODKEY,                  XK_dollar, spawn, SHCMD("notify-send '$'")},
    {MODKEY|ShiftMask,        XK_dollar, spawn, SHCMD("notify-send '£'")},
    {MODKEY|ControlMask,      XK_dollar, spawn, SHCMD("notify-send 'Control dollar'")},
    {MODKEY,                  XK_ugrave, spawn, SHCMD("notify-send 'ù'")},
    {MODKEY|ShiftMask,        XK_ugrave, spawn, SHCMD("notify-send '%'")},
    {MODKEY|ControlMask,      XK_ugrave, spawn, SHCMD("notify-send 'Control ugrave'")},
    {MODKEY,                  XK_semicolon, spawn, SHCMD("notify-send ';'")},
    {MODKEY|ShiftMask,        XK_semicolon, spawn, SHCMD("notify-send '.'")},
    {MODKEY|ControlMask,      XK_semicolon, spawn, SHCMD("notify-send 'Control semicolon'")},
    {MODKEY,                  XK_colon, spawn, SHCMD("notify-send ':'")},
    {MODKEY|ShiftMask,        XK_colon, spawn, SHCMD("notify-send '/'")},
    {MODKEY|ControlMask,      XK_colon, spawn, SHCMD("notify-send 'Control colon'")},
    {MODKEY,                  XK_exclam, spawn, SHCMD("notify-send '!'")},
    {MODKEY|ShiftMask,        XK_exclam, spawn, SHCMD("notify-send '§'")},
    {MODKEY|ControlMask,      XK_exclam, spawn, SHCMD("notify-send 'Control exclam'")},
    {MODKEY,                  XK_Scroll_Lock, spawn, SHCMD("notify-send 'Scroll_lock'")},
    {MODKEY|ShiftMask,        XK_Scroll_Lock, spawn, SHCMD("notify-send 'Shift Scroll_lock'")},
    {ShiftMask,               XK_Scroll_Lock, spawn, SHCMD("notify-send 'Shift Scroll_lock (no modkey)'")},
    {MODKEY|ControlMask,      XK_Scroll_Lock, spawn, SHCMD("notify-send 'Control Scroll_lock'")},
    {ControlMask,             XK_Scroll_Lock, spawn, SHCMD("notify-send 'Control Scroll_lock (no modkey)'")},
    {MODKEY,                  XK_Pause, spawn, SHCMD("notify-send 'Pause'")},
    {MODKEY|ShiftMask,        XK_Pause, spawn, SHCMD("notify-send 'Shift Pause'")},
    {ShiftMask,               XK_Pause, spawn, SHCMD("notify-send 'Shift Pause (no modkey)'")},
    {MODKEY|ControlMask,      XK_Pause, spawn, SHCMD("notify-send 'Control Pause'")},
    {ControlMask,             XK_Pause, spawn, SHCMD("notify-send 'Control Pause (no modkey)'")},
/* END OF THE DOC */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
    {ClkWinTitle,       0,              Button2,        zoom,           {0}},
    {ClkStatusText,     0,              Button1,        sigdwmblocks,   {.i = 1}},
    {ClkStatusText,     0,              Button2,        sigdwmblocks,   {.i = 2}},
    {ClkStatusText,     0,              Button3,        sigdwmblocks,   {.i = 3}},
    {ClkStatusText,     0,              Button4,        sigdwmblocks,   {.i = 4}},
    {ClkStatusText,     0,              Button5,        sigdwmblocks,   {.i = 5}},
    {ClkStatusText,     ShiftMask,      Button1,        sigdwmblocks,   {.i = 6}},
#endif
    {ClkStatusText,     ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h")},
    /* placemouse options, choose which feels more natural:
     *    0 - tiled position is relative to mouse cursor
     *    1 - tiled position is relative to window center
     *    2 - mouse pointer warps to window center
     *
     * The moveorplace uses movemouse or placemouse depending on the floating
     * state of the selected client. Set up individual keybindings for the two
     * if you want to control these separately (i.e. to retain the feature to
     * move a tiled window into a floating position).
     */
    {ClkClientWin,      MODKEY,         Button1,        moveorplace,    {.i = 1}},
    {ClkClientWin,      MODKEY,         Button2,        defaultgaps,    {0}},
    {ClkClientWin,      MODKEY,         Button3,        resizemouse,    {0}},
    {ClkClientWin,      MODKEY,         Button4,        incrgaps,       {.i = +1}},
    {ClkClientWin,      MODKEY,         Button5,        incrgaps,       {.i = -1}},
    {ClkTagBar,         0,              Button1,        view,           {0}},
    {ClkTagBar,         0,              Button3,        toggleview,     {0}},
    {ClkTagBar,         MODKEY,         Button1,        tag,            {0}},
    {ClkTagBar,         MODKEY,         Button3,        toggletag,      {0}},
    {ClkTagBar,         0,              Button4,        shiftview,      {.i = -1}},
    {ClkTagBar,         0,              Button5,        shiftview,      {.i = 1}},
    {ClkRootWin,        0,              Button2,        togglebar,      {0}},
};
