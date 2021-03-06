#pragma once

#include "ignoredlist.h"
#include <inioperator/cinifile.h>
#include <common/utility.h>
using namespace IniFileOperate;

#define IGNORED_FILEPATH			L"\\ignoredlist.ini"
#define SEC_IGNOREDLIST_MAIN		L"Main"
#define KEY_IGNOREDLIST_COMMENT		L"Comment"

class CBeikeSafeSysoptIgnoreDlg
	: public CBkDialogImpl<CBeikeSafeSysoptIgnoreDlg>
	, public CWHRoundRectFrameHelper<CBeikeSafeSysoptIgnoreDlg>
	, public IIgnoredOptLstCBack
{
public:
	CBeikeSafeSysoptIgnoreDlg(CSimpleArray<CKsafeRunInfo*>& pInfoArray,CBeikeSafeMainDlg* refDlg)
		: CBkDialogImpl<CBeikeSafeSysoptIgnoreDlg>(IDR_BK_RUNOPT_IGNORE_DLG)
	{
		m_arrayItem = &pInfoArray;
		m_pMainDlg = refDlg;
		m_bHasCanceled = FALSE;
	}

	CBeikeSafeSysoptIgnoreDlg()
		: CBkDialogImpl<CBeikeSafeSysoptIgnoreDlg>(IDR_BK_RUNOPT_IGNORE_DLG)
	{
	}

	~CBeikeSafeSysoptIgnoreDlg(void)
	{
	}

	BOOL OnInitDialog(HWND,LPARAM)
	{
		InitCtrl();
		CAppPath::Instance().GetLeidianAppPath(m_strIniPath);
		m_strIniPath.Append(IGNORED_FILEPATH);
		return TRUE;
	}

	void OnBkBtnOK()
	{
		if (m_bHasCanceled)
			::PostMessage(m_pMainDlg->m_hWnd,MSG_SYSOPT_REFRESH_ONEKEYOPT,0,0);
		EndDialog(IDOK);
	}

	void OnBkBtnCancelIgnore()
	{
		m_bHasCanceled = TRUE;
		SaveCurrentIgnored();
		m_IgnoredList.DeleteCheckedItem();
		SetItemVisible(IDC_DIV_NO_IGNOREDITEM,m_IgnoredList.GetItemCount() == 0);
		EnableItem(IDC_CHECK_IGNORE_CHECKALL,m_IgnoredList.GetItemCount() > 0);
		SetItemVisible(IDC_LST_IGNORE_LIST,m_IgnoredList.GetItemCount() > 0);
		EnableItem(IDC_BTN_CANCEL_IGNORE,m_IgnoredList.GetCheckedCount() > 0);

		::PostMessage(m_pMainDlg->m_hWnd,MSG_SYSOPT_CHANGE_IGNORED_NUM,0,0);
	}

	void SaveCurrentIgnored()
	{
		int nIgnoredCount = m_IgnoredList.GetItemCount();
		CIniFile ini_IgnoredList(m_strIniPath);
		CString strValue;
		for (int i = 0;i < nIgnoredCount;i++)
		{
			CString strId;
			_IGNORE_ITEM_DATA* pData = m_IgnoredList.GetItemParamData(i);
			if (pData->bCheck == FALSE)
			{
				strId.Format(_T("%d|"),pData->pInfo->nID);
				strValue.Append(strId);
			}
		}
		ini_IgnoredList.SetStrValue(SEC_IGNOREDLIST_MAIN,KEY_IGNOREDLIST_COMMENT,strValue);
	}

	void InitCtrl()
	{
		m_IgnoredList.Create( GetViewHWND(), 
			NULL, NULL, 
			WS_CLIPCHILDREN | WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_OWNERDRAWFIXED | LVS_NOCOLUMNHEADER, 
			0, IDC_LST_IGNORE_LIST, NULL);

		m_IgnoredList.CreateToolTip();
		m_IgnoredList.SetCallback(this);
		m_IgnoredList.InsertColumn(0,BkString::Get(IDS_SYSOPT_4066),LVCFMT_CENTER);
		m_IgnoredList.SetColumnWidth(0,200);
		m_IgnoredList.InsertColumn(1,BkString::Get(IDS_SYSOPT_4067));
		m_IgnoredList.SetColumnWidth(1,340);
		m_IgnoredList.InsertColumn(2,BkString::Get(IDS_SYSOPT_4068));
		m_IgnoredList.SetColumnWidth(2,80);

		m_IgnoredList.InitArray(*m_arrayItem);

		SetItemVisible(IDC_DIV_NO_IGNOREDITEM,m_IgnoredList.GetItemCount() == 0);
		SetItemCheck(IDC_CHECK_IGNORE_CHECKALL,TRUE);
		EnableItem(IDC_CHECK_IGNORE_CHECKALL,m_IgnoredList.GetItemCount() > 0);
		SetItemVisible(IDC_LST_IGNORE_LIST,m_IgnoredList.GetItemCount() > 0);
		EnableItem(IDC_BTN_CANCEL_IGNORE,m_IgnoredList.GetCheckedCount() > 0);
	}

	void ClickCheck()
	{
		EnableItem(IDC_BTN_CANCEL_IGNORE,m_IgnoredList.GetCheckedCount() > 0);
		SetItemCheck(IDC_CHECK_IGNORE_CHECKALL,m_IgnoredList.IsCheckAll());
	}

	void OnBkCkCheckAll()
	{
		BOOL bCheck = GetItemCheck(IDC_CHECK_IGNORE_CHECKALL);
		m_IgnoredList.CheckAll(bCheck);
		ClickCheck();
	}

public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_CLOSE,OnBkBtnOK);
		BK_NOTIFY_ID_COMMAND(IDC_BTN_CANCEL_IGNORE,OnBkBtnCancelIgnore);
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SYS_CLOSE, OnBkBtnOK)
		BK_NOTIFY_ID_COMMAND(IDC_CHECK_IGNORE_CHECKALL,OnBkCkCheckAll);
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX(CBeikeSafeSysoptIgnoreDlg)
		MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
		CHAIN_MSG_MAP(CBkDialogImpl<CBeikeSafeSysoptIgnoreDlg>)
		CHAIN_MSG_MAP(CWHRoundRectFrameHelper<CBeikeSafeSysoptIgnoreDlg>)
		MSG_WM_INITDIALOG(OnInitDialog)
	END_MSG_MAP()

private:
	CBeikeSafeMainDlg*		m_pMainDlg;
	CSimpleArray<CKsafeRunInfo*>*	m_arrayItem;
	CIgnoredOptList			m_IgnoredList;
	CString					m_strIniPath;
	BOOL					m_bHasCanceled;
};