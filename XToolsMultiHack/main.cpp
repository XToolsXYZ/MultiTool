/* Made by ZR2 Please Don't FUCK ME */

/* Including windows functions */
#include <Windows.h>
#include <iostream>
#include <stdint.h>
#include <random>
#include <thread>
#include <string>
#include <math.h>

using namespace std;
int ConsolePos_x = 5;
int ConsolePos_y = 5;

/* Including our header for memory */
#include "MManager.h"

/* Variables */
bool GLOW_activation = false;
bool RADAR_activation = false;
bool SLOWAIM_activation = false;
bool RCS_activation = false;
bool BHOP_activation = false;

/* Memory variables definition */
Memory MManager;
Module Client;
Module Engine;

/* Offsets */
DWORD LocalPlayer;
DWORD EnginePointer;
DWORD EntityList;
DWORD ViewAngle;
DWORD VectorPunch;
DWORD GlowObject;
DWORD Sensitivity;
DWORD ForceJump;
DWORD CrosshairID;

/* Offset Scan */
void PatternScan()
{
	/* LocalPlayer */
	DWORD LocalPlayerArray = MManager.FindPatternArr(Client.dwBase, Client.dwSize, "xxx????xx????xxxxx?", 19, 0x8D, 0x34, 0x85, 0x0, 0x0, 0x0, 0x0, 0x89, 0x15, 0x0, 0x0, 0x0, 0x0, 0x8B, 0x41, 0x8, 0x8B, 0x48, 0x0);
	LocalPlayer = (MManager.Read<DWORD>(LocalPlayerArray + 3) + MManager.Read<BYTE>(LocalPlayerArray + 18)) - Client.dwBase;

	/* EntityList */
	DWORD EntityListArray = MManager.FindPatternArr(Client.dwBase, Client.dwSize, "x????xx?xxx", 11, 0x5, 0x0, 0x0, 0x0, 0x0, 0xC1, 0xE9, 0x0, 0x39, 0x48, 0x4);
	EntityList = (MManager.Read<DWORD>(EntityListArray + 1) + MManager.Read<BYTE>(EntityListArray + 7)) - Client.dwBase;

	/* Vector Punch */
	DWORD VectorPunchArray = MManager.FindPatternArr(Client.dwBase, Client.dwSize, "xxx????xx????xxxxxx", 19, 0x0F, 0x7E, 0x82, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x82, 0x00, 0x00, 0x00, 0x00, 0x66, 0x0F, 0xD6, 0x45, 0xD0, 0xF3);
	VectorPunch = MManager.Read<DWORD>(VectorPunchArray + 3);

	/* CrosshairID */
	DWORD CrosshairIDArray = MManager.FindPatternArr(Client.dwBase, Client.dwSize, "xxxxxx????????xx????xx????xxx????xx", 35, 0x56, 0x57, 0x8B, 0xF9, 0xC7, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x0D, 0x00, 0x00, 0x00, 0x0, 0x81, 0xF9, 0x00, 0x00, 0x00, 0x0, 0x75, 0x07, 0xA1, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x07);
	CrosshairID = MManager.Read<DWORD>(CrosshairIDArray + 6);

	/* Sensitivity */
	DWORD SensitivityArray = MManager.FindPatternArr(Client.dwBase, Client.dwSize, "xxxx????xx????xxxxxx????xxxxxx?xx", 33, 0x7A, 0x2C, 0x8B, 0x0D, 0, 0, 0, 0, 0x81, 0xF9, 0, 0, 0, 0, 0x75, 0x0A, 0xF3, 0x0F, 0x10, 0x05, 0, 0, 0, 0, 0xEB, 0x0F, 0x8B, 0x01, 0x8B, 0x40, 0, 0xFF, 0xD0);
	Sensitivity = MManager.Read<DWORD>(SensitivityArray + 20) - Client.dwBase;

	/* Glow Object */
	DWORD GlowObjectArray = MManager.FindPatternArr(Client.dwBase, Client.dwSize, "x????xxxxxxxx?", 14, 0xB8, 0, 0, 0, 0, 0xC3, 0xCC, 0xCC, 0xCC, 0x55, 0x8B, 0xEC, 0xA1, 0);
	GlowObject = MManager.Read<DWORD>(GlowObjectArray + 1) - Client.dwBase;

	/* View Angle */
	DWORD ViewAngleArray = MManager.FindPatternArr(Engine.dwBase, Engine.dwSize, "xx????xxxxx????xxxx????xxx", 26, 0x8B, 0x15, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x4D, 0x08, 0x8B, 0x82, 0x00, 0x00, 0x00, 0x00, 0x89, 0x01, 0x8B, 0x82, 0x00, 0x00, 0x00, 0x00, 0x89, 0x41, 0x04);
	ViewAngle = MManager.Read<DWORD>(ViewAngleArray + 11);

	/* EnginePointer */
	DWORD EnginePointerArray = MManager.FindPatternArr(Engine.dwBase, Engine.dwSize, "xxxxxxxx????xxxxxxxxxx????xxxx????xxx", 37, 0xF3, 0x0F, 0x5C, 0xC1, 0xF3, 0x0F, 0x10, 0x15, 0x0, 0x0, 0x0, 0x0, 0x0F, 0x2F, 0xD0, 0x76, 0x04, 0xF3, 0x0F, 0x58, 0xC1, 0xA1, 0x0, 0x0, 0x0, 0x0, 0xF3, 0x0F, 0x11, 0x80, 0x0, 0x0, 0x0, 0x0, 0xD9, 0x46, 0x04);
	EnginePointer = MManager.Read<DWORD>(EnginePointerArray + 22) - Engine.dwBase;

	/* ForceJump */
	DWORD ForceJumpArray = MManager.FindPatternArr(Client.dwBase, Client.dwSize, "xx????xx????xxxxxxxxxxx", 23, 0x89, 0x15, 0x0, 0x0, 0x0, 0x0, 0x8B, 0x15, 0x0, 0x0, 0x0, 0x0, 0xF6, 0xC2, 0x3, 0x74, 0x3, 0x83, 0xCE, 0x8, 0xA8, 0x8, 0xBF);
	ForceJump = MManager.Read<DWORD>(ForceJumpArray + 2) - Client.dwBase;
}

/* Hotkeys and Activation */
void ActivationByHotkeys()
{
	/* Start infinity loop */ /* for(;;) has the same effect of while(true) */
	for (;;)
	{
		/* BHOP Activation */
		/* If the key is down */
		if (GetAsyncKeyState(120) & 0x8000) 
		{
			/* While the key is down wait for un-hold */
			while (GetAsyncKeyState(120) & 0x8000) 
			{
				Sleep(50);
			}
			/* Change variable status */
			BHOP_activation = !BHOP_activation;
			/* Print the status in console */
			if (BHOP_activation) 
			{
				cout << "| BHOP is now ON            |" << endl;
			}
			else 
			{
				cout << "| BHOP is now OFF           |" << endl;
			}
		}
		/* RCS Activation */
		/* If the key is down */
		if (GetAsyncKeyState(119) & 0x8000) 
		{
			/* While the key is down wait for un-hold */
			while (GetAsyncKeyState(119) & 0x8000) 
			{
				Sleep(50);
			}
			/* Change variable status */
			RCS_activation = !RCS_activation;
			/* Print the status in console */
			if (RCS_activation)
			{
				cout << "| RCS is now ON             |" << endl;
			}
			else 
			{
				cout << "| RCS is now OFF            |" << endl;
			}
		}
		/* RADAR Activation */
		/* If the key is down */
		if (GetAsyncKeyState(118) & 0x8000)
		{
			/* While the key is down wait for un-hold */
			while (GetAsyncKeyState(118) & 0x8000)
			{
				Sleep(50);
			}
			/* Change variable status */
			RADAR_activation = !RADAR_activation;
			/* Print the status in console */
			if (RADAR_activation)
			{
				cout << "| RADAR is now ON           |" << endl;
			}
			else
			{
				cout << "| RADAR is now OFF          |" << endl;
			}
		}
		/* SLOWAIM Activation */
		/* If the key is down */
		if (GetAsyncKeyState(117) & 0x8000)
		{
			/* While the key is down wait for un-hold */
			while (GetAsyncKeyState(117) & 0x8000)
			{
				Sleep(50);
			}
			/* Change variable status */
			SLOWAIM_activation = !SLOWAIM_activation;
			/* Print the status in console */
			if (SLOWAIM_activation)
			{
				cout << "| SLOWAIM is now ON         |" << endl;
			}
			else
			{
				cout << "| SLOWAIM is now OFF        |" << endl;
			}
		}
		/* Glow Activation */
		/* If the key is down */
		if (GetAsyncKeyState(116) & 0x8000)
		{
			/* While the key is down wait for un-hold */
			while (GetAsyncKeyState(116) & 0x8000)
			{
				Sleep(50);
			}
			/* Change variable status */
			GLOW_activation = !GLOW_activation;
			/* Print the status in console */
			if (GLOW_activation)
			{
				cout << "| GLOW is now ON            |" << endl;
			}
			else
			{
				cout << "| GLOW is now OFF           |" << endl;
			}
		}
		/* EXIT Key */
		/* If the key is down */
		if (GetAsyncKeyState(115) & 0x8000)
		{
			cout << "|                           |" << endl;
			cout << "|   Press ANY key to EXIT   |" << endl;
			cout << "-----------------------------" << endl;
			Sleep(100);
			exit(0);
		}
		/* Sleep to don't burn memory for nothing */
		Sleep(1);
	}
}

/* GLOWESP */
void GLOW()
{
	/* Start infinity loop */ /* for(;;) has the same effect of while(true) */
	for (;;)
	{
		if (GLOW_activation)
		{
			/* Read Engine base */
			DWORD Engine_dwBase = MManager.Read<DWORD>(Engine.dwBase + EnginePointer);
			/* Read Local Player base */
			DWORD LocalPlayer_dwBase = MManager.Read<DWORD>(Client.dwBase + LocalPlayer);
			/* Read if the player is in-game */
			int LocalPlayer_isConnected = MManager.Read<int>(Engine_dwBase + 0x100);
			
			if (LocalPlayer_isConnected == 6)
			{
				/* Read Glow pointer from the game */
				DWORD Client_glowPointer = MManager.Read<DWORD>(Client.dwBase + GlowObject);
				/* Read online object count in the server */
				int Client_objectCount = MManager.Read<int>(Client.dwBase + GlowObject + 0x4);

				/* Start a loop arround entitys in the server according to things that can glow */
				for (int i = 0; i < Client_objectCount; i++)
				{
					/* Read the entity data by the GlowObjDefinition_t that is defined on MManager.h */
					/* glowObj will be our Entity and pEntity = our entity base, r = red, g = green, b = blue, a = alpha, and render modes */
					GlowObjectPlayer glowObj = MManager.Read<GlowObjectPlayer>(Client_glowPointer + (i * sizeof(GlowObjectPlayer)));

					/* Check if everything is ok and nothing null */
					if (Client_glowPointer != NULL && glowObj.pEntity != NULL)
					{
						/* Read our entity dormant state */
						/* Why glowObj.pEntity? Because we already hae his base from the structure */
						bool EntityPlayer_isDormant = MManager.Read<bool>(glowObj.pEntity + 0xE9);
						/* Read our entity team */
						int EntityPlayer_teamNum = MManager.Read<int>(glowObj.pEntity + 0xF0);
						/* Read our entity classID */
						int ClassID = MManager.Read<int>(MManager.Read<int>(MManager.Read<int>(MManager.Read<int>(glowObj.pEntity + 0x8) + 2 * 0x4) + 0x1) + 20);
						/* Read local player team */
						int LocalPlayer_teamNum = MManager.Read<int>(LocalPlayer_dwBase + 0xF0);

						/* Check if the actual entity is a player and not a chicken or C4 */
						if (ClassID == 35)
						{
							/* Check entity dormant state dormant state */
							if (!EntityPlayer_isDormant)
							{
								/* Check if entity team is different from our team so we can separate colors for team and enemys */
								if (LocalPlayer_teamNum != EntityPlayer_teamNum)
								{
									/* Define the colors based on structure to just make 1 WPM call */
									/* The colors vary between 0.0f and 1.0f they are floats you also can do like this your number in RGB(0 -> 255) and divide it by 255 so 100/255 */
									glowObj.r = 1.0f;
									glowObj.g = 0.0f;
									glowObj.b = 0.0f;
									glowObj.a = 1.0f;
								}
								else
								{
									/* Define the colors based on structure to just make 1 WPM call */
									/* The colors vary between 0.0f and 1.0f they are floats you also can do like this your number in RGB(0 -> 255) and divide it by 255 so 100/255 */
									glowObj.r = 0.0f;
									glowObj.g = 0.0f;
									glowObj.b = 1.0f;
									glowObj.a = 1.0f;
								}
								/* Define our render modes */
								glowObj.RenderWhenOccluded = true;
								glowObj.RenderWhenUnoccluded = false;
								glowObj.FullBloom = false;

								/* Write to the game our new structure to this entity */
								MManager.Write<GlowObjectPlayer>(Client_glowPointer + (i * sizeof(GlowObjectPlayer)), glowObj);
							}
						}
					}
				}
			}
		}
		/* Sleep to don't burn memory for nothing */
		Sleep(25);
	}
}

/* RADAR */
void RADAR()
{
	/* Start infinity loop */ /* for(;;) has the same effect of while(true) */
	for (;;)
	{
		if (RADAR_activation)
		{
			/* Read Engine base */
			DWORD Engine_dwBase = MManager.Read<DWORD>(Engine.dwBase + EnginePointer);
			/* Read Local Player base */
			DWORD LocalPlayer_dwBase = MManager.Read<DWORD>(Client.dwBase + LocalPlayer);
			/* Read if the player is in-game */
			int LocalPlayer_isConnected = MManager.Read<int>(Engine_dwBase + 0x100);

			/* Check if the player is in game */
			if (LocalPlayer_isConnected == 6)
			{
				/* Start a for loop "arround the entitys" */
				for (int i = 0; i < 64; i++)
				{
					/* Read entity base */
					DWORD EntityPlayer_dwBase = MManager.Read<DWORD>(Client.dwBase + EntityList + (0x10 * i));
					/* Read entity in the loop team */
					int EntityPlayer_teamNum = MManager.Read<int>(EntityPlayer_dwBase + 0xF0);
					/* Read local player team */
					int LocalPlayer_teamNum = MManager.Read<int>(LocalPlayer_dwBase + 0xF0);

					/* Check if the entity team is different from our player team */
					if (LocalPlayer_teamNum != EntityPlayer_teamNum)
					{
						/* Read the dormant state from the entity */
						bool EntityPlayer_isDormant = MManager.Read<bool>(EntityPlayer_dwBase + 0xE9);
						/* Read the spotted state from the entity */
						bool EntityPlayer_isSpotted = MManager.Read<bool>(EntityPlayer_dwBase + 0x935);

						/* Check if the entity is not dormant and not spotted */
						if (!EntityPlayer_isDormant && !EntityPlayer_isSpotted)
						{
							/* Write spotted to the entity */
							MManager.Write<bool>(EntityPlayer_dwBase + 0x935, 1);
						}
					}
				}
			}
		}
		/* Sleep to don't burn memory for nothing */
		Sleep(1);
	}
}

/* SLOWAIM */
void SLOWAIM()
{
	/* Save our old player sensitivity */
	float LocalPlayer_sensitivity = MManager.Read<float>(Client.dwBase + Sensitivity);
	/* Start infinity loop */ /* for(;;) has the same effect of while(true) */
	for (;;)
	{
		if (SLOWAIM_activation)
		{
			/* Read Engine base */
			DWORD Engine_dwBase = MManager.Read<DWORD>(Engine.dwBase + EnginePointer);
			/* Read Local Player base */
			DWORD LocalPlayer_dwBase = MManager.Read<DWORD>(Client.dwBase + LocalPlayer);
			/* Read if the player is in-game */
			int LocalPlayer_isConnected = MManager.Read<int>(Engine_dwBase + 0x100);

			/* Check if the player is in game */
			if (LocalPlayer_isConnected == 6)
			{
				/* Read the ID of what is in front of the crosshair */
				/* ID - 1 due 0 = World */
				int LocalPlayer_crosshairID = MManager.Read<int>(LocalPlayer_dwBase + CrosshairID) - 1;
				/* Read local player team */
				int LocalPlayer_teamNum = MManager.Read<int>(LocalPlayer_dwBase + 0xF0);
				/* Read entity base */
				DWORD EntityPlayer_dwBase = MManager.Read<DWORD>(Client.dwBase + EntityList + (0x10 * LocalPlayer_crosshairID));
				/* Read entity in the loop team */
				int EntityPlayer_teamNum = MManager.Read<int>(EntityPlayer_dwBase + 0xF0);
				/* Read the life state from the entity */
				bool EntityPlayer_lifeState = MManager.Read<bool>(EntityPlayer_dwBase + 0x25B);
				/* Read the dormant state from the entity */
				bool EntityPlayer_isDormant = MManager.Read<bool>(EntityPlayer_dwBase + 0xE9);

				/* Verifie if the entity number is between 0 and 64, if the entity team is different from our player team if is not dormant and if is alive */
				if (LocalPlayer_crosshairID > 0 && LocalPlayer_crosshairID < 64 && LocalPlayer_teamNum != EntityPlayer_teamNum && !EntityPlayer_isDormant && !EntityPlayer_lifeState)
				{
					/* Write the new sensitivity */
					MManager.Write<float>(Client.dwBase + Sensitivity, 1.0f);

				}
				else
				{
					/* Write the old sentitivity stored */
					MManager.Write<float>(Client.dwBase + Sensitivity, LocalPlayer_sensitivity);
				}
			}
		}
		/* Sleep to don't burn memory for nothing */
		Sleep(1);
	}
}

/* RCS */
void RCS()
{
	/* Define our OLD PUNCH Vector */
	Vector LocalPlayer_oldVectorPunch;
	/* Start infinity loop */ /* for(;;) has the same effect of while(true) */
	for (;;)
	{
		/* If RCS_activation is TRUE */
		if (RCS_activation)
		{
			/* Read Engine base */
			DWORD Engine_dwBase = MManager.Read<DWORD>(Engine.dwBase + EnginePointer);
			/* Read Local Player base */
			DWORD LocalPlayer_dwBase = MManager.Read<DWORD>(Client.dwBase + LocalPlayer);
			/* Read life state */
			bool LocalPlayer_lifeState = MManager.Read<bool>(LocalPlayer_dwBase + 0x25B);
			/* Read if the player is in-game */
			int LocalPlayer_isConnected = MManager.Read<int>(Engine_dwBase + 0x100);
			/* Read shots fired */
			int LocalPlayer_shotsFired = MManager.Read<int>(LocalPlayer_dwBase + 0xBEB0);

			/* Check if the player is alive and is connected */
			if (!LocalPlayer_lifeState && LocalPlayer_isConnected == 6)
			{
				/* Read the weapon punch */
				/* Vector = 3 floats = x , y , z */ /* Definition on MManager.h */
				/* USAGE OF A VECTOR -> LocalPlayer_vectorPunch.x OR LocalPlayer_vectorPunch.y OR LocalPlayer_vectorPunch.z */
				Vector LocalPlayer_vectorPunch = MManager.Read<Vector>(LocalPlayer_dwBase + VectorPunch);

				/* Take the vector punch and multiply by 2 to get the perfect spot on shots */
				/* Why 2? Because if we just subtract it will restore to the old position only */
				/* Using LocalPlayer_vectorPunch.x *= 2.0f have the SAME effect as this LocalPlayer_vectorPunch.x = LocalPlayer_vectorPunch.x * 2.0f;*/
				/* Vector Z will be ALWAYS 0 due its just a X and Y spot/"graph" */

				/* Here the Punch will be controled by the X and Y using Minimum and Maximum control */
				random_device Random;
				mt19937 RandomGen(Random());
				uniform_real<float>RandomXdistrib(1.6f, 1.8f);
				uniform_real<float>RandomYdistrib(1.6f, 1.8f);
				LocalPlayer_vectorPunch.x *= RandomXdistrib(RandomGen);
				LocalPlayer_vectorPunch.y *= RandomYdistrib(RandomGen);
				LocalPlayer_vectorPunch.z = 0.0f;

				/* Normalize the angles to prevent untrusted */ /* Function included in MManager.h */
				LocalPlayer_vectorPunch = MManager.NormalizeAngles(LocalPlayer_vectorPunch);

				/* Check if the player is shooting */
				if (LocalPlayer_shotsFired > 2)
				{
					/* Get our current view angle */
					Vector LocalPlayer_viewAngle = MManager.Read<Vector>(Engine_dwBase + ViewAngle);
					/* Define a new vector to modifie the angles */
					Vector LocalPlayer_modifiedAngle = LocalPlayer_vectorPunch;

					/* Subtract the Old punch from our new punch */
					LocalPlayer_modifiedAngle.x -= LocalPlayer_oldVectorPunch.x;
					LocalPlayer_modifiedAngle.y -= LocalPlayer_oldVectorPunch.y;
					LocalPlayer_modifiedAngle.z = 0.0f;

					/* Normalize the angles to prevent untrusted */ /* Function included in MManager.h */
					LocalPlayer_modifiedAngle = MManager.NormalizeAngles(LocalPlayer_modifiedAngle);					

					/* Subtract our new punch result from our view angle */
					LocalPlayer_viewAngle.x -= LocalPlayer_modifiedAngle.x;
					LocalPlayer_viewAngle.y -= LocalPlayer_modifiedAngle.y;
					LocalPlayer_viewAngle.z = 0.0f;

					/* Normalize the angles to prevent untrusted */ /* Function included in MManager.h */
					LocalPlayer_viewAngle = MManager.NormalizeAngles(LocalPlayer_viewAngle);

					/* Set the new View Angle to the player */
					MManager.Write<Vector>(Engine_dwBase + ViewAngle, LocalPlayer_viewAngle);
				}
				/* Store our new punch on old punch */
				LocalPlayer_oldVectorPunch = LocalPlayer_vectorPunch;
			}
		}
		/* Sleep to don't burn memory for nothing */
		Sleep(1);
	}
}

/* BHOP */
void BHOP()
{
	/* Start infinity loop */ /* for(;;) has the same effect of while(true) */
	for (;;)
	{
		/* If BHOP_activation is TRUE */
		if (BHOP_activation)
		{
			/* Read Local Player base */
			DWORD LocalPlayer_dwBase = MManager.Read<DWORD>(Client.dwBase + LocalPlayer);
			/* Read Local Player flag */
			int LocalPlayer_fFlag = MManager.Read<int>(LocalPlayer_dwBase + 0x100);

			/* Check if the actual player is hold space [Jump Key] */
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			{
				/* Check the flag and see if the player is on_ground */
				if (LocalPlayer_fFlag & (1 << 0))
				{
					/* Write to the memory that the player is actually "holding space" */
					MManager.Write<int>(Client.dwBase + ForceJump, 5);
					/* Sleep */
					Sleep(20);
					/* Write to the memory that the player actually "un-hold space" */
					MManager.Write<int>(Client.dwBase + ForceJump, 4);
				}
			}
		}
		/* Sleep to don't burn memory for nothing */
		Sleep(1);
	}
} 

/* Re-Size Console function */
void SetWindow(int Width, int Height, int x, int y)
{
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;

	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle
	SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size
	SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size

	HWND hWnd = GetConsoleWindow();
	RECT rcScr, rcWnd, rcClient;

	GetWindowRect(hWnd, &rcWnd);
	GetWindowRect(GetDesktopWindow(), &rcScr);
	GetClientRect(hWnd, &rcClient);

	MoveWindow(hWnd, x, y, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top, 1);
	SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
	SetWindowRgn(hWnd, CreateRectRgn(rcClient.left + 2, rcClient.top + 2, rcClient.right + 2, rcClient.bottom + 2), TRUE);
	ShowWindow(hWnd, 1);
}

/* Move Console function */
void MoveConsole()
{
	/* Define our varaible to see if is able to back to old size */
	bool re_size = false;

	/* Start infinity loop */ /* for(;;) has the same effect of while(true) */
	for (;;)
	{
		/* See if the keys are a combination of LEFT_SHIFT and ARROW_UP/DOWN/LEFT/RIGHT */
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			ConsolePos_x += 1;
			SetWindow(30, 30, ConsolePos_x, ConsolePos_y);
			re_size = true;
		}
		else if (GetAsyncKeyState(VK_LEFT) & 0x8000 && GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			ConsolePos_x -= 1;
			SetWindow(30, 30, ConsolePos_x, ConsolePos_y);
			re_size = true;
		}
		else if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			ConsolePos_y -= 1;
			SetWindow(30, 30, ConsolePos_x, ConsolePos_y);
			re_size = true;
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			ConsolePos_y += 1;
			SetWindow(30, 30, ConsolePos_x, ConsolePos_y);
			re_size = true;
		}
		else if (re_size)
		{
			SetWindow(29, 30, ConsolePos_x, ConsolePos_y);
			re_size = false;
		}
		/* Sleep to don't burn memory for nothing */
		Sleep(1);
	}
}

/* Start code */
void main()
{
	/* Rename Console */
	SetConsoleTitle(" XTools Multi-Hack");
	SetWindow(29, 30, ConsolePos_x, ConsolePos_y);

	/* Console output */
	cout << "-----------------------------" << endl;
	cout << "|      XTools Multi Hack    |" << endl;
	cout << "|                           |" << endl;
	cout << "| Hotkeys:                  |" << endl;
	cout << "|        -> BunnyHop: F9    |" << endl;
	cout << "|        ->      RCS: F8    |" << endl;
	cout << "|        ->    Radar: F7    |" << endl;
	cout << "|        ->  SlowAIM: F6    |" << endl;
	cout << "|        ->  GlowESP: F5    |" << endl;
	cout << "|        ->     EXIT: F4    |" << endl;
	cout << "-----------------------------" << endl;

	/* Attach to the GAME process */
	if (!MManager.Attach("csgo.exe"))
	{
		cout << "-----------------------------" << endl;
		cout << "|         :WARNING:         |" << endl;
		cout << "|     CS:GO NOT FOUND!      |" << endl;
		cout << "|     CLOSING IN 3 SEC      |" << endl;
		cout << "-----------------------------" << endl;
		Sleep(1000 * 3);
		exit(0);
	}
	cout << "-----------------------------" << endl;
	cout << "|       :ACTIVATION:        |" << endl;
	cout << "|                           |" << endl;

	/* Get client.dll and engine.dll */
	Client = MManager.GetModule("client.dll");
	Engine = MManager.GetModule("engine.dll");

	/* Find all the offsets that the code need using pattern scan */
	PatternScan();

	/* Threads creation for multiple functions at the same time */
	thread ACTIVATION_thread = thread(ActivationByHotkeys);
	thread CONSOLEMOVE_thread = thread(MoveConsole);
	thread GLOW_thread = thread(GLOW);
	thread RADAR_thread = thread(RADAR);
	thread SLOWAIM_thread = thread(SLOWAIM);
	thread BHOP_thread = thread(BHOP);
	thread RCS_thread = thread(RCS);

	/* Start threads */
	ACTIVATION_thread.join();
	CONSOLEMOVE_thread.join();
	GLOW_thread.join();
	RADAR_thread.join();
	SLOWAIM_thread.join();
	BHOP_thread.join();
	RCS_thread.join();
}