#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Drawing/Font.hpp"
#include "GUI/Context/UIContext.hpp"
#include "Events/Events.hpp"
#include "Events/EventDispatcher.hpp"
#include "System/Types/Drawing/Color.hpp"
#include "System/Types/Drawing/Size.hpp"
#include "System/Types/Drawing/Point.hpp"
#include "System/Types/Drawing/Padding.hpp"
#include "System/Collections/List.hpp"
#include "System/Exceptions.hpp"
#include "HorizontalAlignment.hpp"
#include "VerticalAlignment.hpp"
#include "TextFormat.hpp"

class Window;
struct NativeBackend;
class EventQueue;
class IEventSink;

struct InitializationContext
{
	ControlBase* Parent = nullptr;
	EventQueue* Queue = nullptr;
	Font* Font = nullptr;
	UIContext* UIContext = nullptr;
	IEventSink* EventSink = nullptr;
};

class ControlBase : public Object<ControlBase>
{
public:

	void Initialize(InitializationContext& ctx);

	void SetEnabled(Boolean enabled);
	inline constexpr Boolean IsVisible() const noexcept { return GetState(Flags::Enabled); }
	void Show();
	void Hide();
	void Close();
	void Update();

	inline constexpr Boolean IsEnabled() const noexcept { return GetState(Flags::Enabled); }
	void Enable() noexcept;
	void Disable() noexcept;

	void AddControl(ControlBase* c);
	void RemoveControl(ControlBase* c);

	Boolean OnEvent(Event& e);
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
	inline constexpr ControlBase* GetParent() const noexcept { return _parent; }

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

	template<typename TEvent>
	using EventHandler = void (*)(TEvent&);

	// UI Events
	EventHandler<CloseEvent> OnClose = nullptr;
	EventHandler<DestroyEvent> OnDestroy = nullptr;
	EventHandler<ResizedEvent> OnResized = nullptr;
	EventHandler<ResizingEvent> OnResizing = nullptr;
	EventHandler<MoveEvent> OnMove = nullptr;
	EventHandler<RestoreEvent> OnRestore = nullptr;
	EventHandler<MinimizeEvent> OnMinimize = nullptr;
	EventHandler<MaximizeEvent> OnMaximize = nullptr;
	EventHandler<FocusGainedEvent> OnFocusGained = nullptr;
	EventHandler<FocusLostEvent> OnFocusLost = nullptr;
	EventHandler<ShowEvent> OnShow = nullptr;
	EventHandler<HideEvent> OnHide = nullptr;
	EventHandler<DPIChangedEvent> OnDPIChanged = nullptr;
	EventHandler<FontChangedEvent> OnFontChanged = nullptr;
	EventHandler<PaintEvent> OnPaint = nullptr;
	EventHandler<ControlAddedEvent> OnControlAdded = nullptr;
	EventHandler<ControlRemovedEvent> OnControlRemoved = nullptr;

	// Keyboard Events
	EventHandler<KeyDownEvent> OnKeyDown = nullptr;
	EventHandler<KeyUpEvent> OnKeyUp = nullptr;

	// Mouse Events
	EventHandler<MouseMoveEvent> OnMouseMove = nullptr;
	EventHandler<MouseButtonDownEvent> OnMouseDown = nullptr;
	EventHandler<MouseButtonUpEvent> OnMouseUp = nullptr;
	EventHandler<MouseScrollEvent> OnMouseScroll = nullptr;
	EventHandler<MouseEnterEvent> OnMouseEnter = nullptr;
	EventHandler<MouseLeaveEvent> OnMouseLeave = nullptr;

	// Touch Events
	EventHandler<TouchMoveEvent> OnTouchMove = nullptr;
	EventHandler<TouchDownEvent> OnTouchDown = nullptr;
	EventHandler<TouchUpEvent> OnTouchUp = nullptr;

	// Gamepad Events
	EventHandler<GamepadConnectedEvent> OnGamepadConnectedEvent = nullptr;
	EventHandler<GamepadDisconnectedEvent> OnGamepadDisconnectedEvent = nullptr;
	EventHandler<GamepadButtonDownEvent> OnGamepadButtonDownEvent = nullptr;
	EventHandler<GamepadButtonUpEvent> OnGamepadButtonUpEvent = nullptr;
	EventHandler<GamepadAxisMoveEvent> OnGamepadAxisMoveEvent = nullptr;

	// Behavioral Events
	EventHandler<TextInputEvent> OnTextInput = nullptr;
	EventHandler<ImeCompositionStartEvent> OnImeCompositionStart = nullptr;
	EventHandler<ImeCompositionUpdateEvent> OnImeCompositionUpdate = nullptr;
	EventHandler<ImeCompositionEndEvent> OnImeCompositionEnd = nullptr;
	EventHandler<ImeCompositionCommitEvent> OnImeCompositionCommit = nullptr;

	// Clipboard Events
	EventHandler<ClipboardCopyEvent> OnClipboardCopyEvent = nullptr;
	EventHandler<ClipboardPasteEvent> OnClipboardPasteEvent = nullptr;
	EventHandler<ClipboardCutEvent> OnClipboardCutEvent = nullptr;
	EventHandler<ClipboardChangedEvent> OnClipboardChangedEvent = nullptr;

	// System Events
	EventHandler<SystemQuitEvent> OnSystemQuit = nullptr;
	EventHandler<SystemShutdownEvent> OnSystemShutdown = nullptr;
	EventHandler<SystemResumePowerEvent> OnSystemResumePower = nullptr;
	EventHandler<SystemBatteryLowPowerEvent> OnSystemBatteryLowPower = nullptr;
	EventHandler<SystemUnknownPowerEvent> OnSystemUnknownPower = nullptr;
	EventHandler<SystemSuspendPowerEvent> OnSystemSuspendPower = nullptr;

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

	ControlBase() noexcept;
	ControlBase(const String& text) noexcept;
	ControlBase(const String& text, i32 left, i32 top, i32 width, i32 height) noexcept;
	ControlBase(ControlBase* parent, const String& text) noexcept;
	ControlBase(ControlBase* parent, const String& text, i32 left, i32 top, i32 width, i32 height) noexcept;
	~ControlBase();

	ControlBase(const ControlBase&) = delete;
	ControlBase& operator=(const ControlBase&) = delete;

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
	ControlBase* _parent = nullptr;
	List<ControlBase*> _controls;
	UIContext* _uiContext = nullptr;
	Padding _margin = Padding(3);
	Padding _padding = Padding::Empty;
	void (*InitializeImpl)(ControlBase*, InitializationContext&) = nullptr;

private:

	template<typename TEvent, EventHandler<TEvent> ControlBase::*MemberPtr>
	void Handle(Event& ev)
	{
		if (ev.Handle == UIHandle(this))
		{
			EventHandler<TEvent> handler = this->*MemberPtr;
			if (handler)
			{
				// Chamada correta de ponteiro para função membro
				(*handler)(static_cast<TEvent&>(ev));
			}
		}
	}

	using UIEventsTable = void (ControlBase::*)(Event&);
	static constexpr UIEventsTable UIEventHandlers[] =
	{
		&ControlBase::Handle<CloseEvent, &ControlBase::OnClose>,
		&ControlBase::Handle<DestroyEvent, &ControlBase::OnDestroy>,
		&ControlBase::Handle<ResizedEvent, &ControlBase::OnResized>,
		&ControlBase::Handle<ResizingEvent, &ControlBase::OnResizing>,
		&ControlBase::Handle<MoveEvent, &ControlBase::OnMove>,
		&ControlBase::Handle<RestoreEvent, &ControlBase::OnRestore>,
		&ControlBase::Handle<MinimizeEvent, &ControlBase::OnMinimize>,
		&ControlBase::Handle<MaximizeEvent, &ControlBase::OnMaximize>,
		&ControlBase::Handle<FocusGainedEvent, &ControlBase::OnFocusGained>,
		&ControlBase::Handle<FocusLostEvent, &ControlBase::OnFocusLost>,
		&ControlBase::Handle<ShowEvent, &ControlBase::OnShow>,
		&ControlBase::Handle<HideEvent, &ControlBase::OnHide>,
		&ControlBase::Handle<DPIChangedEvent, &ControlBase::OnDPIChanged>,
		&ControlBase::Handle<FontChangedEvent, &ControlBase::OnFontChanged>,
		&ControlBase::Handle<PaintEvent, &ControlBase::OnPaint>,
		&ControlBase::Handle<ControlAddedEvent, &ControlBase::OnControlAdded>,
		&ControlBase::Handle<ControlRemovedEvent, &ControlBase::OnControlRemoved>
	};

	using KeyboardEventsTable = void (ControlBase::*)(Event&);
	static constexpr KeyboardEventsTable KeyboardEventHandlers[] =
	{
		&ControlBase::Handle<KeyDownEvent, &ControlBase::OnKeyDown>,
		&ControlBase::Handle<KeyUpEvent, &ControlBase::OnKeyUp>,
	};

	using MouseEventsTable = void (ControlBase::*)(Event&);
	static constexpr MouseEventsTable MouseEventHandlers[] =
	{
		&ControlBase::Handle<MouseMoveEvent, &ControlBase::OnMouseMove>,
		&ControlBase::Handle<MouseButtonDownEvent, &ControlBase::OnMouseDown>,
		&ControlBase::Handle<MouseButtonUpEvent, &ControlBase::OnMouseUp>,
		&ControlBase::Handle<MouseScrollEvent, &ControlBase::OnMouseScroll>,
		&ControlBase::Handle<MouseEnterEvent, &ControlBase::OnMouseEnter>,
		&ControlBase::Handle<MouseLeaveEvent, &ControlBase::OnMouseLeave>,
	};

	using TouchEventsTable = void (ControlBase::*)(Event&);
	static constexpr TouchEventsTable TouchEventHandlers[] =
	{
		&ControlBase::Handle<TouchMoveEvent, &ControlBase::OnTouchMove>,
		&ControlBase::Handle<TouchDownEvent, &ControlBase::OnTouchDown>,
		&ControlBase::Handle<TouchUpEvent, &ControlBase::OnTouchUp>,
	};

	using GamepadEventsTable = void (ControlBase::*)(Event&);
	static constexpr GamepadEventsTable GamepadEventHandlers[] =
	{
		&ControlBase::Handle<GamepadConnectedEvent, &ControlBase::OnGamepadConnectedEvent>,
		&ControlBase::Handle<GamepadDisconnectedEvent, &ControlBase::OnGamepadDisconnectedEvent>,
		&ControlBase::Handle<GamepadButtonDownEvent, &ControlBase::OnGamepadButtonDownEvent>,
		&ControlBase::Handle<GamepadButtonUpEvent, &ControlBase::OnGamepadButtonUpEvent>,
		&ControlBase::Handle<GamepadAxisMoveEvent, &ControlBase::OnGamepadAxisMoveEvent>,
	};

	using TextEventsTable = void (ControlBase::*)(Event&);
	static constexpr TextEventsTable TextEventHandlers[] =
	{
		&ControlBase::Handle<TextInputEvent, &ControlBase::OnTextInput>,
		&ControlBase::Handle<ImeCompositionStartEvent, &ControlBase::OnImeCompositionStart>,
		&ControlBase::Handle<ImeCompositionUpdateEvent, &ControlBase::OnImeCompositionUpdate>,
		&ControlBase::Handle<ImeCompositionEndEvent, &ControlBase::OnImeCompositionEnd>,
		&ControlBase::Handle<ImeCompositionCommitEvent, &ControlBase::OnImeCompositionCommit>,
	};

	using ClipboardEventsTable = void (ControlBase::*)(Event&);
	static constexpr ClipboardEventsTable ClipboardEventHandlers[] =
	{
		&ControlBase::Handle<ClipboardCopyEvent, &ControlBase::OnClipboardCopyEvent>,
		&ControlBase::Handle<ClipboardPasteEvent, &ControlBase::OnClipboardPasteEvent>,
		&ControlBase::Handle<ClipboardCutEvent, &ControlBase::OnClipboardCutEvent>,
		&ControlBase::Handle<ClipboardChangedEvent, &ControlBase::OnClipboardChangedEvent>,
	};

	using SystemEventsTable = void (ControlBase::*)(Event&);
	static constexpr SystemEventsTable SystemEventHandlers[] =
	{
		&ControlBase::Handle<SystemQuitEvent, &ControlBase::OnSystemQuit>,
		&ControlBase::Handle<SystemShutdownEvent, &ControlBase::OnSystemShutdown>,
		&ControlBase::Handle<SystemResumePowerEvent, &ControlBase::OnSystemResumePower>,
		&ControlBase::Handle<SystemBatteryLowPowerEvent, &ControlBase::OnSystemBatteryLowPower>,
		&ControlBase::Handle<SystemUnknownPowerEvent, &ControlBase::OnSystemUnknownPower>,
		&ControlBase::Handle<SystemSuspendPowerEvent, &ControlBase::OnSystemSuspendPower>,
	};

	using HandlersTable = void (ControlBase::*)(Event&);
	struct CategoryTable
	{
		const HandlersTable* Handlers;
		u8 Count;
	};

	static constexpr CategoryTable CategoryTables[] =
	{
		{ UIEventHandlers,        CountOf(UIEventHandlers) },
		{ KeyboardEventHandlers,  CountOf(KeyboardEventHandlers) },
		{ MouseEventHandlers,     CountOf(MouseEventHandlers) },
		{ TouchEventHandlers,     CountOf(TouchEventHandlers) },
		{ GamepadEventHandlers,   CountOf(GamepadEventHandlers) },
		{ TextEventHandlers,      CountOf(TextEventHandlers) },
		{ ClipboardEventHandlers, CountOf(ClipboardEventHandlers) },
		{ SystemEventHandlers,    CountOf(SystemEventHandlers) },
	};
};

template<typename T>
class Control : public ControlBase
{
public:

	Control<T>* Type() const noexcept
	{
		return this;
	}

protected:

	Control() noexcept
	{
		InitializeImpl = [](ControlBase* base, InitializationContext& ctx)
        {
            static_cast<T*>(base)->InitializeImpl(ctx);
        };
	}
	
	Control(const String& text) noexcept 
		: 
		ControlBase(text)
	{
		InitializeImpl = [](ControlBase* base, InitializationContext& ctx)
			{
				static_cast<T*>(base)->InitializeImpl(ctx);
			};
	}
	
	Control(const String& text, i32 left, i32 top, i32 width, i32 height) noexcept
		:
		ControlBase(text, left, top, width, height)
	{
		InitializeImpl = [](ControlBase* base, InitializationContext& ctx)
			{
				static_cast<T*>(base)->InitializeImpl(ctx);
			};
	}
	
	Control(Control* parent, const String& text) noexcept
		:
		ControlBase(parent, text)
	{
		InitializeImpl = [](ControlBase* base, InitializationContext& ctx)
			{
				static_cast<T*>(base)->InitializeImpl(ctx);
			};
	}
	
	Control(Control* parent, const String& text, i32 left, i32 top, i32 width, i32 height) noexcept
		:
		ControlBase(parent, text, left, top, width, height)
	{
		InitializeImpl = [](ControlBase* base, InitializationContext& ctx)
			{
				static_cast<T*>(base)->InitializeImpl(ctx);
			};
	}
};