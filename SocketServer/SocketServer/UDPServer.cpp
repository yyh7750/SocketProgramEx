#include "pch.h"
#include "UDPServer.h"
#include "SocketServerDlg.h"

UDPServer::UDPServer()
	: m_bInitialized(FALSE)
{
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

		//BYTE recvBuffer[sizeof(MessageInfo)];
		MessageInfo* recvMessage;

		// ������ ����
		//int recvLen = ReceiveFrom(recvBuffer, sizeof(recvBuffer), (SOCKADDR*)&clientAddr, &addrLen);
		int recvLen = ReceiveFrom(recvMessage, sizeof(MessageInfo), (SOCKADDR*)&clientAddr, &addrLen);
		if (recvLen == SOCKET_ERROR)
		{
			OutputDebugString(_T("error!!!"));
			// ����ó��
			return;
		}

		MessageInfo messageInfo;
		memcpy(&messageInfo, recvBuffer, sizeof(MessageInfo));

		CString outputString;
		outputString.Format(_T("Port: %d, Data: %s\n\n"), messageInfo.m_port, CString(messageInfo.m_data));
		OutputDebugString(outputString);

		CSocketServerDlg* pDlg = (CSocketServerDlg*)AfxGetApp()->GetMainWnd();
		if (pDlg != nullptr) {
			CString str = CString(messageInfo.m_data);
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
	MessageInfo message;
	message.m_port = 9999;
	strcpy_s(message.m_data, sizeof(message.m_data), data); // ������ ����

	// �޽��� ������ ����Ʈ �迭�� ��ȯ
	BYTE* sendData = new BYTE[sizeof(message)];
	memcpy(sendData, &message, sizeof(message));

	// Ŭ���̾�Ʈ���� �޽��� ����
	int sendResult = SendTo(sendData, sizeof(message), (SOCKADDR*)&clientAddr, sizeof(clientAddr), 0);
	if (sendResult == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		CString errorStr;
		errorStr.Format(_T("�޽��� ���� �� ���� �߻�. ���� �ڵ�: %d\n"), errorCode);
		OutputDebugString(errorStr);
	}

	// ���� �Ҵ�� �޸� ����
	delete[] sendData;
}

void UDPServer::Run(UINT nPort)
{
	Initialize(nPort);
}
