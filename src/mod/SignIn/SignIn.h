#pragma once

#include "mod/MyMod.h"

#include <ctime>
#include <ll/api/data/KeyValueDB.h>
#include <ll/api/io/Logger.h>
#include <mc/platform/UUID.h>
#include <mc/world/actor/player/Player.h>
#include <optional>

namespace signin {

ll::io::Logger& getLogger();

void                       initDB();
void                       signIn(mce::UUID uuid, int day);
int                        getAcc(mce::UUID uuid);
int                        getCont(mce::UUID uuid);
void                       setLastTime(mce::UUID uuid);
std::optional<std::string> getLastTime(mce::UUID uuid);
std::bitset<366>           getBitSet(mce::UUID uuid);

void regCommand();
void listenEvents();

namespace utils {
std::tm getCurrentTime();
bool    isLeapYear(int year);
int     getDaysInMonth(int month, int year);
} // namespace utils

namespace form {
void sendChestUI(Player& player);
void sendForm(Player& player);
} // namespace form
} // namespace signin