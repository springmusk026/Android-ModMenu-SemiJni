#include <list>
#include <vector>
#include <string>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <Logger.h>
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_android.h>

#include <android/native_activity.h>
#include <android/input.h>
#include <android/native_activity.h>
#include <android/native_window_jni.h>
#include <android/looper.h>
#include <android/log.h>
#include <android/native_window.h>
#include "KittyMemory/MemoryPatch.h"
#include "Menu.h"
#include <Classes.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
//Target lib here

#include "Includes/Macros.h"

typedef int (*InitializeMotionEventFunc)(void *thiz, void *motionEvent, const void *inputMessage);

// Function pointer to hold the original initializeMotionEvent function
InitializeMotionEventFunc origInitializeMotionEvent = nullptr;

// Custom implementation of the initializeMotionEvent function
int myInitializeMotionEvent(void *thiz, void *motionEvent, const void *inputMessage) {
    // Call the original initializeMotionEvent function
    int result = origInitializeMotionEvent(thiz, motionEvent, inputMessage);

    // Extract touch event information
    AInputEvent *inputEvent = (AInputEvent *)thiz;

    // LOGI("inputEvent %p", inputEvent);
    int32_t action = AMotionEvent_getAction(inputEvent);
    int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;
    int32_t pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
    int32_t pointerCount = AMotionEvent_getPointerCount(inputEvent);

    // Process touch event based on the action
    switch (actionMasked) {
        case AMOTION_EVENT_ACTION_DOWN:
        case AMOTION_EVENT_ACTION_POINTER_DOWN: {
            // Touch down event
            int32_t pointerId = AMotionEvent_getPointerId(inputEvent, pointerIndex);
            float x = AMotionEvent_getX(inputEvent, pointerIndex);
            float y = AMotionEvent_getY(inputEvent, pointerIndex);
            // LOGI("X : %f Y : %f", x, y);
            // Call the ImGui_ImplAndroid_HandleInputEvent function with the touch event information
            ImGui_ImplAndroid_HandleInputEvent(static_cast<int>(x), static_cast<int>(y), AMOTION_EVENT_ACTION_DOWN);
            break;
        }
        case AMOTION_EVENT_ACTION_MOVE: {
            // Touch move event
            for (int32_t i = 0; i < pointerCount; ++i) {
                int32_t pointerId = AMotionEvent_getPointerId(inputEvent, i);
                float x = AMotionEvent_getX(inputEvent, i);
                float y = AMotionEvent_getY(inputEvent, i);

                //LOGI("X : %f Y : %f", x, y);
                // Call the ImGui_ImplAndroid_HandleInputEvent function with the touch event information
                ImGui_ImplAndroid_HandleInputEvent(static_cast<int>(x), static_cast<int>(y), AMOTION_EVENT_ACTION_MOVE);
            }
            break;
        }
        case AMOTION_EVENT_ACTION_UP:
        case AMOTION_EVENT_ACTION_POINTER_UP: {
            // Touch up event
            int32_t pointerId = AMotionEvent_getPointerId(inputEvent, pointerIndex);
            float x = AMotionEvent_getX(inputEvent, pointerIndex);
            float y = AMotionEvent_getY(inputEvent, pointerIndex);

            // LOGI("X : %f Y : %f", x, y);
            // Call the ImGui_ImplAndroid_HandleInputEvent function with the touch event information
            ImGui_ImplAndroid_HandleInputEvent(static_cast<int>(x), static_cast<int>(y), AMOTION_EVENT_ACTION_UP);
            break;
        }
            // Add additional cases as needed for other touch event types

        default:
            break;
    }

    return result;
}
void SetupImgui() {
    if(!bInitDone)return;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2((float)Screen::get_width(), (float)Screen::get_height());

    // Setup Dear ImGui style
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0;

    style.Colors[ImGuiCol_Text]                  = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.0f, 0.0f, 0.0, 1.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_Button]           = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
    style.Colors[ImGuiCol_ButtonHovered]    = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
    style.Colors[ImGuiCol_ButtonActive]     = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDimBg]  = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    style.Colors[ImGuiCol_ChildBg] = ImColor(0,0,0);
    style.Colors[ImGuiCol_Text] = ImColor(255,255,255);
    style.Colors[ImGuiCol_Header] = ImColor(30,30,30);
    style.Colors[ImGuiCol_HeaderActive] = ImColor(28,28,28);
    style.Colors[ImGuiCol_HeaderHovered] = ImColor(28,28,28);

    style.Colors[ImGuiCol_Button] = ImColor(36, 36, 36);
    style.Colors[ImGuiCol_ButtonActive] = ImColor(40, 40, 40);
    style.Colors[ImGuiCol_ButtonHovered] = ImColor(40, 40,40);

    style.Colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30);
    style.Colors[ImGuiCol_FrameBgActive] = ImColor(28, 28, 28);
    style.Colors[ImGuiCol_FrameBgHovered] = ImColor(28, 28, 28);

    style.Colors[ImGuiCol_TitleBg] = ImColor(30, 30, 30);
    style.Colors[ImGuiCol_TitleBgActive] = ImColor(28, 28, 28);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(28, 28, 28);
    style.ScrollbarSize = 10.0f; // Set the size of the scrollbar to 10 pixels
    // Setup Platform/Renderer backends
    ImGui_ImplOpenGL3_Init("#version 100");

    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;

    ImFontConfig CustomFont;
    CustomFont.FontDataOwnedByAtlas = false;
    CustomFont.SizePixels = 50.0f; // Set desired font size

    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 2.5;
    icons_config.OversampleV = 2.5;
    icons_config.SizePixels = 35.0f; // Set desired font size

    CustomFont.FontDataOwnedByAtlas = false;

    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 2.5;
    icons_config.OversampleV = 2.5;

    // We load the default font with increased size to improve readability on many devices with "high" DPI.
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 60.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    // Arbitrary scale-up
    ImGui::GetStyle().ScaleAllSizes(3.0f);
}

//KNFBIPGOEPP

vector<Player *> players;

bool playerFind(void *player) {
    if (player != nullptr) {
        for (auto & i : players) {
            if (player == i) return true;
        }
    }
    return false;
}


Quaternion lookRotation;
Player *myPlayer = NULL;



bool isInsideFOV(int x, int y) {
    if (!Struct::Functions::AIM_SIZE)
        return true;

    int circle_x = Screen::get_width() / 2;
    int circle_y = Screen::get_height() / 2;
    int rad = Struct::Functions::AIM_SIZE;
    return (x - circle_x) * (x - circle_x) + (y - circle_y) * (y - circle_y) <= rad * rad;
}

void (*old_Player_Update)(...);

void Player_Update(Player *player) {
    if (player != NULL) {

        bool isMine = *(bool *) ((uint64_t) player + 0x90); //isMine

        if (isMine) {
            myPlayer = player;
        }

        if (!playerFind(player)) players.push_back(player);
        if (players.size() > 20) {
            players.clear();
        }

        Vector3 MyPos{0, 0, 0};
        Vector3 Head{0, 0, 0};
        if(myPlayer != player) {
            if (player->m_Alive() && player) {
                if (myPlayer->m_Alive()&& myPlayer) {

                    auto local_m_Mesh = (myPlayer)->get_thirdPersonBodyPartReferences();
                    if (local_m_Mesh) {
                        MyPos = local_m_Mesh->get_Head()->get_position();
                    }

                    auto PlayerTransform = ((Component *) player)->get_transform();
                    if (PlayerTransform){

                        if(myPlayer->get_TeamNumber() != player->get_TeamNumber()){
                            auto EBodyPartReferences = player->get_thirdPersonBodyPartReferences();
                            auto enemy_m_Mesh = (EBodyPartReferences)->get_Head();
                            if (enemy_m_Mesh) {
                                Head = enemy_m_Mesh->get_position();
                                if(!Physics::Raycast(MyPos, Head)){
                                    if(myPlayer->get_shooting()){
                                        lookRotation = Quaternion::LookRotation((Head + Vector3(0, - 0.50f, 0)) - MyPos, Vector3(0, 1, 0));
                                        LocalPlayer *LocalPlayer_Instance = 0;
                                        Il2Cpp::GetStaticFieldValue("Assembly-CSharp.dll", "", "LocalPlayer", "Instance", &LocalPlayer_Instance);
                                        if(LocalPlayer_Instance){
                                           // *(int *) ((uint64_t) LocalPlayer_Instance->get_instancedWeapon() + 0x8)  = 14;


                                            auto fpsLook = *(uintptr_t *) ((uintptr_t) LocalPlayer_Instance + 0xC);

                                            if(fpsLook){
                                                auto m_Camera = Camera::get_main();
                                                if(!m_Camera) return old_Player_Update(player);
                                                auto HeadSc = m_Camera->WorldToScreen(Head);
                                                if(isInsideFOV((int) HeadSc.X, (int) HeadSc.Y)){
                                                    if(Struct::Functions::aimbot)
                                                        *(Quaternion *) ((uint64_t) fpsLook + 0x2C) = lookRotation;
                                                }

                                                auto WeaponMy = myPlayer->get_weapon();
                                                if(WeaponMy){

                                                    if(Struct::Functions::Range>0){
                                                        *(float *) ((uint64_t) WeaponMy + 0x80) = Struct::Functions::Range;
                                                    }
                                                    if(Struct::Functions::Damage>0){
                                                        *(float *) ((uint64_t) WeaponMy + 0x84) = Struct::Functions::Damage;
                                                    }
                                                    if(Struct::Functions::Speed>0){
                                                        *(float *) ((uint64_t) WeaponMy + 0x108) = Struct::Functions::Speed;
                                                        *(float *) ((uint64_t) WeaponMy + 0x10C) = Struct::Functions::Speed;
                                                    }

                                                    if(Struct::Functions::NoRecoil){
                                                        *(float *) ((uint64_t) WeaponMy + 0x158) = 0;
                                                        *(float *) ((uint64_t) WeaponMy + 0xCC) = 0;
                                                        *(float *) ((uint64_t) WeaponMy + 0xD0) = 0;
                                                        *(float *) ((uint64_t) WeaponMy + 0x14C) = 0;
                                                    }

                                                }

                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return old_Player_Update(player);
}

void (*old_Player_OnDestroy)(...);

void Player_OnDestroy(void *player) {
    if (player != NULL) {
        old_Player_OnDestroy(player);
        players.clear();
    }
}



void * DrawEspList(ImDrawList *m_Canvas){
    if (!bInitDone) return nullptr;

    int g_screenHeight = Screen::get_height();
    int g_screenWidth = Screen::get_width();

    if(!g_screenHeight || !g_screenWidth) return nullptr;

    Vector3 MyPos{0, 0, 0};

    if(Struct::Functions::aimbot){
        m_Canvas->AddCircle({static_cast<float>(g_screenWidth/2),static_cast<float>(g_screenHeight/2)},Struct::Functions::AIM_SIZE,ImGui::ColorConvertFloat4ToU32(*(ImVec4 *)new float[4] {(float)0, (float)255, (float)0, (float)255}),999);
    }

    LocalPlayer *LocalPlayer_Instance = nullptr;
    Il2Cpp::GetStaticFieldValue("Assembly-CSharp.dll", "", "LocalPlayer", "Instance", &LocalPlayer_Instance);
    if(LocalPlayer_Instance) {
        auto m_Camera = Camera::get_main();
        if(!m_Camera) return nullptr;

        auto LBodyPartReferences = myPlayer->get_thirdPersonBodyPartReferences();
        if (LBodyPartReferences) {
            MyPos = LBodyPartReferences->get_Head()->get_position();
        }
        auto myPosSc = m_Camera->WorldToScreen(MyPos);
        auto MyTeami = myPlayer->get_TeamNumber();
        for (auto Player : players) {
            if (Player->m_Alive() && Player != myPlayer && Player) {
                auto ETeami = Player->get_TeamNumber();

                if(!Struct::Functions::isESPTeamMate){
                    if(MyTeami == ETeami)
                        continue;
                }
                auto LBodyPartReferences = Player->get_thirdPersonBodyPartReferences();

                if (LBodyPartReferences) {

                    auto Head = m_Camera->WorldToScreen(LBodyPartReferences->get_Head()->get_position());
                    auto Spine = m_Camera->WorldToScreen(LBodyPartReferences->get_spine()->get_position());
                    auto Hips = m_Camera->WorldToScreen(LBodyPartReferences->get_hips()->get_position());
                    auto LeftUpperArm = m_Camera->WorldToScreen(LBodyPartReferences->get_leftUpperArm()->get_position());
                    auto LeftLowerArm = m_Camera->WorldToScreen(LBodyPartReferences->get_leftLowerArm()->get_position());
                    auto RightUpperArm = m_Camera->WorldToScreen(LBodyPartReferences->get_rightUpperArm()->get_position());
                    auto RightLowerArm = m_Camera->WorldToScreen(LBodyPartReferences->get_rightLowerArm()->get_position());
                    auto LeftUpperLeg = m_Camera->WorldToScreen(LBodyPartReferences->get_leftUpperLeg()->get_position());
                    auto LeftLowerLeg = m_Camera->WorldToScreen(LBodyPartReferences->get_leftLowerLeg()->get_position());
                    auto RightUpperLeg = m_Camera->WorldToScreen(LBodyPartReferences->get_rightUpperLeg()->get_position());
                    auto RightLowerLeg = m_Camera->WorldToScreen(LBodyPartReferences->get_rightLowerLeg()->get_position());

                    if (Head.Z < 1.f) continue;
                    if (Spine.Z < 1.f) continue;
                    if (Hips.Z < 1.f) continue;
                    if (LeftUpperArm.Z < 1.f) continue;
                    if (LeftLowerArm.Z < 1.f) continue;
                    if (RightUpperArm.Z < 1.f) continue;
                    if (RightLowerArm.Z < 1.f) continue;
                    if (LeftUpperLeg.Z < 1.f) continue;
                    if (LeftLowerLeg.Z < 1.f) continue;
                    if (RightUpperLeg.Z < 1.f) continue;
                    if (RightLowerLeg.Z < 1.f) continue;


                    float Distance = Vector3::Distance(MyPos, LBodyPartReferences->get_Head()->get_position());

                    if (Struct::Functions::isESPLine) {
                        m_Canvas->AddLine(ImVec2((float)g_screenWidth / 2, g_screenHeight), ImVec2(Head.X, g_screenHeight - Head.Y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), 2.0f);
                    }

                    if (Struct::Functions::isESPBox) {
                        float boxHeight = abs(Head.Y - Hips.Y);
                        float boxWidth = boxHeight * 0.65f;
                        Vector2 vBox = {Head.X - (boxWidth / 2), Head.Y};
                        m_Canvas->AddRect(ImVec2(g_screenWidth - (g_screenWidth - vBox.X), g_screenHeight - vBox.Y),ImVec2(g_screenWidth - (g_screenWidth - vBox.X) + boxWidth, g_screenHeight - vBox.Y + boxHeight),ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), 0.0f, 0, 1.5f);

                    }

                    if (Struct::Functions::isESPHealth) {
                        int CurHP = Player->m_Health();
                        int MaxHP = 100;
                        ImVec4 color = ImVec4(1.0f, std::min(((2.0f * (MaxHP - CurHP)) / MaxHP), 1.0f), std::min(((2.0f * CurHP) / MaxHP), 1.0f), 0.0f);
                        ImVec2 aboveHeadSc = ImVec2(Head.X, Head.Y);
                        if (Head.Z > 0.0f) {
                            float mWidth = ImGui::GetIO().FontGlobalScale * 35.0f;
                            float mHeight = mWidth * 0.175f;

                            aboveHeadSc.x -= (mWidth / 2.0f);
                            aboveHeadSc.y += (mHeight * 2.0f);

                            ImVec2 rectMin = ImVec2(g_screenWidth - (g_screenWidth - aboveHeadSc.x), g_screenHeight - aboveHeadSc.y);
                            ImVec2 rectMax = ImVec2(rectMin.x + (CurHP * mWidth / MaxHP), rectMin.y + mHeight);

                            m_Canvas->AddRectFilled(rectMin, rectMax, ImGui::ColorConvertFloat4ToU32(color));
                            m_Canvas->AddRect(rectMin, rectMax, ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)));
                        }

                    }

                    if (Struct::Functions::isESPName || Struct::Functions::isESPDistance) {
                        ImVec2 BelowRootSc = ImVec2(Hips.X, Hips.Y);
                        if (Hips.Z > 0.0f) {
                            std::string ws;
                            if (Struct::Functions::isESPName) {
                                auto m_NickName = *(String**)((uint64_t)Player + 0x17C);
                                if (m_NickName) {
                                    ws += m_NickName->CString();
                                }

                                auto m_WNickName = *(String**)((uint64_t)Player->get_weapon() + 0xC);
                                if (m_WNickName) {
                                    ws += " : ";
                                    ws += m_WNickName->CString();
                                }

                                if (Struct::Functions::isESPDistance) {
                                    if (!ws.empty())
                                        ws += " [";
                                    ws += std::to_string((int)Distance);
                                    ws += "m]";
                                }

                                ImVec2 textPos = ImVec2(g_screenWidth - (g_screenWidth - BelowRootSc.x), g_screenHeight - BelowRootSc.y);

                                m_Canvas->AddText(ImGui::GetFont(), ImGui::GetFontSize(), textPos, ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.0f, 1.0f, 1.0f)), ws.c_str());
                            }


                        }

                    }
                    if(Struct::Functions::isESPSkeleton){
                        if (LBodyPartReferences) {

                            ImU32 lineColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                            float lineSize = 2.0f;

                            m_Canvas->AddLine(ImVec2(Head.X, g_screenHeight - Head.Y), ImVec2(Spine.X, g_screenHeight - Spine.Y), lineColor, lineSize);
                            m_Canvas->AddLine(ImVec2(Spine.X, g_screenHeight - Spine.Y), ImVec2(Hips.X, g_screenHeight - Hips.Y), lineColor, lineSize);
                            m_Canvas->AddLine(ImVec2(Spine.X, g_screenHeight - Spine.Y), ImVec2(LeftUpperArm.X, g_screenHeight - LeftUpperArm.Y), lineColor, lineSize);
                            m_Canvas->AddLine(ImVec2(LeftUpperArm.X, g_screenHeight - LeftUpperArm.Y), ImVec2(LeftLowerArm.X, g_screenHeight - LeftLowerArm.Y), lineColor, lineSize);
                            m_Canvas->AddLine(ImVec2(Spine.X, g_screenHeight - Spine.Y), ImVec2(RightUpperArm.X, g_screenHeight - RightUpperArm.Y), lineColor, lineSize);
                            m_Canvas->AddLine(ImVec2(RightUpperArm.X, g_screenHeight - RightUpperArm.Y), ImVec2(RightLowerArm.X, g_screenHeight - RightLowerArm.Y), lineColor, lineSize);
                            m_Canvas->AddLine(ImVec2(Hips.X, g_screenHeight - Hips.Y), ImVec2(LeftUpperLeg.X, g_screenHeight - LeftUpperLeg.Y), lineColor, lineSize);
                            m_Canvas->AddLine(ImVec2(LeftUpperLeg.X, g_screenHeight - LeftUpperLeg.Y), ImVec2(LeftLowerLeg.X, g_screenHeight - LeftLowerLeg.Y), lineColor, lineSize);
                            m_Canvas->AddLine(ImVec2(Hips.X, g_screenHeight - Hips.Y), ImVec2(RightUpperLeg.X, g_screenHeight - RightUpperLeg.Y), lineColor, lineSize);
                            m_Canvas->AddLine(ImVec2(RightUpperLeg.X, g_screenHeight - RightUpperLeg.Y), ImVec2(RightLowerLeg.X, g_screenHeight - RightLowerLeg.Y), lineColor, lineSize);

                        }
                    }
                }

            }
        }
    }

    return nullptr;
}

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {

    if (!setup) {
        SetupImgui();
        setup = true;
    }

    ImGuiIO &io = ImGui::GetIO();

    if (io.WantTextInput && !io.KeyCtrl && !io.KeyShift) {
        //displayKeyboard(NULL, true);
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    DrawEspList(ImGui::GetBackgroundDrawList());
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (clearMousePos) {
        io.MousePos = ImVec2(-1, -1);
        clearMousePos = false;
    }

    return old_eglSwapBuffers(dpy, surface);
}

void (*old_DoRecoil)(Weapon *OLLPOKDLOLO, int BFEHFMDMFMD, float HFBDADFGGNE);
void hook_DoRecoil(Weapon *OLLPOKDLOLO, int BFEHFMDMFMD, float HFBDADFGGNE){
    if(Struct::Functions::NoRecoil){
        auto WeaponMy = myPlayer->get_weapon();
        if(WeaponMy){
            if(WeaponMy == OLLPOKDLOLO){
                return old_DoRecoil(OLLPOKDLOLO,BFEHFMDMFMD,0);
            }
        }

    }
    return old_DoRecoil(OLLPOKDLOLO,BFEHFMDMFMD,HFBDADFGGNE);
}

// we will run our hacks in a new thread so our while loop doesn't block process main thread
void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread created"));

    while (!g_il2cpp) {
        g_il2cpp = findLibrary("libil2cpp.so");
        sleep(1);
    }

    LOGI("libil2cpp.so: %p", g_il2cpp);

    Il2Cpp::Attach();

    sleep(5);

    Struct::Offsets::Transform::get_position = (uintptr_t) Il2Cpp::GetMethodOffset("UnityEngine.dll","UnityEngine","Transform","get_position");
    Struct::Offsets::Camera::get_main = (uintptr_t) Il2Cpp::GetMethodOffset("Assembly-CSharp.dll", "","CameraManager","get_MainCamera", 0);
    Struct::Offsets::Camera::WorldToScreenPoint = (uintptr_t) Il2Cpp::GetMethodOffset("UnityEngine.dll","UnityEngine","Camera","WorldToScreenPoint",1);
    Struct::Offsets::Component::get_transform = (uintptr_t) Il2Cpp::GetMethodOffset("UnityEngine.dll","UnityEngine","Component","get_transform");

    Struct::Offsets::Physics::Raycast = (uintptr_t) Il2Cpp::GetMethodOffset("UnityEngine.dll", "UnityEngine","Physics", "Raycast",2);

    Struct::Offsets::Player::get_health = (uintptr_t) Il2Cpp::GetMethodOffset("Assembly-CSharp.dll", "","Player", "get_health");
    Struct::Offsets::Player::Update = (uintptr_t) Il2Cpp::GetMethodOffset("Assembly-CSharp.dll", "", "Player", "Update", 0);
    Struct::Offsets::Player::OnDestroy = (uintptr_t) Il2Cpp::GetMethodOffset("Assembly-CSharp.dll", "", "Player", "OnDestroy", 0);

    DobbyHook((void *)Struct::Offsets::Player::Update,(void *) Player_Update, (void **) &old_Player_Update);
    DobbyHook((void *)Struct::Offsets::Player::OnDestroy,(void *) Player_OnDestroy, (void **) &old_Player_OnDestroy);
    DobbyHook(reinterpret_cast<void *>(g_il2cpp + 0xCB09C0), (void *) hook_DoRecoil, (void **) &old_DoRecoil);

    bInitDone = true;

    void *sym_input = DobbySymbolResolver(("/system/lib/libinput.so"), ("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"));

    if (nullptr != sym_input){
        origInitializeMotionEvent = reinterpret_cast<InitializeMotionEventFunc>(sym_input);
        DobbyHook((void *)origInitializeMotionEvent, (void *)myInitializeMotionEvent, (void **)&origInitializeMotionEvent);

    }
    auto addr = (uintptr_t)dlsym(RTLD_NEXT, "eglSwapBuffers");
    DobbyHook((void *)addr, (void *)hook_eglSwapBuffers, (void **)&old_eglSwapBuffers);

    return nullptr;
}

jobjectArray getFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    //Toasts added here so it's harder to remove it
    MakeToast(env, context, OBFUSCATE("Modded by LGL"), Toast::LENGTH_LONG);

    const char *features[] = {
            OBFUSCATE("Category_ESP"),//Not counted
            OBFUSCATE("2_Toggle_ESP Line"),//1
            OBFUSCATE("3_Toggle_ESP Box"),//2
            OBFUSCATE("4_Toggle_ESP Health"),//4
            OBFUSCATE("5_Toggle_ESP Name"),//5
            OBFUSCATE("6_Toggle_ESP Distance"),//6
            OBFUSCATE("7_Toggle_ESP Skeleton"),//7
            OBFUSCATE("8_Toggle_ESP TeamMate"),//8
            OBFUSCATE("Category_Aimbot "),//Not counted
            OBFUSCATE("9_Toggle_Aimbot Simulation"),//9
            OBFUSCATE("10_SeekBar_Aimbot Fov_0_500"),//10
            OBFUSCATE("11_Toggle_No Recoil"),//11
            OBFUSCATE("12_SeekBar_Weapon Range_0_1000"),//12
            OBFUSCATE("13_SeekBar_Damage Multiplier_0_1000"),//13
            OBFUSCATE("14_SeekBar_Speed Hack_0_100"),//14

    };

    //Now you dont have to manually update the number everytime;
    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

void Changes(JNIEnv *env, jclass clazz, jobject obj,jint featNum, jstring featName, jint value,jboolean boolean, jstring str) {

    switch (featNum) {
        case 2:
            Struct::Functions::isESPLine = boolean;
            break;
        case 3:
            Struct::Functions::isESPBox = boolean;
            break;
        case 4:
            Struct::Functions::isESPHealth = boolean;
            break;
        case 5:
            Struct::Functions::isESPName = boolean;
            break;
        case 6:
            Struct::Functions::isESPDistance = boolean;
            break;
        case 7:
            Struct::Functions::isESPSkeleton = boolean;
            break;
        case 8:
            Struct::Functions::isESPTeamMate = boolean;
            break;
        case 9:
            Struct::Functions::aimbot = boolean;
            break;
        case 10:
            Struct::Functions::AIM_SIZE = value;
            break;
        case 11:
            Struct::Functions::NoRecoil = boolean;
            break;
        case 12:
            Struct::Functions::Range = value;
            break;
        case 13:
            Struct::Functions::Damage = value;
            break;
        case 14:
            Struct::Functions::Speed = value;
            break;
    }
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, nullptr, hack_thread, nullptr);
}

int RegisterMain(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("CheckOverlayPermission"), OBFUSCATE("(Landroid/content/Context;)V"), reinterpret_cast<void *>(CheckOverlayPermission)},
    };
    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/MainActivity"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;

    return JNI_OK;
}

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);

    jclass c = globalEnv->FindClass("com/android/support/Menu");
    if (c != nullptr){
        static const JNINativeMethod menuMethods[] = {
              {OBFUSCATE("RichWebView"), OBFUSCATE("(Ljava/lang/String;)Landroid/view/View;"), reinterpret_cast<void *>(RichWebView)},
              {OBFUSCATE("RichTextView"), OBFUSCATE("(Ljava/lang/String;)Landroid/view/View;"), reinterpret_cast<void *>(RichTextView)},
              {OBFUSCATE("Category"), OBFUSCATE("(Ljava/lang/String;)Landroid/view/View;"), reinterpret_cast<void *>(Category)},
              {OBFUSCATE("ButtonLink"), OBFUSCATE("(Ljava/lang/String;Ljava/lang/String;)Landroid/view/View;"), reinterpret_cast<void *>(ButtonLink)},
              {OBFUSCATE("Icon"), OBFUSCATE("()Ljava/lang/String;"), reinterpret_cast<void *>(Icon)},
              {OBFUSCATE("IconWebViewData"),  OBFUSCATE("()Ljava/lang/String;"), reinterpret_cast<void *>(IconWebViewData)},
              {OBFUSCATE("isGameLibLoaded"),  OBFUSCATE("()Z"), reinterpret_cast<void *>(isGameLibLoaded)},
              {OBFUSCATE("InitializeField"),  OBFUSCATE("()Z"), reinterpret_cast<void *>(InitializeField)},
              {OBFUSCATE("isNotInGame"),  OBFUSCATE("()Z"), reinterpret_cast<void *>(isNotInGame)},
              {OBFUSCATE("AddViews"),  OBFUSCATE("()V"), reinterpret_cast<void *>(AddViews)},
              {OBFUSCATE("FeatureLister"),  OBFUSCATE("([Ljava/lang/String;Landroid/widget/LinearLayout;)V"), reinterpret_cast<void *>(FeatureLister)},
              {OBFUSCATE("isViewCollapsed"),  OBFUSCATE("()Z"), reinterpret_cast<void *>(isViewCollapsed)},
              {OBFUSCATE("Thread"),  OBFUSCATE("(Landroid/widget/FrameLayout;Landroid/content/Context;)V"), reinterpret_cast<void *>(Thread)},
              {OBFUSCATE("onDestroy"),  OBFUSCATE("(Landroid/widget/FrameLayout;Landroid/view/WindowManager;)V"), reinterpret_cast<void *>(onDestroy)},
              {OBFUSCATE("dp"),  OBFUSCATE("(I)I"), reinterpret_cast<void *>(dp)},
              {OBFUSCATE("onStartCommandn"), OBFUSCATE("(Landroid/content/Intent;II)I"), reinterpret_cast<void *>(onStartCommand)},
              {OBFUSCATE("convertDipToPixels"),  OBFUSCATE("(I)I"), reinterpret_cast<void *>(convertDipToPixels)},
              {OBFUSCATE("setHeadingText"),  OBFUSCATE("(Landroid/widget/TextView;)V"), reinterpret_cast<void *>(setHeadingText)},
              {OBFUSCATE("setTitleText"),  OBFUSCATE("(Landroid/widget/TextView;)V"), reinterpret_cast<void *>(setTitleText)},
              {OBFUSCATE("MakeWindow"),  OBFUSCATE("()V"), reinterpret_cast<void *>(MakeWindow)},
              {OBFUSCATE("MakeLogo"), OBFUSCATE("()V"), reinterpret_cast<void *>(MakeLogo)},
              {OBFUSCATE("MakeMenu"), OBFUSCATE("()V"), reinterpret_cast<void *>(MakeMenu)},
              {OBFUSCATE("settingsList"),  OBFUSCATE("()[Ljava/lang/String;"), reinterpret_cast<void *>(settingsList)},
              {OBFUSCATE("getFeatureList"),  OBFUSCATE("()[Ljava/lang/String;"), reinterpret_cast<void *>(getFeatureList)},

        };

        int mm = globalEnv->RegisterNatives(c, menuMethods, sizeof(menuMethods) / sizeof(JNINativeMethod));
        if (mm != JNI_OK) {
            LOGE(OBFUSCATE("Menu methods error"));
            return mm;
        }
    }
    else{
        LOGE(OBFUSCATE("JNI error"));
        return JNI_ERR;
    }

    jclass p = globalEnv->FindClass( OBFUSCATE("com/android/support/Preferences"));
    if (p != nullptr){
        static const JNINativeMethod prefmethods[] = {
                { OBFUSCATE("Changes"), OBFUSCATE("(Landroid/content/Context;ILjava/lang/String;IZLjava/lang/String;)V"), reinterpret_cast<void *>(Changes)},
        };

        int pm = globalEnv->RegisterNatives(p, prefmethods, sizeof(prefmethods) / sizeof(JNINativeMethod));
        if (pm != JNI_OK){
            LOGE(OBFUSCATE("Preferences methods error"));
            return pm;
        }
    }
    else{
        LOGE(OBFUSCATE("JNI error"));
        return JNI_ERR;
    }

    if (RegisterMain(globalEnv) != 0)
        return JNI_ERR;

    return JNI_VERSION_1_6;
}

