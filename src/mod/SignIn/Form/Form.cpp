#include "mod/SignIn/SignIn.h"

#include <ll/api/form/CustomForm.h>
#include <string>
#include <vector>

namespace signin::form {

inline int calcDayOfWeek(int y, int m, int d) {
    int w;
    if (m == 1 || m == 2) m += 12, y--;
    w = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400 + 1) % 7;
    return w;
}


void sendForm(Player& player) {
    auto uuid        = player.getUuid();
    auto bitset      = getBitSet(uuid);
    auto tm          = utils::getCurrentTime();
    auto firstDay    = tm.tm_yday - tm.tm_mday;
    auto daysInMonth = utils::getDaysInMonth(tm.tm_mon, tm.tm_year);
    auto fm          = ll::form::CustomForm();
    fm.setTitle("-- 签 到 --");
    fm.appendLabel(fmt::format(
        "  今日{}签到§r  累计签到:§2 {}§r天 连续签到:§2 {}§r天",
        bitset.test(tm.tm_yday) ? "§3已" : "§4未",
        getAcc(uuid),
        getCont(uuid)
    ));
    std::vector<std::string> vStr;
    auto                     WeekDay = calcDayOfWeek(tm.tm_year + 1900, tm.tm_mon + 1, 1);
    fm.appendLabel("  周日  周一  周二  周三  周四  周五  周六   ");
    std::string content;
    for (int i = 0; i < WeekDay; ++i) {
        content.append("  §700§r ");
    };
    for (int index = 1; index <= daysInMonth; index++) {
        if (WeekDay > 6) {
            fm.appendLabel(content);
            content = "";
            WeekDay = 0;
        }
        WeekDay++;
        if (index + firstDay > tm.tm_yday) {
            content.append(fmt::format("  §7{:02d}§r ", index));
            continue;
        }
        auto isSign = bitset.test(index + firstDay);
        content.append(isSign ? fmt::format("  §2{:02d}§r ", index) : fmt::format("  §c{:02d}§r ", index));
    }
    if (!content.empty()) fm.appendLabel(content);
    fm.sendTo(player);
}
} // namespace signin::form