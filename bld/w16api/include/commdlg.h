#ifndef _INC_COMMDLG
#define _INC_COMMDLG

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push,1)

#define	CC_RGBINIT		1
#define	CC_FULLOPEN		2
#define	CC_PREVENTFULLOPEN	4
#define	CC_SHOWHELP		8
#define	CC_ENABLEHOOK		0x10
#define	CC_ENABLETEMPLATE	0x20
#define	CC_ENABLETEMPLATEHANDLE	0x40
#define	CD_LBSELNOITEMS	(-1)
#define	CD_LBSELCHANGE	0
#define	CD_LBSELSUB	1
#define	CD_LBSELADD	2
#define	CF_SCREENFONTS		1
#define	CF_PRINTERFONTS		2
#define	CF_BOTH			3
#define	CF_SHOWHELP		4
#define	CF_ENABLEHOOK		8
#define	CF_ENABLETEMPLATE	0x10
#define	CF_ENABLETEMPLATEHANDLE	0x20
#define	CF_INITTOLOGFONTSTRUCT	0x40
#define	CF_USESTYLE		0x80
#define	CF_EFFECTS		0x100
#define	CF_APPLY		0x200
#define	CF_ANSIONLY		0x400
#define	CF_SCRIPTSONLY		CF_ANSIONLY
#define	CF_NOVECTORFONTS	0x800
#define	CF_NOOEMFONTS		CF_NOVECTORFONTS
#define	CF_NOSIMULATIONS	0x1000
#define	CF_LIMITSIZE		0x2000
#define	CF_FIXEDPITCHONLY	0x4000
#define	CF_WYSIWYG		0x8000
#define	CF_FORCEFONTEXIST	0x10000
#define	CF_SCALABLEONLY		0x20000
#define	CF_TTONLY		0x40000
#define	CF_NOFACESEL		0x80000
#define	CF_NOSTYLESEL		0x100000
#define	CF_NOSIZESEL		0x200000
#define	DN_DEFAULTPRN	1
#define	FR_DOWN			1
#define	FR_WHOLEWORD		2
#define	FR_MATCHCASE		4
#define	FR_FINDNEXT		8
#define	FR_REPLACE		0x10
#define	FR_REPLACEALL		0x20
#define	FR_DIALOGTERM		0x40
#define	FR_SHOWHELP		0x80
#define	FR_ENABLEHOOK		0x100
#define	FR_ENABLETEMPLATE	0x200
#define	FR_NOUPDOWN		0x400
#define	FR_NOMATCHCASE		0x800
#define	FR_NOWHOLEWORD		0x1000
#define	FR_ENABLETEMPLATEHANDLE	0x2000
#define	FR_HIDEUPDOWN		0x4000
#define	FR_HIDEMATCHCASE	0x8000
#define	FR_HIDEWHOLEWORD	0x10000
#define	OFN_SHAREWARN		0
#define	OFN_SHARENOWARN		1
#define	OFN_SHAREFALLTHROUGH	2
#define	OFN_READONLY			1
#define	OFN_OVERWRITEPROMPT		2
#define	OFN_HIDEREADONLY		4
#define	OFN_NOCHANGEDIR			8
#define	OFN_SHOWHELP			0x10
#define	OFN_ENABLEHOOK			0x20
#define	OFN_ENABLETEMPLATE		0x40
#define	OFN_ENABLETEMPLATEHANDLE	0x80
#define	OFN_NOVALIDATE			0x100
#define	OFN_ALLOWMULTISELECT		0x200
#define	OFN_EXTENSIONDIFFERENT		0x400
#define	OFN_PATHMUSTEXIST		0x800
#define	OFN_FILEMUSTEXIST		0x1000
#define	OFN_CREATEPROMPT		0x2000
#define	OFN_SHAREAWARE			0x4000
#define	OFN_NOREADONLYRETURN		0x8000
#define	OFN_NOTESTFILECREATE		0x10000
#define	OFN_NONETWORKBUTTON		0x20000
#define	OFN_NOLONGNAMES			0x40000
#define	OFN_EXPLORER			0x80000
#define	OFN_NODEREFERENCELINKS		0x100000
#define	OFN_LONGNAMES			0x200000
#define	PD_ALLPAGES			0
#define	PD_SELECTION			1
#define	PD_PAGENUMS			2
#define	PD_NOSELECTION			4
#define	PD_NOPAGENUMS			8
#define	PD_COLLATE			0x10
#define	PD_PRINTTOFILE			0x20
#define	PD_PRINTSETUP			0x40
#define	PD_NOWARNING			0x80
#define	PD_RETURNDC			0x100
#define	PD_RETURNIC			0x200
#define	PD_RETURNDEFAULT		0x400
#define	PD_SHOWHELP			0x800
#define	PD_ENABLEPRINTHOOK		0x1000
#define	PD_ENABLESETUPHOOK		0x2000
#define	PD_ENABLEPRINTTEMPLATE		0x4000
#define	PD_ENABLESETUPTEMPLATE		0x8000
#define	PD_ENABLEPRINTTEMPLATEHANDLE	0x10000
#define	PD_ENABLESETUPTEMPLATEHANDLE	0x20000
#define	PD_USEDEVMODECOPIES		0x40000
#define	PD_DISABLEPRINTTOFILE		0x80000
#define	PD_HIDEPRINTTOFILE		0x100000
#define	PD_NONETWORKBUTTON		0x200000
#define	BOLD_FONTTYPE		0x100
#define	ITALIC_FONTTYPE		0x200
#define	REGULAR_FONTTYPE	0x400
#define	SCREEN_FONTTYPE		0x2000
#define	PRINTER_FONTTYPE	0x4000
#define	SIMULATED_FONTTYPE	0x8000
#define	WM_CHOOSEFONT_GETLOGFONT	(WM_USER+1)

#ifndef RC_INVOKED
typedef UINT (CALLBACK *__CDHOOKPROC)(HWND,UINT,WPARAM,LPARAM);
typedef __CDHOOKPROC LPCCHOOKPROC;
typedef __CDHOOKPROC LPCFHOOKPROC;
typedef __CDHOOKPROC LPFRHOOKPROC;
typedef __CDHOOKPROC LPOFNHOOKPROC;
typedef __CDHOOKPROC LPSETUPHOOKPROC;
typedef __CDHOOKPROC LPPRINTHOOKPROC;
typedef struct tagCHOOSECOLOR {
	DWORD	lStructSize;
	HWND	hwndOwner;
	HWND	hInstance;
	COLORREF	rgbResult;
	COLORREF FAR*	lpCustColors;
	DWORD	Flags;
	LPARAM	lCustData;
	LPCCHOOKPROC	lpfnHook;
	LPCSTR	lpTemplateName;
} CHOOSECOLOR,FAR *LPCHOOSECOLOR;
typedef struct tagCHOOSEFONT {
	DWORD	lStructSize;
	HWND	hwndOwner;
	HDC	hDC;
	LPLOGFONT	lpLogFont;
	int	iPointSize;
	DWORD	Flags;
	COLORREF	rgbColors;
	LPARAM	lCustData;
	LPCFHOOKPROC	lpfnHook;
	LPCSTR	lpTemplateName;
	HINSTANCE	hInstance;
	LPSTR	lpszStyle;
	UINT	nFontType;
	int	nSizeMin;
	int	nSizeMax;
} CHOOSEFONT,FAR *LPCHOOSEFONT;
typedef struct tagDEVNAMES {
	UINT wDriverOffset;
	UINT wDeviceOffset;
	UINT wOutputOffset;
	UINT wDefault;
} DEVNAMES,FAR *LPDEVNAMES;
typedef struct tagFINDREPLACE {
	DWORD lStructSize;
	HWND hwndOwner;
	HINSTANCE hInstance;
	DWORD Flags;
	LPSTR lpstrFindWhat;
	LPSTR lpstrReplaceWith;
	UINT wFindWhatLen;
	UINT wReplaceWithLen;
	LPARAM lCustData;
	LPFRHOOKPROC lpfnHook;
	LPCSTR lpTemplateName;
} FINDREPLACE,FAR *LPFINDREPLACE;
typedef struct tagOFN {
	DWORD lStructSize;
	HWND hwndOwner;
	HINSTANCE hInstance;
	LPCSTR lpstrFilter;
	LPSTR lpstrCustomFilter;
	DWORD nMaxCustFilter;
	DWORD nFilterIndex;
	LPSTR lpstrFile;
	DWORD nMaxFile;
	LPSTR lpstrFileTitle;
	DWORD nMaxFileTitle;
	LPCSTR lpstrInitialDir;
	LPCSTR lpstrTitle;
	DWORD Flags;
	UINT nFileOffset;
	UINT nFileExtension;
	LPCSTR lpstrDefExt;
	LPARAM lCustData;
	LPOFNHOOKPROC lpfnHook;
	LPCSTR lpTemplateName;
} OPENFILENAME,FAR *LPOPENFILENAME;
typedef struct tagPD {
	DWORD lStructSize;
	HWND hwndOwner;
	HGLOBAL hDevMode;
	HGLOBAL hDevNames;
	HDC hDC;
	DWORD Flags;
	UINT nFromPage;
	UINT nToPage;
	UINT nMinPage;
	UINT nMaxPage;
	UINT nCopies;
	HINSTANCE hInstance;
	LPARAM lCustData;
	LPPRINTHOOKPROC lpfnPrintHook;
	LPSETUPHOOKPROC lpfnSetupHook;
	LPCSTR lpPrintTemplateName;
	LPCSTR lpSetupTemplateName;
	HGLOBAL hPrintTemplate;
	HGLOBAL hSetupTemplate;
} PRINTDLG,FAR *LPPRINTDLG;

BOOL WINAPI ChooseColor(LPCHOOSECOLOR);
BOOL WINAPI ChooseFont(LPCHOOSEFONT);
HWND WINAPI FindText(LPFINDREPLACE);
short WINAPI GetFileTitle(LPCSTR,LPSTR,UINT);
BOOL WINAPI GetOpenFileName(LPOPENFILENAME);
BOOL WINAPI GetSaveFileName(LPOPENFILENAME);
BOOL WINAPI PrintDlg(LPPRINTDLG);
HWND WINAPI ReplaceText(LPFINDREPLACE);
DWORD WINAPI CommDlgExtendedError(void);
#endif /* RC_INVOKED */

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
