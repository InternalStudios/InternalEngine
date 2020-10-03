/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "ImGuiLayer.h"

#include "Internal/Vulkan/VulkanImGuiRenderer.h"
#include <imgui.h>
#include <time.h>
#include "Internal/Core/Keycodes.h"
#include "Internal/Core/Application.h"
#include <iostream>


namespace Internal
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {

    }

    ImGuiLayer::~ImGuiLayer()
    {

    }

    void ImGuiLayer::OnAttach()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        io.KeyMap[ImGuiKey_Tab] = INTERNAL_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = INTERNAL_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = INTERNAL_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = INTERNAL_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = INTERNAL_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = INTERNAL_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = INTERNAL_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = INTERNAL_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = INTERNAL_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = INTERNAL_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = INTERNAL_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = INTERNAL_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = INTERNAL_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = INTERNAL_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = INTERNAL_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = INTERNAL_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = INTERNAL_KEY_A;
        io.KeyMap[ImGuiKey_C] = INTERNAL_KEY_C;
        io.KeyMap[ImGuiKey_V] = INTERNAL_KEY_V;
        io.KeyMap[ImGuiKey_X] = INTERNAL_KEY_X;
        io.KeyMap[ImGuiKey_Y] = INTERNAL_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = INTERNAL_KEY_Z;
        io.DisplaySize = ImVec2(Application::Get()->GetWindow()->getWidth(), Application::Get()->GetWindow()->getHeight());
        std::chrono::high_resolution_clock clock;
        m_TimePoint = clock.now();
    }

    void ImGuiLayer::OnDetach()
    {

    }

    void ImGuiLayer::OnUpdate()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(Application::Get()->GetWindow()->getWidth(), Application::Get()->GetWindow()->getHeight());

        auto now = std::chrono::high_resolution_clock::now();
        m_Time = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_TimePoint).count();
        m_TimePoint = now;
        std::cout << m_Time <<std::endl;

        io.DeltaTime = m_Time;

        ImGui_ImplVulkan_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
    }

    void ImGuiLayer::OnEvent(Event &e)
    {

    }


}