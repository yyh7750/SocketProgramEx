#pragma once

#include <afxsock.h>
#include "Message.h"

class UDPServer : public CAsyncSocket
{
public:
    UDPServer();

    BOOL Initialize(UINT nPort);
    void Close();
    void SendMessage(const char* data);
    void Run(UINT nPort);

protected:
    virtual void OnReceive(int nErrorCode);

private:
    SOCKADDR_IN m_ServerAddr;
    SOCKADDR_IN clientAddr;
    MessageInfo messageInfo;

    BYTE* MessageToBytes(MessageInfo& message);
    BOOL m_bInitialized;

};
