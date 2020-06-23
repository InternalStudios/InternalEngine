/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#pragma once

namespace Internal
{
    class Layer
    {
    public:
        Layer(const char* name);
        virtual void OnUpdate() = 0;

        virtual void OnEvent() = 0;
    private:
        const char* m_Name;
    };
}