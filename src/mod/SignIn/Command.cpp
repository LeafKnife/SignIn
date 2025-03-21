#include "mod/SignIn/SignIn.h"

#include <ll/api/Expected.h>
#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/i18n/I18n.h>
#include <ll/api/utils/SystemUtils.h>
#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/server/commands/CommandPermissionLevel.h>
#include <mc/world/actor/Actor.h>
#include <mc/world/actor/player/Player.h>
#include <string>

enum class SignParam { cui, gui, acc, cont };

namespace signin {

struct SignInCommand {
    SignParam param;
};

void regCommand() {
    auto& cmd = ll::command::CommandRegistrar::getInstance()
                    .getOrCreateCommand("signin", "LK-SignIn - 签到插件", CommandPermissionLevel::Any);
    cmd.overload<SignInCommand>().optional("param").execute(
        [&](CommandOrigin const& origin, CommandOutput& output, SignInCommand const& param) {
            auto* entity = origin.getEntity();
            if (entity == nullptr || !entity->hasType(::ActorType::Player)) {
                output.error("command.error.notplayer");
            }
            Player* player = (Player*)entity;
            switch (param.param) {
            case SignParam::acc: {
                auto acc = getAcc(player->getUuid());
                output.success("累计签到: " + std::to_string(acc));
                break;
            }
            case SignParam::cont: {
                auto cont = getCont(player->getUuid());
                output.success("连续签到: " + std::to_string(cont));
                break;
            }
            case SignParam::cui:
                form::sendChestUI(*player);
                break;
            case SignParam::gui:
                form::sendForm(*player);
                break;
            default:
                signIn(player->getUuid(), utils::getCurrentTime().tm_yday);
                form::sendForm(*player);
                break;
            }
        }
    );
}
} // namespace signin