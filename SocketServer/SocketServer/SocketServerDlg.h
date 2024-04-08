
// SocketServerDlg.h: 헤더 파일
//

#pragma once

#include "UDPServer.h"

// CSocketServerDlg 대화 상자
class CSocketServerDlg : public CDialogEx
{
private:
	UDPServer m_udpServer;

protected:
	CListBox m_listBox;
	CEdit m_editBox;

// 생성입니다.
public:
	CSocketServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKETSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
	// 수신 메시지 표시
	void AddMessageToListBox(CString& message);
	// 종료
	afx_msg void OnDestory();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedConnect();
};
