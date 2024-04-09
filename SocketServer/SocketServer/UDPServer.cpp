#include "pch.h"
#include "UDPServer.h"
#include "SocketServerDlg.h"

UDPServer::UDPServer()
	: m_bInitialized(FALSE)
{
	srand(static_cast<unsigned int>(time(NULL)));
	sendMessage.m_id = rand() % 2147483647;
}

BOOL UDPServer::Initialize(UINT nPort)
{
	// 소켓 초기화
	if (!AfxSocketInit())
	{
		return FALSE;
	}

	// 소켓 생성
	if (!Create(nPort, SOCK_DGRAM))
	{
		return FALSE;
	}

	// 서버 주소 설정
	memset(&m_ServerAddr, 0, sizeof(m_ServerAddr));
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_ServerAddr.sin_port = htons(nPort);

	// 소켓 바인딩
	if (Bind((const SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr)) == SOCKET_ERROR)
	{
		return FALSE;
	}

	OutputDebugString(_T("Initialize() 호출\n\n"));

	// 소켓 초기화 상태 플래그 설정
	m_bInitialized = TRUE;

	return TRUE;
}

void UDPServer::Close()
{
	CAsyncSocket::Close();
	m_bInitialized = FALSE; // 초기화 상태 플래그 초기화
}

void UDPServer::OnReceive(int nErrorCode)
{
	if (nErrorCode == 0)
	{
		int addrLen = sizeof(clientAddr);

		MessageInfo recvMessage;
		int recvLen = ReceiveFrom(&recvMessage, sizeof(MessageInfo), (SOCKADDR*)&clientAddr, &addrLen);
		if (recvLen == SOCKET_ERROR)
		{
			OutputDebugString(_T("error!!!"));
			// 에러처리
			return;
		}

		CString outputString;
		outputString.Format(_T("<- ID: %d, Port: %d, Data: %s\n\n"), recvMessage.m_id, recvMessage.m_port, CString(recvMessage.m_data));
		OutputDebugString(outputString);

		CSocketServerDlg* pDlg = (CSocketServerDlg*)AfxGetApp()->GetMainWnd();
		if (pDlg != nullptr) {
			CString str = CString(outputString);
			pDlg->AddMessageToListBox(str);
		}
	}

	//CAsyncSocket::OnReceive(nErrorCode);
}

void UDPServer::SendMessage(const char* data)
{
	if (!m_bInitialized)
	{
		OutputDebugString(_T("소켓 초기화에 실패했습니다.\n"));
		return;
	}

	// 메시지 정보 생성
	sendMessage.m_port = 9999;
	strcpy_s(sendMessage.m_data, sizeof(sendMessage.m_data), data); // 데이터 복사

	// 클라이언트에게 메시지 전송
	int sendResult = SendTo((BYTE*)&sendMessage, sizeof(sendMessage), (SOCKADDR*)&clientAddr, sizeof(clientAddr), 0);
	if (sendResult == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		CString errorStr;
		errorStr.Format(_T("메시지 전송 중 오류 발생. 에러 코드: %d\n"), errorCode);
		OutputDebugString(errorStr);
		return;
	}

	CString outputString;
	outputString.Format(_T("-> ID: %d, Port: %d, Data: %s\n\n"), sendMessage.m_id, sendMessage.m_port, CString(sendMessage.m_data));
	OutputDebugString(outputString);

	CSocketServerDlg* pDlg = (CSocketServerDlg*)AfxGetApp()->GetMainWnd();
	if (pDlg != nullptr) {
		CString str = CString(outputString);
		pDlg->AddMessageToListBox(str);
	}
}


void UDPServer::Run(UINT nPort)
{
	Initialize(nPort);
}
