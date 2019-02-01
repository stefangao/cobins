#include "stdlib.h"
#include "BindPipe.h"
#include "wtermin.h"

CBindPipe::CBindPipe()
{
    m_OnPipeReceiveProc = NULL;
    m_UserData = NULL;
}

BOOL CBindPipe::RegisterRxd(OnPipeReceiveProc_t OnPipeReceiveProc, void* UserData)
{
    m_OnPipeReceiveProc = OnPipeReceiveProc;
    m_UserData = UserData;

    return TRUE;
}

void CBindPipe::OnReceive(int nErrorCode)
{
    if (m_OnPipeReceiveProc != NULL)
    {
        m_OnPipeReceiveProc(m_UserData, nErrorCode);
    }
}

void CBindPipe::OnClose(int nErrorCode) 
{
  
}

long CBindPipe::RecvMessage(LPCSTR receiver, LPCSTR msgname, PBYTE msgdata, int msgdatalen)
{

    return 0;
}