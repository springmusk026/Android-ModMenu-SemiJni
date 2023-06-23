//
// Created by springmusk on 6/23/2023.
//

#ifndef FWDASSAULTNEW_CLASSES_H
#define FWDASSAULTNEW_CLASSES_H

#include <Il2Cpp.h>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Quaternion.hpp>
#include <Struct.h>

bool clearMousePos = true, setup = false;

uintptr_t g_il2cpp = 0;
bool bInitDone = false;

class Screen {
public:
    static int get_width() {
        auto Screen_get_width = (int (*)()) (g_il2cpp+0x290B080);
        return Screen_get_width();
    }
    static int get_height() {
        auto Screen_get_height = (int (*)()) (g_il2cpp+0x290B0C8);
        return Screen_get_height();
    }
};
class Transform {
public:
    Vector3 get_position() {
        auto Transform_get_position = (Vector3 (*)(Transform *)) (Struct::Offsets::Transform::get_position);
        return Transform_get_position(this);
    }
};

class Component {
public:
    Transform *get_transform() {
        auto Component_get_transform = (Transform *(*)(
                Component *)) (Struct::Offsets::Component::get_transform);
        return Component_get_transform(this);
    }
};

class Camera {
public:
    static Camera *get_main() {
        auto Camera_get_main = (Camera *(*)()) (Struct::Offsets::Camera::get_main);
        return Camera_get_main();
    }

    Vector3 WorldToScreen(Vector3 pos) {
        if (this) {
            auto Camera_WorldToScreenPoint = (Vector3 (*)(Camera *,Vector3)) (Struct::Offsets::Camera::WorldToScreenPoint);
            return Camera_WorldToScreenPoint(this, pos);
        }
        return {0, 0, 0};
    }

};
class Physics {
public:
    static bool Raycast(Vector3 origin, Vector3 direction) {
        typedef bool (*RaycastFunc)(uintptr_t instance,Vector3, Vector3);
        RaycastFunc PhysicsRaycast = reinterpret_cast<RaycastFunc>(Struct::Offsets::Physics::Raycast);
        return PhysicsRaycast(0,origin, direction);
    }

    template <typename T = bool> static T Raycast_3(Vector3 origin, Vector3 direction) {
        return ((T (*)(void *, Vector3, Vector3))(g_il2cpp + 0x298736C))(0, origin, direction);
    }
};

class ThirdPersonBodyPartReferences {
private:

public :

    Transform *get_Head() {
        return  *(Transform **) ((uint64_t) this + 0xC);
    }
    Transform *get_spine() {
        return  *(Transform **) ((uint64_t) this + 0x10);
    }
    Transform *get_hips() {
        return  *(Transform **) ((uint64_t) this + 0x14);
    }
    Transform *get_leftUpperArm() {
        return  *(Transform **) ((uint64_t) this + 0x18);
    }
    Transform *get_leftLowerArm() {
        return  *(Transform **) ((uint64_t) this + 0x1C);
    }
    Transform *get_rightUpperArm() {
        return  *(Transform **) ((uint64_t) this + 0x20);
    }
    Transform *get_rightLowerArm() {
        return  *(Transform **) ((uint64_t) this + 0x24);
    }
    Transform *get_leftUpperLeg() {
        return  *(Transform **) ((uint64_t) this + 0x28);
    }
    Transform *get_leftLowerLeg() {
        return  *(Transform **) ((uint64_t) this + 0x2C);
    }
    Transform *get_rightUpperLeg() {
        return  *(Transform **) ((uint64_t) this + 0x30);
    }
    Transform *get_rightLowerLeg() {
        return  *(Transform **) ((uint64_t) this + 0x34);
    }
};

class Weapon {
public :

};

class Weapon_GLAEFNAJBJN {
public :

};
class LocalPlayer {
public :

    Weapon_GLAEFNAJBJN * get_instancedWeapon()
    {
        auto LocalPlayer_get_instancedWeapon = (Weapon_GLAEFNAJBJN *(*)(LocalPlayer *)) (g_il2cpp + 0xCEC0F8);
        return LocalPlayer_get_instancedWeapon(this);
    }
};

class Player {
public :
    int get_TeamNumber()
    {
        auto Player_get_TeamNumber = (int (*)(Player *)) (g_il2cpp + 0xD6AB58);
        return Player_get_TeamNumber(this);
    }
    bool get_shooting()
    {
        auto Player_get_shooting = (bool (*)(Player *)) (g_il2cpp + 0xD522B4);
        return Player_get_shooting(this);
    }
    bool get_visible()
    {
        auto Player_get_visible = (bool (*)(Player *)) (g_il2cpp + 0xD52C8C);
        return Player_get_visible(this);
    }
    ThirdPersonBodyPartReferences * get_thirdPersonBodyPartReferences()
    {
        auto Player_get_shooting = (ThirdPersonBodyPartReferences *(*)(Player *)) (g_il2cpp + 0xD552CC);
        return Player_get_shooting(this);
    }
    Weapon * get_weapon()
    {
        auto Player_get_weapon = (Weapon *(*)(Player *)) (g_il2cpp + 0xD54854);
        return Player_get_weapon(this);
    }

    int m_Health() {
        return *(float *) ((uint64_t) this + 0x178);
    }

    bool m_Alive() {
        return this->m_Health();
    }

};

#endif //FWDASSAULTNEW_CLASSES_H
