/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019 Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "protocolcheck.h"
#include "configmanager.h"
#include "game.h"
#include "outputmessage.h"

extern ConfigManager g_config;
extern Game g_game;

void ProtocolCheck::onRecvFirstMessage(NetworkMessage& /*msg*/) {
	std::string ipStr = convertIPToString(getIP());
	if (ipStr != g_config.getString(ConfigManager::IP)) {
		disconnect();
		return;
	}

	disconnect();
}

void ProtocolCheck::parsePacket(NetworkMessage& msg)
{
	if (msg.getLength() <= 0) {
		return;
	}

	uint8_t recvByte = msg.getByte();
	switch (recvByte) {
		case 0xFF: break;
		case 0x01: break;
		default: break;
	}


}
