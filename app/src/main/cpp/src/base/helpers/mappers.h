#pragma once

#include <string>
#include <string_view>

#include "../Structs.h"

namespace NMappers {

std::string actionTypeToStr(const EActionType& actionType);
EActionType strToActionType(std::string_view actionType);

std::string entityToStr(const EEntity& entity);
EEntity strToEntity(std::string_view entity);

std::string roleToStr(const ERole& role);
ERole strToRole(std::string_view role);

} // namespace NMappers