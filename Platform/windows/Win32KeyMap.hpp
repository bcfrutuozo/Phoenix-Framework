#pragma once

#include "System/Input/KeyCode.hpp"
#include <cstdint>

// Traduz VK_* para KeyCode da engine
KeyCode TranslateKey(uint32_t vk, uint32_t lparam) noexcept;