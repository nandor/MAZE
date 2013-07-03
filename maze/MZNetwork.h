// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZNETWORK_H
#define MZNETWORK_H
#pragma once

namespace MAZE
{
	class Engine;

	class Network : public Thread 
	{
	public:

		/**
			Initializes the network manager
		*/
		Network(Engine *engine);

		/**
			Destroys the network manager, closing connections
		*/
		~Network();

		/**
			Starts the network manager
		*/
		void Init();
		
	private:

		/**
			Types of packets
		*/
		enum PacketType
		{
			PK_CONNECT,
			PK_DISCONNECT,
			PK_PING,
			PK_MOVE
		};

		/**
			Packet sent over UDP
		*/
		struct Packet
		{
			/// ID of the sender
			unsigned ID;

			/// Checksum of the message
			unsigned CRC32;

			/// Type of the packet
			PacketType Type;

			/// Header of the event
			unsigned Header;

			/// Data
			char Data[224];
		};

		/**
			Worker thread
		*/
		int Worker();
		
	private:

		/// Reference to the game engine
		Engine * mEngine;

		/// Receiver socket
		SOCKET mRecv;

		/// Sender socket
		SOCKET mSend;

		/// Server port
		unsigned short mPort;
	};
};

#endif
