/*
*/
#ifndef __V_DESKTOP_H__
#define __V_DESKTOP_H__
#include <windows.h>
class CVDeskTop
{
public:
	CVDeskTop(HANDLE hMutex);
	~CVDeskTop();
	bool Begin();
	void End();
private:
	void Lock();
	void UnLock();
	void SaveDesktopScreen(HBITMAP hBmp, char * szFileName);
private:
	HANDLE	m_hMutex;
	char	m_szTempFileName[MAX_PATH+1];
	char	m_szWallPaperName[MAX_PATH+1];
	HDESK   m_hdeskOld;
	HDESK   m_hdeskNew;
	HWND	m_hDesktopWnd;

};
#endif //__V_DESKTOP_H__