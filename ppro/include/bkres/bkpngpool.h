//////////////////////////////////////////////////////////////////////////
//  Class Name: BKPngPool
// Description: PNG Pool
//     Creator: ZhangXiaoxuan
//     Version: 2010.2.24 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")

#include <atlcoll.h>

#include "bkresutil.h"

class BkPngPool
{
public:
    BkPngPool()
    {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    }
    virtual ~BkPngPool()
    {
        Gdiplus::Image *pImg = NULL;

        POSITION pos = m_mapPng.GetStartPosition();

        while (pos != NULL) 
        {
            pImg = m_mapPng.GetNextValue(pos);
            if (pImg)
                delete pImg;
        }

        m_mapPng.RemoveAll();

        Gdiplus::GdiplusShutdown(m_gdiplusToken);
    }

    typedef CAtlMap<UINT, Gdiplus::Image *> _TypePngPool;

    static Gdiplus::Image* Get(UINT uResID)
    {
        _TypePngPool::CPair* pPairRet = _Instance()->m_mapPng.Lookup(uResID);
        Gdiplus::Image* pImg = NULL;

        if (NULL == pPairRet)
        {
            _LoadPNGImageFromResourceID(uResID, pImg);
            if (pImg)
                _Instance()->m_mapPng[uResID] = pImg;
        }
        else
            pImg = pPairRet->m_value;

        return pImg;
    }

    static size_t GetCount()
    {
        return _Instance()->m_mapPng.GetCount();
    }

protected:

    ULONG_PTR m_gdiplusToken;

    _TypePngPool m_mapPng;

    static BkPngPool* ms_pInstance;

    static BkPngPool* _Instance()
    {
        if (!ms_pInstance)
            ms_pInstance = new BkPngPool;
        return ms_pInstance;
    }

    static BOOL _LoadPNGImageFromResourceID(UINT nID, Gdiplus::Image* &pImg)
    {
		HMODULE		hModule = NULL;
		HRSRC		hRsrc	= NULL;


		if (!BkResUtil::LoadResourse(nID, BKRES_PNG_TYPE, hModule, hRsrc))
			return FALSE;


        DWORD len = ::SizeofResource(hModule, hRsrc);
        BYTE* lpRsrc = (BYTE*)::LoadResource(hModule, hRsrc);
        if (!lpRsrc)
            return FALSE;

        HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, len);
        BYTE* pMem = (BYTE*)::GlobalLock(hMem);

        memcpy(pMem, lpRsrc, len);

        IStream* pStm = NULL;
        ::CreateStreamOnHGlobal(hMem, FALSE, &pStm);

        pImg = Gdiplus::Image::FromStream(pStm);

        ::GlobalUnlock(hMem);
        pStm->Release();
        FreeResource(lpRsrc);

        return TRUE;
    }
};

__declspec(selectany) BkPngPool* BkPngPool::ms_pInstance = NULL;