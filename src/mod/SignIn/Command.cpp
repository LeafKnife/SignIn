#include "mod/MyMod.h"
#include "mod/SignIn/SignIn.h"
#include "mod/SignIn/Form/Form.h"

#include <ll/api/Expected.h>
#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/i18n/I18n.h>
#include <ll/api/service/PlayerInfo.h>
#include <ll/api/utils/SystemUtils.h>
#include <mc/platform/UUID.h>
#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/server/commands/CommandPermissionLevel.h>
#include <mc/world/actor/Actor.h>
#include <mc/world/actor/player/Player.h>
#include <string>

enum class UIType { cui, gui };

namespace signin {

struct SignInUI {
    int month;
    int year;
};

struct SignInInfo {
    enum class IdType { name, xuid, uuid } type;
    std::string playerId;
};

void regCommand() {
    auto& logger = lk::MyMod::getInstance().getSelf().getLogger();
    auto& cmd    = ll::command::CommandRegistrar::getInstance()
                    .getOrCreateCommand("signin", "LK-SignIn - 签到插件", CommandPermissionLevel::Any);

    cmd.overload<SignInUI>().text("gui").optional("month").optional("year").execute(
        [&logger](CommandOrigin const& origin, CommandOutput& output, SignInUI const& param) {
            auto* entity = origin.getEntity();
            if (entity == nullptr || !entity->hasType(::ActorType::Player)) {
                output.error("command.error.notplayer");
            }
            Player* player = (Player*)entity;
            // logger.info("gui {} {}", param.month, param.year);
            form::sendForm(*player,param.month,param.year);
        }
    );

    cmd.overload<SignInUI>().text("cui").optional("month").optional("year").execute(
        [&logger](CommandOrigin const& origin, CommandOutput& output, SignInUI const& param) {
            auto* entity = origin.getEntity();
            if (entity == nullptr || !entity->hasType(::ActorType::Player)) {
                output.error("command.error.notplayer");
            }
            Player* player = (Player*)entity;
            // logger.info("gui {} {}", param.month, param.year);
            form::sendChestUI(*player,param.month, param.year);
        }
    );

    // cmd.overload<SignInInfo>()
    //     .text("info")
    //     .optional("type")
    //     .optional("playerId")
    //     .execute([&logger](CommandOrigin const& origin, CommandOutput& output, SignInInfo const& param) {
    //         mce::UUID uuid;
    //         auto&     playerInfo = ll::service::PlayerInfo::getInstance();
    //         switch (param.type) {
    //         case SignInInfo::IdType::name:
    //             if (auto info = playerInfo.fromName(param.playerId); info.has_value()) {
    //                 uuid = info->uuid;
    //             }
    //             break;
    //         case SignInInfo::IdType::xuid:
    //             if (auto info = playerInfo.fromXuid(param.playerId); info.has_value()) {
    //                 uuid = info->uuid;
    //             }
    //             break;
    //         case SignInInfo::IdType::uuid:
    //             if (auto info = playerInfo.fromUuid(param.playerId); info.has_value()) {
    //                 uuid = info->uuid;
    //             }
    //             break;
    //         default:
    //             return output.error("TODO");
    //         }
    //         // if (uuid == mce::UUID::EMPTY()) {
    //         //     return output.error("Error");
    //         // }
    //         output.success(uuid.asString());
    //     });

    cmd.overload().execute([](CommandOrigin const& origin, CommandOutput& output) {
        auto* entity = origin.getEntity();
        if (entity == nullptr || !entity->hasType(::ActorType::Player)) {
            output.error("command.error.notplayer");
        }
        Player* player = (Player*)entity;
        signin::signIn(player->getUuid(), utils::getCurrentTime().tm_yday);
    });
}
} // namespace signin