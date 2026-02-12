#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Drawing/Font.hpp"
#include "Events/Event.hpp"
#include "System/Types/Drawing/Color.hpp"
#include "System/Types/Drawing/Size.hpp"
#include "System/Types/Drawing/Point.hpp"
#include "System/Types/Drawing/Padding.hpp"
#include "System/Collections/List.hpp"
#include "HorizontalAlignment.hpp"
#include "VerticalAlignment.hpp"
#include "TextFormat.hpp"

class Window;
struct NativeBackend;
class EventQueue;
class UIContext;
class IEventSink;

// Behavioral Events
class TextInputEvent;
class ImeCompositionEvent;

// Keyboard Events
class KeyDownEvent;
class KeyUpEvent;

// Mouse Events
class MouseMoveEvent;
class MouseButtonDownEvent;
class MouseButtonUpEvent;
class MouseScrollEvent;
class MouseEnterEvent;
class MouseLeaveEvent;

// Events
class SystemQuitEvent;
class SystemShutdownEvent;
class SystemResumePowerEvent;
class SystemBatteryLowPowerEvent;
class SystemUnknownPowerEvent;
class SystemSuspendPowerEvent;

// UI Events
class CloseEvent;
class DestroyEvent;
class ResizedEvent;
class ResizingEvent;
class MoveEvent;
class RestoreEvent;
class MinimizeEvent;
class MaximizeEvent;
class FocusGainedEvent;
class FocusLostEvent;
class ShowEvent;
class HideEvent;
class DPIChangedEvent;
class FontChangedEvent;
class PaintEvent;
class ControlAddedEvent;
class ControlRemovedEvent;

class Control : public Object<Control>
{
public:

	struct InitializationContext
	{
		Control* Parent = nullptr;
		EventQueue* Queue = nullptr;
		Font* Font = nullptr;
		UIContext* UIContext = nullptr;
		IEventSink* EventSink = nullptr;
	};

	virtual ~Control();

	Control(const Control&) = delete;
	Control& operator=(const Control&) = delete;

	void SetEnabled(Boolean enabled);
	inline constexpr Boolean IsVisible() const noexcept { return GetState(Flags::Enabled); }
	void Show();
	void Hide();
	void Close();
	void Update();

	inline constexpr Boolean IsEnabled() const noexcept { return GetState(Flags::Enabled); }
	void Enable() noexcept;
	void Disable() noexcept;

	void AddControl(Control* c);
	void RemoveControl(Control* c);
	virtual void Initialize(InitializationContext ctx) = 0;

	virtual Boolean OnEvent(Event& e);
	inline String GetText() const noexcept { return _text; }
	inline constexpr Point GetLocation() const noexcept { return Point(_x, _y); }
	inline constexpr i32 GetX() const noexcept { return _x; }
	void SetX(i32 x) noexcept;
	inline constexpr i32 GetY() const noexcept { return _y; }
	void SetY(i32 y) noexcept;
	inline constexpr void SetLocation(Point location) noexcept { _x = location.GetX(); _y = location.GetY(); }
	inline constexpr Size GetSize() const noexcept { return Size(_width, _height); }
	inline constexpr i32 GetWidth() const noexcept { return _width; }
	void SetWidth(i32 width) noexcept;
	inline constexpr i32 GetHeight() const noexcept { return _height; }
	void SetHeight(i32 height) noexcept;
	void SetSize(i32 x, i32 y) noexcept;
	void SetSize(Size s) noexcept;

	inline constexpr Boolean HasParent() const noexcept { return GetParent() != nullptr ? true : false; }
	inline constexpr Control* GetParent() const noexcept { return _parent; }

	inline constexpr Font* GetFont() const noexcept { return _font; }
	void SetFont(Font* font) noexcept;

	inline constexpr Color GetBackgroundColor() const noexcept { return _backgroundColor; }
	inline constexpr Color GetForeColor() const noexcept { return _foreColor; }

	inline constexpr void SetBackgroundColor(Color c) noexcept { _backgroundColor = c; }
	inline constexpr void SetForeColor(Color c) noexcept { _foreColor = c; }

	inline constexpr HorizontalAlignment GetHorizontalAlignment() const noexcept { return _hAlignment; }
	inline constexpr VerticalAlignment GetVerticalAlignment() const noexcept { return _vAlignment; }
	inline constexpr TextFormat GetTextFormat() const noexcept { return _textFormat; }
	inline constexpr void SetHorizontalAlignment(HorizontalAlignment ha) { _hAlignment = ha; }
	inline constexpr void SetVerticalAlignment(VerticalAlignment va) { _vAlignment = va; }
	inline constexpr void SetTextFormat(TextFormat tf) { _textFormat = tf; }

	inline constexpr Boolean IsAutoSizeEnabled() const noexcept { return GetState(Flags::AutoSize); }
	inline constexpr void EnableAutoSize() noexcept { SetState(Flags::AutoSize, true); }
	inline constexpr void DisableAutoSize() noexcept { SetState(Flags::AutoSize, false); }
	Boolean IsHandledCreated() const noexcept;
	Window* FindWindow() noexcept;

	inline constexpr Boolean IsResizable() const noexcept { return GetState(Flags::Resizable) != 0; }
	void EnableResize();
	void DisableResize();

	// Behavioral Events
	void (*OnTextInput)(TextInputEvent*) = nullptr;
	void (*OnImeComposition)(ImeCompositionEvent*) = nullptr;

	// Keyboard Events
	void (*OnKeyDown)(KeyDownEvent*) = nullptr;
	void (*OnKeyUp)(KeyUpEvent*) = nullptr;

	// Mouse Events
	void (*OnMouseMove)(MouseMoveEvent*) = nullptr;
	void (*OnMouseDown)(MouseButtonDownEvent*) = nullptr;
	void (*OnMouseUp)(MouseButtonUpEvent*) = nullptr;
	void (*OnMouseScroll)(MouseScrollEvent*) = nullptr;
	void (*OnMouseEnter)(MouseEnterEvent*) = nullptr;
	void (*OnMouseLeave)(MouseLeaveEvent*) = nullptr;

	// System Events
	void (*OnSystemQuit)(SystemQuitEvent*) = nullptr;
	void (*OnSystemShutdown)(SystemShutdownEvent*) = nullptr;
	void (*OnSystemResumePower)(SystemResumePowerEvent*) = nullptr;
	void (*OnSystemBatteryLowPower)(SystemBatteryLowPowerEvent*) = nullptr;
	void (*OnSystemUnknownPower)(SystemUnknownPowerEvent*) = nullptr;
	void (*OnSystemSuspendPower)(SystemSuspendPowerEvent*) = nullptr;

	// UI Events
	void (*OnClose)(CloseEvent*) = nullptr;
	void (*OnDestroy)(DestroyEvent*) = nullptr;
	void (*OnResized)(ResizedEvent*) = nullptr;
	void (*OnResizing)(ResizingEvent*) = nullptr;
	void (*OnMove)(MoveEvent*) = nullptr;
	void (*OnRestore)(RestoreEvent*) = nullptr;
	void (*OnMinimize)(MinimizeEvent*) = nullptr;
	void (*OnMaximize)(MaximizeEvent*) = nullptr;
	void (*OnFocusGained)(FocusGainedEvent*) = nullptr;
	void (*OnFocusLost)(FocusLostEvent*) = nullptr;
	void (*OnShow)(ShowEvent*) = nullptr;
	void (*OnHide)(HideEvent*) = nullptr;
	void (*OnDPIChanged)(DPIChangedEvent*) = nullptr;
	void (*OnFontChanged)(FontChangedEvent*) = nullptr;
	void (*OnPaint)(PaintEvent*) = nullptr;
	void (*OnControlAdded)(ControlAddedEvent*) = nullptr;
	void (*OnControlRemoved)(ControlRemovedEvent*) = nullptr;

protected:

	enum class Flags
	{
		None = 0x00000000,
		Created = 0x00000001,
		Visible = 0x00000002,
		Enabled = 0x00000004,
		TabStop = 0x00000008,
		AutoSize = 0x00000010,
		BackendDestroyed = 0x00000020,
		Resizable = 0x00000040
	};
	
	friend inline constexpr Flags operator&(const Flags& a, const Flags& b)
	{
		return static_cast<Flags>(static_cast<int32_t>(a) & static_cast<int32_t>(b));
	}

	friend inline constexpr Flags operator|(const Flags& a, const Flags& b)
	{
		return static_cast<Flags>(static_cast<int32_t>(a) | static_cast<int32_t>(b));
	}

	friend inline constexpr Flags operator~(const Flags& a)
	{
		return static_cast<Flags>(~static_cast<int32_t>(a));
	}

	inline constexpr Boolean GetState(Flags flag) const noexcept { return (_state & flag) != Flags::None; }
	inline constexpr void SetState(Flags flag, Boolean value) noexcept { _state = value ? _state | flag : _state & ~flag; }

	Control() noexcept;
	Control(const String& text) noexcept;
	Control(const String& text, i32 left, i32 top, i32 width, i32 height) noexcept;
	Control(Control* parent, const String& text) noexcept;
	Control(Control* parent, const String& text, i32 left, i32 top, i32 width, i32 height) noexcept;

	void RedrawWithSize(Size s) noexcept;
	NativeBackend* GetParentBackend() const noexcept { return _parent ? _parent->_impl : nullptr; }
	
	NativeBackend* _impl = nullptr;
	i32 _width = 0;
	i32 _height = 0;
	Color _backgroundColor = Color::White;
	Color _foreColor = Color::Black;
	String _text;
	Font* _font = nullptr;
	i32 _x = 0;
	i32 _y = 0;
	Flags _state = Flags::AutoSize | Flags::Visible | Flags::Enabled;
	HorizontalAlignment _hAlignment = HorizontalAlignment::Left;
	VerticalAlignment _vAlignment = VerticalAlignment::Top;
	TextFormat _textFormat = TextFormat::None;
	Control* _parent = nullptr;
	UIContext* _uiContext = nullptr;
	List<Control*> _controls;
	Padding _margin = Padding(3);
	Padding _padding = Padding::Empty;
};