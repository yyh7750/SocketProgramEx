#pragma once
#pragma pack(push, 1)

struct MessageInfo
{
    int m_port;
    char m_data[50]; // 메시지 데이터
};

#pragma pack(pop)