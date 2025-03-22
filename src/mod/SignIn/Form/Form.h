#pragma once

#include "gmlib/gm/form/ChestForm.h"
#include <ll/api/form/CustomForm.h>
#include <mc/platform/UUID.h>
#include <mc/world/actor/player/Player.h>

namespace signin::form {
void renderSignInForm(ll::form::CustomForm& fm, int month, int year);
void renderSignInChestUI(gmlib::form::ChestForm& fm, int month, int year);
void sendChestUI(Player& player, int month, int year);
void sendForm(Player& player, int month, int year);
} // namespace signin::form