#pragma once

#include <encryption.hpp>
#include <memory.hpp>
#include <winapi.hpp>
#include <globals.hpp>
#include <unordered_map>
#include <map>

typedef struct _VECTORED_HANDLER_ENTRY
{
	LIST_ENTRY ExecuteHandlerList;
	union
	{
		struct
		{
			ULONG Refs;
			PVOID Handler;
		} Old;
		struct
		{
			PVOID Unknown1;
			ULONG Unknown2;
			PVOID Handler;
		} New;
	};
} VECTORED_HANDLER_ENTRY, * PVECTORED_HANDLER_ENTRY;

typedef struct _VECTORED_HANDLER_LIST
{
	SRWLOCK SrwLock;
	LIST_ENTRY ExecuteHandlerList;
} VECTORED_HANDLER_LIST, * PVECTORED_HANDLER_LIST;

inline void* fwAlloc(size_t size)
{
	return malloc(size);
}

inline void fwFree(void* p)
{
	free(p);
}

typedef std::string fwString;
typedef std::wstring fwWString;

template<class TValue>
using fwVector = std::vector<TValue>;

template<class TValue>
using fwList = std::list<TValue>;

template<class TKey, class TValue>
using fwHashMap = std::unordered_map<TKey, TValue>;

template<class TKey, class TValue>
using fwMap = std::map<TKey, TValue>;

#include <functional>

template<class T>
class fwRefContainer
{
private:
	T* m_ref;

public:
	fwRefContainer()
		: m_ref(nullptr)
	{

	}

	fwRefContainer(T* ref)
		: m_ref(ref)
	{
		if (m_ref)
		{
			m_ref->AddRef();
		}
	}

	~fwRefContainer()
	{
		if (m_ref)
		{
			if (m_ref->Release())
			{
				m_ref = nullptr;
			}
		}
	}

	fwRefContainer(const fwRefContainer& rc)
	{
		m_ref = rc.m_ref;

		if (m_ref)
		{
			m_ref->AddRef();
		}
	}

	template<typename TOther>
	fwRefContainer(const fwRefContainer<TOther>& rc)
	{
		m_ref = static_cast<T*>(rc.GetRef());

		if (m_ref)
		{
			m_ref->AddRef();
		}
	}

	uint32_t GetRefCount() const
	{
		return m_ref->GetRefCount();
	}

	T* GetRef() const
	{
		return m_ref;
	}

	T* operator->() const
	{
		return m_ref;
	}

	fwRefContainer& operator=(const fwRefContainer& other)
	{
		if (m_ref)
		{
			m_ref->Release();
		}

		m_ref = other.GetRef();

		if (m_ref)
		{
			m_ref->AddRef();
		}

		return *this;
	}

	template<class TOther>
	fwRefContainer& operator=(const fwRefContainer<TOther>& other)
	{
		if (m_ref)
		{
			m_ref->Release();
		}

		m_ref = other.GetRef();

		if (m_ref)
		{
			m_ref->AddRef();
		}

		return *this;
	}

	fwRefContainer& operator=(T* ref)
	{
		if (m_ref)
		{
			m_ref->Release();
		}

		m_ref = ref;

		if (m_ref)
		{
			m_ref->AddRef();
		}

		return *this;
	}
};

template<typename T>
bool operator<(const fwRefContainer<T>& left, const fwRefContainer<T>& right)
{
	return (left.GetRef() < right.GetRef());
}

class fwRefCountable
{
private:
	class RefCount
	{
	private:
		std::atomic<uint32_t> m_count;

	public:
		RefCount()
			: m_count(0)
		{
		}

		inline std::atomic<uint32_t>& GetCount()
		{
			return m_count;
		}
	};

	RefCount m_refCount;

public:
	inline uint32_t GetRefCount()
	{
		return m_refCount.GetCount();
	}

	virtual ~fwRefCountable();

	virtual void AddRef();

	virtual bool Release();
};

template<typename... Args>
class fwActionImpl : public fwRefCountable
{
private:
	std::function<void(Args&...)> m_func;

public:
	fwActionImpl(const std::function<void(Args...)>& stlFunc)
	{
		m_func = stlFunc;
	}

	virtual void Invoke(Args&... args)
	{
		m_func(args...);
	}
};

template<typename... Args>
class fwAction : public fwRefContainer<fwActionImpl<Args...>>
{
public:
	fwAction()
		: fwRefContainer<fwActionImpl<Args...>>()
	{

	}

	fwAction(const std::function<void(Args...)>& stlFunc)
		: fwRefContainer<fwActionImpl<Args...>>(new fwActionImpl<Args...>(stlFunc))
	{

	}

	template<typename Fx>
	fwAction(const Fx& func)
		: fwRefContainer<fwActionImpl<Args...>>(new fwActionImpl<Args...>(std::function<void(Args...)>(func)))
	{

	}

	void operator()(Args... args)
	{
		this->GetRef()->Invoke(args...);
	}
};

template<typename... Args>
class fwEvent
{
public:
	using TFunc = std::function<bool(Args...)>;

private:
	struct callback
	{
		TFunc function;
		std::unique_ptr<callback> next = nullptr;
		int order = 0;
		size_t cookie = -1;

		callback(TFunc func)
			: function(func)
		{

		}
	};

	std::unique_ptr<callback> m_callbacks;
	std::atomic<size_t> m_connectCookie = 0;

public:
	fwEvent()
	{
		m_callbacks = nullptr;
	}

	~fwEvent()
	{
		Reset();
	}

	template<typename T>
	auto Connect(T func)
	{
		return Connect(func, 0);
	}

	template<typename T>
	auto Connect(T func, int order)
	{
		if constexpr (std::is_same_v<std::invoke_result_t<T, Args...>, bool>)
		{
			return ConnectInternal(func, order);
		}
		else
		{
			return ConnectInternal([func](Args&&... args)
				{
					std::invoke(func, args...);
					return true;
				},
				order);
		}
	}

	void Reset()
	{
		m_callbacks.reset();
	}

private:
	size_t ConnectInternal(TFunc func, int order)
	{
		if (!func)
		{
			return -1;
		}

		auto cookie = m_connectCookie++;
		auto cb = std::unique_ptr<callback>(new callback(func));
		cb->order = order;
		cb->cookie = cookie;

		if (!m_callbacks)
		{
			m_callbacks = std::move(cb);
		}
		else
		{
			auto cur = &m_callbacks;
			callback* last = nullptr;

			while (*cur && order >= (*cur)->order)
			{
				last = cur->get();
				cur = &(*cur)->next;
			}

			cb->next = std::move(*cur);
			(!last ? m_callbacks : last->next) = std::move(cb);
		}

		return cookie;
	}

public:
	void Disconnect(size_t cookie)
	{
		if (cookie == -1)
		{
			return;
		}

		callback* prev = nullptr;

		for (auto cb = m_callbacks.get(); cb; cb = cb->next.get())
		{
			if (cb->cookie == cookie)
			{
				if (prev)
				{
					prev->next = std::move(cb->next);
				}
				else
				{
					m_callbacks = std::move(cb->next);
				}

				break;
			}

			prev = cb;
		}
	}

	auto ConnectInternal(TFunc func)
	{
		return ConnectInternal(func, 0);
	}

public:
	operator bool() const
	{
		return m_callbacks.get() != nullptr;
	}

	bool operator()(Args... args) const
	{
		if (!m_callbacks)
		{
			return true;
		}

		decltype(m_callbacks.get()) next = {};

		for (auto cb = m_callbacks.get(); cb; cb = next)
		{
			next = cb->next.get();

			if (!std::invoke(cb->function, args...))
			{
				return false;
			}
		}

		return true;
	}
};

namespace offsets
{
	inline std::uint64_t citizenCore;
	inline std::uint64_t g_globalManager;
	inline std::uint64_t Resource_Constructor;
	inline std::uint64_t ResourceImpl_vtbl;
	inline std::uint64_t ResourceImpl_ComponentHolder;
	// 48 8B 05 ? ? ? ? 49 8D 1C C0
	inline std::uint64_t ResourceMetaDataComponent = 0xAA420;
	// 48 8B 05 ? ? ? ? 48 83 C4 ? C3 CC 48 89 4C 24 ? 57 48 83 EC ? 48 C7 44 24 ? ? ? ? ? 48 89 5C 24 ? 48 8B F9
	inline std::uint64_t ResourceEventComponent = 0x9A3E0;
}

namespace fx
{
	class ResourceImpl;

	enum class ResourceState
	{
		Uninitialized,
		Stopped,
		Starting,
		Started,
		Stopping
	};

	class ResourceEventComponent;
	class ResourceImpl
	{
	public:
		union
		{
			DEFINE_MEMBER_N(fwEvent<>, OnStart, 0x50);
			DEFINE_MEMBER_N(fwEvent<>, OnStop, 0x60);
			DEFINE_MEMBER_N(std::string, m_name, 0xD0);
			DEFINE_MEMBER_N(std::string, m_rootPath, 0xF0);
			DEFINE_MEMBER_N(void*, m_manager, 0x110);
			DEFINE_MEMBER_N(ResourceState, m_state, 0x118);
		};

		static fwEvent<ResourceImpl*> OnInitializeInstance;
		bool start();
		bool stop();
		bool load_from(const std::string& rootPath);
		std::string state_to_str();
		ResourceEventComponent* get_event_comp();
	};

	using m_resources_t = std::unordered_map<std::string, ResourceImpl*>;
	class ResourceManagerImpl
	{
	private:
		union
		{
			DEFINE_MEMBER_N(m_resources_t, m_resources, 0xA0);
		};

	public:
		std::vector<ResourceImpl*> get_resources()
		{
			std::vector<ResourceImpl*> resources;
			for (const auto& it : m_resources)
				if (!it.first.empty() && it.second)
					resources.push_back(it.second);
			return resources;
		}

		void for_all_resources(std::function<void(ResourceImpl*)> callback);
		void for_all_resources(std::function<void(ResourceImpl*, std::uint32_t)> callback);
		ResourceImpl* get_resource(const std::string& resourceName);
		ResourceImpl* create_resource(const std::string& resourceName);
		bool execute_code(std::string code);
		int get_num_instances();
		void clear_instances();

		static inline ResourceManagerImpl* get_impl()
		{
			return *(ResourceManagerImpl**)(offsets::g_globalManager);
		}
	};

	class LuaMetaDataLoader // : public ResourceMetaDataLoader
	{
	public:
		union
		{
			DEFINE_MEMBER_N(void*, m_luaState, 0x10);
			DEFINE_MEMBER_N(void*, m_component, 0x40);
		};
	};

	class ResourceMetaDataComponent
	{
	public:
		union { DEFINE_MEMBER_N(LuaMetaDataLoader*, m_metaDataLoader, 0x28); };
	};

	typedef fwEvent<const std::string&, const std::string&, const std::string&, bool*> on_trigger_event;
	class ResourceEventComponent
	{
	public:
		union { DEFINE_MEMBER_N(on_trigger_event, OnTriggerEvent, 0x28); };
	};
}
