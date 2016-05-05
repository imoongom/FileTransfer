/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:	winmain.cpp
--
-- PROGRAM:		Windows network lookup application	
--
-- FUNCTIONS:
--		int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,	LPSTR lspszCmdParam, int nCmdShow)
--		LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
--		void dialogInit(HWND hwnd);
--		void initHwnd(HWND);
--		void changeType(HWND, int);
--		void menuAction(HWND , WPARAM);
--		void ListOut(char *);
--
--
-- DATE:		January 14, 2016
--
-- REVISIONS:	January 16, 2016    - seperate functions from WndProc
--				January 18, 2016	- add error message dialog and font type
--
-- DESIGNER:	Eunwon Moon
--
-- PROGRAMMER:	Eunwon Moon
--
-- NOTES:
-- The program is to trabsfer message packet or file using TCP or UDP
-- and know how different between to mode.
-- IT could choose either Server or Client and possible to send based on connection.
-- TCP mode is connection based so need to check if client and server are connected
-- Otherwisem, UDP just send packet regardless of connection.
--
----------------------------------------------------------------------------------------------------------------------*/
#include <Winsock2.h>
#include "winmain.h"
#include "define.h"

/*--------------------------------------------------------------------------------------------------------------------------
--FUNCTION:		WinMain
--
--DATE :		Sep 26, 2015
--
--REVISIONS :	
--
--DESIGNER :	Eunwon Moon
--
--PROGRAMMER :	Eunwon Moon
--
--INTERFACE : int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow)
--
--RETURNS : int
--
--NOTES :
-- Call this function When start a program. it forms the window  .
--
----------------------------------------------------------------------------------------------------------------------*/

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
	LPSTR lspszCmdParam, int nCmdShow)
{
	MSG Msg;
	hInstance = hInst;

	hwndMain = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_MENU), 0, WndProc, 0);
	ShowWindow(hwndMain, nCmdShow);
	initHwnd(hwndMain);

	
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

/*--------------------------------------------------------------------------------------------------------------------------
--FUNCTION:		WndProcMain
--
--DATE :		Sep 26, 2015
--
--REVISIONS :	Sep 26, 2015     - actions after connecting port.
--				Oct 20, 2015		- added edit box, added listener for window resize
--				Jan 18, 2016	- add font for textbox
--
--DESIGNER :	 Eunwon Moon
--
--PROGRAMMER :	 Eunwon Moon
--
--INTERFACE : LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--					HWND hwnd     : a handle to the winmain window procedure which received the message.
--					UNIT Message  : the received control message which is generated by user action.
--					WPARAM wParam : tha additional message information de
--					LPARAM lParam : the additional message information  
--
--RETURNS : LRESULT
--
--NOTES :
-- Call this functions depending on users action after connecting. initialize dialog information
-- and detect menu item use or event which is set using WSAAsycSelect.
--
----------------------------------------------------------------------------------------------------------------------*/
INT_PTR CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{


	switch (Message)
	{

		case WM_INITDIALOG:
			dialogInit(hwnd);
			
			return TRUE;
		
		case WM_COMMAND:
			menuAction(hwnd, wParam);
			break;
		case WM_TCPSERV:
		case WM_UDPSERV:
		case WM_TCPSERVCONN:
			ServWinProc(hwnd, Message, wParam, lParam, &server_sock);
			break;
		case WM_TCPCLENT:
		case WM_UDPCLENT:
			ClientProc(hwnd, Message, wParam, lParam, &client_sock);
			break;

			// Terminate program
		case WM_DESTROY:
			PostQuitMessage(0);
			shutdown(op_type == SERVER ? server_sock : client_sock, SD_BOTH);
			closesocket(op_type == CLIENT ? server_sock : client_sock);

			WSACleanup();
			break;

		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
			break;
	}
	return 0;
}

/*--------------------------------------------------------------------------------------------------------------------------
--FUNCTION:		dialogInit
--
--DATE :		Feb 5, 2016
--
--REVISIONS :	
--
--DESIGNER :	 Eunwon Moon
--
--PROGRAMMER :	 Eunwon Moon
--
--INTERFACE : void dialogInit(HWND hwnd)
--					HWND hwnd     : a handle to the winmain window procedure which received the message.
--
--RETURNS : voif
--
--NOTES :
--   This function is to initilize set up when dialog open.
--	 Set each options and value as a default 
--
----------------------------------------------------------------------------------------------------------------------*/
void dialogInit(HWND hwnd) {

	CheckRadioButton(hwnd, IDC_RADIO_SERVER, IDC_RADIO_CLIENT, IDC_RADIO_SERVER);
	op_type = SERVER;
	CheckRadioButton(hwnd, IDC_RADIO_TCP, IDC_RADIO_UDP, IDC_RADIO_TCP);
	op_protocol = TCP;
	op_msg = MESGSEND;

	SetDlgItemText(hwnd, IDC_EDIT_PORT, (LPCSTR)"7000");
	SendDlgItemMessage(hwnd, IDC_EDIT_ADDR, EM_SETREADONLY, 1, 0);
	SendDlgItemMessage(hwnd, IDC_EDIT_SIZE, EM_SETREADONLY, 1, 0);
	SendDlgItemMessage(hwnd, IDC_EDIT_NUM, EM_SETREADONLY, 1, 0);
	SendDlgItemMessage(hwnd, IDC_EDIT_FILE, EM_SETREADONLY, 1, 0);
	EnableWindow(GetDlgItem(hwnd, IDC_BTN_FILE), FALSE);

}

/*--------------------------------------------------------------------------------------------------------------------------
--FUNCTION:		initHwnd
--
--DATE :		Feb 5, 2016
--
--REVISIONS :
--
--DESIGNER :	 Eunwon Moon
--
--PROGRAMMER :	 Eunwon Moon
--
--INTERFACE : void initHwnd(HWND hwnd)
--					HWND hwnd     : a handle to the winmain window procedure which received the message.
--
--RETURNS : void
--
--NOTES :
--   This function is to initilize individual dialog item handle which is global variable.
--
----------------------------------------------------------------------------------------------------------------------*/
void initHwnd(HWND hwnd) {
	CheckRadioButton(hwnd, IDC_RADIO_SERVER, IDC_RADIO_CLIENT, IDC_RADIO_SERVER);
	CheckRadioButton(hwnd, IDC_RADIO_TCP, IDC_RADIO_UDP, IDC_RADIO_TCP);
	hServer = GetDlgItem(hwnd, IDC_RADIO_SERVER);
	hClient = GetDlgItem(hwnd, IDC_RADIO_CLIENT);
	hTcp	= GetDlgItem(hwnd, IDC_RADIO_TCP);
	hUdp	= GetDlgItem(hwnd, IDC_RADIO_UDP);
	hAddress= GetDlgItem(hwnd, IDC_EDIT_ADDR);
	hPkSize = GetDlgItem(hwnd, IDC_EDIT_SIZE);
	hNumPk	= GetDlgItem(hwnd, IDC_EDIT_NUM);
	hFileT = GetDlgItem(hwnd, IDC_EDIT_FILE);
	hPort	= GetDlgItem(hwnd, IDC_EDIT_PORT);
	hOK		= GetDlgItem(hwnd, IDOK);
	hList	= GetDlgItem(hwnd, IDC_LIST);
	hConn	= GetDlgItem(hwnd, IDC_BTN_CONN);
	
}
/*--------------------------------------------------------------------------------------------------------------------------
--FUNCTION:		ListOut
--
--DATE :		Feb 5, 2016
--
--REVISIONS :
--
--DESIGNER :	 Eunwon Moon
--
--PROGRAMMER :	 Eunwon Moon
--
--INTERFACE : void ListOut(char * str) 
--					char * str     : character pointer to display
--
--RETURNS : void
--
--NOTES :
--   This function will be displayed in Listview
--
----------------------------------------------------------------------------------------------------------------------*/
void ListOut(char * str) 
{
	SendMessage(hList, LB_GETCURSEL, 0, 0);
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)str);
}




/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	menuAction
--
-- DATE:		January 14, 2016
--
-- REVISIONS:	Feb 5, 2016			- change menu value and connect others
--
-- DESIGNER:	Eunwon Moon
--
-- PROGRAMMER:	Eunwon Moon
--
-- INTERFACE: BOOL menuAction(HWND hwnd, WPARAM wParam)
--					HWND hwnd     : a handle to the window procedure which received the message.
--					WPARAM wParam : tha additional message information 
--
-- RETURNS: bool
--
--
-- NOTES:
-- This function is used to read command message and perform the menu action in dialog item
--
----------------------------------------------------------------------------------------------------------------------*/
void menuAction(HWND hwnd, WPARAM wParam)
{
	TCHAR str_port[STRSIZE], str[STRSIZE], str2[STRSIZE], str_addr[STRSIZE], str_file[STRSIZE];
	char temp[STRSIZE] = "";
	int check = 0;
	int packet_size, trial_num, port_num = DEFAULTPORT;
	
	WCHAR szFileName[STRSIZE];


	switch (LOWORD(wParam)) {

		case IDC_RADIO_SERVER:
			op_type = SERVER;
			ListOut("SERVER");
			changeType(hwnd, op_type % 2);
			break;
		case IDC_RADIO_CLIENT:
			op_type = CLIENT;
			ListOut("CLIENT");
			changeType(hwnd, op_type % 2);
			break;

		case IDC_RADIO_TCP:
			op_protocol = TCP;
			ListOut("TCP");
			return ;

		case IDC_RADIO_UDP:
			op_protocol = UDP;
			ListOut("UDP");
			return ;

		case IDC_BTN_FILE:
			GetDlgItemText(hwnd, IDC_EDIT_FILE, str_file, STRSIZE);
			if (strcmp(str_file, "") == 0)
			{
				ListOut("Input File Name!!");
				return;
			}
			op_msg = FILESEND;

		case IDC_BTN_CONN:
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO_SERVER), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO_CLIENT), FALSE);

			GetDlgItemText(hwnd, IDC_EDIT_PORT, str_port, STRSIZE);
			if (!isdigit(*str_port) && strcmp(str_port, "") != 0) {
				MessageBox(NULL, TEXT("Please Input valid port number"), NULL, MB_OK);
				break;
			}
			else if (strcmp(str_port, "") != 0)
				port_num = atoi(str_port);
			else
				port_num = DEFAULTPORT;

			if (op_type == CLIENT) {
				GetDlgItemText(hwnd, IDC_EDIT_ADDR, str_addr, STRSIZE);
				GetDlgItemText(hwnd, IDC_EDIT_SIZE, str, STRSIZE);
				GetDlgItemText(hwnd, IDC_EDIT_NUM, str2, STRSIZE);
				if (strcmp(str, "") == 0 || (op_msg !=FILESEND && strcmp(str2, "") == 0)) {
					SendMessage(hList, LB_RESETCONTENT, 0, 0);
					ListOut("Please input the value");
					break;
				}
				else if (!isdigit(*str) && !isdigit(*str2)) {
					SendMessage(hList, LB_RESETCONTENT, 0, 0);
					ListOut("Please input the valid address");
					break;
				}
				else {
					wsprintf(temp, "START CLIENT IP : %s, PORT: %d", str_addr, port_num);
					ListOut(temp);
					packet_size = atoi(str);
					trial_num = atoi(str2);
				}
				cl_winsock(hwnd, &client_sock, op_protocol, str_addr, port_num, op_msg);
			}
			else if (op_type == SERVER) {
				wsprintf(temp, "START SERVER PORT: %d", port_num);
				ListOut(temp);
				sv_winsock(hwnd, &server_sock, op_protocol, port_num);
			}
			else {
				ListOut("Sth wrong");
				ListOut(" ");
			}
			break;

		case IDCLEAR:
			SendMessage(hList, LB_RESETCONTENT, 0, 0);
			break;

		case IDDISCONNECT:
			ListOut(" disconnected!");

			shutdown(op_type == SERVER ? server_sock : client_sock, SD_BOTH);
			closesocket(op_type == CLIENT ? server_sock : client_sock);
		
			WSACleanup();
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO_SERVER), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO_CLIENT), TRUE);
			break;
		case IDCLOSE:
			PostQuitMessage(0);
			return;
	}


}


/*--------------------------------------------------------------------------------------------------------------------------
--FUNCTION:		changeType
--
--DATE :		Feb 5, 2016
--
--REVISIONS :
--
--DESIGNER :	 Eunwon Moon
--
--PROGRAMMER :	 Eunwon Moon
--
--INTERFACE : void changeType(HWND hwnd, int op_type)
--					HWND hwnd: : a handle to the window procedure which received the message.
--					int op_type : option type number depending on server(0) or client(1)
--
--RETURNS : void
--
--NOTES :
--   This function is to handle lock and unlock each dialog items depending on mode
--
----------------------------------------------------------------------------------------------------------------------*/
void changeType(HWND hwnd, int op_type)
{
	SendDlgItemMessage(hwnd, IDC_EDIT_ADDR, EM_SETREADONLY, op_type, 0);
	SendDlgItemMessage(hwnd, IDC_EDIT_SIZE, EM_SETREADONLY, op_type, 0);
	SendDlgItemMessage(hwnd, IDC_EDIT_NUM, EM_SETREADONLY, op_type, 0);
	SendDlgItemMessage(hwnd, IDC_EDIT_FILE, EM_SETREADONLY, op_type, 0);
	EnableWindow(GetDlgItem(hwnd, IDC_BTN_FILE), !op_type);
}
