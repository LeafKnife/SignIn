#pragma once

#include "mod/SignIn/SignIn.h"

#include <gmlib/gm/ui/ChestForm.h>
#include <ll/api/form/CustomForm.h>
#include <mc/platform/UUID.h>
#include <mc/world/actor/player/Player.h>

namespace signin::form {
void renderSignInForm(ll::form::CustomForm& fm, MonthData const& md);
void renderSignInChestUI(gmlib::ui::ChestForm& fm, MonthData const& md, bool isPE = false);
void sendChestUI(Player& player, int month, int year);
void sendForm(Player& player, int month, int year);
} // namespace signin::form