#ifndef WINMAIN_H
#define WINMAIN_H

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "winmenu.h"


HANDLE hComm;
HWND hwndMain;
HWND hwndConn;

HDC hdc;
WNDPROC defaultTextProc;
HINSTANCE		hInstance;

INT_PTR CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MenuProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK server(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK client(HWND, UINT, WPARAM, LPARAM);


HWND hServer, hClient, hTcp, hUdp, hPort, hConn, hIP, hHost, hAddress;
HWND hPkSize, hNumPk, hFileT, hOK, hList;


void dialogInit(HWND);
void initHwnd(HWND);
void changeType(HWND, int);
void menuAction(HWND , WPARAM);

void ListOut(char * );





extern void sv_winsock(HWND, SOCKET *,int, int);
extern void cl_winsock(HWND, SOCKET*, int, char*, int, int);
extern void FreeSocketInfomation(SOCKET);
extern void FreeSocketInfo(SOCKET);
extern void sv_disconn(SOCKET);

extern void ServWinProc(HWND, UINT, WPARAM, LPARAM, SOCKET*);
extern void ClientProc(HWND, UINT, WPARAM, LPARAM, SOCKET*);


int op_id = 0;
int op_type = 0;
int op_protocol = 0;
int op_msg ;
SOCKET server_sock, client_sock;

#endif 