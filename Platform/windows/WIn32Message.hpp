#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"

#define GET_X_LPARAM(lp) ((int)(short)(lp & 0xffff))
#define GET_Y_LPARAM(lp) ((int)(short)((lp >> 16) & 0xffff))

class Message : public Object<Message>
{
public:

	constexpr Message(u16 code, i64 wp, i64 lp) noexcept
		:
		_code(code),
		wParam(wp),
		lParam(lp)
	{ }

	inline u32 GetHashCode() const noexcept { return _code.GetHashCode(); }

	String ToString() const noexcept
	{
		switch ((uint16_t)_code)
		{
		case 0x0000: return String::Concat("WM_NULL"); 
		case 0x0001: return String::Concat("WM_CREATE"); 
		case 0x0002: return String::Concat("WM_DESTROY"); 
		case 0x0003: return String::Concat("WM_MOVE"); 
		case 0x0005: return String::Concat("WM_SIZE"); 
		case 0x0006: return String::Concat("WM_ACTIVATE"); 
		case 0x0007: return String::Concat("WM_SETFOCUS"); 
		case 0x0008: return String::Concat("WM_KILLFOCUS"); 
		case 0x000A: return String::Concat("WM_ENABLE"); 
		case 0x000B: return String::Concat("WM_SETREDRAW"); 
		case 0x000C: return String::Concat("WM_SETTEXT"); 
		case 0x000D: return String::Concat("WM_GETTEXT"); 
		case 0x000E: return String::Concat("WM_GETTEXTLENGTH"); 
		case 0x000F: return String::Concat("WM_PAINT"); 
		case 0x0010: return String::Concat("WM_CLOSE"); 
		case 0x0011: return String::Concat("WM_QUERYENDSESSION"); 
		case 0x0013: return String::Concat("WM_QUERYOPEN"); 
		case 0x0016: return String::Concat("WM_ENDSESSION"); 
		case 0x0012: return String::Concat("WM_QUIT"); 
		case 0x0014: return String::Concat("WM_ERASEBKGND"); 
		case 0x0015: return String::Concat("WM_SYSCOLORCHANGE"); 
		case 0x0018: return String::Concat("WM_SHOWWINDOW"); 
		case 0x001A: return String::Concat("WM_WININICHANGE / WM_SETTINGCHANGE"); 
		case 0x001B: return String::Concat("WM_DEVMODECHANGE"); 
		case 0x001C: return String::Concat("WM_ACTIVATEAPP"); 
		case 0x001D: return String::Concat("WM_FONTCHANGE"); 
		case 0x001E: return String::Concat("WM_TIMECHANGE"); 
		case 0x001F: return String::Concat("WM_CANCELMODE"); 
		case 0x0020: return String::Concat("WM_SETCURSOR"); 
		case 0x0021: return String::Concat("WM_MOUSEACTIVATE"); 
		case 0x0022: return String::Concat("WM_CHILDACTIVATE"); 
		case 0x0023: return String::Concat("WM_QUEUESYNC"); 
		case 0x0024: return String::Concat("WM_GETMINMAXINFO"); 
		case 0x0026: return String::Concat("WM_PAINTICON"); 
		case 0x0027: return String::Concat("WM_ICONERASEBKGND"); 
		case 0x0028: return String::Concat("WM_NEXTDLGCTL"); 
		case 0x002A: return String::Concat("WM_SPOOLERSTATUS"); 
		case 0x002B: return String::Concat("WM_DRAWITEM"); 
		case 0x002C: return String::Concat("WM_MEASUREITEM"); 
		case 0x002D: return String::Concat("WM_DELETEITEM"); 
		case 0x002E: return String::Concat("WM_VKEYTOITEM"); 
		case 0x002F: return String::Concat("WM_CHARTOITEM"); 
		case 0x0030: return String::Concat("WM_SETFONT"); 
		case 0x0031: return String::Concat("WM_GETFONT"); 
		case 0x0032: return String::Concat("WM_SETHOTKEY"); 
		case 0x0033: return String::Concat("WM_GETHOTKEY"); 
		case 0x0037: return String::Concat("WM_QUERYDRAGICON"); 
		case 0x0039: return String::Concat("WM_COMPAREITEM"); 
		case 0x003D: return String::Concat("WM_GETOBJECT"); 
		case 0x0041: return String::Concat("WM_COMPACTING"); 
		case 0x0044: return String::Concat("WM_COMMNOTIFY"); 
		case 0x0046: return String::Concat("WM_WINDOWPOSCHANGING"); 
		case 0x0047: return String::Concat("WM_WINDOWPOSCHANGED"); 
		case 0x0048: return String::Concat("WM_POWER"); 
		case 0x004A: return String::Concat("WM_COPYDATA"); 
		case 0x004B: return String::Concat("WM_CANCELJOURNAL"); 
		case 0x004E: return String::Concat("WM_NOTIFY"); 
		case 0x0050: return String::Concat("WM_INPUTLANGCHANGEREQUEST"); 
		case 0x0051: return String::Concat("WM_INPUTLANGCHANGE"); 
		case 0x0052: return String::Concat("WM_TCARD"); 
		case 0x0053: return String::Concat("WM_HELP"); 
		case 0x0054: return String::Concat("WM_USERCHANGED"); 
		case 0x0055: return String::Concat("WM_NOTIFYFORMAT"); 
		case 0x007B: return String::Concat("WM_CONTEXTMENU"); 
		case 0x007C: return String::Concat("WM_STYLECHANGING"); 
		case 0x007D: return String::Concat("WM_STYLECHANGED"); 
		case 0x007E: return String::Concat("WM_DISPLAYCHANGE"); 
		case 0x007F: return String::Concat("WM_GETICON"); 
		case 0x0080: return String::Concat("WM_SETICON"); 
		case 0x0081: return String::Concat("WM_NCCREATE"); 
		case 0x0082: return String::Concat("WM_NCDESTROY"); 
		case 0x0083: return String::Concat("WM_NCCALCSIZE"); 
		case 0x0084: return String::Concat("WM_NCHITTEST"); 
		case 0x0085: return String::Concat("WM_NCPAINT"); 
		case 0x0086: return String::Concat("WM_NCACTIVATE"); 
		case 0x0087: return String::Concat("WM_GETDLGCODE"); 
		case 0x0088: return String::Concat("WM_SYNCPAINT"); 
		case 0x00A0: return String::Concat("WM_NCMOUSEMOVE"); 
		case 0x00A1: return String::Concat("WM_NCLBUTTONDOWN"); 
		case 0x00A2: return String::Concat("WM_NCLBUTTONUP"); 
		case 0x00A3: return String::Concat("WM_NCLBUTTONDBLCLK"); 
		case 0x00A4: return String::Concat("WM_NCRBUTTONDOWN"); 
		case 0x00A5: return String::Concat("WM_NCRBUTTONUP"); 
		case 0x00A6: return String::Concat("WM_NCRBUTTONDBLCLK"); 
		case 0x00A7: return String::Concat("WM_NCMBUTTONDOWN"); 
		case 0x00A8: return String::Concat("WM_NCMBUTTONUP"); 
		case 0x00A9: return String::Concat("WM_NCMBUTTONDBLCLK"); 
		case 0x00AB: return String::Concat("WM_NCXBUTTONDOWN"); 
		case 0x00AC: return String::Concat("WM_NCXBUTTONUP"); 
		case 0x00AD: return String::Concat("WM_NCXBUTTONDBLCLK"); 
		case 0x00FF: return String::Concat("WM_INPUT"); 
		case 0x0100: return String::Concat("WM_KEYFIRST / WM_KEYDOWN"); 
		case 0x0101: return String::Concat("WM_KEYUP"); 
		case 0x0102: return String::Concat("WM_CHAR"); 
		case 0x0103: return String::Concat("WM_DEADCHAR"); 
		case 0x0104: return String::Concat("WM_SYSKEYDOWN"); 
		case 0x0105: return String::Concat("WM_SYSKEYUP"); 
		case 0x0106: return String::Concat("WM_SYSCHAR"); 
		case 0x0107: return String::Concat("WM_SYSDEADCHAR"); 
		case 0x0109: return String::Concat("WM_KEYLAST / WM_UNICHAR"); 
		case 0x010D: return String::Concat("WM_IME_STARTCOMPOSITION"); 
		case 0x010E: return String::Concat("WM_IME_ENDCOMPOSITION"); 
		case 0x010F: return String::Concat("WM_IME_COMPOSITION / WM_IME_KEYLAST"); 
		case 0x0110: return String::Concat("WM_INITDIALOG"); 
		case 0x0111: return String::Concat("WM_COMMAND"); 
		case 0x0112: return String::Concat("WM_SYSCOMMAND"); 
		case 0x0113: return String::Concat("WM_TIMER"); 
		case 0x0114: return String::Concat("WM_HSCROLL"); 
		case 0x0115: return String::Concat("WM_VSCROLL"); 
		case 0x0116: return String::Concat("WM_INITMENU"); 
		case 0x0117: return String::Concat("WM_INITMENUPOPUP"); 
		case 0x011F: return String::Concat("WM_MENUSELECT"); 
		case 0x0120: return String::Concat("WM_MENUCHAR"); 
		case 0x0121: return String::Concat("WM_ENTERIDLE"); 
		case 0x0122: return String::Concat("WM_MENURBUTTONUP"); 
		case 0x0123: return String::Concat("WM_MENUDRAG"); 
		case 0x0124: return String::Concat("WM_MENUGETOBJECT"); 
		case 0x0125: return String::Concat("WM_UNINITMENUPOPUP"); 
		case 0x0126: return String::Concat("WM_MENUCOMMAND"); 
		case 0x0127: return String::Concat("WM_CHANGEUISTATE"); 
		case 0x0128: return String::Concat("WM_UPDATEUISTATE"); 
		case 0x0129: return String::Concat("WM_QUERYUISTATE"); 
		case 0x0132: return String::Concat("WM_CTLCOLORMSGBOX"); 
		case 0x0133: return String::Concat("WM_CTLCOLOREDIT"); 
		case 0x0134: return String::Concat("WM_CTLCOLORLISTBOX"); 
		case 0x0135: return String::Concat("WM_CTLCOLORBTN"); 
		case 0x0136: return String::Concat("WM_CTLCOLORDLG"); 
		case 0x0137: return String::Concat("WM_CTLCOLORSCROLLBAR"); 
		case 0x0138: return String::Concat("WM_CTLCOLORSTATIC"); 
		case 0x0200: return String::Concat("WM_MOUSEFIRST / WM_MOUSEMOVE\t-- WParam: ", i64(wParam).ToString(), "\t -- lParam: ", i64(lParam).ToString(), "\t-> X: ", i32(GET_X_LPARAM(lParam)).ToString(), "\tY: ", i32(GET_Y_LPARAM(lParam)).ToString());
		case 0x0201: return String::Concat("WM_LBUTTONDOWN"); 
		case 0x0202: return String::Concat("WM_LBUTTONUP"); 
		case 0x0203: return String::Concat("WM_LBUTTONDBLCLK"); 
		case 0x0204: return String::Concat("WM_RBUTTONDOWN"); 
		case 0x0205: return String::Concat("WM_RBUTTONUP"); 
		case 0x0206: return String::Concat("WM_RBUTTONDBLCLK"); 
		case 0x0207: return String::Concat("WM_MBUTTONDOWN"); 
		case 0x0208: return String::Concat("WM_MBUTTONUP"); 
		case 0x0209: return String::Concat("WM_MBUTTONDBLCLK / WM_MOUSELAST(95)"); 
		case 0x020A: return String::Concat("WM_MOUSEWHEEL / WM_MOUSELAST(NT4,98)"); 
		case 0x020B: return String::Concat("WM_XBUTTONDOWN"); 
		case 0x020C: return String::Concat("WM_XBUTTONUP"); 
		case 0x020D: return String::Concat("WM_XBUTTONDBLCLK / WM_MOUSELAST(2K,XP,2k3)"); 
		case 0x0210: return String::Concat("WM_PARENTNOT IFY"); 
		case 0x0211: return String::Concat("WM_ENTERMENULOOP"); 
		case 0x0212: return String::Concat("WM_EXITMENULOOP"); 
		case 0x0213: return String::Concat("WM_NEXTMENU"); 
		case 0x0214: return String::Concat("WM_SIZING"); 
		case 0x0215: return String::Concat("WM_CAPTURECHANGED"); 
		case 0x0216: return String::Concat("WM_MOVING"); 
		case 0x0218: return String::Concat("WM_POWERBROADCAST"); 
		case 0x0219: return String::Concat("WM_DEVICECHANGE"); 
		case 0x0220: return String::Concat("WM_MDICREATE"); 
		case 0x0221: return String::Concat("WM_MDIDESTROY"); 
		case 0x0222: return String::Concat("WM_MDIACTIVATE"); 
		case 0x0223: return String::Concat("WM_MDIRESTORE"); 
		case 0x0224: return String::Concat("WM_MDINEXT"); 
		case 0x0225: return String::Concat("WM_MDIMAXIMIZE"); 
		case 0x0226: return String::Concat("WM_MDITILE"); 
		case 0x0227: return String::Concat("WM_MDICASCADE"); 
		case 0x0228: return String::Concat("WM_MDIICONARRANGE"); 
		case 0x0229: return String::Concat("WM_MDIGETACTIVE"); 
		case 0x0230: return String::Concat("WM_MDISETMENU"); 
		case 0x0231: return String::Concat("WM_ENTERSIZEMOVE"); 
		case 0x0232: return String::Concat("WM_EXITSIZEMOVE"); 
		case 0x0233: return String::Concat("WM_DROPFILES"); 
		case 0x0234: return String::Concat("WM_MDIREFRESHMENU"); 
		case 0x0281: return String::Concat("WM_IME_SETCONTEXT"); 
		case 0x0282: return String::Concat("WM_IME_NOTIFY"); 
		case 0x0283: return String::Concat("WM_IME_CONTROL"); 
		case 0x0284: return String::Concat("WM_IME_COMPOSITIONFULL"); 
		case 0x0285: return String::Concat("WM_IME_SELECT"); 
		case 0x0286: return String::Concat("WM_IME_CHAR"); 
		case 0x0288: return String::Concat("WM_IME_REQUEST"); 
		case 0x0290: return String::Concat("WM_IME_KEYDOWN"); 
		case 0x0291: return String::Concat("WM_IME_KEYUP"); 
		case 0x02A1: return String::Concat("WM_MOUSEHOVER"); 
		case 0x02A3: return String::Concat("WM_MOUSELEAVE"); 
		case 0x02A0: return String::Concat("WM_NCMOUSEHOVER"); 
		case 0x02A2: return String::Concat("WM_NCMOUSELEAVE"); 
		case 0x02B1: return String::Concat("WM_WTSSESSION_CHANGE"); 
		case 0x02C0: return String::Concat("WM_TABLET_FIRST"); 
		case 0x02DF: return String::Concat("WM_TABLET_LAST"); 
		case 0x0300: return String::Concat("WM_CUT"); 
		case 0x0301: return String::Concat("WM_COPY"); 
		case 0x0302: return String::Concat("WM_PASTE"); 
		case 0x0303: return String::Concat("WM_CLEAR"); 
		case 0x0304: return String::Concat("WM_UNDO"); 
		case 0x0305: return String::Concat("WM_RENDERFORMAT"); 
		case 0x0306: return String::Concat("WM_RENDERALLFORMATS"); 
		case 0x0307: return String::Concat("WM_DESTROYCLIPBOARD"); 
		case 0x0308: return String::Concat("WM_DRAWCLIPBOARD"); 
		case 0x0309: return String::Concat("WM_PAINTCLIPBOARD"); 
		case 0x030A: return String::Concat("WM_VSCROLLCLIPBOARD"); 
		case 0x030B: return String::Concat("WM_SIZECLIPBOARD"); 
		case 0x030C: return String::Concat("WM_ASKCBFORMATNAME"); 
		case 0x030D: return String::Concat("WM_CHANGECBCHAIN"); 
		case 0x030E: return String::Concat("WM_HSCROLLCLIPBOARD"); 
		case 0x030F: return String::Concat("WM_QUERYNEWPALETTE"); 
		case 0x0310: return String::Concat("WM_PALETTEISCHANGING"); 
		case 0x0311: return String::Concat("WM_PALETTECHANGED"); 
		case 0x0312: return String::Concat("WM_HOTKEY"); 
		case 0x0317: return String::Concat("WM_PRINT"); 
		case 0x0318: return String::Concat("WM_PRINTCLIENT"); 
		case 0x0319: return String::Concat("WM_APPCOMMAND"); 
		case 0x031A: return String::Concat("WM_THEMECHANGED"); 
		case 0x0358: return String::Concat("WM_HANDHELDFIRST"); 
		case 0x035F: return String::Concat("WM_HANDHELDLAST"); 
		case 0x0360: return String::Concat("WM_AFXFIRST"); 
		case 0x037F: return String::Concat("WM_AFXLAST"); 
		case 0x0380: return String::Concat("WM_PENWINFIRST"); 
		case 0x038F: return String::Concat("WM_PENWINLAST"); 
		case 0x0400: return String::Concat("WM_USER"); 
		case 0x8000: return String::Concat("WM_APP"); 
		default: return "Unknown Message"; 
		}
	}

private:

	u16 _code;
	i64 wParam;
	i64 lParam;
};