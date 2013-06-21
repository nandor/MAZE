// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Network::Network(Engine *engine)
	: mEngine(engine),
	  mRecv(INVALID_SOCKET),
	  mSend(INVALID_SOCKET)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData))
	{
		throw std::runtime_error("[Network] WSA Startup failed");
	}
}

// ------------------------------------------------------------------------------------------------
Network::~Network()
{
	if (mRecv != INVALID_SOCKET)
	{
		closesocket(mRecv);
		mRecv = INVALID_SOCKET;
	}

	if (mSend != INVALID_SOCKET)
	{
		closesocket(mSend);
		mSend = INVALID_SOCKET;
	}

	WSACleanup();
}

// ------------------------------------------------------------------------------------------------
void Network::Init()
{	
    struct sockaddr_in server;
	u_long utrue = TRUE;

    if((mRecv = socket(AF_INET , SOCK_DGRAM , 0)) == INVALID_SOCKET)
    {
        throw Exception("[Network] Could not create socket: ") << WSAGetLastError();
    }
	
	mPort = mEngine->GetSetup().Port;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(mPort);
	
	if (bind(mRecv, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		throw Exception("[Network] Cannot bind socket") << WSAGetLastError();
	}
	
	ioctlsocket(mRecv, FIONBIO, &utrue);
	Log::Inst() << "[Network] Listening on port: " << mPort;
}

// ------------------------------------------------------------------------------------------------
int Network::Worker()
{
	Packet packet;
    sockaddr_in client;
    int clientSize = sizeof(sockaddr_in);

	try
	{
		Log::Inst() << "[Network] Thread started";

		while (IsRunning())
		{
			if (recvfrom(mRecv, (char*)&packet, sizeof(Packet), 0, (SOCKADDR*)&client, &clientSize) == SOCKET_ERROR)
			{
				continue;
			}
		}
		
		Log::Inst() << "[Network] Thread stopped";
		return 0;
	} 
	catch (std::exception& e) 
	{
		Log::Inst() << "[Network] Unhandled exception: " << e.what();
		mEngine->Quit();		
		return -1;
	}
}