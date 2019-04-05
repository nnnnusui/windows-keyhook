#include <windows.h>
#include <iostream>

using namespace std;

int KEY_CONVERT =  28;
int KEY_CAMMA   = 188;

HHOOK _hook_keyboard;
KBDLLHOOKSTRUCT kbdStruct;

/*
 *
*/

LRESULT __stdcall HookCallbackKeyboard(int nCode, WPARAM wParam, LPARAM lParam)
{
  if (nCode >= 0)
  {
    kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
    if(wParam == WM_KEYDOWN){
      int keyNo = kbdStruct.vkCode;
      char c = MapVirtualKey(kbdStruct.vkCode, 2);
      cout << "keyCode: " << keyNo << endl;

      INPUT ip;
      ip.type = INPUT_KEYBOARD;
      ip.ki.wScan = 0;
      ip.ki.time = 0;
      ip.ki.dwExtraInfo = 0;
      ip.ki.dwFlags = 0;

      if(keyNo == VK_NONCONVERT) {
      	cout << "convert." << endl;
        ip.ki.wVk = VK_OEM_PERIOD;
    		SendInput(1, &ip, sizeof(INPUT));
    		ip.ki.dwFlags = KEYEVENTF_KEYUP;
    		SendInput(1, &ip, sizeof(INPUT));
        return 1;
      } else if(keyNo == VK_CONVERT) {
      	cout << "noconvert." << endl;
        ip.ki.wVk = VK_OEM_COMMA;
    		SendInput(1, &ip, sizeof(INPUT));
    		ip.ki.dwFlags = KEYEVENTF_KEYUP;
    		SendInput(1, &ip, sizeof(INPUT));
        return 1;
      }
    }
  }
  return CallNextHookEx(_hook_keyboard, nCode, wParam, lParam);
}

void SetHook()
{
    if (!(_hook_keyboard = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallbackKeyboard, NULL, 0)))
    {
        MessageBox(NULL, "Failed to install hook on keyboard!", "Error", MB_ICONERROR);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetHook();
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}




// https://causeyourestuck.io/2016/04/20/keyboard-hook-win32api-2/
