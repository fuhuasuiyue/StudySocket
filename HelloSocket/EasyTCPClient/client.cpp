#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

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
		char recvBuf[256] = {};
		re = recv(_sock, recvBuf, 256, 0);
		recvBuf[255] = '\0';
		if (re <= 0)
		{
			printf("���� ������Ϣ����...\n");
		}
		else
		{
			printf("������Ϣ�ɹ�...\n");
			printf("���յ���Ϣ�ǣ�%s \n", recvBuf);

		}
		// 3 ��������
		scanf("%s", cmdBuf);
		// 4 ������������
		if (0 == strcmp(cmdBuf, "exit"))
		{
			printf("�յ�Exit�˳�����\n");
			break;
		}
		else
		{
			printf("�ҷ��͵���Ϣ�ǣ�%s \n", cmdBuf);
			send(_sock, cmdBuf, strlen(cmdBuf) + 1, 0);
		}

		memset(cmdBuf, 0, 128);

	}

	closesocket(_sock);
	getchar();




	WSACleanup();
	return 0;

}