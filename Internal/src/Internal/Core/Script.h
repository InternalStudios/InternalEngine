#pragma once

#include "Module.h"
#include "Internal/Events/Event.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <stdlib.h>

#ifndef FALSE
#define FALSE 0
#endif

namespace Internal
{
	class Script : public Module
	{
	public:
		Script(const char* className, const char* location);
		void OnUpdate() override;
		void OnEvent(Event& e) override;
		void OnTick() override;
	private:
		MonoDomain* m_Domain;
		MonoAssembly* m_Assembly;
		const char* m_Location;
		const char* m_Name;
	};
}