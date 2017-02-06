#ifndef _TREE_MAIN_WND_H_
#define _TREE_MAIN_WND_H_

#include <duilib/duilibmenu/DuilibInclude.h>

class MainWnd : public CWindowWnd, public INotifyUI
{
public:
    MainWnd();
    virtual ~MainWnd();
protected:
    LPCTSTR GetWindowClassName() const
    {
        return TEXT("UIMainFrame");
    }

    UINT GetClassStyle() const
    {
        return CS_DBLCLKS;
    }

    void OnFinalMessage(HWND hWnd)
    {
        delete this;
    }

public:
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual void Notify(TNotifyUI& msg);

    void OnNewDir();
private:
    CPaintManagerUI m_pm;
    CTreeViewUI* m_treeLeft;
};

#endif