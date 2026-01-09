#include "Win32KeyMap.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------

static inline bool IsExtended(uint32_t lparam) noexcept
{
    // Bit 24 indica tecla estendida
    return (lparam & (1u << 24)) != 0;
}

// ------------------------------------------------------------
// VK_* → KeyCode
// ------------------------------------------------------------

KeyCode TranslateKey(uint32_t vk, uint32_t lparam) noexcept
{
    switch (vk)
    {
        // ----------------------------------------------------
        // Letters
        // ----------------------------------------------------
    case 'A': return KeyCode::A;
    case 'B': return KeyCode::B;
    case 'C': return KeyCode::C;
    case 'D': return KeyCode::D;
    case 'E': return KeyCode::E;
    case 'F': return KeyCode::F;
    case 'G': return KeyCode::G;
    case 'H': return KeyCode::H;
    case 'I': return KeyCode::I;
    case 'J': return KeyCode::J;
    case 'K': return KeyCode::K;
    case 'L': return KeyCode::L;
    case 'M': return KeyCode::M;
    case 'N': return KeyCode::N;
    case 'O': return KeyCode::O;
    case 'P': return KeyCode::P;
    case 'Q': return KeyCode::Q;
    case 'R': return KeyCode::R;
    case 'S': return KeyCode::S;
    case 'T': return KeyCode::T;
    case 'U': return KeyCode::U;
    case 'V': return KeyCode::V;
    case 'W': return KeyCode::W;
    case 'X': return KeyCode::X;
    case 'Y': return KeyCode::Y;
    case 'Z': return KeyCode::Z;

        // ----------------------------------------------------
        // Numbers (top row)
        // ----------------------------------------------------
    case '0': return KeyCode::Num0;
    case '1': return KeyCode::Num1;
    case '2': return KeyCode::Num2;
    case '3': return KeyCode::Num3;
    case '4': return KeyCode::Num4;
    case '5': return KeyCode::Num5;
    case '6': return KeyCode::Num6;
    case '7': return KeyCode::Num7;
    case '8': return KeyCode::Num8;
    case '9': return KeyCode::Num9;

        // ----------------------------------------------------
        // Function keys
        // ----------------------------------------------------
    case VK_F1:  return KeyCode::F1;
    case VK_F2:  return KeyCode::F2;
    case VK_F3:  return KeyCode::F3;
    case VK_F4:  return KeyCode::F4;
    case VK_F5:  return KeyCode::F5;
    case VK_F6:  return KeyCode::F6;
    case VK_F7:  return KeyCode::F7;
    case VK_F8:  return KeyCode::F8;
    case VK_F9:  return KeyCode::F9;
    case VK_F10: return KeyCode::F10;
    case VK_F11: return KeyCode::F11;
    case VK_F12: return KeyCode::F12;

        // ----------------------------------------------------
        // Navigation / control
        // ----------------------------------------------------
    case VK_ESCAPE: return KeyCode::Escape;
    case VK_RETURN:
        return IsExtended(lparam)
            ? KeyCode::NumpadEnter
            : KeyCode::Enter;

    case VK_TAB:       return KeyCode::Tab;
    case VK_BACK:      return KeyCode::Backspace;
    case VK_SPACE:     return KeyCode::Space;
    case VK_INSERT:    return KeyCode::Insert;
    case VK_DELETE:    return KeyCode::Delete;
    case VK_HOME:      return KeyCode::Home;
    case VK_END:       return KeyCode::End;
    case VK_PRIOR:     return KeyCode::PageUp;
    case VK_NEXT:      return KeyCode::PageDown;

    case VK_LEFT:  return KeyCode::ArrowLeft;
    case VK_RIGHT: return KeyCode::ArrowRight;
    case VK_UP:    return KeyCode::ArrowUp;
    case VK_DOWN:  return KeyCode::ArrowDown;

        // ----------------------------------------------------
        // Modifiers
        // ----------------------------------------------------
    case VK_SHIFT:
        return IsExtended(lparam)
            ? KeyCode::RightShift
            : KeyCode::LeftShift;

    case VK_CONTROL:
        return IsExtended(lparam)
            ? KeyCode::RightControl
            : KeyCode::LeftControl;

    case VK_MENU: // ALT
        return IsExtended(lparam)
            ? KeyCode::RightAlt
            : KeyCode::LeftAlt;

    case VK_LWIN: return KeyCode::LeftSuper;
    case VK_RWIN: return KeyCode::RightSuper;

        // ----------------------------------------------------
        // Numpad
        // ----------------------------------------------------
    case VK_NUMPAD0: return KeyCode::Numpad0;
    case VK_NUMPAD1: return KeyCode::Numpad1;
    case VK_NUMPAD2: return KeyCode::Numpad2;
    case VK_NUMPAD3: return KeyCode::Numpad3;
    case VK_NUMPAD4: return KeyCode::Numpad4;
    case VK_NUMPAD5: return KeyCode::Numpad5;
    case VK_NUMPAD6: return KeyCode::Numpad6;
    case VK_NUMPAD7: return KeyCode::Numpad7;
    case VK_NUMPAD8: return KeyCode::Numpad8;
    case VK_NUMPAD9: return KeyCode::Numpad9;

    case VK_ADD:      return KeyCode::NumpadAdd;
    case VK_SUBTRACT: return KeyCode::NumpadSubtract;
    case VK_MULTIPLY: return KeyCode::NumpadMultiply;
    case VK_DIVIDE:   return KeyCode::NumpadDivide;
    case VK_DECIMAL:  return KeyCode::NumpadDecimal;

        // ----------------------------------------------------
        // Locks
        // ----------------------------------------------------
    case VK_CAPITAL: return KeyCode::CapsLock;
    case VK_NUMLOCK: return KeyCode::NumLock;
    case VK_SCROLL:  return KeyCode::ScrollLock;

        // ----------------------------------------------------
        // System / special
        // ----------------------------------------------------
    case VK_SNAPSHOT: return KeyCode::PrintScreen;
    case VK_PAUSE:    return KeyCode::Pause;
    case VK_APPS:     return KeyCode::Menu;

        // ----------------------------------------------------
        // Media keys (opcional)
        // ----------------------------------------------------
    case VK_VOLUME_MUTE: return KeyCode::VolumeMute;
    case VK_VOLUME_UP:   return KeyCode::VolumeUp;
    case VK_VOLUME_DOWN: return KeyCode::VolumeDown;
    case VK_MEDIA_PLAY_PAUSE: return KeyCode::MediaPlayPause;
    case VK_MEDIA_STOP:       return KeyCode::MediaStop;
    case VK_MEDIA_NEXT_TRACK: return KeyCode::MediaNextTrack;
    case VK_MEDIA_PREV_TRACK: return KeyCode::MediaPrevTrack;

    default:
        return KeyCode::None;
    }
}