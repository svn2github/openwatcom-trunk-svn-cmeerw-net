#ifndef	__WINDOWS_H
#define	__WINDOWS_H

#pragma	pack(push,1)

#ifdef	__cplusplus
extern	"C" {
#endif

/* Default version is 3.1 */
#ifndef	WINVER
#define	WINVER	0x30a
#endif

/* Default is STRICT syntax */
#ifdef	NO_STRICT
#ifdef	STRICT
#undef	STRICT
#endif
#else
#ifndef	STRICT
#define STRICT
#endif
#endif

#define	VOID	void
#define	FAR	_far
#define	NEAR	_near
#define	PASCAL	_pascal
#define	CDECL	_cdecl
#define	WINAPI	_far _pascal
#define	CALLBACK	_far _pascal

#define	FALSE	0
#define	TRUE	1

typedef	int	BOOL;
typedef	unsigned char	BYTE;
typedef	unsigned short	WORD;
typedef	unsigned long	DWORD;
typedef	unsigned int	UINT;
#ifdef	STRICT
typedef	signed long	LONG;
#else
#define	LONG	long
#endif

#if !defined(NULL)
#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#	define NULL	0
#else
#	define NULL	0L
#endif
#endif

typedef	UINT	WPARAM;
typedef	LONG	LPARAM;
typedef	LONG	LRESULT;
typedef	char	*PSTR;
typedef	char	NEAR *NPSTR;
typedef	char	FAR *LPSTR;
typedef	const char	FAR *LPCSTR;
typedef	BYTE	*PBYTE;
typedef	BYTE	FAR *LPBYTE;
typedef	int	*PINT;
typedef	int	FAR *LPINT;
typedef	WORD	*PWORD;
typedef	WORD	FAR *LPWORD;
typedef	long	*PLONG;
typedef	long	FAR *LPLONG;
typedef	DWORD	*PDWORD;
typedef	DWORD	FAR *LPDWORD;
typedef	void	FAR *LPVOID;
#ifdef	STRICT
typedef	const void	NEAR* HANDLE;
typedef	void	(CALLBACK* FARPROC)(void);
typedef	void	(NEAR PASCAL* NEARPROC)(void);
#define	DECLARE_HANDLE(name)	struct name##__ {int unused;}; typedef const struct name##__ NEAR* name
#define	DECLARE_HANDLE32(name)	struct name##__ {int unused;}; typedef const struct name##__ FAR* name
#else
typedef	UINT	HANDLE;
typedef	int	(CALLBACK* FARPROC)();
typedef	int	(NEAR PASCAL* NEARPROC)();
#define	DECLARE_HANDLE(name)	typedef	UINT	name
#define	DECLARE_HANDLE32(name)	typedef	DWORD	name
#endif
typedef	HANDLE	HGLOBAL;
typedef	HANDLE	HLOCAL;
typedef	HANDLE	GLOBALHANDLE;
typedef	HANDLE	LOCALHANDLE;
typedef	UINT	ATOM;
typedef	DWORD	COLORREF;

#define	FIELDOFFSET(t,f)	((int)(&((t NEAR*)1)->f)-1)
#define	GetBValue(rgb)	((BYTE)((rgb)>>16))
#define	GetGValue(rgb)	((BYTE)(((WORD)(rgb))>>8))
#define	GetRValue(rgb)	((BYTE)(rgb))
#define	GlobalDiscard(h)	GlobalReAlloc(h,0L,GMEM_MOVEABLE)
#define	HIBYTE(w)	((BYTE)(((UINT)(w)>>8)&0xFF))
#define	HIWORD(l)	((WORD)((((DWORD)(l))>>16)&0xFFFF))
#define	LOBYTE(w)	((BYTE)(w))
#define	LocalDiscard(h)	LocalReAlloc(h,0,LMEM_MOVEABLE)
#define	LockData(dummy)	LockSegment((UINT)-1)
#define	LOWORD(l)	((WORD)(DWORD)(l))
#define	MAKEINTATOM(i)	((LPCSTR)MAKELP(NULL,(i)))
#define	MAKEINTRESOURCE(i)	((LPCSTR)MAKELP(NULL,(i)))
#define	MAKELONG(l,h) ((LONG)(((WORD)(l))|(((DWORD)((WORD)(h)))<<16)))
#define	MAKELP(s,o)	((void	FAR*)MAKELONG((o),(s)))
#define	MAKELPARAM(l,h)	((LPARAM)MAKELONG(l,h))
#define	MAKELRESULT(l,h)	((LRESULT)MAKELONG(l,h))
#define	MAKEPOINT(l)	(*((POINT FAR*)&(l)))
#ifndef	max
#define	max(a,b)	(((a)>(b))?(a):(b))
#endif
#ifndef	min
#define	min(a,b)	(((a)<(b))?(a):(b))
#endif
#define	OFFSETOF(lp)	LOWORD(lp)
#define	PALETTEINDEX(i)	((COLORREF)(0x1000000L|(DWORD)(WORD)(i)))
#define	PALETTERGB(r,g,b)	(0x2000000L|RGB(r,g,b))
#define	RGB(r,g,b)	((COLORREF)(((BYTE)(r)|((WORD)(g)<<8))|(((DWORD)(BYTE)(b))<<16)))
#define	SELECTOROF(lp)	HIWORD(lp)
#define	UnlockData(dummy)	UnlockSegment((UINT)-1)
#define	UnlockResource(h)	GlobalUnlock(h)

#define	wsprintf	WSPRINTF
#define	HINSTANCE_ERROR	((HINSTANCE)32)

#define WM_NULL	0
#define WM_CREATE	1
#define WM_DESTROY	2
#define WM_MOVE	3
#define WM_SIZE	5
#define WM_ACTIVATE	6
#define WM_SETFOCUS	7
#define WM_KILLFOCUS	8
#define WM_ENABLE	0xA
#define WM_SETREDRAW	0xB
#define WM_SETTEXT	0xC
#define WM_GETTEXT	0xD
#define WM_GETTEXTLENGTH	0xE
#define WM_PAINT	0xF
#define WM_CLOSE	0x10
#define WM_QUERYENDSESSION	0x11
#define WM_QUIT	0x12
#define WM_QUERYOPEN	0x13
#define WM_ERASEBKGND	0x14
#define WM_SYSCOLORCHANGE	0x15
#define WM_ENDSESSION	0x16
#define WM_SYSTEMERROR	0x17
#define WM_SHOWWINDOW	0x18
#define WM_CTLCOLOR	0x19
#define WM_WININICHANGE	0x1A
#define WM_DEVMODECHANGE	0x1B
#define WM_ACTIVATEAPP	0x1C
#define WM_FONTCHANGE	0x1D
#define WM_TIMECHANGE	0x1E
#define WM_CANCELMODE	0x1F
#define WM_SETCURSOR	0x20
#define WM_MOUSEACTIVATE	0x21
#define WM_CHILDACTIVATE	0x22
#define WM_QUEUESYNC	0x23
#define WM_GETMINMAXINFO	0x24
#define WM_ICONERASEBKGND	0x27
#define WM_NEXTDLGCTL	0x28
#define WM_SPOOLERSTATUS	0x2A
#define WM_DRAWITEM	0x2B
#define WM_MEASUREITEM	0x2C
#define WM_DELETEITEM	0x2D
#define WM_VKEYTOITEM	0x2E
#define WM_CHARTOITEM	0x2F
#define WM_SETFONT	0x30
#define WM_GETFONT	0x31
#define WM_QUERYDRAGICON	0x37
#define WM_COMPAREITEM	0x39
#define WM_COMPACTING	0x41
#define WM_COMMNOTIFY	0x44
#define WM_WINDOWPOSCHANGING	0x46
#define WM_WINDOWPOSCHANGED	0x47
#define WM_POWER	0x48
#define WM_NCCREATE	0x81
#define WM_NCDESTROY	0x82
#define WM_NCCALCSIZE	0x83
#define WM_NCHITTEST	0x84
#define WM_NCPAINT	0x85
#define WM_NCACTIVATE	0x86
#define WM_GETDLGCODE	0x87
#define WM_NCMOUSEMOVE	0xA0
#define WM_NCLBUTTONDOWN	0xA1
#define WM_NCLBUTTONUP	0xA2
#define WM_NCLBUTTONDBLCLK	0xA3
#define WM_NCRBUTTONDOWN	0xA4
#define WM_NCRBUTTONUP	0xA5
#define WM_NCRBUTTONDBLCLK	0xA6
#define WM_NCMBUTTONDOWN	0xA7
#define WM_NCMBUTTONUP	0xA8
#define WM_NCMBUTTONDBLCLK	0xA9
#define WM_KEYFIRST	0x100
#define WM_KEYDOWN	0x100
#define WM_KEYUP	0x101
#define WM_CHAR	0x102
#define WM_DEADCHAR	0x103
#define WM_SYSKEYDOWN	0x104
#define WM_SYSKEYUP	0x105
#define WM_SYSCHAR	0x106
#define WM_SYSDEADCHAR	0x107
#define WM_KEYLAST	0x108
#define WM_INITDIALOG	0x110
#define WM_COMMAND	0x111
#define WM_SYSCOMMAND	0x112
#define WM_TIMER	0x113
#define WM_HSCROLL	0x114
#define WM_VSCROLL	0x115
#define WM_INITMENU	0x116
#define WM_INITMENUPOPUP	0x117
#define WM_MENUSELECT	0x11F
#define WM_MENUCHAR	0x120
#define WM_ENTERIDLE	0x121
#define WM_MOUSEFIRST	0x200
#define WM_MOUSEMOVE	0x200
#define WM_LBUTTONDOWN	0x201
#define WM_LBUTTONUP	0x202
#define WM_LBUTTONDBLCLK	0x203
#define WM_RBUTTONDOWN	0x204
#define WM_RBUTTONUP	0x205
#define WM_RBUTTONDBLCLK	0x206
#define WM_MBUTTONDOWN	0x207
#define WM_MBUTTONUP	0x208
#define WM_MBUTTONDBLCLK	0x209
#define WM_MOUSELAST	0x209
#define WM_PARENTNOTIFY	0x210
#define WM_MDICREATE	0x220
#define WM_MDIDESTROY	0x221
#define WM_MDIACTIVATE	0x222
#define WM_MDIRESTORE	0x223
#define WM_MDINEXT	0x224
#define WM_MDIMAXIMIZE	0x225
#define WM_MDITILE	0x226
#define WM_MDICASCADE	0x227
#define WM_MDIICONARRANGE	0x228
#define WM_MDIGETACTIVE	0x229
#define WM_MDISETMENU	0x230
#define WM_DROPFILES	0x233
#define WM_CUT	0x300
#define WM_COPY	0x301
#define WM_PASTE	0x302
#define WM_CLEAR	0x303
#define WM_UNDO	0x304
#define WM_RENDERFORMAT	0x305
#define WM_RENDERALLFORMATS	0x306
#define WM_DESTROYCLIPBOARD	0x307
#define WM_DRAWCLIPBOARD	0x308
#define WM_PAINTCLIPBOARD	0x309
#define WM_SIZECLIPBOARD	0x30B
#define WM_VSCROLLCLIPBOARD	0x30A
#define WM_HSCROLLCLIPBOARD	0x30E
#define WM_ASKCBFORMATNAME	0x30C
#define WM_CHANGECBCHAIN	0x30D
#define WM_QUERYNEWPALETTE	0x30F
#define WM_PALETTEISCHANGING	0x310
#define WM_PALETTECHANGED	0x311
#define WM_PENWINFIRST	0x380
#define WM_PENWINLAST	0x38F
#define WM_COALESCE_FIRST	0x390
#define WM_COALESCE_LAST	0x39F
#define WM_USER	0x400

/* Dialog styles */
#define DS_ABSALIGN	1L
#define DS_SYSMODAL	2L
#define DS_LOCALEDIT	0x20L
#define DS_SETFONT	0x40L
#define DS_MODALFRAME	0x80L
#define DS_NOIDLEMSG	0x100L

/* Basic window types */
#define WS_OVERLAPPED	0L
#define WS_POPUP	0x80000000L
#define WS_CHILD	0x40000000L
/* Clipping styles */
#define WS_CLIPSIBLINGS	0x4000000L
#define WS_CLIPCHILDREN	0x2000000L
/* Generic window states */
#define WS_VISIBLE	0x10000000L
#define WS_DISABLED	0x8000000L
/* Main window states */
#define WS_MINIMIZE	0x20000000L
#define WS_MAXIMIZE	0x1000000L
/* Main window styles */
#define WS_CAPTION	0xC00000L	/* WS_BORDER | WS_DLGFRAME	*/
#define WS_BORDER	0x800000L
#define WS_DLGFRAME	0x400000L
#define WS_VSCROLL	0x200000L
#define WS_HSCROLL	0x100000L
#define WS_SYSMENU	0x80000L
#define WS_THICKFRAME	0x40000L
#define WS_MINIMIZEBOX	0x20000L
#define WS_MAXIMIZEBOX	0x10000L
/* Control window styles */
#define WS_GROUP	0x20000L
#define WS_TABSTOP	0x10000L
/* Common Window Styles */
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
#define WS_POPUPWINDOW	(WS_POPUP | WS_BORDER | WS_SYSMENU)
#define WS_CHILDWINDOW	(WS_CHILD)

/* Static Control Styles */
#define SS_LEFT	0L
#define SS_CENTER	1L
#define SS_RIGHT	2L
#define SS_ICON	3L
#define SS_BLACKRECT	4L
#define SS_GRAYRECT	5L
#define SS_WHITERECT	6L
#define SS_BLACKFRAME	7L
#define SS_GRAYFRAME	8L
#define SS_WHITEFRAME	9L
#define SS_SIMPLE	0xBL
#define SS_LEFTNOWORDWRAP	0xCL
#define SS_NOPREFIX	0x80L

/* Edit control styles */
#define ES_LEFT	0L
#define ES_CENTER	1L
#define ES_RIGHT	2L
#define ES_MULTILINE	4L
#define ES_UPPERCASE	8L
#define ES_LOWERCASE	0x10L
#define ES_PASSWORD	0x20L
#define ES_AUTOVSCROLL	0x40L
#define ES_AUTOHSCROLL	0x80L
#define ES_NOHIDESEL	0x100L
#define ES_OEMCONVERT	0x400L
#define ES_READONLY	0x800L
#define ES_WANTRETURN	0x1000L

/* Combo box styles */
#define CBS_SIMPLE	1L
#define CBS_DROPDOWN	2L
#define CBS_DROPDOWNLIST	3L
#define CBS_OWNERDRAWFIXED	0x10L
#define CBS_OWNERDRAWVARIABLE	0x20L
#define CBS_AUTOHSCROLL	0x40L
#define CBS_OEMCONVERT	0x80L
#define CBS_SORT	0x100L
#define CBS_HASSTRINGS	0x200L
#define CBS_NOINTEGRALHEIGHT	0x400L
#define CBS_DISABLENOSCROLL	0x800L

/* Listbox styles */
#define LBS_NOTIFY	1L
#define LBS_SORT	2L
#define LBS_NOREDRAW	4L
#define LBS_MULTIPLESEL	8L
#define LBS_OWNERDRAWFIXED	0x10L
#define LBS_OWNERDRAWVARIABLE	0x20L
#define LBS_HASSTRINGS	0x40L
#define LBS_USETABSTOPS	0x80L
#define LBS_NOINTEGRALHEIGHT	0x100L
#define LBS_MULTICOLUMN	0x200L
#define LBS_WANTKEYBOARDINPUT	0x400L
#define LBS_EXTENDEDSEL	0x800L
#define LBS_DISABLENOSCROLL	0x1000L
#define LBS_STANDARD	(LBS_NOTIFY | LBS_SORT | WS_VSCROLL | WS_BORDER)

/* Brush Styles */
#define BS_SOLID	0
#define BS_NULL	1
#define BS_HOLLOW	BS_NULL
#define BS_HATCHED	2
#define BS_PATTERN	3
#define BS_INDEXED	4
#define BS_DIBPATTERN	5
/* Button Control Styles */
#define BS_PUSHBUTTON	0L
#define BS_DEFPUSHBUTTON	1L
#define BS_CHECKBOX	2L
#define BS_AUTOCHECKBOX	3L
#define BS_RADIOBUTTON	4L
#define BS_3STATE	5L
#define BS_AUTO3STATE	6L
#define BS_GROUPBOX	7L
#define BS_USERBUTTON	8L
#define BS_AUTORADIOBUTTON	9L
#define BS_OWNERDRAW	0xBL
#define BS_LEFTTEXT	0x20L

/* Standard dialog button IDs */
#define IDOK	1
#define IDCANCEL	2
#define IDABORT	3
#define IDRETRY	4
#define IDIGNORE	5
#define IDYES	6
#define IDNO	7

/* Virtual key codes */
#define VK_LBUTTON	1
#define VK_RBUTTON	2
#define VK_CANCEL	3
#define VK_MBUTTON	4
#define VK_BACK	8
#define VK_TAB	9
#define VK_CLEAR	0xC
#define VK_RETURN	0xD
#define VK_SHIFT	0x10
#define VK_CONTROL	0x11
#define VK_MENU	0x12
#define VK_PAUSE	0x13
#define VK_CAPITAL	0x14
#define VK_ESCAPE	0x1B
#define VK_SPACE	0x20
#define VK_PRIOR	0x21
#define VK_NEXT	0x22
#define VK_END	0x23
#define VK_HOME	0x24
#define VK_LEFT	0x25
#define VK_UP	0x26
#define VK_RIGHT	0x27
#define VK_DOWN	0x28
#define VK_SELECT	0x29
#define VK_PRINT	0x2A
#define VK_EXECUTE	0x2B
#define VK_SNAPSHOT	0x2C
#define VK_INSERT	0x2D
#define VK_DELETE	0x2E
#define VK_HELP	0x2F
#define VK_NUMPAD0	0x60
#define VK_NUMPAD1	0x61
#define VK_NUMPAD2	0x62
#define VK_NUMPAD3	0x63
#define VK_NUMPAD4	0x64
#define VK_NUMPAD5	0x65
#define VK_NUMPAD6	0x66
#define VK_NUMPAD7	0x67
#define VK_NUMPAD8	0x68
#define VK_NUMPAD9	0x69
#define VK_MULTIPLY	0x6A
#define VK_ADD	0x6B
#define VK_SEPARATOR	0x6C
#define VK_SUBTRACT	0x6D
#define VK_DECIMAL	0x6E
#define VK_DIVIDE	0x6F
#define VK_F1	0x70
#define VK_F2	0x71
#define VK_F3	0x72
#define VK_F4	0x73
#define VK_F5	0x74
#define VK_F6	0x75
#define VK_F7	0x76
#define VK_F8	0x77
#define VK_F9	0x78
#define VK_F10	0x79
#define VK_F11	0x7A
#define VK_F12	0x7B
#define VK_F13	0x7C
#define VK_F14	0x7D
#define VK_F15	0x7E
#define VK_F16	0x7F
#define VK_F17	0x80
#define VK_F18	0x81
#define VK_F19	0x82
#define VK_F20	0x83
#define VK_F21	0x84
#define VK_F22	0x85
#define VK_F23	0x86
#define VK_F24	0x87
#define VK_NUMLOCK	0x90
#define VK_SCROLL	0x91

/* Predefined Resource Types */
#define RT_CURSOR	MAKEINTRESOURCE(1)
#define RT_BITMAP	MAKEINTRESOURCE(2)
#define RT_ICON	MAKEINTRESOURCE(3)
#define RT_MENU	MAKEINTRESOURCE(4)
#define RT_DIALOG	MAKEINTRESOURCE(5)
#define RT_STRING	MAKEINTRESOURCE(6)
#define RT_FONTDIR	MAKEINTRESOURCE(7)
#define RT_FONT	MAKEINTRESOURCE(8)
#define RT_ACCELERATOR	MAKEINTRESOURCE(9)
#define RT_RCDATA	MAKEINTRESOURCE(10)
#define RT_GROUP_CURSOR	MAKEINTRESOURCE(12)
#define RT_GROUP_ICON	MAKEINTRESOURCE(14)

/* Listbox messages */
#define LB_ADDSTRING	(WM_USER+1)
#define LB_INSERTSTRING	(WM_USER+2)
#define LB_DELETESTRING	(WM_USER+3)
#define LB_RESETCONTENT	(WM_USER+5)
#define LB_SETSEL	(WM_USER+6)
#define LB_SETCURSEL	(WM_USER+7)
#define LB_GETSEL	(WM_USER+8)
#define LB_GETCURSEL	(WM_USER+9)
#define LB_GETTEXT	(WM_USER+10)
#define LB_GETTEXTLEN	(WM_USER+11)
#define LB_GETCOUNT	(WM_USER+12)
#define LB_SELECTSTRING	(WM_USER+13)
#define LB_DIR	(WM_USER+14)
#define LB_GETTOPINDEX	(WM_USER+15)
#define LB_FINDSTRING	(WM_USER+16)
#define LB_GETSELCOUNT	(WM_USER+17)
#define LB_GETSELITEMS	(WM_USER+18)
#define LB_SETTABSTOPS	(WM_USER+19)
#define LB_GETHORIZONTALEXTENT (WM_USER+20)
#define LB_SETHORIZONTALEXTENT (WM_USER+21)
#define LB_SETCOLUMNWIDTH	(WM_USER+22)
#define LB_SETTOPINDEX	(WM_USER+24)
#define LB_GETITEMRECT	(WM_USER+25)
#define LB_GETITEMDATA	(WM_USER+26)
#define LB_SETITEMDATA	(WM_USER+27)
#define LB_SELITEMRANGE	(WM_USER+28)
#define LB_SETCARETINDEX	(WM_USER+31)
#define LB_GETCARETINDEX	(WM_USER+32)
#define LB_SETITEMHEIGHT	(WM_USER+33)
#define LB_GETITEMHEIGHT	(WM_USER+34)
#define LB_FINDSTRINGEXACT	(WM_USER+35)
/* Listbox message return values */
#define LB_OKAY	0
#define LB_ERR	(-1)
#define LB_ERRSPACE	(-2)

/* Window field offsets for GetWindowLong() and GetWindowWord() */
#define GWL_WNDPROC	(-4)
#define GWW_HINSTANCE	(-6)
#define GWW_HWNDPARENT	(-8)
#define GWW_ID	(-12)
#define GWL_STYLE	(-16)
#define GWL_EXSTYLE	(-20)

#define MB_OK	0
#define MB_OKCANCEL	1
#define MB_ABORTRETRYIGNORE	2
#define MB_YESNOCANCEL	3
#define MB_YESNO	4
#define MB_RETRYCANCEL	5
#define MB_TYPEMASK	0xF

#define MB_ICONHAND	0x10
#define MB_ICONQUESTION	0x20
#define MB_ICONEXCLAMATION	0x30
#define MB_ICONASTERISK	0x40
#define MB_ICONMASK	0xF0
#define MB_ICONINFORMATION	MB_ICONASTERISK
#define MB_ICONSTOP	MB_ICONHAND

#define MB_DEFBUTTON1	0
#define MB_DEFBUTTON2	0x100
#define MB_DEFBUTTON3	0x200
#define MB_DEFMASK	0xF00

#define MB_APPLMODAL	0
#define MB_SYSTEMMODAL	0x1000
#define MB_TASKMODAL	0x2000

#define MB_NOFOCUS	0x8000

/* Standard cursor resource IDs */
#define IDC_ARROW	MAKEINTRESOURCE(32512)
#define IDC_IBEAM	MAKEINTRESOURCE(32513)
#define IDC_WAIT	MAKEINTRESOURCE(32514)
#define IDC_CROSS	MAKEINTRESOURCE(32515)
#define IDC_UPARROW	MAKEINTRESOURCE(32516)
#define IDC_SIZE	MAKEINTRESOURCE(32640)
#define IDC_ICON	MAKEINTRESOURCE(32641)
#define IDC_SIZENWSE	MAKEINTRESOURCE(32642)
#define IDC_SIZENESW	MAKEINTRESOURCE(32643)
#define IDC_SIZEWE	MAKEINTRESOURCE(32644)
#define IDC_SIZENS	MAKEINTRESOURCE(32645)

/* Global Memory Flags */

#define GMEM_FIXED	0
#define GMEM_MOVEABLE	2
#define GMEM_NOCOMPACT	0x10
#define GMEM_NODISCARD	0x20
#define GMEM_ZEROINIT	0x40
#define GMEM_MODIFY	0x80
#define GMEM_DISCARDABLE	0x100
#define GMEM_NOT_BANKED	0x1000
#define GMEM_SHARE	0x2000
#define GMEM_DDESHARE	0x2000
#define GMEM_NOTIFY	0x4000
#define GMEM_LOWER	GMEM_NOT_BANKED

#define GHND	(GMEM_MOVEABLE | GMEM_ZEROINIT)
#define GPTR	(GMEM_FIXED | GMEM_ZEROINIT)

/* weight values */
#define FW_DONTCARE	0
#define FW_THIN	100
#define FW_EXTRALIGHT	200
#define FW_LIGHT	300
#define FW_NORMAL	400
#define FW_MEDIUM	500
#define FW_SEMIBOLD	600
#define FW_BOLD	700
#define FW_EXTRABOLD	800
#define FW_HEAVY	900
#define FW_ULTRALIGHT	FW_EXTRALIGHT
#define FW_REGULAR	FW_NORMAL
#define FW_DEMIBOLD	FW_SEMIBOLD
#define FW_ULTRABOLD	FW_EXTRABOLD
#define FW_BLACK	FW_HEAVY

/* OutPrecision values */
#define OUT_DEFAULT_PRECIS	0
#define OUT_STRING_PRECIS	1
#define OUT_CHARACTER_PRECIS	2
#define OUT_STROKE_PRECIS	3
#define OUT_TT_PRECIS	4
#define OUT_DEVICE_PRECIS	5
#define OUT_RASTER_PRECIS	6
#define OUT_TT_ONLY_PRECIS	7

/* ClipPrecision values */
#define CLIP_DEFAULT_PRECIS	0
#define CLIP_CHARACTER_PRECIS	1
#define CLIP_STROKE_PRECIS	2
#define CLIP_MASK	0xF
#define CLIP_LH_ANGLES	0x10
#define CLIP_TT_ALWAYS	0x20
#define CLIP_EMBEDDED	0x80

/* Quality values */
#define DEFAULT_QUALITY	0
#define DRAFT_QUALITY	1
#define PROOF_QUALITY	2

/* PitchAndFamily pitch values (low 4 bits) */
#define DEFAULT_PITCH	0
#define FIXED_PITCH	1
#define VARIABLE_PITCH	2

/* PitchAndFamily family values (high 4 bits) */
#define FF_DONTCARE	0
#define FF_ROMAN	0x10
#define FF_SWISS	0x20
#define FF_MODERN	0x30
#define FF_SCRIPT	0x40
#define FF_DECORATIVE	0x50

/* Stock fonts for use with GetStockObject() */
#define OEM_FIXED_FONT	10
#define ANSI_FIXED_FONT	11
#define ANSI_VAR_FONT	12
#define SYSTEM_FONT	13
#define DEVICE_DEFAULT_FONT 14
#define DEFAULT_PALETTE	15
#define SYSTEM_FIXED_FONT	16

/* DrawText() Format Flags */
#define DT_TOP	0
#define DT_LEFT	0
#define DT_CENTER	1
#define DT_RIGHT	2
#define DT_VCENTER	4
#define DT_BOTTOM	8
#define DT_WORDBREAK	0x10
#define DT_SINGLELINE	0x20
#define DT_EXPANDTABS	0x40
#define DT_TABSTOP	0x80
#define DT_NOCLIP	0x100
#define DT_EXTERNALLEADING	0x200
#define DT_CALCRECT	0x400
#define DT_NOPREFIX	0x800
#define DT_INTERNAL	0x1000

/* Edit control messages */
#define EM_GETSEL	(WM_USER+0)
#define EM_SETSEL	(WM_USER+1)
#define EM_GETRECT	(WM_USER+2)
#define EM_SETRECT	(WM_USER+3)
#define EM_SETRECTNP	(WM_USER+4)
#define EM_LINESCROLL	(WM_USER+6)
#define EM_GETMODIFY	(WM_USER+8)
#define EM_SETMODIFY	(WM_USER+9)
#define EM_GETLINECOUNT	(WM_USER+10)
#define EM_LINEINDEX	(WM_USER+11)
#define EM_SETHANDLE	(WM_USER+12)
#define EM_GETHANDLE	(WM_USER+13)
#define EM_LINELENGTH	(WM_USER+17)
#define EM_REPLACESEL	(WM_USER+18)
#define EM_SETFONT	(WM_USER+19)	/* NOT IMPLEMENTED: use WM_SETFONT */
#define EM_GETLINE	(WM_USER+20)
#define EM_LIMITTEXT	(WM_USER+21)
#define EM_CANUNDO	(WM_USER+22)
#define EM_UNDO	(WM_USER+23)
#define EM_FMTLINES	(WM_USER+24)
#define EM_LINEFROMCHAR	(WM_USER+25)
#define EM_SETWORDBREAK	(WM_USER+26)	/* NOT IMPLEMENTED: use EM_SETWORDBREAK */
#define EM_SETTABSTOPS	(WM_USER+27)
#define EM_SETPASSWORDCHAR	(WM_USER+28)
#define EM_EMPTYUNDOBUFFER	(WM_USER+29)
#define EM_GETFIRSTVISIBLELINE	(WM_USER+30)
#define EM_SETREADONLY	(WM_USER+31)
#define EM_SETWORDBREAKPROC	(WM_USER+32)
#define EM_GETWORDBREAKPROC	(WM_USER+33)
#define EM_GETPASSWORDCHAR	(WM_USER+34)

#define COLOR_SCROLLBAR	0
#define COLOR_BACKGROUND	1
#define COLOR_ACTIVECAPTION	2
#define COLOR_INACTIVECAPTION	3
#define COLOR_MENU	4
#define COLOR_WINDOW	5
#define COLOR_WINDOWFRAME	6
#define COLOR_MENUTEXT	7
#define COLOR_WINDOWTEXT	8
#define COLOR_CAPTIONTEXT	9
#define COLOR_ACTIVEBORDER	10
#define COLOR_INACTIVEBORDER	11
#define COLOR_APPWORKSPACE	12
#define COLOR_HIGHLIGHT	13
#define COLOR_HIGHLIGHTTEXT	14
#define COLOR_BTNFACE	15
#define COLOR_BTNSHADOW	16
#define COLOR_GRAYTEXT	17
#define COLOR_BTNTEXT	18
#define COLOR_INACTIVECAPTIONTEXT 19
#define COLOR_BTNHIGHLIGHT	20

/* Listbox notification codes */
#define LBN_ERRSPACE	(-2)
#define LBN_SELCHANGE	1
#define LBN_DBLCLK	2
#define LBN_SELCANCEL	3
#define LBN_SETFOCUS	4
#define LBN_KILLFOCUS	5

/* Class styles */
#define CS_VREDRAW	1
#define CS_HREDRAW	2
#define CS_KEYCVTWINDOW	4
#define CS_DBLCLKS	8
#define CS_OWNDC	0x20
#define CS_CLASSDC	0x40
#define CS_PARENTDC	0x80
#define CS_NOKEYCVT	0x100
#define CS_NOCLOSE	0x200
#define CS_SAVEBITS	0x800
#define CS_BYTEALIGNCLIENT	0x1000
#define CS_BYTEALIGNWINDOW	0x2000
#define CS_GLOBALCLASS	0x4000

/* Pen Styles */
#define PS_SOLID	0
#define PS_DASH	1
#define PS_DOT	2
#define PS_DASHDOT	3
#define PS_DASHDOTDOT	4
#define PS_NULL	5
#define PS_INSIDEFRAME	6

/* Ternary raster operations */
#define SRCCOPY	0xCC0020L
#define SRCPAINT	0xEE0086L
#define SRCAND	0x8800C6L
#define SRCINVERT	0x660046L
#define SRCERASE	0x440328L
#define NOTSRCCOPY	0x330008L
#define NOTSRCERASE	0x1100A6L
#define MERGECOPY	0xC000CAL
#define MERGEPAINT	0xBB0226L
#define PATCOPY	0xF00021L
#define PATPAINT	0xFB0A09L
#define PATINVERT	0x5A0049L
#define DSTINVERT	0x550009L
#define BLACKNESS	0x42L
#define WHITENESS	0xFF0062L

/* Stock brushes for use with GetStockObject() */
#define WHITE_BRUSH	0
#define LTGRAY_BRUSH	1
#define GRAY_BRUSH	2
#define DKGRAY_BRUSH	3
#define BLACK_BRUSH	4
#define NULL_BRUSH	5
#define HOLLOW_BRUSH	NULL_BRUSH

/* new StretchBlt() Modes (simpler names) */
#define STRETCH_ANDSCANS	1
#define STRETCH_ORSCANS	2
#define STRETCH_DELETESCANS	3

/* Button Control Messages	*/
#define BM_GETCHECK	(WM_USER+0)
#define BM_SETCHECK	(WM_USER+1)
#define BM_GETSTATE	(WM_USER+2)
#define BM_SETSTATE	(WM_USER+3)
#define BM_SETSTYLE	(WM_USER+4)

/* Menu flags for Add/Check/EnableMenuItem() */
#define MF_BYCOMMAND	0
#define MF_BYPOSITION	0x400
#define MF_SEPARATOR	0x800
#define MF_ENABLED	0
#define MF_GRAYED	1
#define MF_DISABLED	2
#define MF_UNCHECKED	0
#define MF_CHECKED	8
#define MF_USECHECKBITMAPS	0x200
#define MF_STRING	0
#define MF_BITMAP	4
#define MF_OWNERDRAW	0x100
#define MF_POPUP	0x10
#define MF_MENUBARBREAK	0x20
#define MF_MENUBREAK	0x40
#define MF_UNHILITE	0
#define MF_HILITE	0x80
#define MF_SYSMENU	0x2000
#define MF_HELP	0x4000
#define MF_MOUSESELECT	0x8000
#define MF_END	0x80	/* Only valid in menu resource templates */

/* Standard icon resource IDs */
#define IDI_APPLICATION	MAKEINTRESOURCE(32512)
#define IDI_HAND	MAKEINTRESOURCE(32513)
#define IDI_QUESTION	MAKEINTRESOURCE(32514)
#define IDI_EXCLAMATION	MAKEINTRESOURCE(32515)
#define IDI_ASTERISK	MAKEINTRESOURCE(32516)

#define SW_HIDE	0
#define SW_SHOWNORMAL	1
#define SW_NORMAL	1
#define SW_SHOWMINIMIZED	2
#define SW_SHOWMAXIMIZED	3
#define SW_MAXIMIZE	3
#define SW_SHOWNOACTIVATE	4
#define SW_SHOW	5
#define SW_MINIMIZE	6
#define SW_SHOWMINNOACTIVE	7
#define SW_SHOWNA	8
#define SW_RESTORE	9
/* WM_SHOWWINDOW wParam codes */
#define SW_PARENTCLOSING	1
#define SW_OTHERMAXIMIZED	2
#define SW_PARENTOPENING	3
#define SW_OTHERRESTORED	4

/* GetSystemMetrics() codes */
#define SM_CXSCREEN	0
#define SM_CYSCREEN	1
#define SM_CXVSCROLL	2
#define SM_CYHSCROLL	3
#define SM_CYCAPTION	4
#define SM_CXBORDER	5
#define SM_CYBORDER	6
#define SM_CXDLGFRAME	7
#define SM_CYDLGFRAME	8
#define SM_CYVTHUMB	9
#define SM_CXHTHUMB	10
#define SM_CXICON	11
#define SM_CYICON	12
#define SM_CXCURSOR	13
#define SM_CYCURSOR	14
#define SM_CYMENU	15
#define SM_CXFULLSCREEN	16
#define SM_CYFULLSCREEN	17
#define SM_CYKANJIWINDOW	18
#define SM_MOUSEPRESENT	19
#define SM_CYVSCROLL	20
#define SM_CXHSCROLL	21
#define SM_DEBUG	22
#define SM_SWAPBUTTON	23
#define SM_RESERVED1	24
#define SM_RESERVED2	25
#define SM_RESERVED3	26
#define SM_RESERVED4	27
#define SM_CXMIN	28
#define SM_CYMIN	29
#define SM_CXSIZE	30
#define SM_CYSIZE	31
#define SM_CXFRAME	32
#define SM_CYFRAME	33
#define SM_CXMINTRACK	34
#define SM_CYMINTRACK	35
#define SM_CXDOUBLECLK	36
#define SM_CYDOUBLECLK	37
#define SM_CXICONSPACING	38
#define SM_CYICONSPACING	39
#define SM_MENUDROPALIGNMENT 40
#define SM_PENWINDOWS	41
#define SM_DBCSENABLED	42
#define SM_CMETRICS	43

/* WM_SIZE message wParam values */
#define SIZE_RESTORED	0
#define SIZE_MINIMIZED	1
#define SIZE_MAXIMIZED	2
#define SIZE_MAXSHOW	3
#define SIZE_MAXHIDE	4

/* WinHelp() commands */
#define HELP_CONTEXT	1
#define HELP_QUIT	2
#define HELP_INDEX	3
#define HELP_CONTENTS	3
#define HELP_HELPONHELP	4
#define HELP_SETINDEX	5
#define HELP_SETCONTENTS	5
#define HELP_CONTEXTPOPUP	8
#define HELP_FORCEFILE	9
#define HELP_KEY	0x101
#define HELP_COMMAND	0x102
#define HELP_PARTIALKEY	0x105
#define HELP_MULTIKEY	0x201
#define HELP_SETWINPOS	0x203

#define RDW_INVALIDATE	1
#define RDW_INTERNALPAINT	2
#define RDW_ERASE	4
#define RDW_VALIDATE	8
#define RDW_NOINTERNALPAINT	0x10
#define RDW_NOERASE	0x20
#define RDW_NOCHILDREN	0x40
#define RDW_ALLCHILDREN	0x80
#define RDW_UPDATENOW	0x100
#define RDW_ERASENOW	0x200
#define RDW_FRAME	0x400
#define RDW_NOFRAME	0x800

/* Predefined Clipboard Formats */
#define CF_TEXT	1
#define CF_BITMAP	2
#define CF_METAFILEPICT	3
#define CF_SYLK	4
#define CF_DIF	5
#define CF_TIFF	6
#define CF_OEMTEXT	7
#define CF_DIB	8
#define CF_PALETTE	9
#define CF_PENDATA	10
#define CF_RIFF	11
#define CF_WAVE	12
#define CF_OWNERDISPLAY	0x80
#define CF_DSPTEXT	0x81
#define CF_DSPBITMAP	0x82
#define CF_DSPMETAFILEPICT	0x83
/* "Private" formats don't get GlobalFree()'d */
#define CF_PRIVATEFIRST	0x200
#define CF_PRIVATELAST	0x2FF
/* "GDIOBJ" formats do get DeleteObject()'d */
#define CF_GDIOBJFIRST	0x300
#define CF_GDIOBJLAST	0x3FF

/* SetWindowPos() and WINDOWPOS flags */
#define SWP_NOSIZE	1
#define SWP_NOMOVE	2
#define SWP_NOZORDER	4
#define SWP_NOREDRAW	8
#define SWP_NOACTIVATE	0x10
#define SWP_FRAMECHANGED	0x20	/* The frame changed: send WM_NCCALCSIZE */
#define SWP_SHOWWINDOW	0x40
#define SWP_HIDEWINDOW	0x80
#define SWP_NOCOPYBITS	0x100
#define SWP_NOOWNERZORDER	0x200	/* Don't do owner Z ordering */
#define SWP_DRAWFRAME	SWP_FRAMECHANGED
#define SWP_NOREPOSITION	SWP_NOOWNERZORDER
#define SWP_NOSENDCHANGING	0x400
#define SWP_DEFERERASE	0x2000

/* Map modes */
#define MM_TEXT	1
#define MM_LOMETRIC	2
#define MM_HIMETRIC	3
#define MM_LOENGLISH	4
#define MM_HIENGLISH	5
#define MM_TWIPS	6
#define MM_ISOTROPIC	7
#define MM_ANISOTROPIC	8

/* constants for CreateDIBitmap */
#define CBM_INIT	4L

/* DIB color table identifiers */
#define DIB_RGB_COLORS	0
#define DIB_PAL_COLORS	1

/* User Button Notification Codes */
#define BN_CLICKED	0
#define BN_PAINT	1
#define BN_HILITE	2
#define BN_UNHILITE	3
#define BN_DISABLE	4
#define BN_DOUBLECLICKED	5

#define WF_PMODE	1
#define WF_CPU286	2
#define WF_CPU386	4
#define WF_CPU486	8
#define WF_STANDARD	0x10
#define WF_WIN286	0x10
#define WF_ENHANCED	0x20
#define WF_WIN386	0x20
#define WF_CPU086	0x40
#define WF_CPU186	0x80
#define WF_LARGEFRAME	0x100
#define WF_SMALLFRAME	0x200
#define WF_80x87	0x400
#define WF_PAGING	0x800
#define WF_WLO	0x8000

/* WM_H/VSCROLL commands */
#define SB_LINEUP	0
#define SB_LINELEFT	0
#define SB_LINEDOWN	1
#define SB_LINERIGHT	1
#define SB_PAGEUP	2
#define SB_PAGELEFT	2
#define SB_PAGEDOWN	3
#define SB_PAGERIGHT	3
#define SB_THUMBPOSITION	4
#define SB_THUMBTRACK	5
#define SB_TOP	6
#define SB_LEFT	6
#define SB_BOTTOM	7
#define SB_RIGHT	7
#define SB_ENDSCROLL	8
/* Scroll bar selection constants */
#define SB_HORZ	0
#define SB_VERT	1
#define SB_CTL	2
#define SB_BOTH	3

/* Get/SetWindowWord/Long offsets for use with WC_DIALOG windows */
#define DWL_MSGRESULT	0
#define DWL_DLGPROC	4
#define DWL_USER	8

/* Owner draw control types */
#define ODT_MENU	1
#define ODT_LISTBOX	2
#define ODT_COMBOBOX	3
#define ODT_BUTTON	4

/* Owner draw state */
#define ODS_SELECTED	1
#define ODS_GRAYED	2
#define ODS_DISABLED	4
#define ODS_CHECKED	8
#define ODS_FOCUS	0x10

#define ETO_GRAYED	1
#define ETO_OPAQUE	2
#define ETO_CLIPPED	4

/* Scroll bar styles */
#define SBS_HORZ	0L
#define SBS_VERT	1L
#define SBS_TOPALIGN	2L
#define SBS_LEFTALIGN	2L
#define SBS_BOTTOMALIGN	4L
#define SBS_RIGHTALIGN	4L
#define SBS_SIZEBOXTOPLEFTALIGN	2L
#define SBS_SIZEBOXBOTTOMRIGHTALIGN	4L
#define SBS_SIZEBOX	8L

#define WH_GETMESSAGE	3
#define WH_CALLWNDPROC	4
#define WH_MSGFILTER	(-1)
#define WH_SYSMSGFILTER	6

/* SetWindowPos() hwndInsertAfter field values */
#define HWND_TOP	((HWND)0)
#define HWND_BOTTOM	((HWND)1)
#define HWND_TOPMOST	((HWND)-1)
#define HWND_NOTOPMOST	((HWND)-2)
/* Special value for CreateWindow, et al. */
#define HWND_DESKTOP	((HWND)0)

/* Binary raster ops */
#define R2_BLACK	1
#define R2_NOTMERGEPEN	2
#define R2_MASKNOTPEN	3
#define R2_NOTCOPYPEN	4
#define R2_MASKPENNOT	5
#define R2_NOT	6
#define R2_XORPEN	7
#define R2_NOTMASKPEN	8
#define R2_MASKPEN	9
#define R2_NOTXORPEN	10
#define R2_NOP	11
#define R2_MERGENOTPEN	12
#define R2_COPYPEN	13
#define R2_MERGEPENNOT	14
#define R2_MERGEPEN	15
#define R2_WHITE	16

/* Combo box messages */
#define CB_GETEDITSEL	(WM_USER+0)
#define CB_LIMITTEXT	(WM_USER+1)
#define CB_SETEDITSEL	(WM_USER+2)
#define CB_ADDSTRING	(WM_USER+3)
#define CB_DELETESTRING	(WM_USER+4)
#define CB_DIR	(WM_USER+5)
#define CB_GETCOUNT	(WM_USER+6)
#define CB_GETCURSEL	(WM_USER+7)
#define CB_GETLBTEXT	(WM_USER+8)
#define CB_GETLBTEXTLEN	(WM_USER+9)
#define CB_INSERTSTRING	(WM_USER+10)
#define CB_RESETCONTENT	(WM_USER+11)
#define CB_FINDSTRING	(WM_USER+12)
#define CB_SELECTSTRING	(WM_USER+13)
#define CB_SETCURSEL	(WM_USER+14)
#define CB_SHOWDROPDOWN	(WM_USER+15)
#define CB_GETITEMDATA	(WM_USER+16)
#define CB_SETITEMDATA	(WM_USER+17)
#define CB_GETDROPPEDCONTROLRECT (WM_USER+18)
#define CB_SETITEMHEIGHT	(WM_USER+19)
#define CB_GETITEMHEIGHT	(WM_USER+20)
#define CB_SETEXTENDEDUI	(WM_USER+21)
#define CB_GETEXTENDEDUI	(WM_USER+22)
#define CB_GETDROPPEDSTATE	(WM_USER+23)
#define CB_FINDSTRINGEXACT	(WM_USER+24)
/* Combo box message return values */
#define CB_OKAY	0
#define CB_ERR	(-1)
#define CB_ERRSPACE	(-2)

#define CW_USEDEFAULT	((int)0x8000)

/* Class field offsets for GetClassLong() and GetClassWord() */
#define GCL_MENUNAME	(-8)
#define GCW_HBRBACKGROUND	(-10)
#define GCW_HCURSOR	(-12)
#define GCW_HICON	(-14)
#define GCW_HMODULE	(-16)
#define GCW_CBWNDEXTRA	(-18)
#define GCW_CBCLSEXTRA	(-20)
#define GCL_WNDPROC	(-24)
#define GCW_STYLE	(-26)
#define GCW_ATOM	(-32)

/* Extended Window Styles */
#define WS_EX_DLGMODALFRAME	1L
#define WS_EX_NOPARENTNOTIFY	4L
#define WS_EX_TOPMOST	8L
#define WS_EX_ACCEPTFILES	0x10L
#define WS_EX_TRANSPARENT	0x20L

#define MK_LBUTTON	1
#define MK_RBUTTON	2
#define MK_SHIFT	4
#define MK_CONTROL	8
#define MK_MBUTTON	0x10

/* System Menu Command Values */
#define SC_SIZE	0xF000
#define SC_MOVE	0xF010
#define SC_MINIMIZE	0xF020
#define SC_MAXIMIZE	0xF030
#define SC_NEXTWINDOW	0xF040
#define SC_PREVWINDOW	0xF050
#define SC_CLOSE	0xF060
#define SC_VSCROLL	0xF070
#define SC_HSCROLL	0xF080
#define SC_MOUSEMENU	0xF090
#define SC_KEYMENU	0xF100
#define SC_ARRANGE	0xF110
#define SC_RESTORE	0xF120
#define SC_TASKLIST	0xF130
#define SC_SCREENSAVE	0xF140
#define SC_HOTKEY	0xF150
/* Obsolete names */
#define SC_ICON	SC_MINIMIZE
#define SC_ZOOM	SC_MAXIMIZE

/* CombineRgn() command values */
#define RGN_AND	1
#define RGN_OR	2
#define RGN_XOR	3
#define RGN_DIFF	4
#define RGN_COPY	5

/* SetErrorMode() constants */
#define SEM_FAILCRITICALERRORS	1
#define SEM_NOGPFAULTERRORBOX	2
#define SEM_NOOPENFILEERRORBOX	0x8000

/* Stock pens for use with GetStockObject(); */
#define WHITE_PEN	6
#define BLACK_PEN	7
#define NULL_PEN	8

/* Edit control notification codes */
#define EN_SETFOCUS	0x100
#define EN_KILLFOCUS	0x200
#define EN_CHANGE	0x300
#define EN_UPDATE	0x400
#define EN_ERRSPACE	0x500
#define EN_MAXTEXT	0x501
#define EN_HSCROLL	0x601
#define EN_VSCROLL	0x602

/* WM_CTLCOLOR control IDs */
#define CTLCOLOR_MSGBOX	0
#define CTLCOLOR_EDIT	1
#define CTLCOLOR_LISTBOX	2
#define CTLCOLOR_BTN	3
#define CTLCOLOR_DLG	4
#define CTLCOLOR_SCROLLBAR	5
#define CTLCOLOR_STATIC	6

/* WM_NCHITTEST return codes */
#define HTERROR	(-2)
#define HTTRANSPARENT	(-1)
#define HTNOWHERE	0
#define HTCLIENT	1
#define HTCAPTION	2
#define HTSYSMENU	3
#define HTSIZE	4
#define HTMENU	5
#define HTHSCROLL	6
#define HTVSCROLL	7
#define HTMINBUTTON	8
#define HTMAXBUTTON	9
#define HTLEFT	10
#define HTRIGHT	11
#define HTTOP	12
#define HTTOPLEFT	13
#define HTTOPRIGHT	14
#define HTBOTTOM	15
#define HTBOTTOMLEFT	16
#define HTBOTTOMRIGHT	17
#define HTBORDER	18
#define HTGROWBOX	HTSIZE
#define HTREDUCE	HTMINBUTTON
#define HTZOOM	HTMAXBUTTON

/* GetWindow() constants */
#define GW_HWNDFIRST	0
#define GW_HWNDLAST	1
#define GW_HWNDNEXT	2
#define GW_HWNDPREV	3
#define GW_OWNER	4
#define GW_CHILD	5

/* Device Parameters for GetDeviceCaps() */
#define DRIVERVERSION 0
#define TECHNOLOGY	2
#define HORZSIZE	4
#define VERTSIZE	6
#define HORZRES	8
#define VERTRES	10
#define BITSPIXEL	12
#define PLANES	14
#define NUMBRUSHES	16
#define NUMPENS	18
#define NUMMARKERS	20
#define NUMFONTS	22
#define NUMCOLORS	24
#define PDEVICESIZE	26
#define CURVECAPS	28
#define LINECAPS	30
#define POLYGONALCAPS 32
#define TEXTCAPS	34
#define CLIPCAPS	36
#define RASTERCAPS	38
#define ASPECTX	40
#define ASPECTY	42
#define ASPECTXY	44

#define LOGPIXELSX	88
#define LOGPIXELSY	90

#define SIZEPALETTE	104
#define NUMRESERVED	106
#define COLORRES	108

#define MDITILE_VERTICAL	0
#define MDITILE_HORIZONTAL	1
#define MDITILE_SKIPDISABLED	2

/* Combo box notification codes */
#define CBN_ERRSPACE	(-1)
#define CBN_SELCHANGE	1
#define CBN_DBLCLK	2
#define CBN_SETFOCUS	3
#define CBN_KILLFOCUS	4
#define CBN_EDITCHANGE	5
#define CBN_EDITUPDATE	6
#define CBN_DROPDOWN	7
#define CBN_CLOSEUP	8
#define CBN_SELENDOK	9
#define CBN_SELENDCANCEL	10

/* EnumFonts font type values */
#define RASTER_FONTTYPE	1
#define DEVICE_FONTTYPE	2
#define TRUETYPE_FONTTYPE	4

/* StretchBlt() Modes */
#define BLACKONWHITE	1
#define WHITEONBLACK	2
#define COLORONCOLOR	3

/* ExtFloodFill style flags */
#define	FLOODFILLBORDER	0
#define	FLOODFILLSURFACE	1

/* Background Modes */
#define TRANSPARENT	1
#define OPAQUE	2

/* constants for the biCompression field */
#define BI_RGB	0L
#define BI_RLE8	1L
#define BI_RLE4	2L

#define MDIS_ALLCHILDSTYLES	1

/* Flags returned by GetSystemDebugState.*/
#define SDS_MENU	1
#define SDS_SYSMODAL	2
#define SDS_NOTASKQUEUE	4
#define SDS_DIALOG	8
#define SDS_TASKLOCKED	0x10

/* WM_MOUSEACTIVATE return codes */
#define MA_ACTIVATE	1
#define MA_ACTIVATEANDEAT	2
#define MA_NOACTIVATE	3
#define MA_NOACTIVATEANDEAT 4

/* PeekMessage() options */
#define PM_NOREMOVE	0
#define PM_REMOVE	1
#define PM_NOYIELD	2

/* Dialog messages */
#define DM_GETDEFID	(WM_USER+0)
#define DM_SETDEFID	(WM_USER+1)

/* Flags for TrackPopupMenu */
#define TPM_LEFTBUTTON	0
#define TPM_RIGHTBUTTON	2
#define TPM_LEFTALIGN	0
#define TPM_CENTERALIGN	4
#define TPM_RIGHTALIGN	8

/* OEM Resource Ordinal Numbers */
#define OBM_CLOSE	32754
#define OBM_UPARROW	32753
#define OBM_DNARROW	32752
#define OBM_RGARROW	32751
#define OBM_LFARROW	32750
#define OBM_REDUCE	32749
#define OBM_ZOOM	32748
#define OBM_RESTORE	32747
#define OBM_REDUCED	32746
#define OBM_ZOOMD	32745
#define OBM_RESTORED	32744
#define OBM_UPARROWD	32743
#define OBM_DNARROWD	32742
#define OBM_RGARROWD	32741
#define OBM_LFARROWD	32740
#define OBM_MNARROW	32739
#define OBM_COMBO	32738
#define OBM_UPARROWI	32737
#define OBM_DNARROWI	32736
#define OBM_RGARROWI	32735
#define OBM_LFARROWI	32734

/* Local Memory Flags */
#define LMEM_FIXED	0
#define LMEM_MOVEABLE	2
#define LMEM_NOCOMPACT	0x10
#define LMEM_NODISCARD	0x20
#define LMEM_ZEROINIT	0x40
#define LMEM_MODIFY	0x80
#define LMEM_DISCARDABLE	0xF00

/* GDI Escapes */
#define NEWFRAME	1
#define ABORTDOC	2
#define NEXTBAND	3
#define SETCOLORTABLE	4
#define GETCOLORTABLE	5
#define FLUSHOUTPUT	6
#define DRAFTMODE	7
#define QUERYESCSUPPORT	8
#define SETABORTPROC	9
#define STARTDOC	10
#define ENDDOC	11
#define GETPHYSPAGESIZE	12
#define GETPRINTINGOFFSET	13
#define GETSCALINGFACTOR	14
#define MFCOMMENT	15
#define GETPENWIDTH	16
#define SETCOPYCOUNT	17
#define SELECTPAPERSOURCE	18
#define DEVICEDATA	19
#define PASSTHROUGH	19
#define GETTECHNOLGY	20
#define GETTECHNOLOGY	20
#define SETLINECAP	21
#define SETLINEJOIN	22
#define SETMITERLIMIT	23
#define BANDINFO	24
#define DRAWPATTERNRECT	25
#define GETVECTORPENSIZE	26
#define GETVECTORBRUSHSIZE	27
#define ENABLEDUPLEX	28
#define GETSETPAPERBINS	29
#define GETSETPRINTORIENT	30
#define ENUMPAPERBINS	31
#define SETDIBSCALING	32
#define EPSPRINTING	33
#define ENUMPAPERMETRICS	34
#define GETSETPAPERMETRICS	35
#define POSTSCRIPT_DATA	37
#define POSTSCRIPT_IGNORE	38
#define MOUSETRAILS	39

/* CharSet values */
#define ANSI_CHARSET	0
#define DEFAULT_CHARSET	1
#define SYMBOL_CHARSET	2
#define SHIFTJIS_CHARSET	128
#define HANGEUL_CHARSET	129
#define CHINESEBIG5_CHARSET 136
#define OEM_CHARSET	255

/* tmPitchAndFamily values */
#define TMPF_FIXED_PITCH	1
#define TMPF_VECTOR	2
#define TMPF_DEVICE	8
#define TMPF_TRUETYPE	4

/* Text Alignment Options */
#define TA_NOUPDATECP	0
#define TA_UPDATECP	1
#define TA_LEFT	0
#define TA_RIGHT	2
#define TA_CENTER	6
#define TA_TOP	0
#define TA_BOTTOM	8
#define TA_BASELINE	0x18

/* Hatch Styles */
#define HS_HORIZONTAL	0
#define HS_VERTICAL	1
#define HS_FDIAGONAL	2
#define HS_BDIAGONAL	3
#define HS_CROSS	4
#define HS_DIAGCROSS	5

DECLARE_HANDLE(HTASK);
DECLARE_HANDLE(HRSRC);
DECLARE_HANDLE(HBITMAP);
DECLARE_HANDLE(HPEN);
DECLARE_HANDLE(HBRUSH);
DECLARE_HANDLE(HRGN);
DECLARE_HANDLE(HPALETTE);
DECLARE_HANDLE(HFONT);
DECLARE_HANDLE(HMETAFILE);
DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HMENU);
DECLARE_HANDLE(HICON);
DECLARE_HANDLE(HDRVR);
DECLARE_HANDLE(HACCEL);
DECLARE_HANDLE(HINSTANCE);
DECLARE_HANDLE(HDWP);
DECLARE_HANDLE(HDC);
DECLARE_HANDLE32(HHOOK);
#ifdef	STRICT
typedef	const void	NEAR* HGDIOBJ;
#else
DECLARE_HANDLE(HGDIOBJ);
#endif
typedef	HINSTANCE	HMODULE;

typedef struct tagHANDLETABLE {
	HGDIOBJ objectHandle[1];
} HANDLETABLE,*PHANDLETABLE,FAR *LPHANDLETABLE;
typedef struct tagMETARECORD {
	DWORD	rdSize;
	UINT	rdFunction;
	UINT	rdParm[1];
} METARECORD,*PMETARECORD,FAR *LPMETARECORD;
#define LF_FACESIZE	32
typedef struct tagLOGFONT {
	int	lfHeight;
	int	lfWidth;
	int	lfEscapement;
	int	lfOrientation;
	int	lfWeight;
	BYTE	lfItalic;
	BYTE	lfUnderline;
	BYTE	lfStrikeOut;
	BYTE	lfCharSet;
	BYTE	lfOutPrecision;
	BYTE	lfClipPrecision;
	BYTE	lfQuality;
	BYTE	lfPitchAndFamily;
	char	lfFaceName[LF_FACESIZE];
} LOGFONT,*PLOGFONT,NEAR *NPLOGFONT,FAR *LPLOGFONT;
typedef struct tagTEXTMETRIC {
	int	tmHeight;
	int	tmAscent;
	int	tmDescent;
	int	tmInternalLeading;
	int	tmExternalLeading;
	int	tmAveCharWidth;
	int	tmMaxCharWidth;
	int	tmWeight;
	BYTE	tmItalic;
	BYTE	tmUnderlined;
	BYTE	tmStruckOut;
	BYTE	tmFirstChar;
	BYTE	tmLastChar;
	BYTE	tmDefaultChar;
	BYTE	tmBreakChar;
	BYTE	tmPitchAndFamily;
	BYTE	tmCharSet;
	int	tmOverhang;
	int	tmDigitizedAspectX;
	int	tmDigitizedAspectY;
} TEXTMETRIC,*PTEXTMETRIC,NEAR *NPTEXTMETRIC,FAR *LPTEXTMETRIC;
#define LF_FULLFACESIZE	64
typedef struct tagENUMLOGFONT {
	LOGFONT elfLogFont;
	char	elfFullName[LF_FULLFACESIZE];
	char	elfStyle[LF_FACESIZE];
} ENUMLOGFONT,FAR *LPENUMLOGFONT;
typedef struct tagNEWTEXTMETRIC {
	int	tmHeight;
	int	tmAscent;
	int	tmDescent;
	int	tmInternalLeading;
	int	tmExternalLeading;
	int	tmAveCharWidth;
	int	tmMaxCharWidth;
	int	tmWeight;
	BYTE	tmItalic;
	BYTE	tmUnderlined;
	BYTE	tmStruckOut;
	BYTE	tmFirstChar;
	BYTE	tmLastChar;
	BYTE	tmDefaultChar;
	BYTE	tmBreakChar;
	BYTE	tmPitchAndFamily;
	BYTE	tmCharSet;
	int	tmOverhang;
	int	tmDigitizedAspectX;
	int	tmDigitizedAspectY;
	DWORD	ntmFlags;
	UINT	ntmSizeEM;
	UINT	ntmCellHeight;
	UINT	ntmAvgWidth;
} NEWTEXTMETRIC,*PNEWTEXTMETRIC,NEAR *NPNEWTEXTMETRIC,FAR *LPNEWTEXTMETRIC;
typedef struct tagPOINT {
	int x;
	int y;
} POINT,*PPOINT,NEAR *NPPOINT,FAR *LPPOINT;
typedef struct tagLOGBRUSH {
	UINT	lbStyle;
	COLORREF lbColor;
	int	lbHatch;
} LOGBRUSH,*PLOGBRUSH,NEAR *NPLOGBRUSH,FAR *LPLOGBRUSH;
typedef struct tagPALETTEENTRY {
	BYTE	peRed;
	BYTE	peGreen;
	BYTE	peBlue;
	BYTE	peFlags;
} PALETTEENTRY,FAR *LPPALETTEENTRY;
typedef struct tagLOGPALETTE {
	WORD	palVersion;
	WORD	palNumEntries;
	PALETTEENTRY palPalEntry[1];
} LOGPALETTE,*PLOGPALETTE,NEAR *NPLOGPALETTE,FAR *LPLOGPALETTE;
typedef struct tagRECT {
	int left;
	int top;
	int right;
	int bottom;
} RECT,*PRECT,NEAR *NPRECT,FAR *LPRECT;
typedef struct tagSIZE {
	int cx;
	int cy;
} SIZE,*PSIZE,NEAR *NPSIZE,FAR *LPSIZE;
typedef struct tagBITMAPINFOHEADER {
	DWORD	biSize;
	LONG	biWidth;
	LONG	biHeight;
	WORD	biPlanes;
	WORD	biBitCount;
	DWORD	biCompression;
	DWORD	biSizeImage;
	LONG	biXPelsPerMeter;
	LONG	biYPelsPerMeter;
	DWORD	biClrUsed;
	DWORD	biClrImportant;
} BITMAPINFOHEADER,*PBITMAPINFOHEADER,FAR *LPBITMAPINFOHEADER;
typedef struct tagBITMAP {
	int	bmType;
	int	bmWidth;
	int	bmHeight;
	int	bmWidthBytes;
	BYTE	bmPlanes;
	BYTE	bmBitsPixel;
	void FAR* bmBits;
} BITMAP,*PBITMAP,NEAR *NPBITMAP,FAR *LPBITMAP;
typedef struct tagRGBQUAD {
	BYTE	rgbBlue;
	BYTE	rgbGreen;
	BYTE	rgbRed;
	BYTE	rgbReserved;
} RGBQUAD,FAR *LPRGBQUAD;
typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD	bmiColors[1];
} BITMAPINFO,*PBITMAPINFO,FAR *LPBITMAPINFO;
typedef struct tagDOCINFO {
	int	cbSize;
	LPCSTR	lpszDocName;
	LPCSTR	lpszOutput;
} DOCINFO,FAR *LPDOCINFO;
typedef struct tagMSG {
	HWND	hwnd;
	UINT	message;
	WPARAM	wParam;
	LPARAM	lParam;
	DWORD	time;
	POINT	pt;
} MSG,*PMSG,NEAR *NPMSG,FAR *LPMSG;
typedef	HICON	HCURSOR;
typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef struct tagWNDCLASS {
	UINT	style;
	WNDPROC	lpfnWndProc;
	int	cbClsExtra;
	int	cbWndExtra;
	HINSTANCE	hInstance;
	HICON	hIcon;
	HCURSOR	hCursor;
	HBRUSH	hbrBackground;
	LPCSTR	lpszMenuName;
	LPCSTR	lpszClassName;
} WNDCLASS,*PWNDCLASS,NEAR *NPWNDCLASS,FAR *LPWNDCLASS;
typedef struct tagWINDOWPLACEMENT {
	UINT	length;
	UINT	flags;
	UINT	showCmd;
	POINT ptMinPosition;
	POINT ptMaxPosition;
	RECT	rcNormalPosition;
} WINDOWPLACEMENT,*PWINDOWPLACEMENT,FAR *LPWINDOWPLACEMENT;
typedef struct tagPAINTSTRUCT {
	HDC	hdc;
	BOOL	fErase;
	RECT	rcPaint;
	BOOL	fRestore;
	BOOL	fIncUpdate;
	BYTE	rgbReserved[16];
} PAINTSTRUCT,*PPAINTSTRUCT,NEAR *NPPAINTSTRUCT,FAR *LPPAINTSTRUCT;
typedef struct tagOFSTRUCT {
	BYTE cBytes;
	BYTE fFixedDisk;
	UINT nErrCode;
	BYTE reserved[4];
	char szPathName[128];
} OFSTRUCT,*POFSTRUCT,NEAR *NPOFSTRUCT,FAR *LPOFSTRUCT;
typedef struct tagLOGPEN {
	UINT	lopnStyle;
	POINT	lopnWidth;
	COLORREF lopnColor;
} LOGPEN,*PLOGPEN,NEAR *NPLOGPEN,FAR *LPLOGPEN;
typedef struct tagCREATESTRUCT {
	void FAR* lpCreateParams;
	HINSTANCE hInstance;
	HMENU	hMenu;
	HWND	hwndParent;
	int	cy;
	int	cx;
	int	y;
	int	x;
	LONG	style;
	LPCSTR	lpszName;
	LPCSTR	lpszClass;
	DWORD	dwExStyle;
} CREATESTRUCT,FAR *LPCREATESTRUCT;
typedef struct tagMINMAXINFO {
	POINT ptReserved;
	POINT ptMaxSize;
	POINT ptMaxPosition;
	POINT ptMinTrackSize;
	POINT ptMaxTrackSize;
} MINMAXINFO;
typedef struct tagMEASUREITEMSTRUCT {
	UINT	CtlType;
	UINT	CtlID;
	UINT	itemID;
	UINT	itemWidth;
	UINT	itemHeight;
	DWORD	itemData;
} MEASUREITEMSTRUCT,*PMEASUREITEMSTRUCT,NEAR *NPMEASUREITEMSTRUCT,FAR *LPMEASUREITEMSTRUCT;
typedef struct tagDRAWITEMSTRUCT {
	UINT	CtlType;
	UINT	CtlID;
	UINT	itemID;
	UINT	itemAction;
	UINT	itemState;
	HWND	hwndItem;
	HDC	hDC;
	RECT	rcItem;
	DWORD	itemData;
} DRAWITEMSTRUCT,*PDRAWITEMSTRUCT,NEAR *NPDRAWITEMSTRUCT,FAR *LPDRAWITEMSTRUCT;
typedef struct tagMDICREATESTRUCT {
	LPCSTR	szClass;
	LPCSTR	szTitle;
	HINSTANCE hOwner;
	int	x;
	int	y;
	int	cx;
	int	cy;
	DWORD	style;
	LPARAM	lParam;
} MDICREATESTRUCT,FAR *LPMDICREATESTRUCT;
typedef struct tagCLIENTCREATESTRUCT {
	HMENU	hWindowMenu;
	UINT	idFirstChild;
} CLIENTCREATESTRUCT,FAR *LPCLIENTCREATESTRUCT;
typedef struct tagBITMAPCOREHEADER {
	DWORD	bcSize;
	short	bcWidth;
	short	bcHeight;
	WORD	bcPlanes;
	WORD	bcBitCount;
} BITMAPCOREHEADER,*PBITMAPCOREHEADER,FAR *LPBITMAPCOREHEADER;
typedef struct tagBITMAPFILEHEADER {
	UINT	bfType;
	DWORD	bfSize;
	UINT	bfReserved1;
	UINT	bfReserved2;
	DWORD	bfOffBits;
} BITMAPFILEHEADER,*PBITMAPFILEHEADER,FAR *LPBITMAPFILEHEADER;
typedef struct tagRGBTRIPLE {
	BYTE	rgbtBlue;
	BYTE	rgbtGreen;
	BYTE	rgbtRed;
} RGBTRIPLE,FAR *LPRGBTRIPLE;
typedef struct tagBITMAPCOREINFO {
	BITMAPCOREHEADER bmciHeader;
	RGBTRIPLE	bmciColors[1];
} BITMAPCOREINFO,*PBITMAPCOREINFO,FAR *LPBITMAPCOREINFO;
typedef struct tagWINDOWPOS {
	HWND	hwnd;
	HWND	hwndInsertAfter;
	int	x;
	int	y;
	int	cx;
	int	cy;
	UINT	flags;
} WINDOWPOS,FAR *LPWINDOWPOS;

typedef	int	CATCHBUF[9];
typedef	int	FAR *LPCATCHBUF;
typedef	int	HFILE;
typedef	LRESULT	(CALLBACK* DRIVERPROC)(DWORD,HDRVR,UINT,LPARAM,LPARAM);

#ifdef	STRICT
typedef	LRESULT	(CALLBACK* HOOKPROC)(int code,WPARAM wParam,LPARAM lParam);
typedef	BOOL	(CALLBACK* DLGPROC)(HWND,UINT,WPARAM,LPARAM);
typedef	void	(CALLBACK* TIMERPROC)(HWND,UINT,UINT,DWORD);
typedef	BOOL	(CALLBACK* PROPENUMPROC)(HWND,LPCSTR,HANDLE);
typedef	BOOL	(CALLBACK* WNDENUMPROC)(HWND,LPARAM);
typedef	int	(CALLBACK* MFENUMPROC)(HDC,HANDLETABLE FAR*,METARECORD FAR*,int,LPARAM);
typedef	int	(CALLBACK* OLDFONTENUMPROC)(const LOGFONT FAR*,const TEXTMETRIC FAR*,int,LPARAM);
typedef	int	(CALLBACK* FONTENUMPROC)(const ENUMLOGFONT FAR*,const NEWTEXTMETRIC FAR*,int,LPARAM);
typedef	void	(CALLBACK* LINEDDAPROC)(int,int,LPARAM);
typedef	BOOL	(CALLBACK* GRAYSTRINGPROC)(HDC,LPARAM,int);
typedef	int	(CALLBACK* GOBJENUMPROC)(void FAR*,LPARAM);
typedef	BOOL	(CALLBACK* GNOTIFYPROC)(HGLOBAL);
typedef	HGLOBAL	(CALLBACK* RSRCHDLRPROC)(HGLOBAL,HINSTANCE,HRSRC);
#else
typedef	FARPROC	HOOKPROC;
typedef	FARPROC	DLGPROC;
typedef	FARPROC	TIMERPROC;
typedef	FARPROC	PROPENUMPROC;
typedef	FARPROC	WNDENUMPROC;
typedef	FARPROC	MFENUMPROC;
typedef	FARPROC	OLDFONTENUMPROC;
typedef	FARPROC	FONTENUMPROC;
typedef	FARPROC	LINEDDAPROC;
typedef	FARPROC	GRAYSTRINGPROC;
typedef	FARPROC	GOBJENUMPROC;
typedef	FARPROC	GNOTIFYPROC;
typedef	FARPROC	RSRCHDLRPROC;
#endif

DWORD	WINAPI	GetVersion(void);
DWORD	WINAPI	GetFreeSpace(UINT);
UINT	WINAPI	GetWindowsDirectory(LPSTR,UINT);
UINT	WINAPI	GetSystemDirectory(LPSTR,UINT);
DWORD	WINAPI	GetWinFlags(void);
LPSTR	WINAPI	GetDOSEnvironment(void);
DWORD	WINAPI	GetCurrentTime(void);
DWORD	WINAPI	GetTickCount(void);
void	WINAPI	FatalExit(int);
void	WINAPI	FatalAppExit(UINT,LPCSTR);
UINT	WINAPI	SetErrorMode(UINT);
int	WINAPI	Catch(int FAR*);
void	WINAPI	Throw(const int FAR*,int);
HINSTANCE	WINAPI	LoadModule(LPCSTR,LPVOID);
BOOL	WINAPI	FreeModule(HINSTANCE);
HINSTANCE	WINAPI	LoadLibrary(LPCSTR);
void	WINAPI	FreeLibrary(HINSTANCE);
UINT	WINAPI	WinExec(LPCSTR,UINT);
HMODULE	WINAPI	GetModuleHandle(LPCSTR);
int	WINAPI	GetModuleUsage(HINSTANCE);
int	WINAPI	GetModuleFileName(HINSTANCE,LPSTR,int);
FARPROC	WINAPI	GetProcAddress(HINSTANCE,LPCSTR);
int	WINAPI	GetInstanceData(HINSTANCE,PBYTE,int);
FARPROC	WINAPI	MakeProcInstance(FARPROC,HINSTANCE);
void	WINAPI	FreeProcInstance(FARPROC);
LONG	WINAPI	SetSwapAreaSize(UINT);
UINT	WINAPI	GetNumTasks(void);
HTASK	WINAPI	GetCurrentTask(void);
void	WINAPI	Yield(void);
void	WINAPI	DirectedYield(HTASK);
HGLOBAL	WINAPI	GlobalAlloc(UINT,DWORD);
HGLOBAL	WINAPI	GlobalReAlloc(HGLOBAL,DWORD,UINT);
HGLOBAL	WINAPI	GlobalFree(HGLOBAL);
DWORD	WINAPI	GlobalDosAlloc(DWORD);
UINT	WINAPI	GlobalDosFree(UINT);
BOOL	WINAPI	GlobalUnlock(HGLOBAL);
DWORD	WINAPI	GlobalSize(HGLOBAL);
DWORD	WINAPI	GlobalHandle(UINT);
UINT	WINAPI	GlobalPageLock(HGLOBAL);
UINT	WINAPI	GlobalPageUnlock(HGLOBAL);
HGLOBAL	WINAPI	GlobalLRUNewest(HGLOBAL);
HGLOBAL	WINAPI	GlobalLRUOldest(HGLOBAL);
DWORD	WINAPI	GlobalCompact(DWORD);
void	WINAPI	GlobalNotify(GNOTIFYPROC);
UINT	WINAPI	AllocSelector(UINT);
UINT	WINAPI	FreeSelector(UINT);
HLOCAL	WINAPI	LocalAlloc(UINT,UINT);
HLOCAL	WINAPI	LocalReAlloc(HLOCAL,UINT,UINT);
HLOCAL	WINAPI	LocalFree(HLOCAL);
BOOL	WINAPI	LocalUnlock(HLOCAL);
UINT	WINAPI	LocalSize(HLOCAL);
UINT	WINAPI	LocalCompact(UINT);
UINT	WINAPI	_lread(HFILE,void _huge*,UINT);
UINT	WINAPI	_lwrite(HFILE,const void _huge*,UINT);
HRSRC	WINAPI	FindResource(HINSTANCE,LPCSTR,LPCSTR);
HGLOBAL	WINAPI	LoadResource(HINSTANCE,HRSRC);
BOOL	WINAPI	FreeResource(HGLOBAL);
DWORD	WINAPI	SizeofResource(HINSTANCE,HRSRC);
RSRCHDLRPROC	WINAPI	SetResourceHandler(HINSTANCE,LPCSTR,RSRCHDLRPROC);
UINT	WINAPI	GetProfileInt(LPCSTR,LPCSTR,int);
int	WINAPI	GetProfileString(LPCSTR,LPCSTR,LPCSTR,LPSTR,int);
BOOL	WINAPI	WriteProfileString(LPCSTR,LPCSTR,LPCSTR);
UINT	WINAPI	GetPrivateProfileInt(LPCSTR,LPCSTR,int,LPCSTR);
int	WINAPI	GetPrivateProfileString(LPCSTR,LPCSTR,LPCSTR,LPSTR,int,LPCSTR);
BOOL	WINAPI	WritePrivateProfileString(LPCSTR,LPCSTR,LPCSTR,LPCSTR);
LPSTR	WINAPI	AnsiNext(LPCSTR);
LPSTR	WINAPI	AnsiPrev(LPCSTR,LPCSTR);
BOOL	WINAPI	IsDBCSLeadByte(BYTE);
int	WINAPI	LoadString(HINSTANCE,UINT,LPSTR,int);
int	WINAPI	GetKBCodePage(void);
int	WINAPI	ToAscii(UINT,UINT,BYTE FAR*,DWORD FAR*,UINT);
HDC	WINAPI	CreateCompatibleDC(HDC);
BOOL	WINAPI	DeleteDC(HDC);
int	WINAPI	SaveDC(HDC);
BOOL	WINAPI	RestoreDC(HDC,int);
int	WINAPI	MulDiv(int,int,int);
int	WINAPI	SetMapMode(HDC,int);
DWORD	WINAPI	SetWindowOrg(HDC,int,int);
DWORD	WINAPI	GetWindowOrg(HDC);
DWORD	WINAPI	SetWindowExt(HDC,int,int);
DWORD	WINAPI	GetWindowExt(HDC);
DWORD	WINAPI	SetViewportOrg(HDC,int,int);
DWORD	WINAPI	GetViewportOrg(HDC);
DWORD	WINAPI	SetViewportExt(HDC,int,int);
DWORD	WINAPI	GetViewportExt(HDC);
BOOL	WINAPI	SetWindowOrgEx(HDC,int,int,POINT FAR*);
BOOL	WINAPI	GetWindowOrgEx(HDC,POINT FAR*);
BOOL	WINAPI	DPtoLP(HDC,POINT FAR*,int);
BOOL	WINAPI	LPtoDP(HDC,POINT FAR*,int);
COLORREF	WINAPI	GetNearestColor(HDC,COLORREF);
COLORREF	WINAPI	GetSysColor(int);
void	WINAPI	SetSysColors(int,const int FAR*,const COLORREF FAR*);
HGDIOBJ	WINAPI	GetStockObject(int);
BOOL	WINAPI	DeleteObject(HGDIOBJ);
HGDIOBJ	WINAPI	SelectObject(HDC,HGDIOBJ);
int	WINAPI	GetObject(HGDIOBJ,int,void FAR*);
BOOL	WINAPI	UnrealizeObject(HGDIOBJ);
HPEN	WINAPI	CreatePen(int,int,COLORREF);
HBRUSH	WINAPI	CreateSolidBrush(COLORREF);
HBRUSH	WINAPI	CreatePatternBrush(HBITMAP);
HBRUSH	WINAPI	CreateBrushIndirect(LOGBRUSH FAR*);
DWORD	WINAPI	SetBrushOrg(HDC,int,int);
HRGN	WINAPI	CreateRectRgn(int,int,int,int);
int	WINAPI	CombineRgn(HRGN,HRGN,HRGN,int);
HPALETTE	WINAPI	CreatePalette(const LOGPALETTE FAR*);
HPALETTE	WINAPI	SelectPalette(HDC,HPALETTE,BOOL);
UINT	WINAPI	RealizePalette(HDC);
UINT	WINAPI	GetSystemPaletteEntries(HDC,UINT,UINT,PALETTEENTRY FAR*);
BOOL	WINAPI	RectVisible(HDC,const RECT FAR*);
DWORD	WINAPI	MoveTo(HDC,int,int);
BOOL	WINAPI	MoveToEx(HDC,int,int,POINT FAR*);
BOOL	WINAPI	GetCurrentPositionEx(HDC,POINT FAR*);
BOOL	WINAPI	LineTo(HDC,int,int);
BOOL	WINAPI	Polyline(HDC,const POINT FAR*,int);
void	WINAPI	LineDDA(int,int,int,int,LINEDDAPROC,LPARAM);
BOOL	WINAPI	Rectangle(HDC,int,int,int,int);
BOOL	WINAPI	Ellipse(HDC,int,int,int,int);
BOOL	WINAPI	Arc(HDC,int,int,int,int,int,int,int,int);
BOOL	WINAPI	Chord(HDC,int,int,int,int,int,int,int,int);
BOOL	WINAPI	Pie(HDC,int,int,int,int,int,int,int,int);
BOOL	WINAPI	Polygon(HDC,const POINT FAR*,int);
BOOL	WINAPI	PolyPolygon(HDC,const POINT FAR*,int FAR*,int);
BOOL	WINAPI	ExtFloodFill(HDC,int,int,COLORREF,UINT);
int	WINAPI	FillRect(HDC,const RECT FAR*,HBRUSH);
int	WINAPI	FrameRect(HDC,const RECT FAR*,HBRUSH);
void	WINAPI	DrawFocusRect(HDC,const RECT FAR*);
BOOL	WINAPI	TextOut(HDC,int,int,LPCSTR,int);
LONG	WINAPI	TabbedTextOut(HDC,int,int,LPCSTR,int,int,int FAR*,int);
BOOL	WINAPI	ExtTextOut(HDC,int,int,UINT,const RECT FAR*,LPCSTR,UINT,int FAR*);
DWORD	WINAPI	GetTextExtent(HDC,LPCSTR,int);
DWORD	WINAPI	GetTabbedTextExtent(HDC,LPCSTR,int,int,int FAR*);
BOOL	WINAPI	GetTextExtentPoint(HDC,LPCSTR,int,SIZE FAR*);
int	WINAPI	DrawText(HDC,LPCSTR,int,RECT FAR*,UINT);
BOOL	WINAPI	GrayString(HDC,HBRUSH,GRAYSTRINGPROC,LPARAM,int,int,int,int,int);
COLORREF	WINAPI	SetTextColor(HDC,COLORREF);
COLORREF	WINAPI	GetTextColor(HDC);
COLORREF	WINAPI	SetBkColor(HDC,COLORREF);
COLORREF	WINAPI	GetBkColor(HDC);
int	WINAPI	SetBkMode(HDC,int);
int	WINAPI	GetBkMode(HDC);
UINT	WINAPI	SetTextAlign(HDC,UINT);
UINT	WINAPI	GetTextAlign(HDC);
HFONT	WINAPI	CreateFont(int,int,int,int,int,BYTE,BYTE,BYTE,BYTE,BYTE,BYTE,BYTE,BYTE,LPCSTR);
HFONT	WINAPI	CreateFontIndirect(const LOGFONT FAR*);
BOOL	WINAPI	GetTextMetrics(HDC,TEXTMETRIC FAR*);
HBITMAP	WINAPI	CreateBitmap(int,int,UINT,UINT,const void FAR*);
HBITMAP	WINAPI	CreateBitmapIndirect(BITMAP FAR*);
HBITMAP	WINAPI	CreateCompatibleBitmap(HDC,int,int);
HBITMAP	WINAPI	CreateDiscardableBitmap(HDC,int,int);
HBITMAP	WINAPI	CreateDIBitmap(HDC,BITMAPINFOHEADER FAR*,DWORD,const void FAR*,BITMAPINFO FAR*,UINT);
HBITMAP	WINAPI	LoadBitmap(HINSTANCE,LPCSTR);
BOOL	WINAPI	BitBlt(HDC,int,int,int,int,HDC,int,int,DWORD);
BOOL	WINAPI	PatBlt(HDC,int,int,int,int,DWORD);
BOOL	WINAPI	StretchBlt(HDC,int,int,int,int,HDC,int,int,int,int,DWORD);
int	WINAPI	StretchDIBits(HDC,int,int,int,int,int,int,int,int,const void FAR*,LPBITMAPINFO,UINT,DWORD);
COLORREF	WINAPI	SetPixel(HDC,int,int,COLORREF);
COLORREF	WINAPI	GetPixel(HDC,int,int);
int	WINAPI	SetStretchBltMode(HDC,int);
int	WINAPI	GetStretchBltMode(HDC);
int	WINAPI	SetROP2(HDC,int);
int	WINAPI	GetROP2(HDC);
LONG	WINAPI	SetBitmapBits(HBITMAP,DWORD,const void FAR*);
LONG	WINAPI	GetBitmapBits(HBITMAP,LONG,void FAR*);
int	WINAPI	SetDIBits(HDC,HBITMAP,UINT,UINT,const void FAR*,BITMAPINFO FAR*,UINT);
int	WINAPI	GetDIBits(HDC,HBITMAP,UINT,UINT,void FAR*,BITMAPINFO FAR*,UINT);
int	WINAPI	SetDIBitsToDevice(HDC,int,int,int,int,int,int,UINT,UINT,void FAR*,BITMAPINFO FAR*,UINT);
BOOL	WINAPI	EnumMetaFile(HDC,HMETAFILE,MFENUMPROC,LPARAM);
int	WINAPI	StartDoc(HDC,DOCINFO FAR*);
int	WINAPI	GetSystemMetrics(int);
UINT	WINAPI	GetDoubleClickTime(void);
void	WINAPI	SetDoubleClickTime(UINT);
void	WINAPI	SetRect(RECT FAR*,int,int,int,int);
void	WINAPI	SetRectEmpty(RECT FAR*);
void	WINAPI	CopyRect(RECT FAR*,const RECT FAR*);
BOOL	WINAPI	IsRectEmpty(const RECT FAR*);
BOOL	WINAPI	EqualRect(const RECT FAR*,const RECT FAR*);
BOOL	WINAPI	IntersectRect(RECT FAR*,const RECT FAR*,const RECT FAR*);
BOOL	WINAPI	UnionRect(RECT FAR*,const RECT FAR*,const RECT FAR*);
BOOL	WINAPI	SubtractRect(RECT FAR*,const RECT FAR*,const RECT FAR*);
void	WINAPI	OffsetRect(RECT FAR*,int,int);
void	WINAPI	InflateRect(RECT FAR*,int,int);
BOOL	WINAPI	PtInRect(const RECT FAR*,POINT);
BOOL	WINAPI	GetMessage(MSG FAR*,HWND,UINT,UINT);
BOOL	WINAPI	PeekMessage(MSG FAR*,HWND,UINT,UINT,UINT);
BOOL	WINAPI	TranslateMessage(const MSG FAR*);
LONG	WINAPI	DispatchMessage(const MSG FAR*);
BOOL	WINAPI	PostMessage(HWND,UINT,WPARAM,LPARAM);
LRESULT	WINAPI	SendMessage(HWND,UINT,WPARAM,LPARAM);
BOOL	WINAPI	PostAppMessage(HTASK,UINT,WPARAM,LPARAM);
void	WINAPI	ReplyMessage(LRESULT);
BOOL	WINAPI	InSendMessage(void);
void	WINAPI	PostQuitMessage(int);
ATOM	WINAPI	RegisterClass(const WNDCLASS FAR*);
BOOL	WINAPI	UnregisterClass(LPCSTR,HINSTANCE);
BOOL	WINAPI	GetClassInfo(HINSTANCE,LPCSTR,WNDCLASS FAR*);
int	WINAPI	GetClassName(HWND,LPSTR,int);
WORD	WINAPI	GetClassWord(HWND,int);
WORD	WINAPI	SetClassWord(HWND,int,WORD);
LONG	WINAPI	GetClassLong(HWND,int);
LONG	WINAPI	SetClassLong(HWND,int,LONG);
BOOL	WINAPI	IsWindow(HWND);
HWND	WINAPI	CreateWindowEx(DWORD,LPCSTR,LPCSTR,DWORD,int,int,int,int,HWND,HMENU,HINSTANCE,void FAR*);
HWND	WINAPI	CreateWindow(LPCSTR,LPCSTR,DWORD,int,int,int,int,HWND,HMENU,HINSTANCE,void FAR*);
BOOL	WINAPI	DestroyWindow(HWND);
HTASK	WINAPI	GetWindowTask(HWND);
BOOL	WINAPI	IsChild(HWND,HWND);
HWND	WINAPI	GetParent(HWND);
HWND	WINAPI	SetParent(HWND,HWND);
BOOL	WINAPI	IsWindowVisible(HWND);
BOOL	WINAPI	ShowWindow(HWND,int);
BOOL	WINAPI	EnableWindow(HWND,BOOL);
BOOL	WINAPI	IsWindowEnabled(HWND);
void	WINAPI	SetWindowText(HWND,LPCSTR);
int	WINAPI	GetWindowText(HWND,LPSTR,int);
int	WINAPI	GetWindowTextLength(HWND);
WORD	WINAPI	GetWindowWord(HWND,int);
WORD	WINAPI	SetWindowWord(HWND,int,WORD);
LONG	WINAPI	GetWindowLong(HWND,int);
LONG	WINAPI	SetWindowLong(HWND,int,LONG);
void	WINAPI	GetClientRect(HWND,RECT FAR*);
void	WINAPI	GetWindowRect(HWND,RECT FAR*);
BOOL	WINAPI	GetWindowPlacement(HWND,WINDOWPLACEMENT FAR*);
BOOL	WINAPI	SetWindowPlacement(HWND,const WINDOWPLACEMENT FAR*);
BOOL	WINAPI	SetWindowPos(HWND,HWND,int,int,int,int,UINT);
HDWP	WINAPI	BeginDeferWindowPos(int);
HDWP	WINAPI	DeferWindowPos(HDWP,HWND,HWND,int,int,int,int,UINT);
BOOL	WINAPI	EndDeferWindowPos(HDWP);
BOOL	WINAPI	MoveWindow(HWND,int,int,int,int,BOOL);
BOOL	WINAPI	BringWindowToTop(HWND);
LRESULT	WINAPI	DefWindowProc(HWND,UINT,WPARAM,LPARAM);
void	WINAPI	ShowOwnedPopups(HWND,BOOL);
BOOL	WINAPI	IsIconic(HWND);
BOOL	WINAPI	IsZoomed(HWND);
void	WINAPI	ClientToScreen(HWND,POINT FAR*);
void	WINAPI	ScreenToClient(HWND,POINT FAR*);
void	WINAPI	MapWindowPoints(HWND,HWND,POINT FAR*,UINT);
HWND	WINAPI	WindowFromPoint(POINT);
HWND	WINAPI	ChildWindowFromPoint(HWND,POINT);
HWND	WINAPI	GetDesktopWindow(void);
BOOL	WINAPI	EnumWindows(WNDENUMPROC,LPARAM);
BOOL	WINAPI	EnumChildWindows(HWND,WNDENUMPROC,LPARAM);
BOOL	WINAPI	EnumTaskWindows(HTASK,WNDENUMPROC,LPARAM);
HWND	WINAPI	GetWindow(HWND,UINT);
HWND	WINAPI	GetNextWindow(HWND,UINT);
int	WINAPI	EnumProps(HWND,PROPENUMPROC);
HDC	WINAPI	GetDC(HWND);
int	WINAPI	ReleaseDC(HWND,HDC);
HDC	WINAPI	GetWindowDC(HWND);
HDC	WINAPI	BeginPaint(HWND,PAINTSTRUCT FAR*);
void	WINAPI	EndPaint(HWND,const PAINTSTRUCT FAR*);
void	WINAPI	UpdateWindow(HWND);
BOOL	WINAPI	GetUpdateRect(HWND,RECT FAR*,BOOL);
int	WINAPI	GetUpdateRgn(HWND,HRGN,BOOL);
void	WINAPI	InvalidateRect(HWND,const RECT FAR*,BOOL);
void	WINAPI	ValidateRect(HWND,const RECT FAR*);
void	WINAPI	InvalidateRgn(HWND,HRGN,BOOL);
void	WINAPI	ValidateRgn(HWND,HRGN);
BOOL	WINAPI	RedrawWindow(HWND,const RECT FAR*,HRGN,UINT);
void	WINAPI	ScrollWindow(HWND,int,int,const RECT FAR*,const RECT FAR*);
HWND	WINAPI	SetActiveWindow(HWND);
HWND	WINAPI	GetActiveWindow(void);
HWND	WINAPI	SetFocus(HWND);
HWND	WINAPI	GetFocus(void);
int	WINAPI	GetKeyState(int);
int	WINAPI	GetAsyncKeyState(int);
void	WINAPI	GetKeyboardState(BYTE FAR*);
void	WINAPI	SetKeyboardState(BYTE FAR*);
HWND	WINAPI	SetCapture(HWND);
void	WINAPI	ReleaseCapture(void);
HWND	WINAPI	GetCapture(void);
UINT	WINAPI	SetTimer(HWND,UINT,UINT,TIMERPROC);
BOOL	WINAPI	KillTimer(HWND,UINT);
HACCEL	WINAPI	LoadAccelerators(HINSTANCE,LPCSTR);
int	WINAPI	TranslateAccelerator(HWND,HACCEL,MSG FAR*);
HMENU	WINAPI	CreateMenu(void);
HMENU	WINAPI	CreatePopupMenu(void);
HMENU	WINAPI	LoadMenu(HINSTANCE,LPCSTR);
HMENU	WINAPI	LoadMenuIndirect(const void FAR*);
BOOL	WINAPI	DestroyMenu(HMENU);
HMENU	WINAPI	GetMenu(HWND);
BOOL	WINAPI	SetMenu(HWND,HMENU);
HMENU	WINAPI	GetSystemMenu(HWND,BOOL);
void	WINAPI	DrawMenuBar(HWND);
BOOL	WINAPI	InsertMenu(HMENU,UINT,UINT,UINT,LPCSTR);
BOOL	WINAPI	AppendMenu(HMENU,UINT,UINT,LPCSTR);
BOOL	WINAPI	ModifyMenu(HMENU,UINT,UINT,UINT,LPCSTR);
BOOL	WINAPI	RemoveMenu(HMENU,UINT,UINT);
BOOL	WINAPI	DeleteMenu(HMENU,UINT,UINT);
BOOL	WINAPI	ChangeMenu(HMENU,UINT,LPCSTR,UINT,UINT);
BOOL	WINAPI	EnableMenuItem(HMENU,UINT,UINT);
BOOL	WINAPI	CheckMenuItem(HMENU,UINT,UINT);
HMENU	WINAPI	GetSubMenu(HMENU,int);
int	WINAPI	GetMenuItemCount(HMENU);
UINT	WINAPI	GetMenuItemID(HMENU,int);
int	WINAPI	GetMenuString(HMENU,UINT,LPSTR,int,UINT);
UINT	WINAPI	GetMenuState(HMENU,UINT,UINT);
BOOL	WINAPI	TrackPopupMenu(HMENU,UINT,int,int,int,HWND,const RECT FAR*);
int	WINAPI	SetScrollPos(HWND,int,int,BOOL);
int	WINAPI	GetScrollPos(HWND,int);
void	WINAPI	SetScrollRange(HWND,int,int,int,BOOL);
void	WINAPI	GetScrollRange(HWND,int,int FAR*,int FAR*);
void	WINAPI	ShowScrollBar(HWND,int,BOOL);
BOOL	WINAPI	EnableScrollBar(HWND,int,UINT);
BOOL	WINAPI	OpenClipboard(HWND);
BOOL	WINAPI	CloseClipboard(void);
BOOL	WINAPI	EmptyClipboard(void);
HWND	WINAPI	GetClipboardOwner(void);
HANDLE	WINAPI	SetClipboardData(UINT,HANDLE);
HANDLE	WINAPI	GetClipboardData(UINT);
BOOL	WINAPI	IsClipboardFormatAvailable(UINT);
UINT	WINAPI	RegisterClipboardFormat(LPCSTR);
HCURSOR	WINAPI	LoadCursor(HINSTANCE,LPCSTR);
HCURSOR	WINAPI	CreateCursor(HINSTANCE,int,int,int,int,const void FAR*,const void FAR*);
BOOL	WINAPI	DestroyCursor(HCURSOR);
int	WINAPI	ShowCursor(BOOL);
void	WINAPI	SetCursorPos(int,int);
void	WINAPI	GetCursorPos(POINT FAR*);
HCURSOR	WINAPI	SetCursor(HCURSOR);
HCURSOR	WINAPI	GetCursor(void);
HICON	WINAPI	LoadIcon(HINSTANCE,LPCSTR);
HICON	WINAPI	CreateIcon(HINSTANCE,int,int,BYTE,BYTE,const void FAR*,const void FAR*);
BOOL	WINAPI	DestroyIcon(HICON);
BOOL	WINAPI	DrawIcon(HDC,int,int,HICON);
int	WINAPI	MessageBox(HWND,LPCSTR,LPCSTR,UINT);
void	WINAPI	MessageBeep(UINT);
void	WINAPI	CreateCaret(HWND,HBITMAP,int,int);
void	WINAPI	DestroyCaret(void);
void	WINAPI	SetCaretPos(int,int);
void	WINAPI	GetCaretPos(POINT FAR*);
void	WINAPI	HideCaret(HWND);
void	WINAPI	ShowCaret(HWND);
UINT	WINAPI	GetCaretBlinkTime(void);
void	WINAPI	SetCaretBlinkTime(UINT);
LRESULT	WINAPI	DefFrameProc(HWND,HWND,UINT,WPARAM,LPARAM);
LRESULT	WINAPI	DefMDIChildProc(HWND,UINT,WPARAM,LPARAM);
BOOL	WINAPI	TranslateMDISysAccel(HWND,MSG FAR*);
BOOL	WINAPI	IsDialogMessage(HWND,MSG FAR*);
HWND	WINAPI	CreateDialog(HINSTANCE,LPCSTR,HWND,DLGPROC);
HWND	WINAPI	CreateDialogIndirect(HINSTANCE,const void FAR*,HWND,DLGPROC);
HWND	WINAPI	CreateDialogParam(HINSTANCE,LPCSTR,HWND,DLGPROC,LPARAM);
HWND	WINAPI	CreateDialogIndirectParam(HINSTANCE,const void FAR*,HWND,DLGPROC,LPARAM);
int	WINAPI	DialogBox(HINSTANCE,LPCSTR,HWND,DLGPROC);
int	WINAPI	DialogBoxIndirect(HINSTANCE,HGLOBAL,HWND,DLGPROC);
int	WINAPI	DialogBoxParam(HINSTANCE,LPCSTR,HWND,DLGPROC,LPARAM);
int	WINAPI	DialogBoxIndirectParam(HINSTANCE,HGLOBAL,HWND,DLGPROC,LPARAM);
void	WINAPI	EndDialog(HWND,int);
int	WINAPI	GetDlgCtrlID(HWND);
HWND	WINAPI	GetDlgItem(HWND,int);
LRESULT	WINAPI	SendDlgItemMessage(HWND,int,UINT,WPARAM,LPARAM);
void	WINAPI	SetDlgItemInt(HWND,int,UINT,BOOL);
UINT	WINAPI	GetDlgItemInt(HWND,int,BOOL FAR*,BOOL);
void	WINAPI	SetDlgItemText(HWND,int,LPCSTR);
int	WINAPI	GetDlgItemText(HWND,int,LPSTR,int);
void	WINAPI	CheckDlgButton(HWND,int,UINT);
void	WINAPI	CheckRadioButton(HWND,int,int,int);
UINT	WINAPI	IsDlgButtonChecked(HWND,int);
void	WINAPI	MapDialogRect(HWND,RECT FAR*);
BOOL	WINAPI	UnhookWindowsHook(int,HOOKPROC);
HHOOK	WINAPI	SetWindowsHookEx(int,HOOKPROC,HINSTANCE,HTASK);
BOOL	WINAPI	UnhookWindowsHookEx(HHOOK);
LRESULT	WINAPI	CallNextHookEx(HHOOK,int,WPARAM,LPARAM);
BOOL	WINAPI	QuerySendMessage(HANDLE,HANDLE,HANDLE,LPMSG);
BOOL	WINAPI	LockInput(HANDLE,HWND,BOOL);
LONG	WINAPI	GetSystemDebugState(void);
BOOL	WINAPI	WinHelp(HWND,LPCSTR,UINT,DWORD);
int	WINAPI	GetDeviceCaps(HDC,int);
int	FAR CDECL	wsprintf(LPSTR,LPCSTR,...);
UINT	WINAPI	SetHandleCount(UINT);
int WINAPI	Escape(HDC,int,int,LPCSTR,void FAR*);

#ifdef	STRICT
HHOOK	WINAPI	SetWindowsHook(int,HOOKPROC);
int	WINAPI	EnumObjects(HDC,int,GOBJENUMPROC,LPARAM);
HLOCAL	WINAPI	LocalHandle(void NEAR*);
LRESULT	WINAPI	CallWindowProc(WNDPROC,HWND,UINT,WPARAM,LPARAM);
int	WINAPI	EnumFontFamilies(HDC,LPCSTR,FONTENUMPROC,LPARAM);
int	WINAPI	EnumFonts(HDC,LPCSTR,OLDFONTENUMPROC,LPARAM);
void	NEAR* WINAPI	LocalLock(HLOCAL);
void	FAR* WINAPI	LockResource(HGLOBAL);
void	FAR* WINAPI	GlobalLock(HGLOBAL);
#else
HOOKPROC	WINAPI	SetWindowsHook(int,HOOKPROC);
int	WINAPI	EnumObjects(HDC,int,GOBJENUMPROC,LPSTR);
HLOCAL	WINAPI	LocalHandle(UINT);
LRESULT	WINAPI	CallWindowProc(FARPROC,HWND,UINT,WPARAM,LPARAM);
int	WINAPI	EnumFonts(HDC,LPCSTR,OLDFONTENUMPROC,LPSTR);
int	WINAPI	EnumFontFamilies(HDC,LPCSTR,FONTENUMPROC,LPSTR);
char	NEAR* WINAPI	LocalLock(HLOCAL);
char	FAR* WINAPI	LockResource(HGLOBAL);
char	FAR* WINAPI	GlobalLock(HGLOBAL);
#endif

#ifndef	__WINDOWS_386__
#if	defined(STRICT)
int	PASCAL	WinMain(HINSTANCE,HINSTANCE,LPSTR,int);
int	CALLBACK	LibMain(HINSTANCE,WORD,WORD,LPSTR);
#else
int	PASCAL	WinMain(HANDLE,HANDLE,LPSTR,int);
int	CALLBACK	LibMain(HANDLE,WORD,WORD,LPSTR);
#endif
//int	CALLBACK	WEP(int);
#endif

#ifdef	__cplusplus
}
#endif

#pragma	pack(pop)

#endif
