#pragma once
#include "stdafx.h"
#include "MessageType.h"
#include "SaveStateManager.h"
#include "../Utilities/Socket.h"
#include "../Utilities/Serializer.h"

class NetMessage
{
protected:
	MessageType _type;
	stringstream _receivedData;

	NetMessage(MessageType type)
	{
		_type = type;
	}

	NetMessage(void* buffer, uint32_t length)
	{
		_type = (MessageType)((uint8_t*)buffer)[0];
		_receivedData.write((char*)buffer + 1, length - 1);
	}

public:
	virtual ~NetMessage() 
	{	
	}

	void Initialize()
	{
		Serializer s(_receivedData, SaveStateManager::FileFormatVersion);
		Serialize(s);
	}

	MessageType GetType()
	{
		return _type;
	}

	void Send(Socket &socket)
	{
		Serializer s(SaveStateManager::FileFormatVersion);
		Serialize(s);

		stringstream out;
		s.Save(out);

		string data = out.str();
		uint32_t messageLength = (uint32_t)data.size() + 1;
		data = string((char*)&messageLength, 4) + (char)_type + data;
		socket.Send((char*)data.c_str(), (int)data.size(), 0);
	}

protected:
	virtual void Serialize(Serializer &s) = 0;
};
