// #include "mod/SignIn/SignIn.h"
// #include <gmlib/gm/form/ChestForm.h>
// #include <gmlib/mc/world/ItemStack.h>
// #include <gmlib/mc/world/actor/Player.h>
// #include <string_view>

// namespace signin::form {

// void sendChestUI(Player& player) {
//     auto fm = gmlib::tools::ChestUI("测试");
//     auto gmPlayer =
//         gmlib::world::actor::GMPlayer::getServerPlayer(player.getNetworkIdentifier(), player.getClientSubId());
//     auto item = gmlib::world::GMItemStack("minecraft:bread", 14);
//     getLogger().info("item {} Count {} type {}", item.isNull(), item.mCount, item.getTypeName());
//     fm.registerSlot(1, item).registerSlot(5, gmlib::world::GMItemStack("minecraft:bread", 14)).sendTo(gmPlayer);
// }

// } // namespace signin::form