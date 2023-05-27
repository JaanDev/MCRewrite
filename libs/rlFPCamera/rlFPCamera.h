/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   rlFPCamera 
*
*   LICENSE: MIT
*
*   Copyright (c) 2022 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#pragma once

#include "raylib.h"
#include "raymath.h"

#include <functional>

class rlFPCamera
{
public:

    rlFPCamera();

    void Setup(float fovY, Vector3&& position);
    void ViewResized();
    void ViewResized(float w, float h);
    void ViewResized(const RenderTexture& target);

    void Update();

    float GetFOVX() const;
    Vector3 GetCameraPosition() const;
    void SetCameraPosition(const Vector3&& pos);

    Ray GetViewRay() const;
    Ray GetMouseRay(Vector2 mouse) const;
    Vector3 GetForwardVector() const;
    Vector3 GetFowardGroundVector() const;

    inline Vector2 GetViewAngles() const { return Vector2Scale(Angle, 1.0f / DEG2RAD); }

    typedef enum
    {
        MOVE_FRONT = 0,
        MOVE_BACK,
        MOVE_RIGHT,
        MOVE_LEFT,
        MOVE_UP,
        MOVE_DOWN,
        TURN_LEFT,
        TURN_RIGHT,
        TURN_UP,
        TURN_DOWN,
        SPRINT,
        LAST_CONTROL
    }CameraControls;

    int ControlsKeys[LAST_CONTROL];

    Vector3 MoveSpeed = { 1, 1, 1 };
    Vector2 TurnSpeed = { 90, 90 };

    float MouseSensitivity = 600;

    float MinimumViewY = -65.0f;
    float MaximumViewY = 89.0f;

    float ViewBobbleFreq = 0.0f;
    float ViewBobbleMagnatude = 0.02f;
    float ViewBobbleWaverMagnitude = 0.002f;

    typedef std::function<bool(rlFPCamera& camera, Vector3& newPosition, const Vector3& oldPosition)> PositionCallback;
    PositionCallback ValidateCamPosition = nullptr;

    inline const Camera& GetCamera() const { return ViewCamera; }

    // start drawing using the camera, with near/far plane support
    void BeginMode3D();

    // end drawing with the camera
    void EndMode3D();

    bool UseMouseX = true;
    bool UseMouseY = true;

    bool InvertY = false;

    bool UseKeyboard = true;

    bool UseController = true;
    bool ControlerID = 0;

    int ControlerForwardAxis = GAMEPAD_AXIS_LEFT_Y;
    int ControllerSideAxis = GAMEPAD_AXIS_LEFT_X;
    int ControllerPitchAxis = GAMEPAD_AXIS_RIGHT_Y;
    int ControllerYawAxis = GAMEPAD_AXIS_RIGHT_X;
    int ControlerSprintButton = GAMEPAD_BUTTON_LEFT_TRIGGER_1;

    //clipping planes
    // note must use BeginMode3D and EndMode3D on the camera object for clipping planes to work
    double NearPlane = 0.01;
    double FarPlane = 1000;

    bool HideCursor = true;

protected:
    bool Focused = true;
    Vector3 CameraPosition = { 0.0f,0.0f,0.0f };

    Camera ViewCamera = { 0 };
    Vector2 FOV = { 0.0f, 0.0f };

    float TargetDistance = 0;               // Camera distance from position to target
    float PlayerEyesPosition = 0.5f;       // Player eyes position from ground (in meters)
    Vector2 Angle = { 0,0 };                // Camera angle in plane XZ

    float CurrentBobble = 0;

    Vector2 TargetSize = { 0,0 };

    float GetSpeedForAxis(CameraControls axis, float speed);
};