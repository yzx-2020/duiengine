//////////////////////////////////////////////////////////////////////////
//  Class Name: DUIFontPool
// Description: Font Pool
//     Creator: ZhangXiaoxuan
//     Version: 2009.4.22 - 1.0 - Change stl::map to CAtlMap
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "duisingletonmap.h"



#define DUIF_BOLD        0x0004U
#define DUIF_UNDERLINE   0x0002U
#define DUIF_ITALIC      0x0001U

#define DUIF_MAKEKEY(bold, underline, italic, adding) \
    (WORD)((adding << 8) \
    | (bold ? DUIF_BOLD : 0) \
    | (underline ? DUIF_UNDERLINE : 0) \
    | (italic ? DUIF_ITALIC : 0))

#define DUIF_ISBOLD(key)         ((key & DUIF_BOLD) == DUIF_BOLD)
#define DUIF_ISUNDERLINE(key)    ((key & DUIF_UNDERLINE) == DUIF_UNDERLINE)
#define DUIF_ISITALIC(key)       ((key & DUIF_ITALIC) == DUIF_ITALIC)
#define DUIF_GETADDING(key)      (key >> 8)

#define DUIF_DEFAULTFONT         (DUIF_MAKEKEY(FALSE, FALSE, FALSE, 0))
#define DUIF_BOLDFONT            (DUIF_MAKEKEY(TRUE, FALSE, FALSE, 0))

#define DUIF_GetFont(key)        DuiFontPool::GetFont(key)

class DUI_EXP FontKey
{
public:
	FontKey(DWORD dwStyle,CString strFaceName="")
	{
		this->strFaceName=strFaceName;
		this->dwStyle=dwStyle;
	}

	CString strFaceName;
	DWORD	 dwStyle;
};

template<>
class DUI_EXP CElementTraits< FontKey > :
	public CElementTraitsBase< FontKey >
{
public:
	static ULONG Hash( INARGTYPE fontKey )
	{
		ULONG_PTR uRet=0;
		CString strType=fontKey.strFaceName;
		strType.MakeLower();
		for(int i=0;i<strType.GetLength();i++)
		{
			uRet=uRet*68+strType[i];
		}

		return (ULONG)(uRet*10000+(UINT)fontKey.dwStyle+1);
	}

	static bool CompareElements( INARGTYPE element1, INARGTYPE element2 )
	{
		return _tcsicmp(element1.strFaceName,element2.strFaceName)==0 && element1.dwStyle==element2.dwStyle;
	}

	static int CompareElementsOrdered( INARGTYPE element1, INARGTYPE element2 )
	{
		int nRet=_tcsicmp(element1.strFaceName,element2.strFaceName);
		if(nRet<0) return -1;
		if(nRet>0) return 1;
		return element1.dwStyle-element2.dwStyle;
	}
};

namespace DuiEngine
{

class DUI_EXP DuiFontPool :public DuiSingletonMap<DuiFontPool,HFONT,FontKey>
{
public:
    DuiFontPool();

    HFONT GetFont(WORD uKey,CString strFaceName="");

    HFONT GetFont(BOOL bBold, BOOL bUnderline, BOOL bItalic, char chAdding = 0,CString strFaceName="");
    void SetDefaultFont(LPCTSTR lpszFaceName, LONG lSize);
protected:
	static void OnKeyRemoved(const HFONT & obj)
	{
		DeleteObject(obj);
	}

    HFONT _CreateDefaultGUIFont();
                                                                           
    HFONT _CreateNewFont(BOOL bBold, BOOL bUnderline, BOOL bItalic, char chAdding,CString strFaceName="");

    LONG _GetFontAbsHeight(LONG lSize);

	LOGFONT m_lfDefault;
	CString m_strFaceName;
	LONG m_lFontSize;
};

}//namespace DuiEngine
