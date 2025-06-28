#pragma once

#include "mod/MyMod.h"

#include <ctime>
#include <ll/api/data/KeyValueDB.h>
#include <ll/api/io/Logger.h>
#include <mc/platform/UUID.h>
#include <mc/world/actor/player/Player.h>
#include <optional>
#include <vector>

namespace signin {

ll::io::Logger& getLogger();

enum class SignStatus { NotSign, HasSign, CanSign, WillSign };

struct SignData {
    int        yday;
    SignStatus status;
};

struct MonthData {
    int                   month            = -1;
    int                   year             = -1;
    int                   acc              = 0;
    int                   cont             = 0;
    bool                  isSignToday      = false;
    std::string           last_signin_time = "";
    std::vector<SignData> data;
};

void                       initDB();
void                       signIn(mce::UUID uuid, int day);
void                       retro(mce::UUID uuid);
int                        getLastNotSignedDay(mce::UUID uuid);
void                       getMonthData(mce::UUID uuid, MonthData& md, int month, int year);
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
int     getSumDay(int year, int month, int day = 1);
int     calcDayOfWeek(int year, int month, int day);
int     getDaysInMonth(int month, int year);
} // namespace utils
} // namespace signin