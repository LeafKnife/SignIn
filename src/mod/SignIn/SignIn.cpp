#include "mod/SignIn/SignIn.h"

#include <bitset>
#include <ll/api/data/KeyValueDB.h>
#include <mc/platform/UUID.h>
#include <optional>
#include <string>
#include <string_view>

namespace signin {
namespace {
ll::data::KeyValueDB DB(lk::MyMod::getInstance().getSelf().getDataDir() / "KV");
}

inline std::string getPlayerKey(mce::UUID uuid, std::string year) { return "P:" + uuid.asString() + ":Y:" + year; }
inline std::string getPlayerLastSigninKey(mce::UUID uuid) { return "LAST_SIGNIN_AT:" + uuid.asString(); }

ll::io::Logger& getLogger() { return lk::MyMod::getInstance().getSelf().getLogger(); }

void initDB() {
    if (!DB.has("SIGNIN_INIT")) {
        DB.set("SIGNIN_INIT", std::to_string(utils::getCurrentTime().tm_year + 1900));
    }
};

void signIn(mce::UUID uuid, int day) {
    auto key = getPlayerKey(uuid, std::to_string(utils::getCurrentTime().tm_year + 1900));
    if (auto data = DB.get(key); data.has_value()) {
        std::bitset<366> bitset(data.value());
        if (bitset.test(day)) return;
        bitset.set(day);
        DB.set(key, bitset.to_string());
    } else {
        std::bitset<366> bitset;
        bitset.set(day);
        DB.set(key, bitset.to_string());
    }
    setLastTime(uuid);
};

int getAcc(mce::UUID uuid) {
    auto key   = getPlayerKey(uuid, std::to_string(utils::getCurrentTime().tm_year + 1900));
    auto count = 0;
    if (auto data = DB.get(key); data.has_value()) {
        std::bitset<366> bitset(data.value());
        count = bitset.count();
    }
    return count;
};

int getCont(mce::UUID uuid) {
    auto key   = getPlayerKey(uuid, std::to_string(utils::getCurrentTime().tm_year + 1900));
    int  count = 0;
    auto day   = utils::getCurrentTime().tm_yday;
    if (auto data = DB.get(key); data.has_value()) {
        std::bitset<366> bitset(data.value());
        for (int index = day - 1; index >= 0; index--) {
            if (!bitset.test(index)) break;
            count++;
        };
        if (bitset.test(day)) count++;
    }
    return count;
};

void setLastTime(mce::UUID uuid) {
    auto key = getPlayerLastSigninKey(uuid);
    auto tm  = utils::getCurrentTime();
    DB.set(
        key,
        fmt::format(
            "{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
            tm.tm_year + 1900,
            tm.tm_mon + 1,
            tm.tm_mday + 1,
            tm.tm_hour,
            tm.tm_min,
            tm.tm_sec
        )
    );
}

std::optional<std::string> getLastTime(mce::UUID uuid) {
    auto key = getPlayerLastSigninKey(uuid);
    if (DB.has(key)) {
        return DB.get(key);
    } else {
        return std::nullopt;
    }
}

std::bitset<366> getBitSet(mce::UUID uuid) {
    auto key = getPlayerKey(uuid, std::to_string(utils::getCurrentTime().tm_year + 1900));
    if (auto data = DB.get(key); data.has_value()) {
        std::bitset<366> bitset(data.value());
        return bitset;
    } else {
        std::bitset<366> bitset;
        return bitset;
    }
}
} // namespace signin