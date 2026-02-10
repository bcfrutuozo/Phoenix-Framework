#pragma once

#include "System/Collections/Dictionary.hpp"
#include "System/Types/Fundamentals/Pointer.hpp"
#include "GUI/Drawing/Font.hpp"
#include "System/Exceptions.hpp"

class FontManager : public Object<FontManager>
{
public:

	FontManager()
		:
		_cache()
	{
	}

	void Initialize(Font* fi, Boolean shouldCache = true) noexcept
	{
		if (IsFontCached(fi))
			return;

		create_native_object(fi, shouldCache);
	}

	Pointer GetNativeObject(Font* fi) noexcept
	{
		auto ptr = _cache.Find(fi);
		if (ptr != nullptr) return *ptr;

		return create_native_object(fi, true);
	}

	Boolean IsFontCached(Font* fi) noexcept
	{
		auto ptr = _cache.Find(fi);
		if (ptr != nullptr) return true;

		return false;
	}

	void Shutdown() noexcept
	{
		for (const auto& [key, val] : _cache)
			destroy_native_object(val);

		_cache.Clear();
	}

private:

    Pointer create_native_object(Font* instance, Boolean shouldCache) noexcept;
	void destroy_native_object(Pointer ptr) noexcept;

    Map<Font*, Pointer> _cache;
};