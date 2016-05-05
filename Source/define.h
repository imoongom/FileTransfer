#ifndef DEFINE_H
#define DEFINE_H



#define WM_TCPSERV (WM_USER + 1)
#define WM_TCPSERVCONN (WM_USER + 5)
#define WM_UDPSERV (WM_USER + 2)

#define WM_TCPCLENT (WM_USER + 3)
#define WM_UDPCLENT (WM_USER + 4)

#define PORT 5150
#define BUFSIZE 100


#define STRSIZE		128
#define DEFAULTPORT	7000
#define SERVER		1001
#define CLIENT		1002
#define TCP			1003
#define UDP			1004
#define IP			1005
#define HOST		1006
#define MESGSEND	1007
#define FILESEND	1008

#define DATA_BUFSIZE 500000



extern HWND hClient, hTcp, hUdp, hPort,hHost, hAddress, hPkSize, hNumPk, hFileT;



#endif