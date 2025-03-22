#include "fmt/core.h"
#include "mod/SignIn/SignIn.h"

#include <gmlib/gm/form/ChestForm.h>
#include <gmlib/mc/world/ItemStack.h>
#include <gmlib/mc/world/actor/Player.h>
#include <ll/api/form/CustomForm.h>
#include <string>
#include <vector>

namespace signin::form {

void renderSignInForm(ll::form::CustomForm& fm, MonthData const& md) {
    auto year  = md.year;
    auto month = md.month;
    fm.setTitle(fmt::format("签到 {:04d}-{:02d}", year, month));

    fm.appendLabel(fmt::format(
        "  今日{}签到§r  累计签到:§2 {}§r天 连续签到:§2 {}§r天",
        md.isSignToday ? "§3已" : "§4未",
        md.acc,
        md.cont
    ));
    if (!md.last_signin_time.empty()) {
        fm.appendLabel(fmt::format("  上次签到时间：{}", md.last_signin_time));
    }

    auto WeekDay = utils::calcDayOfWeek(year, month, 1);
    fm.appendLabel("  周日  周一  周二  周三  周四  周五  周六   ");
    std::string content;
    for (int i = 0; i < WeekDay; ++i) {
        content.append("  §700§r ");
    };
    for (size_t index = 0; index < md.data.size(); index++) {
        if (WeekDay > 6) {
            fm.appendLabel(content);
            content = "";
            WeekDay = 0;
        }
        WeekDay++;
        auto status = md.data[index].status;
        switch (status) {
        case SignStatus::NotSign:
            content.append(fmt::format("  §c{:02d}§r ", index + 1));
            break;
        case SignStatus::HasSign:
            content.append(fmt::format("  §2{:02d}§r ", index + 1));
            break;
        case SignStatus::CanSign:
            break;
        case SignStatus::WillSign:
            content.append(fmt::format("  §7{:02d}§r ", index + 1));
            break;
        }
    }
    if (!content.empty()) fm.appendLabel(content);
}
void renderSignInChestUI(gmlib::form::ChestForm& fm, MonthData const& md) {
    auto year  = md.year;
    auto month = md.month;
    fm.setTitle(fmt::format("签到 {:04d}-{:02d}", year, month));
    auto WeekDay = utils::calcDayOfWeek(year, month, 1);
    int  y       = 0;
    for (size_t index = 0; index < md.data.size(); index++) {
        auto slot = WeekDay + y * 9;
        if (WeekDay > 6) {
            y++;
            WeekDay = 0;
        }
        WeekDay++;
        auto status = md.data[index].status;
        switch (status) {
        case SignStatus::NotSign: {
            auto                              item = gmlib::world::GMItemStack("minecraft:red_concrete", index + 1);
            Bedrock::Safety::RedactableString customName;
            customName.append(std::string("未签到"));
            item.setCustomName(customName);
            fm.registerSlot(slot, gmlib::world::GMItemStack(item));
            break;
        }
        case SignStatus::HasSign: {
            auto                              item = gmlib::world::GMItemStack("minecraft:lime_concrete", index + 1);
            Bedrock::Safety::RedactableString customName;
            customName.append(std::string("已签到"));
            item.setCustomName(customName);
            fm.registerSlot(slot, gmlib::world::GMItemStack(item));
            break;
        }
        case SignStatus::CanSign: {
            auto                              item = gmlib::world::GMItemStack("minecraft:yellow_concrete", index + 1);
            Bedrock::Safety::RedactableString customName;
            customName.append(std::string("今日未签到"));
            item.setCustomName(customName);
            auto yday = md.data[index].yday;
            fm.registerSlot(slot, gmlib::world::GMItemStack(item), [yday](gmlib::world::actor::GMPlayer& gmp) {
                auto uuid = gmp.getUuid();
                signin::signIn(uuid, yday);
            });
            break;
        }
        case SignStatus::WillSign: {
            auto item = gmlib::world::GMItemStack("minecraft:light_gray_concrete", index + 1);
            Bedrock::Safety::RedactableString customName;
            customName.append(std::string("不可签到"));
            item.setCustomName(customName);
            fm.registerSlot(slot, gmlib::world::GMItemStack(item));
            break;
        }
        }
    }
    for (auto index = 0; index < 6; index++) {
        auto item = gmlib::world::GMItemStack("minecraft:barrier", 1);
        fm.registerSlot(index * 9 + 0, gmlib::world::GMItemStack(item));
        fm.registerSlot(index * 9 + 8, gmlib::world::GMItemStack(item));
    }
    auto                              item = gmlib::world::GMItemStack("minecraft:clock", 1);
    Bedrock::Safety::RedactableString customName;
    customName.append(std::string("签到信息"));
    auto lore = std::vector<std::string>();
    lore.push_back(fmt::format(
        "§r今日{}签到§r\n累计签到:§2 {}§r天\n连续签到:§2 {}§r天",
        md.isSignToday ? "§3已" : "§4未",
        md.acc,
        md.cont
    ));
    lore.push_back(fmt::format("§r上次签到时间\n{}", md.last_signin_time));
    item.setCustomName(customName);
    item.setCustomLore(lore);
    fm.registerSlot(52, item);
}
void sendForm(Player& player, int month, int year) {
    auto      uuid = player.getUuid();
    auto      fm   = ll::form::CustomForm();
    MonthData md;
    getMonthData(uuid, md, month, year);
    renderSignInForm(fm, md);
    fm.sendTo(player);
}

// 另一种排序，现在注释掉，说不定以后会用到
// void sendChestUI(Player& player) {
//     auto uuid        = player.getUuid();
//     auto bitset      = getBitSet(uuid);
//     auto tm          = utils::getCurrentTime();
//     auto firstDay    = tm.tm_yday - tm.tm_mday;
//     auto daysInMonth = utils::getDaysInMonth(tm.tm_mon, tm.tm_year);
//     auto fm          = gmlib::form::ChestForm();
//     fm.setTitle(fmt::format("签到 {:04d}-{:02d}", tm.tm_year + 1900, tm.tm_mon + 1));
//     // auto WeekDay = calcDayOfWeek(tm.tm_year + 1900, tm.tm_mon + 1, 1);
//     // auto y       = 0;
//     for (int index = 1; index <= daysInMonth; index++) {
//         auto slot = index + 8;
//         auto yday = index + firstDay;
//         if (yday > tm.tm_yday) {
//             auto                              item = gmlib::world::GMItemStack("minecraft:light_gray_concrete",
//             index); Bedrock::Safety::RedactableString customName; customName.append(std::string("不可签到"));
//             item.setCustomName(customName);
//             fm.registerSlot(slot, gmlib::world::GMItemStack(item));
//             continue;
//         }
//         if (bitset.test(index + firstDay)) {
//             auto                              item = gmlib::world::GMItemStack("lime_concrete", index);
//             Bedrock::Safety::RedactableString customName;
//             customName.append(std::string("已签到"));
//             item.setCustomName(customName);
//             fm.registerSlot(slot, gmlib::world::GMItemStack(item));
//         } else {
//             auto                              item = gmlib::world::GMItemStack("minecraft:pink_concrete", index);
//             Bedrock::Safety::RedactableString customName;
//             customName.append(std::string("未签到"));
//             item.setCustomName(customName);
//             fm.registerSlot(slot, gmlib::world::GMItemStack(item), [yday](gmlib::world::actor::GMPlayer& gmp) {
//                 auto uuid = gmp.getUuid();
//                 // TODO
//                 signIn(uuid, yday);
//             });
//         }
//     }
//     auto gmPlayer =
//         gmlib::world::actor::GMPlayer::getServerPlayer(player.getNetworkIdentifier(), player.getClientSubId());
//     fm.sendTo(gmPlayer);
// }

void sendChestUI(Player& player, int month, int year) {
    auto      uuid = player.getUuid();
    auto      fm   = gmlib::form::ChestForm();
    MonthData md;
    getMonthData(uuid, md, month, year);
    renderSignInChestUI(fm, md);
    auto gmPlayer =
        gmlib::world::actor::GMPlayer::getServerPlayer(player.getNetworkIdentifier(), player.getClientSubId());
    fm.sendTo(gmPlayer);
}
} // namespace signin::form