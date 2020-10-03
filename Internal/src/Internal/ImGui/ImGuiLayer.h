/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

#include "Internal/Core/Layer.h"
#include "Internal/Events/Event.h"
#include <chrono>

namespace Internal
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate() override;
        virtual void OnEvent(Event& e) override;
    private:
        float m_Time = 0.0f;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_TimePoint;
    };
}