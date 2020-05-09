/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

namespace Internal
{
    struct WindowData
    {
        const char* Title;
        uint32_t width, height;
        bool IsFocused;
    };

    // Base Window Class
    class Window
    {
    public:
        Window(WindowData& data = {"InternalEngine", 1280, 720, true});
        virtual void setTitle(const char* title) = 0;
        const char* getTitle() const {return m_Data.Title;}

        virtual void setWidth(uint32_t width) = 0;
        virtual void setHeight(uint32_t height) = 0;
        uint32_t getWidth() {return m_Data.width;}
        uint32_t getHeight() {return m_Data.height;}

        bool getFocused() {return m_Data.IsFocused;}

        virtual void OnUpdate() = 0;
    private:
        WindowData m_Data;
    };
}