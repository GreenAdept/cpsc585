#ifndef XBOX360CONTROLLER_H
#define XBOX360CONTROLLER_H

////////////////////////////////////////////////////////////////////////////////////////////////////
// XBox360Controller.h
// By: Cody Watts (wattsc@cpsc.ucalgary.ca)
// January 22, 2009
////////////////////////////////////////////////////////////////////////////////////////////////////
/// This file contains the declaration and implementation of the XBox360Controller class, along
/// with the associated classes, XBox360Button, XBox360Trigger, and XBox360Thumbstick.
/// These classes serve as a wrapper around XInput, and should hopefully make it easy to use XBox
/// 360 gamepads with your games.
///
/// Pieces of code in this file were taken from the following websites:
///   * http://www.codeproject.com/KB/directx/xbox360_cont_xinput.aspx
///   * http://msdn.microsoft.com/en-us/library/bb173044(VS.85).aspx
///
/// This file will not compile without the DirectX SDK. If you have problem compiling this file,
/// please make sure the appropriate DirectX SDK directories are included under Tools -> Options ->
/// Projects and Solutions -> VC++ Directories.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <XInput.h>
#include <math.h>
#pragma comment(lib, "XInput.lib")

#ifndef USHRT_MAX
#define USHRT_MAX 65535
#endif
#ifndef SHRT_MAX
#define SHRT_MAX 32767
#endif
#ifndef UCHAR_MAX
#define UCHAR_MAX 255
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
// XBox360Button
////////////////////////////////////////////////////////////////////////////////////////////////////
/// This class holds information on gamepad objects which can be pressed and released, including
/// the directional pad, the thumbsticks, the triggers, and of course, the face buttons.
////////////////////////////////////////////////////////////////////////////////////////////////////
class XBox360Button
{
	public:
		XBox360Button();

		bool WasPressed();
		bool WasPressedOrHeld();
		bool WasReleased();

	private:
		friend class XBox360Controller;

		void SetButtonValue(bool a_nNewState);

	protected:
		bool m_nState;
		bool m_nOldState;
};

inline XBox360Button::XBox360Button()
:m_nState(false)
,m_nOldState(false)
{}

inline void XBox360Button::SetButtonValue(bool a_nNewState)
{
	m_nOldState = m_nState;
	m_nState = a_nNewState;
}

inline bool XBox360Button::WasPressed()
{
	return ((m_nState == true) && (m_nOldState == false));
}

inline bool XBox360Button::WasPressedOrHeld()
{
	return (m_nState == true);
}

inline bool XBox360Button::WasReleased()
{
	return ((m_nState == false) && (m_nOldState == true));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// XBox360Trigger
////////////////////////////////////////////////////////////////////////////////////////////////////
/// A special instance of the XBox360Button class, which also contains an analog value describing
/// exactly how much the trigger has been depressed.
////////////////////////////////////////////////////////////////////////////////////////////////////
class XBox360Trigger: public XBox360Button
{
	public:
		XBox360Trigger(unsigned char a_nDefaultDeadZone);

		void SetDeadZone(float a_fDeadZoneRadius);
		float GetDeadZone() { return m_fDeadZone; }

		float GetValue() { return m_fValue; }

	private:
		friend class XBox360Controller;

		void SetTriggerValue(unsigned char a_nValue);

		float m_fDeadZone;
		float m_fValue;
};

inline XBox360Trigger::XBox360Trigger(unsigned char a_nDefaultDeadZone)
{
	m_fDeadZone = a_nDefaultDeadZone / (float)UCHAR_MAX;
}

inline void XBox360Trigger::SetDeadZone(float a_fDeadZoneRadius)
{
	if (a_fDeadZoneRadius > 1.0f) a_fDeadZoneRadius = 1.0f;
	if (a_fDeadZoneRadius < 0.0f) a_fDeadZoneRadius = 0.0f;
	m_fDeadZone = a_fDeadZoneRadius;
}

inline void XBox360Trigger::SetTriggerValue(unsigned char a_nValue)
{
	m_nOldState = m_nState;

	unsigned char nDeadZone = (unsigned char)(m_fDeadZone * UCHAR_MAX);

	// If the value is within our dead-zone...
	if (a_nValue < nDeadZone)
	{
		m_nState = false;
		m_fValue = 0.0f;
	}
	else
	{
		m_nState = true;
		m_fValue = (a_nValue - nDeadZone)/(float)(UCHAR_MAX - nDeadZone);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// XBox360Thumbstick
////////////////////////////////////////////////////////////////////////////////////////////////////
/// A class which holds information about an XBox 360 thumbstick, including its position and
/// dead-zone settings.
////////////////////////////////////////////////////////////////////////////////////////////////////
class XBox360Thumbstick: public XBox360Button
{
	public:
		XBox360Thumbstick(short a_nDefaultDeadZone);

		float GetX() { return m_fX; }
		float GetY() { return m_fY; }

		void SetDeadZone(float a_fDeadZoneRadius);
		float GetDeadZone() { return m_fDeadZone; }

	private:
		friend class XBox360Controller;

		void SetThumbstickPosition(short a_nX, short a_nY);

		float m_fDeadZone;
		float m_fX;
		float m_fY;
};

inline XBox360Thumbstick::XBox360Thumbstick(short a_nDefaultDeadZone)
{
	m_fDeadZone = a_nDefaultDeadZone / (float)SHRT_MAX;
}

inline void XBox360Thumbstick::SetDeadZone(float a_fDeadZoneRadius)
{
	if (a_fDeadZoneRadius > 1.0f) a_fDeadZoneRadius = 1.0f;
	if (a_fDeadZoneRadius < 0.0f) a_fDeadZoneRadius = 0.0f;
	m_fDeadZone = a_fDeadZoneRadius;
}

inline void XBox360Thumbstick::SetThumbstickPosition(short a_nX, short a_nY)
{
	float LX = a_nX;
	float LY = a_nY;

	//determine how far the controller is pushed
	float magnitude = sqrt(LX*LX + LY*LY);

	if (magnitude > SHRT_MAX)
	{
		LX *= (SHRT_MAX/magnitude);
		LY *= (SHRT_MAX/magnitude);
		magnitude = SHRT_MAX;
	}

	//determine the direction the controller is pushed
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	float normalizedMagnitude = 0;

	unsigned short m_nDeadZone = (unsigned short)(m_fDeadZone * SHRT_MAX);

	//check if the controller is outside a circular dead zone
	if (magnitude > m_nDeadZone)
	{
	  //adjust magnitude relative to the end of the dead zone
	  magnitude -= m_nDeadZone;

	  //optionally normalize the magnitude with respect to its expected range
	  //giving a magnitude value of 0.0 to 1.0
	  normalizedMagnitude = magnitude / (SHRT_MAX - m_nDeadZone);
	}

	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	m_fX = (LX/(float)SHRT_MAX) * normalizedMagnitude;
	m_fY = (LY/(float)SHRT_MAX) * normalizedMagnitude;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// XBox360Controller
////////////////////////////////////////////////////////////////////////////////////////////////////
/// The highest-level class in the file; this class contains information about the entire XBox 360
/// gamepad, including data members to represent all of the pad's thumbsticks and buttons.
////////////////////////////////////////////////////////////////////////////////////////////////////
class XBox360Controller
{
	public:
		XBox360Controller(int a_nControllerID);
		bool IsConnected();
		void Update(float a_fSecondsElapsed);
		void Vibrate(float a_fLeftMotor, float a_fRightMotor, float a_fVibrationTime = -1.0f);
		void StopVibration();

		XBox360Thumbstick LeftThumbstick;
		XBox360Thumbstick RightThumbstick;

		XBox360Trigger LeftTrigger;
		XBox360Trigger RightTrigger;

		XBox360Button Up;
		XBox360Button Down;
		XBox360Button Left;
		XBox360Button Right;
		XBox360Button Start;
		XBox360Button Back;
		XBox360Button LeftShoulder;
		XBox360Button RightShoulder;
		XBox360Button A;
		XBox360Button B;
		XBox360Button X;
		XBox360Button Y;

		bool canVibrate;

	private:
		short unsigned int m_nControllerID;
		bool m_bTimedVibrationActive;
		float m_fVibrationTimeRemaining;
};

inline XBox360Controller::XBox360Controller(int a_nControllerID)
:m_nControllerID(a_nControllerID)
,LeftThumbstick(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
,RightThumbstick(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
,LeftTrigger(XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
,RightTrigger(XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
,m_bTimedVibrationActive(false)
,m_fVibrationTimeRemaining(0.0f)
{}

inline bool XBox360Controller::IsConnected()
{
	XINPUT_STATE _controllerState;

    // Get the state
    DWORD Result = XInputGetState(m_nControllerID, &_controllerState);

    if(Result == ERROR_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline void XBox360Controller::Update(float a_fSecondsElapsed)
{
	XINPUT_STATE _controllerState;

	// Zeroise the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    XInputGetState(m_nControllerID, &_controllerState);

	LeftThumbstick.SetThumbstickPosition(_controllerState.Gamepad.sThumbLX, _controllerState.Gamepad.sThumbLY);
	RightThumbstick.SetThumbstickPosition(_controllerState.Gamepad.sThumbRX, _controllerState.Gamepad.sThumbRY);

	LeftThumbstick.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0);
	RightThumbstick.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0);

	LeftTrigger.SetTriggerValue(_controllerState.Gamepad.bLeftTrigger);
	RightTrigger.SetTriggerValue(_controllerState.Gamepad.bRightTrigger);

	Up.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);
	Down.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
	Left.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);
	Right.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);
	Start.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0);
	Back.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0);
	LeftShoulder.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0);
	RightShoulder.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
	A.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
	B.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
	X.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);
	Y.SetButtonValue((_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);

	if (m_bTimedVibrationActive == true)
	{
		m_fVibrationTimeRemaining -= a_fSecondsElapsed;
		if (m_fVibrationTimeRemaining < 0.0f)
		{
			m_fVibrationTimeRemaining = 0.0f;
			m_bTimedVibrationActive = false;
			StopVibration();
		}
	}
}

inline void XBox360Controller::Vibrate(float a_fLeftMotor, float a_fRightMotor, float a_fVibrationTime /*= -1.0f*/)
{
	// Clamp values between 0 and 1
	if (a_fLeftMotor > 1.0f)
	{
		a_fLeftMotor = 1.0f;
	}
	else if (a_fLeftMotor < 0.0f)
	{
		a_fLeftMotor = 0.0f;
	}

	if (a_fRightMotor > 1.0f)
	{
		a_fRightMotor = 1.0f;
	}
	else if (a_fRightMotor < 0.0f)
	{
		a_fRightMotor = 0.0f;
	}

    // Create a Vibraton State
    XINPUT_VIBRATION Vibration;

    // Zeroise the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = (unsigned int)(a_fLeftMotor * USHRT_MAX);
    Vibration.wRightMotorSpeed = (unsigned int)(a_fRightMotor * USHRT_MAX);

    // Vibrate the controller
    XInputSetState(m_nControllerID, &Vibration);

	// Set up a vibration timer, if necessary
	if (a_fVibrationTime > 0.0f)
	{
		m_bTimedVibrationActive = true;
		m_fVibrationTimeRemaining = a_fVibrationTime;
	}

	else
	{
		m_bTimedVibrationActive = false;
		m_fVibrationTimeRemaining = 0.0f;
	}
}

inline void XBox360Controller::StopVibration()
{
    // Create a Vibraton State
    XINPUT_VIBRATION Vibration;

    // Zeroise the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = 0;
    Vibration.wRightMotorSpeed = 0;

    // Vibrate the controller
    XInputSetState(m_nControllerID, &Vibration);

	m_bTimedVibrationActive = false;
	m_fVibrationTimeRemaining = 0.0f;
}

#endif