#include "sdk.hpp"

fwEvent<fx::ResourceImpl*> fx::ResourceImpl::OnInitializeInstance = {};
std::vector<fx::ResourceImpl*> ResourceInstances;

namespace fx
{
	bool ResourceImpl::start()
	{
		int v2; // eax
		__int64 v3; // rax
		__int64 v4; // rcx
		__int64 v5; // rbx
		__int64 v6; // rax
		__int64 v7; // rcx
		__int64 v8; // rbx
		__int64 a1 = (__int64)this;

		v2 = *(DWORD*)(a1 + 280);
		if (v2 == 3)
		{
		LABEL_11:
			*(DWORD*)(a1 + 280) = 3;
			return 1;
		}
		if (v2 == 2 || (*(DWORD*)(a1 + 280) = 2, (v3 = *(unsigned __int64*)(a1 + 48)) == 0))
		{
		LABEL_7:
			v6 = *(unsigned __int64*)(a1 + 80);
			if (v6)
			{
				while (1)
				{
					v7 = *(unsigned __int64*)(v6 + 56);
					v8 = *(unsigned __int64*)(v6 + 64);
					if (!v7)
						break;

					if (!(*(unsigned __int8(__fastcall**)(__int64))(*(unsigned __int64*)v7 + 16i64))(v7))
						goto LABEL_12;
					v6 = v8;
					if (!v8)
						goto LABEL_11;
				}
			LABEL_15:
				return 0;
			}
			goto LABEL_11;
		}
		while (1)
		{
			v4 = *(unsigned __int64*)(v3 + 56);
			v5 = *(unsigned __int64*)(v3 + 64);
			if (!v4)
				goto LABEL_15;

			unsigned __int64 temp = *(unsigned __int64*)(*(unsigned __int64*)v4 + 16i64);


			if (*(uint8_t*)temp == 0xF1 || *(uint8_t*)temp == 0xE9 ||
				*(uint8_t*)((*(unsigned __int64*)v4 + 16i64)) == 0xF1 || *(uint8_t*)((*(unsigned __int64*)v4 + 16i64)) == 0xE9)
			{
				memory::popup(xorstr_("Couldn't start resource. Please contact support team (#2)."));
				return false;
			}

			if (!(*(unsigned __int8(__fastcall**)(__int64))(*(unsigned __int64*)v4 + 16i64))(v4))
				break;
			v3 = v5;
			if (!v5)
				goto LABEL_7;
		}
	LABEL_12:
		if (*(DWORD*)(a1 + 280) != 3)
			*(DWORD*)(a1 + 280) = 1;
		return 0;

		return true;
	}

	bool ResourceImpl::stop()
	{
		__int64 OnStop; 
		__int64 v3; 
		__int64 v4; 
		__int64 a1 = (__int64)this;

		if (*(DWORD*)(a1 + 0x118) == 1) return 1;
		OnStop = *(unsigned __int64*)(a1 + 0x60);
		if (!OnStop)
		{
		LABEL_6:
			*(DWORD*)(a1 + 0x118) = 1;
			return 1;
		}
		while (1)
		{
			v3 = *(unsigned __int64*)(OnStop + 0x38);
			v4 = *(unsigned __int64*)(OnStop + 0x40);
			if (!v3) return 0;
			
			unsigned __int64 temp = *(unsigned __int64*)(*(unsigned __int64*)v3 + 0x10i64);
			if (!(*(unsigned __int8(__fastcall**)(__int64))(*(unsigned __int64*)v3 + 0x10i64))(v3))
				return 0;
			OnStop = v4;
			if (!v4)
				goto LABEL_6;
		}

		return 1;
	}

	bool ResourceImpl::load_from(const std::string& resourcePath)
	{
		std::uint64_t offset = *(std::uint64_t*)(offsets::citizenCore + offsets::ResourceMetaDataComponent);

		__int64 a1 = (__int64)this;
		__int64* ResourceMetaDataComponentPtr = (__int64*)(*(unsigned __int64*)(*(unsigned __int64*)(**(__int64(__fastcall***)(__int64))(a1 + 0x10))(a1 + 0x10)+ 0x10) + 8 * offset);
		if (!ResourceMetaDataComponentPtr) return false;

		fx::ResourceMetaDataComponent* component = (fx::ResourceMetaDataComponent*)(*ResourceMetaDataComponentPtr);
		if (!component) return false;

		BYTE* (__fastcall * LoadMetaData)(void*, BYTE*, void*, void**);
		LoadMetaData = (decltype(LoadMetaData))(memory::pattern(xorstr_("citizen-resources-metadata-lua.dll"), xorstr_("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 C7 44 24")).get());
		if (!LoadMetaData) return false;

		BYTE opcode = *reinterpret_cast<BYTE*>(LoadMetaData);
		if (opcode == 0xF1 || opcode == 0xE9) return false;

		BYTE retval[512];
		LoadMetaData(component->m_metaDataLoader, retval, component, (void**)&resourcePath);
		return true;
	}

	std::string ResourceImpl::state_to_str()
	{
		switch (m_state)
		{
		case ResourceState::Started:
			return xorstr_("[STARTED]");
			break;
		case ResourceState::Starting:
			return xorstr_("[STARTING]");
			break;
		case ResourceState::Stopped:
			return xorstr_("[STOPPED]");
			break;
		case ResourceState::Stopping:
			return xorstr_("[STOPPING]");
			break;

		default:
			return xorstr_("[UNKNOWN]");
		}
	}

	ResourceEventComponent* ResourceImpl::get_event_comp()
	{
		uint64_t a1 = (uint64_t)this;
		uint64_t rcx = *(uint64_t*)(offsets::citizenCore + offsets::ResourceEventComponent);
		uint64_t rax = (*(__int64(__fastcall**)(__int64, const char*))(*(unsigned __int64*)rcx + 0x8))(rcx, xorstr_("fx::ResourceEventComponent"));
		uint64_t rbx = (*(uint64_t*)(*(unsigned __int64*)(*(unsigned __int64*)(**(__int64(__fastcall***)(__int64))(a1 + 0x10))(a1 + 0x10) + 0x10) + 0x8 * rax));

		return (ResourceEventComponent*)rbx;
	}

	void ResourceManagerImpl::for_all_resources(std::function<void(ResourceImpl*)> callback)
	{
		for (ResourceImpl* resource : get_resources())
			callback(resource);
	}

	void ResourceManagerImpl::for_all_resources(std::function<void(ResourceImpl*, std::uint32_t)> callback)
	{
		std::vector<ResourceImpl*> resources = get_resources();
		for (std::uint32_t i = 0; i < resources.size(); i++)
			callback(resources[i], i);
	}

	ResourceImpl* ResourceManagerImpl::get_resource(const std::string& resourceName)
	{
		ResourceImpl* resource = nullptr;

		for (ResourceImpl* it : get_resources())
			if (it->m_name == resourceName)
			{
				resource = it;
				break;
			}

		return resource;
	}

	ResourceImpl* ResourceManagerImpl::create_resource(const std::string& resourceName)
	{
		// fx::Resource::Resource()
		__int64(__fastcall * sub_157D0)(__int64);
		sub_157D0 = (decltype(sub_157D0))offsets::Resource_Constructor;

		/*
		auto sub_157D0 = [&](__int64 a1)
		{
			unsigned __int64* v2; // r14
			unsigned __int64* v3; // rsi
			unsigned __int64* v4; // rax
			unsigned __int64* v5; // rbx
			char* v6; // rax
			WORD* v7; // rdx
			WORD* v8; // rax
			__int64 v9; // rcx
			__int64(__fastcall * **v10)(unsigned __int64); // rax

			unsigned __int64 addr1 = (unsigned __int64)(base + 0x65AB0); // &fwRefCountable::`vftable'
			unsigned __int64 addr2 = (unsigned __int64)(base + 0x66348); // &fx::ComponentHolderImpl<fx::Resource>::`vftable';
			unsigned __int64 addr3 = (unsigned __int64)(base + 0x66578); // &InstanceRegistryBase<fwRefContainer<fwRefCountable>>::`vftable';
			unsigned __int64 addr4 = (unsigned __int64)(base + 0x66358); // &fx::Resource::`vftable'
			unsigned __int64 addr5 = (unsigned __int64)(base + 0x663C0); // &fx::Resource::`vftable'{for `fx::ComponentHolderImpl < fx::Resource>'};
			unsigned __int64 addr6 = *(unsigned __int64*)(base + 0xAA418); // qword_AA418

			*(unsigned __int64*)a1 = addr1;
				* (unsigned long*)(a1 + 8) = 0;
			v2 = (unsigned __int64*)(a1 + 0x10);
			*(unsigned __int64*)(a1 + 0x10) = addr2;
				v3 = (unsigned __int64*)(a1 + 0x18);
			*(unsigned __int64*)(a1 + 0x18) = 0i64;
			v4 = (unsigned __int64*)malloc(0x28ui64);
			v5 = v4;
			if (v4)
			{
				*((unsigned long*)v4 + 2) = 0;
				*v4 = addr3;
				v4[2] = 0i64;
				v4[3] = 0i64;
				v4[4] = 0i64;
				v6 = (char*)malloc(0x400ui64);
				v5[2] = (unsigned long long)v6;
				v5[4] = (unsigned long long)(v6 + 0x400);
				v7 = (WORD*)v5[2];
				v8 = v7;
				v9 = 8i64;
				do
				{
					*v8 = 0i64;
					v8[1] = 0i64;
					v8[2] = 0i64;
					v8[3] = 0i64;
					v8[4] = 0i64;
					v8[5] = 0i64;
					v8[6] = 0i64;
					v8 += 8;
					v8[0xFFFFFFFF] = 0i64;
					--v9;
				} while (v9);
				v5[3] = (unsigned long long)(v7 + 0x40);
				v10 = (__int64(__fastcall***)(unsigned __int64))addr6;
			}
			else
			{
				v5 = 0i64;
			}
			if (*v3)
				(*(void(__fastcall**)(unsigned __int64))(*(unsigned __int64*)*v3 + 0x10i64))(*v3);
			*v3 = (unsigned long long)v5;
			if (v5)
				(*(void(__fastcall**)(unsigned __int64*))(*v5 + 8i64))(v5);
			*(unsigned __int64*)a1 = addr4;
			*v2 = addr5;
			return a1;
		};
		*/
		const auto sub_9A40 = [](unsigned __int64* a1, unsigned __int64* a2)
		{
			const void* v2; // r14
			unsigned __int64 v4; // rsi
			__int64 v5; // rdi
			unsigned __int64* v6; // rax

			v2 = a2;
			*a1 = 0i64;
			a1[2] = 0i64;
			a1[3] = 0i64;
			v4 = a2[2];
			if (a2[3] >= 0x10)
				v2 = (const void*)*a2;
			v5 = 0x7FFFFFFFFFFFFFFFi64;
			a1[3] = 0xFi64;
			if (v4 >= 0x10)
			{
				if ((v4 | 0xF) <= 0x7FFFFFFFFFFFFFFFi64)
				{
					v5 = v4 | 0xF;
					if ((v4 | 0xF) < 0x16)
						v5 = 0x16i64;
				}
				v6 = (unsigned long long*)malloc(v5 + 1);// malloc wrapper
															// 
				*a1 = (unsigned long long)v6;
				a1[2] = v4;
				a1[3] = v5;
				memory::_memcpy(v6, v2, v4 + 1);
			}
			else
			{
				a1[2] = v4;
				memory::_memmove(a1, v2, 0x10ui64);
			}
			return a1;
		};

		__int64 a1 = (__int64)malloc(0x120);
		__int64 a2 = (__int64)&resourceName;
		__int64 a3 = (__int64)this;

		sub_157D0(a1);

		*(unsigned __int64*)(a1 + 0x0) = offsets::ResourceImpl_vtbl;
		*(unsigned __int64*)(a1 + 0x10) = offsets::ResourceImpl_ComponentHolder;

	    sub_9A40((unsigned __int64*)(a1 + 0xD0), (unsigned __int64*)a2);

		*(unsigned __int64*)(a1 + 0xF0) = 0;
		*(unsigned __int64*)(a1 + 0x100) = 0;
		*(unsigned __int64*)(a1 + 0x108) = 0xF;
		*(unsigned char*)(a1 + 0xF0) = 0;
		*(unsigned __int64*)(a1 + 0x110) = a3;
		*(unsigned long*)(a1 + 0x118) = 0;

		std::uint64_t rax = *(std::uint64_t*)(*(std::uint64_t*)&fx::ResourceImpl::OnInitializeInstance);

		while (rax)
		{
			__int64 allocation_pointer = a1;
			unsigned __int64 v3 = *(unsigned __int64*)(rax + 0x38);
			if (!v3) break;

			if (!(*(unsigned __int8(__fastcall**)(__int64, __int64*))(*(unsigned __int64*)v3 + 0x10i64))(v3, &allocation_pointer))
				break;

			rax = *(unsigned __int64*)(rax + 0x40);
		}

		return (fx::ResourceImpl*)a1;
	}

	bool ResourceManagerImpl::execute_code(std::string code)
	{
		if (code.empty()) return false;

		const auto random_string = [](std::uint32_t length)->std::string
		{
			std::string characters = xorstr_("abcdeghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
			std::string result{};
			srand((unsigned)time(0));
			for (int i = 0; i < length; i++)
				result += characters[rand() % characters.length()];
			return result;
		};

		ResourceImpl* resource = create_resource(xorstr_("thebakeddog"));
		if (!resource) return false;

		bool found = false;
		for (auto it : ResourceInstances)
			if (it == resource)
			{
				found = true;
				break;
			}

		if (!found)
			ResourceInstances.push_back(resource);

		resource->m_state = ResourceState::Uninitialized;

		std::string path = xorstr_("C:\\ProgramData\\") + random_string(8);

		if (path.empty()) return false;

		std::filesystem::create_directory(path);

		// create file with our code inside of it
		std::ofstream lua_file(std::string(path) + xorstr_("\\client.lua"));
		lua_file << code;
		lua_file.close();

		// create metadata
		std::ofstream metadata_file(std::string(path) + xorstr_("\\__resource.lua"));
		metadata_file << xorstr_("fx_version 'cerulean'") << '\n';
		metadata_file << xorstr_("games { 'gta5' }") << '\n';
		metadata_file << xorstr_("client_script 'client.lua'") << '\n';
		metadata_file.close();

		if (!resource->load_from(path)) return false;

		resource->m_rootPath = path;
		resource->m_state = ResourceState::Stopped;

		resource->start();

		if (std::filesystem::exists(path))
			std::filesystem::remove_all(path);

		// ((char(__fastcall*)(ResourceImpl*))(offsets::citizenCore + 0x16950))(resource);
		return true;
	}

	int ResourceManagerImpl::get_num_instances()
	{
		return (int)ResourceInstances.size();
	}

	void ResourceManagerImpl::clear_instances()
	{
		for (auto& it : ResourceInstances)
		{
			if (!it)
				continue;

			it->stop();
			it->m_state = ResourceState::Uninitialized;
		}

		ResourceInstances.clear();
	}
}
