#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
using std::string;

// this header file contains the path to all the texture that will be used in the project
// under the namespace of Resources

namespace Resources
{
    namespace Kitsune
    {

        inline const string BASE_PATH = "resources/yokai/Kitsune/";
        inline const string RUN_TEXTURE = BASE_PATH + "Run.png";
        inline const string IDLE_TEXTURE = BASE_PATH + "Idle.png";
        inline const string ATTACK_TEXTURE = BASE_PATH + "Attack_3.png";
        inline const string JUMP_TEXTURE = BASE_PATH + "Jump.png";

        constexpr int RUN_FRAMES = 8;
        constexpr int IDLE_FRAMES = 8;
        constexpr int ATTACK_FRAMES = 7;
        constexpr int JUMP_FRAMES = 10;
    }

}

#endif 
