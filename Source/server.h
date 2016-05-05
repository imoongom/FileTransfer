#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <winsock2.h>
#include <WinInet.h>
#include <chrono>


using namespace std;
using namespace std::chrono;

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib,"wininet.lib")


typedef struct _SOCKET_INFORMATION {
	SOCKET Socket;
	WSABUF DataBuf;
	OVERLAPPED Overlapped;
	DWORD BytesSEND;
	DWORD BytesRECV;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

void sv_disconn(SOCKET);
LPSOCKET_INFORMATION SocketInfo;
DWORD WINAPI UDPServThread(LPVOID);
DWORD WINAPI TCPRecvThread(LPVOID);



HWND  hSvHwnd;
HANDLE udpread;
void ServerResult();
int RecvDataCal = 0;
bool RdNotStart = true;
bool firstRecv = false;
high_resolution_clock::time_point t1, t2;


extern SOCKET client_sock;
extern void ListOut(char *);

#endif