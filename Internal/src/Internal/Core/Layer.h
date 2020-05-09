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
        virtual void OnUpdate();

        virtual void OnEvent();
    private:
    };
}