#ifndef		CLIENT_H
#define		CLIENT_H

#include <stdio.h>
#include <winsock2.h>
#include <WinInet.h>

extern void ListOut(char * str);
int MesgType;
int size;
int protoMode;


typedef struct _SOCKET_INFORMATION {
	SOCKET Socket;
	WSABUF DataBuf;
	OVERLAPPED Overlapped;
	DWORD BytesSEND;
	DWORD BytesRECV;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

void cl_winsock(HWND, SOCKET*, int, char *, int, int);
void ClientProc(HWND, UINT, WPARAM, LPARAM, SOCKET*);
void UDPMsgSend(LPSOCKET_INFORMATION SI, SOCKADDR_IN netAddr, int pkSize, int times);
void UDPFileSend(LPSOCKET_INFORMATION SI, SOCKADDR_IN netAddr, int pkSize, int times);
DWORD WINAPI UDPThread(LPVOID);
DWORD WINAPI TCPSendThread(LPVOID);
void TCPFileSend(HWND hwnd, SOCKET);
void DummyPacket(int packetsize, char *packet);
void clDisconnect(SOCKET sock);
#endif