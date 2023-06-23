//
// Created by sprin on 6/23/2023.
//

#ifndef FWDASSAULTNEW_STRUCT_H
#define FWDASSAULTNEW_STRUCT_H



namespace Struct {
    namespace Offsets{
        namespace Component{
            uintptr_t get_transform = 0;
        }
        namespace Transform{
            uintptr_t get_position = 0;
        }
        namespace Camera{
            uintptr_t get_main = 0;
            uintptr_t WorldToScreenPoint = 0;
        }
        namespace Player{
            uintptr_t get_health = 0;
            uintptr_t Update = 0;
            uintptr_t OnDestroy = 0;
        }
        namespace Physics{
            uintptr_t Raycast = 0;
        }
    }

    namespace Functions{
        int AIM_SIZE = 100;
        bool aimbot;
        bool NoRecoil;
        float Range = 0;
        float Damage = 0;
        float Speed = 0;

        bool isESP = false;
        bool isESPLine = false;
        bool isESPBox = false;
        bool isESPName = false;
        bool isESPHealth = false;
        bool isESPDistance = false;
        bool isESPTeamMate= false;
        bool isESPSkeleton = false;

    }
}


#endif //FWDASSAULTNEW_STRUCT_H
