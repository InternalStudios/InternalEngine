#include "Script.h"

#include <iostream>

namespace Internal
{

	struct color_t {
		uint8_t r,g,b;
		color_t(uint32_t rgb)
			: r((rgb >> 16) & 0xff)
			, g((rgb >> 8) & 0xff)
			, b(rgb & 0xff)
		{}
		color_t(uint8_t r, uint8_t g, uint8_t b)
			: r(r), g(g), b(b) {}
	};

	void Print(MonoString* string);

	Script::Script(const char* className, const char* location)
		: m_Location(location), m_Name(className)
	{
		m_Domain = mono_jit_init(location);

		m_Assembly = mono_domain_assembly_open(m_Domain, location);
		if(!m_Assembly)
		{
			return;
		}

		mono_add_internal_call("Test.Test::cout", (const void*)&Print);

		int argc = 1;
		char* argv[1] = {(char*)"CSharp"};

		mono_jit_exec(m_Domain, m_Assembly, argc, argv);
	}

	void Print(MonoString* string)
	{
		char* CString = mono_string_to_utf8(string);
		std::cout << "\033[38;2;" << 0xF5 << ";" << 0xA9 << ";" << 0xB8 << "m" << CString;
		mono_free(CString);
	}

	void Script::OnUpdate()
	{

	}

	void Script::OnEvent(Event& e)
	{

	}

	void Script::OnTick()
	{

	}
}