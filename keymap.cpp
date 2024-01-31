// Remap capslock key to ctrl
// compiled by the command:
// cl keymap.cpp user32.lib

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

HHOOK hHook = 0;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // Only run when a keyboard message arrives (nCode = HC_ACTION 0)
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*) lParam;
        if (p->vkCode == VK_CAPITAL) // VK_CAPITAL = 0x14 CAPS LOCK key
        {
           switch (wParam){

            case WM_KEYDOWN :{ // use KEYDOWN for non ALT keys
                // cout << "Key down" << endl;

                keybd_event(VK_LCONTROL, 0x1D, KEYEVENTF_EXTENDEDKEY | 0, 0 );
            break;
            }
            case WM_KEYUP: // use regular keyup
             {
                // cout << "Key up" << endl;

                keybd_event( VK_LCONTROL, 0x1D, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
                return 1;

            break;
             }
            default:
                wParam = WM_SYSKEYDOWN;  //ALT key is down
                break;
           }
            return 1;
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
      
   // WH_KEYBOARD_LL: global hook for low-level keyboard input events
   hHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, 
            hThisInstance, 0);
            
    if (hHook == 0)
    {
        _tprintf(TEXT("Key remap failed"));
        return 1;
    }
    else
    {
        _tprintf("Key remapped successfully: %d\n",hHook);        
    }

    // Keep running in the background
    // GetMessage sleeps until there's a message in the queue
    
    MSG messages;

    while (GetMessage (&messages, NULL, 0, 0) >0 )
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }  

    return messages.wParam;
}         
