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

#include "guild.h"

#include "game.h"
#include <fmt/format.h>


extern Game g_game;

void Guild::addMember(Player* player)
{
	membersOnline.push_back(player);
	for (Player* member : membersOnline) {
		g_game.updatePlayerHelpers(*member);
	}
}

void Guild::removeMember(Player* player)
{
	membersOnline.remove(player);
	for (Player* member : membersOnline) {
		g_game.updatePlayerHelpers(*member);
	}
	g_game.updatePlayerHelpers(*player);

	if (membersOnline.empty()) {
		g_game.removeGuild(id);
		delete this;
	}
}

GuildRank_ptr Guild::getRankById(uint32_t rankId)
{
	for (auto rank : ranks) {
		if (rank->id == rankId) {
			return rank;
		}
	}
	return nullptr;
}

GuildRank_ptr Guild::getRankByName(const std::string& name) const
{
	for (auto rank : ranks) {
		if (rank->name == name) {
			return rank;
		}
	}
	return nullptr;
}

GuildRank_ptr Guild::getRankByLevel(uint8_t level) const
{
	for (auto rank : ranks) {
		if (rank->level == level) {
			return rank;
		}
	}
	return nullptr;
}

void Guild::addRank(uint32_t rankId, const std::string& rankName, uint8_t level)
{
	ranks.emplace_back(std::make_shared<GuildRank>(rankId,rankName,level));
}

void Guild::setPoints(uint32_t _points) {
	points = _points;
	IOGuild::setPoints(id, points);
}

void Guild::setLevel(uint32_t _level) {
	level = _level;
	IOGuild::setLevel(id, level);
}

void Guild::setBalance(uint64_t _balance) {
	balance = _balance;
	IOGuild::setBalance(id, balance);
}

void Guild::setPrivateWarRival(uint32_t rival) {
	privateWarRival = rival;
}

Guild* IOGuild::loadGuild(uint32_t guildId)
{
	Database& db = Database::getInstance();
	std::ostringstream query;
	query << "SELECT `name`, `balance`, `level`, `points` FROM `guilds` WHERE `id` = " << guildId;
	if (DBResult_ptr result = db.storeQuery(query.str())) {
		Guild* guild = new Guild(guildId, result->getString("name"));

		guild->setLevel(result->getNumber<uint32_t>("level"));
		guild->setPoints(result->getNumber<uint32_t>("points"));
		guild->setBalance(result->getNumber<uint64_t>("balance"));

		query.str(std::string());
		query << "SELECT `id`, `name`, `level` FROM `guild_ranks` WHERE `guild_id` = " << guildId;

		if ((result = db.storeQuery(query.str()))) {
			do {
				guild->addRank(result->getNumber<uint32_t>("id"), result->getString("name"), result->getNumber<uint16_t>("level"));
			} while (result->next());
		}
		return guild;
	}
	return nullptr;
}

uint32_t IOGuild::getGuildIdByName(const std::string& name)
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(fmt::format("SELECT `id` FROM `guilds` WHERE `name` = {:s}", db.escapeString(name)));
	if (!result) {
		return 0;
	}
	return result->getNumber<uint32_t>("id");
}

void IOGuild::setLevel(uint32_t guildId, uint32_t newlevel)
{
	Database& db = Database::getInstance();
	std::ostringstream query;
	query << "UPDATE `guilds` SET `level` = " << newlevel << " WHERE `id` = " << guildId;
	db.executeQuery(query.str());
}

void IOGuild::setPoints(uint32_t guildId, uint32_t newPoints)
{
	Database& db = Database::getInstance();
	std::ostringstream query;
	query << "UPDATE `guilds` SET `points` = " << newPoints << " WHERE `id` = " << guildId;
	db.executeQuery(query.str());
}

void IOGuild::setBalance(uint32_t guildId, uint64_t amount)
{
	Database& db = Database::getInstance();
	std::ostringstream query;
	// Updating balance
	query << "UPDATE `guilds` SET `balance` = " << amount << " WHERE `id` = " << guildId;
	db.executeQuery(query.str());
}
