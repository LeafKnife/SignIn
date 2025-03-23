#include "mod/SignIn/Form/Form.h"
#include "fmt/core.h"
#include "mod/SignIn/SignIn.h"

#include <cstddef>
#include <gmlib/gm/form/ChestForm.h>
#include <gmlib/mc/world/ItemStack.h>
#include <gmlib/mc/world/actor/Player.h>
#include <ll/api/form/CustomForm.h>
#include <mc/network/ConnectionRequest.h>
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

inline void registerSlot(gmlib::form::ChestForm& fm, SignData const& data, int slot, int index) {
    auto status = data.status;
    auto day    = index + 1;
    switch (status) {
    case SignStatus::NotSign: {
        auto                              item = gmlib::world::GMItemStack("minecraft:red_concrete", day);
        Bedrock::Safety::RedactableString customName;
        customName.append(fmt::format("§r日期 {}号 未签到", day));
        item.setCustomName(customName);
        fm.registerSlot(slot, gmlib::world::GMItemStack(item));
        break;
    }
    case SignStatus::HasSign: {
        auto                              item = gmlib::world::GMItemStack("minecraft:lime_concrete", day);
        Bedrock::Safety::RedactableString customName;
        customName.append(fmt::format("§r日期 {}号 已签到", day));
        item.setCustomName(customName);
        fm.registerSlot(slot, gmlib::world::GMItemStack(item));
        break;
    }
    case SignStatus::CanSign: {
        auto                              item = gmlib::world::GMItemStack("minecraft:yellow_concrete", day);
        Bedrock::Safety::RedactableString customName;
        customName.append(fmt::format("§r日期 {}号 未签到", day));
        auto lore = std::vector<std::string>{"§r§7点击签到§r"};
        item.setCustomName(customName);
        item.setCustomLore(lore);
        auto yday = data.yday;
        fm.registerSlot(slot, gmlib::world::GMItemStack(item), [yday](gmlib::world::actor::GMPlayer& gmp) {
            auto uuid = gmp.getUuid();
            signin::signIn(uuid, yday);
        });
        break;
    }
    case SignStatus::WillSign: {
        auto                              item = gmlib::world::GMItemStack("minecraft:light_gray_concrete", day);
        Bedrock::Safety::RedactableString customName;
        customName.append(std::string("不可签到"));
        item.setCustomName(customName);
        fm.registerSlot(slot, gmlib::world::GMItemStack(item));
        break;
    }
    }
}

inline gmlib::world::GMItemStack getSignInfoItem(MonthData const& md) {
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
    return item;
}

inline void renderChestUI1(gmlib::form::ChestForm& fm, MonthData const& md) {
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
        registerSlot(fm, md.data[index], slot, index);
    }
    for (auto index = 0; index < 6; index++) {
        auto item = gmlib::world::GMItemStack("minecraft:barrier", 1);
        fm.registerSlot(index * 9 + 0, gmlib::world::GMItemStack(item));
        fm.registerSlot(index * 9 + 8, gmlib::world::GMItemStack(item));
    }
    fm.registerSlot(52, getSignInfoItem(md));
}

inline void renderChestUI2(gmlib::form::ChestForm& fm, MonthData const& md) {
    auto year  = md.year;
    auto month = md.month;
    fm.setTitle(fmt::format("签到 {:04d}-{:02d}", year, month));
    for (size_t index = 0; index < md.data.size(); index++) {
        auto slot = index + 9;
        registerSlot(fm, md.data[index], slot, index);
    }
    for (auto index = 0; index < 9; index++) {
        auto item = gmlib::world::GMItemStack("minecraft:barrier", 1);
        fm.registerSlot(index, gmlib::world::GMItemStack(item));
        fm.registerSlot(index + 5 * 9, gmlib::world::GMItemStack(item));
    }
    fm.registerSlot(md.data.size() + 9, getSignInfoItem(md));
}

void renderSignInChestUI(gmlib::form::ChestForm& fm, MonthData const& md, bool isPE) {
    if (isPE) {
        renderChestUI2(fm, md);
    } else {
        renderChestUI1(fm, md);
    }
}
void sendForm(Player& player, int month, int year) {
    auto      uuid = player.getUuid();
    auto      fm   = ll::form::CustomForm();
    MonthData md;
    getMonthData(uuid, md, month, year);
    renderSignInForm(fm, md);
    fm.sendTo(player);
}

void sendChestUI(Player& player, int month, int year) {
    auto      uuid = player.getUuid();
    auto      fm   = gmlib::form::ChestForm();
    MonthData md;
    getMonthData(uuid, md, month, year);
    auto gmPlayer =
        gmlib::world::actor::GMPlayer::getServerPlayer(player.getNetworkIdentifier(), player.getClientSubId());
    auto os = gmPlayer->mBuildPlatform;

    // gmPlayer->getConnectionRequest()->mRawToken->mDataInfo;
    renderSignInChestUI(fm, md, (int)os < 3);
    fm.sendTo(gmPlayer);
}
} // namespace signin::form