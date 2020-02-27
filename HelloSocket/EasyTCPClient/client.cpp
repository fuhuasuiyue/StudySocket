#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

enum CMD
{
	CMD_LOGIN,
	CMD_LOGOUT,
	CMD_ERROR
};

struct DataHeader
{
	short dataLengh;	// ���ݳ���
	short cmd;			// ����		
};

// DataPackage
struct Login
{
	char userName[32];
	char PassWord[32];
};

struct LoginResult
{
	int result;

};

struct LogOut
{
	char userName[32];
};

struct LogOutResult
{
	int result;

};

int main(int argc, char* argv[])
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);

	if (_sock == INVALID_SOCKET)
	{
		printf("����SOCKETʧ��...\n");
	}
	else
	{
		printf("����SOCKET�ɹ�...\n");
	}

	// 2�����ӷ�����
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//_sin.sin_addr.S_un.S_addr = inet_addr("192.168.109.1");
	int re = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));

	if (re == SOCKET_ERROR)
	{
		printf("���� ���Ӵ���...\n");
	}
	else
	{
		printf("���ӳɹ�...\n");
	}


	char cmdBuf[128] = {};
	while (true)
	{

		// 3 ��������
		scanf("%s", cmdBuf);
		printf("\n\n����������ǣ� %s\n\n", cmdBuf);
		// 4 ������������
		if (0 == strcmp(cmdBuf, "exit"))
		{
			printf("�յ�Exit�˳�����\n");
			break;
		}
		else if (0 == strcmp(cmdBuf, "login"))
		{
			printf("�ҷ��͵���Ϣ�ǣ�%s \n", cmdBuf);
			Login login = {"lyd", "lydmm"};
			DataHeader header = { sizeof(Login),CMD_LOGIN};
			send(_sock, (char*)&header, sizeof(DataHeader), 0);
			send(_sock, (char*)&login, sizeof(Login), 0);

			// ���շ�������������
			DataHeader retHeader = {};

			LoginResult retLogin = {};
			recv(_sock, (char*)&retHeader, sizeof(DataHeader), 0);
			recv(_sock, (char*)&retLogin, sizeof(LoginResult), 0);

			printf("LogInResult: %d\n", retLogin.result);
			
		}
		else if (0 == strcmp(cmdBuf, "logout"))
		{
			printf("�ҷ��͵���Ϣ�ǣ�%s \n", cmdBuf);
			LogOut logout = {"lyd"};
			DataHeader dh = { sizeof(logout), CMD_LOGOUT};
			send(_sock, (char*)&dh, sizeof(DataHeader), 0);
			send(_sock, (char*)&logout, sizeof(LogOut), 0);

			// ���շ�������������
			DataHeader retHeader = {};

			LogOutResult retLogout = {};
			recv(_sock, (char*)&retHeader, sizeof(DataHeader), 0);
			recv(_sock, (char*)&retLogout, sizeof(LogOutResult), 0);

			printf("LogOutResult: %d\n", retLogout.result);
			
		}
		else
		{
			printf("��֧�ֵ�������������룡\n");
		}

		

	}

	closesocket(_sock);
	getchar();




	WSACleanup();
	return 0;

}