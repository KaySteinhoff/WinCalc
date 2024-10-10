#include <windows.h>
#include <cstring.h>
#include <lexer.h>
#include <parser.h>
#include <stdio.h>

static const int screenWidth = 260;
static const int screenHeight = 440;

static const int numpadOffsetX = 0;
static const int numpadOffsetY = 100;

RECT rect = { .top = 0, .left = 0, .bottom = 100, .right = 250};
string_t equation;
int bnt_id = 0, equationIsResult = 0;
TokenList list;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const char g_szClassName[] = "GameWindow";
	equation = newString("", 0);

    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
	
	HICON icon = (HICON)LoadImage(NULL, "CalcIcon.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE | LR_LOADTRANSPARENT | LR_SHARED);

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = icon;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = icon;

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        0,
        g_szClassName,
        "Calculator",
        WS_SYSMENU,
        0, 0, screenWidth, screenHeight,
        NULL, NULL, hInstance, NULL);

    if(!hwnd)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
	
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
		
    return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	
    switch(msg)
    {
		case WM_CREATE:
			//Create numberpad
			if(!CreateWindowExA(0, "BUTTON", "0", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 70, numpadOffsetY + 250, 50, 50, hwnd, (HMENU)0, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "1", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 10, numpadOffsetY + 190, 50, 50, hwnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "2", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 70, numpadOffsetY + 190, 50, 50, hwnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "3", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 130, numpadOffsetY + 190, 50, 50, hwnd, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "4", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 10, numpadOffsetY + 130, 50, 50, hwnd, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "5", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 70, numpadOffsetY + 130, 50, 50, hwnd, (HMENU)5, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "6", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 130, numpadOffsetY + 130, 50, 50, hwnd, (HMENU)6, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "7", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 10, numpadOffsetY + 70, 50, 50, hwnd, (HMENU)7, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "8", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 70, numpadOffsetY + 70, 50, 50, hwnd, (HMENU)8, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "9", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 130, numpadOffsetY + 70, 50, 50, hwnd, (HMENU)9, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			
			//Create Ans, '.', '(', ')', '+', etc.
			if(!CreateWindowExA(0, "BUTTON", ".", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 130, numpadOffsetY + 250, 50, 50, hwnd, (HMENU)10, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "Ans", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 10, numpadOffsetY + 250, 50, 50, hwnd, (HMENU)19, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);

			if(!CreateWindowExA(0, "BUTTON", "(", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 10, numpadOffsetY + 10, 50, 50, hwnd, (HMENU)11, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", ")", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 70, numpadOffsetY + 10, 50, 50, hwnd, (HMENU)12, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "Del", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 130, numpadOffsetY + 10, 50, 50, hwnd, (HMENU)17, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);

			if(!CreateWindowExA(0, "BUTTON", "+", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 190, numpadOffsetY + 10, 50, 50, hwnd, (HMENU)13, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "-", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 190, numpadOffsetY + 70, 50, 50, hwnd, (HMENU)14, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "*", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 190, numpadOffsetY + 130, 50, 50, hwnd, (HMENU)15, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);
			if(!CreateWindowExA(0, "BUTTON", "/", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 190, numpadOffsetY + 190, 50, 50, hwnd, (HMENU)16, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);

			if(!CreateWindowExA(0, "BUTTON", "=", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, numpadOffsetX + 190, numpadOffsetY + 250, 50, 50, hwnd, (HMENU)18, ((LPCREATESTRUCT)lParam)->hInstance, NULL))
				MessageBox(NULL, "Button creation", "Error!", MB_ICONEXCLAMATION | MB_OK);

			break;
		case WM_CHAR:
			switch(wParam)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case '+':
				case '-':
				case '*':
				case '/':
				case '.':
				case '(':
				case ')':
				case '[':
				case ']':
				case '{':
				case '}':
					if(equationIsResult)
					{
						clearString(&equation);
						equationIsResult = 0;
					}
					stringPushBack(&equation, (char)wParam);
					InvalidateRect(hwnd, &rect, TRUE);
					break;
			}
			break;
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_BACK:
					stringPopBack(&equation);
					InvalidateRect(hwnd, &rect, TRUE);
					break;
				case VK_RETURN:
					list = LexEquation(equation);
					if(!list.tokens)
						break;
					Node *root = ParseTokens(list);
					if(!root)
						break;
					clearString(&equation);
					snprintf(equation.c_str, equation.capacity-1, "%g", ResolveNode(root));
					equation.length = strlen(equation.c_str);
					free(list.tokens);
					freeNode(root);
					InvalidateRect(hwnd, &rect, TRUE);
					equationIsResult = 1;
					break;
			}
			break;
		case WM_COMMAND:
			if(!(HWND)lParam || HIWORD(wParam) != BN_CLICKED)
				break;
			//MessageBox(NULL, "Button pressed!", "", MB_ICONEXCLAMATION | MB_OK);
			if(equationIsResult)
			{
				clearString(&equation);
				equationIsResult = 0;
			}
			
			bnt_id = LOWORD(wParam);
			switch(bnt_id)
			{
				case 0:
					stringPushBack(&equation, '0');
					break;
				case 1:
					stringPushBack(&equation, '1');
					break;
				case 2:
					stringPushBack(&equation, '2');
					break;
				case 3:
					stringPushBack(&equation, '3');
					break;
				case 4:
					stringPushBack(&equation, '4');
					break;
				case 5:
					stringPushBack(&equation, '5');
					break;
				case 6:
					stringPushBack(&equation, '6');
					break;
				case 7:
					stringPushBack(&equation, '7');
					break;
				case 8:
					stringPushBack(&equation, '8');
					break;
				case 9:
					stringPushBack(&equation, '9');
					break;
				case 10:
					stringPushBack(&equation, '.');
					break;
				case 11:
					stringPushBack(&equation, '(');
					break;
				case 12:
					stringPushBack(&equation, ')');
					break;
				case 13:
					stringPushBack(&equation, '+');
					break;
				case 14:
					stringPushBack(&equation, '-');
					break;
				case 15:
					stringPushBack(&equation, '*');
					break;
				case 16:
					stringPushBack(&equation, '/');
					break;
				case 17:
					stringPopBack(&equation);
					break;
				case 18:
					list = LexEquation(equation);
					if(!list.tokens)
						break;
					Node *root = ParseTokens(list);
					if(!root)
						break;
					clearString(&equation);
					snprintf(equation.c_str, equation.capacity-1, "%g", ResolveNode(root));
					equation.length = strlen(equation.c_str);
					free(list.tokens);
					freeNode(root);
					equationIsResult = 1;
					break;
				default:
					MessageBox(NULL, "Invalid id", "", MB_ICONEXCLAMATION | MB_OK);
					break;
			}
			InvalidateRect(hwnd, &rect, TRUE);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			DrawText(hdc, equation.c_str, equation.length, &rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
			EndPaint(hwnd, &ps);
			break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
			break;
        case WM_DESTROY:
            PostQuitMessage(0);
			break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}