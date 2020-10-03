/*
    Author: Shaun Fulham
    Copyright: Internal Media Group
    Licence: IMG Public Licence
*/

#include "Internal/Renderer/Shader.h"
#include <string>


namespace Internal
{
    class VulkanShader : public Shader
    {
    public:
        VulkanShader(std::string& vertexShader, std::string& fragmentShader);
    private:

    };
}