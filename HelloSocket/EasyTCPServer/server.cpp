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

	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;
	int re = bind(_sock, (sockaddr*)&_sin, sizeof(_sin));
	if (re == SOCKET_ERROR)
	{
		printf("ERROR, �����ڽ��ֿͻ��˵����ӵ�����˿�ʧ��\n");
		return -1;
	}
	{
		printf("������˿ڳɹ�...\n");
	}

	re = listen(_sock, 5);
	if (re == SOCKET_ERROR)
	{
		printf("ERROR, �������ڽ��ֿͻ��˵����ӵ�����˿�ʧ��\n");
		return -1;
	}
	{
		printf("��������˿ڳɹ�...\n");
	}
	// 4 accept �ȴ����տͻ�������
	sockaddr_in clientAddr;
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;
	char msgBuf[] = "Hello, I'm Server!\n";

	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);

	if (INVALID_SOCKET == _cSock)
	{
		printf("���� ���ܵ���Ч�Ŀͻ�������...\n");

	}

	printf("�¿ͻ��˼��룺socket = %d, IP = %s \n", (int)(_cSock), inet_ntoa(clientAddr.sin_addr));
	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
	
	while (true)
	{
		char cmdBuf[128] = {};
		// 5 ���տͻ��˵���������
		int nLen = recv(_cSock, cmdBuf, 128, 0);
		if (nLen <= 0)
		{
			printf("�ͻ����Ѿ��˳���,�������\n");
			break;
		}
		else
		{
			printf("���������յ�����Ϊ��%s \n", cmdBuf);
		}
		// 6 ��������
		if (0 == strcmp(cmdBuf, "getName"))
		{
			char msgBuf[] = "xiaoqiang\n";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
		else if (0 == strcmp(cmdBuf, "getAge"))
		{
			char msgBuf[] = "80��\n";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
		else
		{
			char msgBuf[] = "?????...\n";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);

		}

		

	}


	closesocket(_sock);

	getchar();

	WSACleanup();
	return 0;
	
}