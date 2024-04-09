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
	// ���� �ʱ�ȭ
	if (!AfxSocketInit())
	{
		return FALSE;
	}

	// ���� ����
	if (!Create(nPort, SOCK_DGRAM))
	{
		return FALSE;
	}

	// ���� �ּ� ����
	memset(&m_ServerAddr, 0, sizeof(m_ServerAddr));
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_ServerAddr.sin_port = htons(nPort);

	// ���� ���ε�
	if (Bind((const SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr)) == SOCKET_ERROR)
	{
		return FALSE;
	}

	OutputDebugString(_T("Initialize() ȣ��\n\n"));

	// ���� �ʱ�ȭ ���� �÷��� ����
	m_bInitialized = TRUE;

	return TRUE;
}

void UDPServer::Close()
{
	CAsyncSocket::Close();
	m_bInitialized = FALSE; // �ʱ�ȭ ���� �÷��� �ʱ�ȭ
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
			// ����ó��
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
		OutputDebugString(_T("���� �ʱ�ȭ�� �����߽��ϴ�.\n"));
		return;
	}

	// �޽��� ���� ����
	sendMessage.m_port = 9999;
	strcpy_s(sendMessage.m_data, sizeof(sendMessage.m_data), data); // ������ ����

	// Ŭ���̾�Ʈ���� �޽��� ����
	int sendResult = SendTo((BYTE*)&sendMessage, sizeof(sendMessage), (SOCKADDR*)&clientAddr, sizeof(clientAddr), 0);
	if (sendResult == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		CString errorStr;
		errorStr.Format(_T("�޽��� ���� �� ���� �߻�. ���� �ڵ�: %d\n"), errorCode);
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
