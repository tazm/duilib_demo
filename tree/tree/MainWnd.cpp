#include "MainWnd.h"
#include "resource.h"

#include <Shlwapi.h>
#include <ShlObj.h>

#include <list>
#include <map>
#include <lib/stringprintf.h>

#pragma comment(lib, "shlwapi.lib")
using namespace std;

#define  TREE_WIDTH          L"1800"
#define  TREE_ITEMATTR       L"padding=\"4,0,0,0\" font=\"1\""
#define  TREE_CHECXBOXATTR   L"width=\"16\" height=\"16\" normalimage=\"file='%s'\""
#define  TREE_FOLDERATTR     L"padding=\"0,1,0,0\" width=\"16\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,16,16' \" hotimage=\"file='treeview_b.png' source='16,0,32,16' \" selectedimage=\"file='treeview_a.png' source='0,0,16,16' \" selectedhotimage=\"file='treeview_a.png' source='16,0,32,16' \""

map<wstring, wstring> gs_mapExt2Path;
BOOL WINAPI _fnGdFileHandlerW(LPCWSTR szFile, void*)
{
    wstring str = PathFindFileNameW(szFile);
    size_t nPos = str.find(L'.');
    if (nPos != wstring::npos)
    {
        wstring strTmp = L".";
        str = str.substr(0, nPos);
        strTmp += str;
        gs_mapExt2Path.insert(make_pair<wstring, wstring>(strTmp, szFile));
    }
    return TRUE;
}

static void _InitFileExts()
{
    WCHAR szDir[MAX_PATH] = {0};
    //FileSiblingFileNameW(NULL, L"skin\\fileext", EXPAND_ARG(szDir));
    //FileEnumFileW(szDir, FALSE, _fnGdFileHandlerW, NULL);
}

MainWnd::MainWnd()
{
}

MainWnd::~MainWnd()
{
}

LRESULT MainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL bHandled = FALSE;
    LRESULT rslt = 0;

    switch(uMsg)
    {
    case WM_CREATE:
        rslt = OnCreate(uMsg, wParam, lParam);
        break;
    case WM_NCHITTEST:
        rslt = OnNcHitTest(uMsg, wParam, lParam);
        break;
    case WM_CLOSE:
        rslt = OnClose(uMsg, wParam, lParam);
        break;
    case WM_GETMINMAXINFO:
        rslt = OnGetMinMaxInfo(uMsg, wParam, lParam);
        break;
    default:
        break;
    }

    if( uMsg == WM_NCACTIVATE ) 
    {
        if(!::IsIconic(m_hWnd)) 
        {
            return (wParam == 0) ? TRUE : FALSE;
        }
    }
    if( uMsg == WM_NCCALCSIZE )
    {
        return 0;
    }
     if( uMsg == WM_NCPAINT ) 
    {
        return 0;
    }

    if (!bHandled)
    {
        if (!m_pm.MessageHandler(uMsg, wParam, lParam, rslt))
        {
            rslt = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
        }
    }

    return rslt;
}

static BOOL WINAPI _AddFileName(LPCWSTR wszName, void* lpParam)
{
    DWORD dwAttrib = GetFileAttributesW(wszName);
    if (dwAttrib & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))
    {
        return TRUE;
    }

    list<wstring>* fileNameList = (list<wstring>*)lpParam;
    fileNameList->push_back(wszName);
    return TRUE;
}

void AddChildNode(CTreeNodeUI* pTreeNodeParent, list<wstring>& listFile)
{
    for (list<wstring>::reverse_iterator it = listFile.rbegin(); it != listFile.rend(); it++)
    {
        wstring strPng;
        if (PathIsDirectoryW(it->c_str()))
        {
            strPng = L"dir.png";
        }
        else
        {
            LPTSTR szTmp = PathFindExtensionW(it->c_str());
            if (szTmp != NULL)
            {
                map<wstring,wstring>::const_iterator it = gs_mapExt2Path.find(szTmp);
                if (it != gs_mapExt2Path.end())
                {
                    strPng = it->second;
                }
                else
                {
                     strPng = L"file.png";
                }
            }
            else
            {
                strPng = L"file.png";
            }
        }

        CTreeNodeUI* pTreeNode = new CTreeNodeUI();
        pTreeNode->SetAttribute(L"width", TREE_WIDTH);
        pTreeNode->SetAttribute(L"itemattr", TREE_ITEMATTR);
        pTreeNode->SetAttribute(L"checkboxattr", google::StringPrintf(TREE_CHECXBOXATTR, strPng.c_str()).c_str());
        pTreeNode->SetAttribute(L"folderattr", TREE_FOLDERATTR);
        pTreeNode->SetUserData(it->c_str());
        pTreeNode->GetItemButton()->SetText(PathFindFileNameW(it->c_str()));
        pTreeNodeParent->AddAt(pTreeNode, pTreeNodeParent->GetTreeIndex());
    }
}

void AddChildNodeDriver(CTreeNodeUI* pTreeNodeParent, list<wstring>& listFile)
{
    for (list<wstring>::reverse_iterator it = listFile.rbegin(); it != listFile.rend(); it++)
    {
        wstring strPng = L"driver.png";
        CTreeNodeUI* pTreeNode = new CTreeNodeUI();
        pTreeNode->SetAttribute(L"width", TREE_WIDTH);
        pTreeNode->SetAttribute(L"itemattr", TREE_ITEMATTR);
        pTreeNode->SetAttribute(L"checkboxattr", google::StringPrintf(TREE_CHECXBOXATTR, strPng.c_str()).c_str());
        pTreeNode->SetAttribute(L"folderattr", TREE_FOLDERATTR);
        pTreeNode->SetUserData(it->c_str());
        pTreeNode->GetItemButton()->SetText(PathFindFileNameW(it->c_str()));
        pTreeNodeParent->AddAt(pTreeNode, pTreeNodeParent->GetTreeIndex());
    }
}

void ExpendTree(CTreeNodeUI* pTreeNodeParent)
{
    const CDuiString& duiString = pTreeNodeParent->GetUserData();

    list<wstring> fileNameList;
    //FileEnumFileW(duiString, FALSE, _AddFileName, &fileNameList);

    AddChildNode(pTreeNodeParent, fileNameList);

    list<wstring> DirNameList;
    //FileEnumDirW(duiString, FALSE, _AddFileName, &DirNameList);

    AddChildNode(pTreeNodeParent, DirNameList);
}

CTreeNodeUI* AddTreeNode(CTreeViewUI* pTree, const wchar_t* szText, const wchar_t* szIcon, const wchar_t* szUserData)
{
    CTreeNodeUI* pTreeNode = new CTreeNodeUI();
    pTreeNode->SetAttribute(L"width", TREE_WIDTH);
    pTreeNode->SetAttribute(L"itemattr", TREE_ITEMATTR);
    pTreeNode->SetAttribute(L"checkboxattr", google::StringPrintf(TREE_CHECXBOXATTR, szIcon).c_str());
    pTreeNode->SetAttribute(L"folderattr", TREE_FOLDERATTR);
    pTreeNode->GetItemButton()->SetText(szText);

    pTreeNode->SetUserData(szUserData);
    pTree->Add(pTreeNode);
    return pTreeNode;
}

LRESULT MainWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
    styleValue &= ~WS_CAPTION;
    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

    _InitFileExts();;

    m_pm.Init(m_hWnd);
    CDialogBuilder builder;
    CControlUI* pRoot = builder.Create(TEXT("tree.xml"), (UINT)0, NULL, &m_pm);

    m_pm.AttachDialog(pRoot);
    m_pm.AddNotifier(this);

    CenterWindow();
    SetIcon(IDI_MAIN);

    m_treeLeft = (CTreeViewUI*)m_pm.FindControl(L"treeLeft"); 

    {
        WCHAR wszPath[MAX_PATH] = {0};
        if (SHGetSpecialFolderPathW(NULL, wszPath, CSIDL_DESKTOP, FALSE))
        {
            AddTreeNode(m_treeLeft, L"桌面", L"desktop.png", wszPath);
        }
    }
    {
        WCHAR wszPath[MAX_PATH] = {0};
        if (SHGetSpecialFolderPathW(NULL, wszPath, CSIDL_PERSONAL, FALSE))
        {
            AddTreeNode(m_treeLeft, L"我的文档", L"document.png", wszPath);
        }
    }

    {
        DWORD dwVols = GetLogicalDrives();
        list<wstring> fileNameList;
        for (int i = 0; i < 26; ++i)
        {
            if (!(dwVols & (1 << i)))
            {
                continue;
            }

            WCHAR wszDrive[16] = {0};
            wnsprintfW(wszDrive, RTL_NUMBER_OF(wszDrive), L"%c:", 'A' + i);
            if ((DRIVE_FIXED == GetDriveTypeW(wszDrive)))
            {
                fileNameList.push_back(wszDrive);
            }
        }

        CTreeNodeUI* pThreeNode = AddTreeNode(m_treeLeft, L"计算机", L"computer.png", L"computer");
        AddChildNodeDriver(pThreeNode, fileNameList);
    }
    m_treeLeft->SelectItem(0, false, false);
    return 0;
}

LRESULT MainWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);

    RECT rcClient;
    ::GetClientRect(*this, &rcClient);

    if( !::IsZoomed(*this) )
    {
        RECT rcSizeBox = m_pm.GetSizeBox();
        if( pt.y < rcClient.top + rcSizeBox.top )
        {
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
            return HTTOP;
        }
        else if( pt.y > rcClient.bottom - rcSizeBox.bottom )
        {
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
            return HTBOTTOM;
        }

        if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
        if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
    }

    RECT rcCaption = m_pm.GetCaptionRect();
    if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
        && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
            CControlUI* pControl = m_pm.FindControl(pt);
            if( pControl && _tcsicmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 && 
                _tcsicmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
                _tcsicmp(pControl->GetClass(), DUI_CTR_TEXT) != 0 )
                return HTCAPTION;
    }

    return HTCLIENT;
}

LRESULT MainWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ::ShowWindow(m_hWnd, SW_HIDE);
    PostQuitMessage(0);
    return 0;
}

void MainWnd::Notify(TNotifyUI& msg)
{
    if (msg.sType == DUI_MSGTYPE_CLICK)
    {
        CDuiString uiName = msg.pSender->GetName();
        if (0 == uiName.Compare(L"btnClose") || 0 == uiName.Compare(L"btnCancel"))
        {
            ShowWindow(false);
            Close(0);
        }
        else if (0 == uiName.Compare(L"btnMin"))
        {
            SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
        }
        else if (0 == uiName.Compare(L"btnMax"))
        {
            if (IsZoomed(m_hWnd))
            {
                SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
            }
            else
            {
                SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
            }
        }
        else if (0 == uiName.Compare(L"btnNewDir"))
        {
            OnNewDir();
        }
    }
    else if (msg.sType == _T("setfocus"))
    {
        CCheckBoxUI* pCheck = dynamic_cast<CCheckBoxUI*>(msg.pSender);
        if (pCheck != NULL)
        {
            CControlUI* pTemp = pCheck->GetParent();
            if (pTemp != NULL)
            {
                CTreeNodeUI* pTreeNode = dynamic_cast<CTreeNodeUI*>(pTemp->GetParent());
                if (pTreeNode != NULL)
                {
                    if (pTreeNode->IsHasChild())
                    {
                        return;
                    }
                    if (!PathIsDirectoryW(pTreeNode->GetUserData()))
                    {
                        return;
                    }
                    ExpendTree(pTreeNode);
                    pCheck->SetCheck(true, false);
                }
            }
        }
    }
    else if (msg.sType == _T("itemdbclick"))
    {
        CTreeNodeUI* pTreeNode = dynamic_cast<CTreeNodeUI*>(msg.pSender);
        if (pTreeNode != NULL)
        {
            if (pTreeNode->IsHasChild())
            {
                return;
            }
            if (!PathIsDirectoryW(pTreeNode->GetUserData()))
            {
                return;
            }
            ExpendTree(pTreeNode);

            CCheckBoxUI* pFolder = pTreeNode->GetFolderButton();
            if (pFolder != NULL)
            {
                pFolder->SetCheck(false, false);
            }
        }
    }
}

void MainWnd::OnNewDir()
{
    int j = m_treeLeft->GetCurSel();
    CTreeNodeUI* pTreeNodeRight = dynamic_cast<CTreeNodeUI*>(m_treeLeft->GetItemAt(j));
    if (NULL == pTreeNodeRight || pTreeNodeRight->GetUserData() == L"computer")
    {
        MessageBoxW(m_hWnd, L"请选择要存放的目录", L"提示", MB_OK | MB_ICONERROR);
        return;
    }
    WCHAR szDir[MAX_PATH] = {0};
    lstrcpy(szDir, pTreeNodeRight->GetUserData());

    PathAppendW(szDir, L"新建文件夹");
    WCHAR szTemp[MAX_PATH] = {0};
    lstrcpy(szTemp, szDir);

    int i = 1;
    while (PathFileExistsW(szTemp))
    {
        wnsprintfW(szTemp, MAX_PATH, L"%s(%d)", szDir, i++);
    }
    CreateDirectoryW(szTemp, NULL);

    list<wstring> listFile;
    listFile.push_back(szTemp);
    AddChildNode(pTreeNodeRight, listFile);
}

LRESULT MainWnd::OnGetMinMaxInfo( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    MONITORINFO oMonitor = {};
    oMonitor.cbSize = sizeof(oMonitor);
    ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
    CDuiRect rcWork = oMonitor.rcWork;
    rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

    LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
    lpMMI->ptMaxPosition.x	= rcWork.left;
    lpMMI->ptMaxPosition.y	= rcWork.top;
    lpMMI->ptMaxSize.x		= rcWork.right;
    lpMMI->ptMaxSize.y		= rcWork.bottom;

    return HRESULT_CONTINUE;
}
