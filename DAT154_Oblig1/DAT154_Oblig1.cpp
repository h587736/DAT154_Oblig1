#include "framework.h"
#include "DAT154_Oblig1.h"
#include "Car.h"
#include <vector>
#include <map>
#include <iterator>
#define MAX_LOADSTRING 100



// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
static HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
static HBRUSH hYellowBrush = CreateSolidBrush(RGB(255, 255, 0));
static HBRUSH hGreenBrush = CreateSolidBrush(RGB(50, 205, 50));
static HBRUSH hGreyBrush = CreateSolidBrush(RGB(128, 128, 128));
void TegnSannsynlighet(HDC, double, double);

void drawCars(HDC);
void drawRoad(HDC);
bool checkAvailable(Car);
using namespace		std;
static vector<Car> cars;
int	carNr = 0;
static int lightState = 0;
static map <int, pair < int, int>> available;

static double		sannsynligVest = 0.20;
static double		sannsynligNord = 0.20;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DAT154OBLIG1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DAT154OBLIG1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DAT154OBLIG1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DAT154OBLIG1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable


    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);


    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void TegnSannsynlighet(HDC hdc, double nord, double vest) {
	
	
	TCHAR text[256];
	swprintf_s(text, 256, L"Sannsynlighet Vest: %lf", vest);
	TextOut(hdc, 10, 70, text, wcslen(text));
	swprintf_s(text, 256, L"Sannsynlighet Nord: %lf", nord);
	TextOut(hdc, 10, 85, text, wcslen(text));

}

void drawRoad(HDC hdc) {
    HBRUSH hBrush = CreateSolidBrush(RGB(128, 128, 128));;
    HGDIOBJ	hOrg = SelectObject(hdc, hBrush);
    Rectangle(hdc, 0, 300, 1500, 400);
    Rectangle(hdc, 600, 0, 700, 700);
    DeleteObject(hBrush);
    DeleteObject(hOrg);
}

void drawCars(HDC hdc) {
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    HGDIOBJ	hOrg = SelectObject(hdc, hBrush);

    for (Car& c : cars) {
        Rectangle(hdc, c.getLeft(), c.getTop(), c.getRight(), c.getBottom());
    }

    DeleteObject(hBrush);
    DeleteObject(hOrg);
}

bool checkAvailable(Car c1) {
    for (auto &testPos : available) {
        if (c1.getDirection() == 0) {
            if ((c1.getLeft() + 40) == testPos.second.second) {
                return false;
            }
        }
        else {
            if (c1.getBottom() + 40 == testPos.second.second) {
                return false;
            }
        }
    }
    return true;
}

void makeCar(int direction)
{
    int top, left, right, bottom;
    if (direction == 1) { //North
        top = SCREEN_HEIGHT;
        left = CAR_NORTH_START;
        right = left + CAR_WIDTH;
        bottom = top + CAR_HEIGHT;
    }
    else { //West
        left = SCREEN_WIDTH;
        top = CAR_WEST_START;
        bottom = top + CAR_WIDTH;
        right = left + CAR_HEIGHT;
    }

    // Create a new car and set its properties
    Car* newCar = new Car(carList.size());
    newCar->setTop(top);
    newCar->setLeft(left);
    newCar->setRight(right);
    newCar->setBottom(bottom);
    newCar->setDirection(direction);

    // Add the new car to the list
    carList.push_back(newCar);
}

    // Add new car to the list
    cars.push_back(newCar);
}

void updateCars() {
    for (Car& c : cars) {
        if (checkAvailable(c)) {
            if (c.getDirection() == 0) {
                pair<int, int> par = pair<int, int>(c.getRight(), c.getBottom());
                available.insert(pair<int, pair<int, int>>(c.getNr(), par));

                if (!(c.getRight() == 440) || lightState == 2 || lightState == 3) {

                    c.setLeft(c.getLeft() + 10);
                    c.setRight(c.getRight() + 10);
                }
            }
            if (c.getDirection() == 1) {
                if (!(c.getBottom() == 190) || lightState == 1 || lightState == 0) {
                    c.setTop(c.getTop() + 10);
                    c.setBottom(c.getBottom() + 10);
                }
            }
        }
        map <int, pair < int, int>>::iterator itit = available.find(c.getNr());
        if (itit != available.end()) {
            available.erase(itit);
        }

    }
    auto it = cars.begin();
    while (it != cars.end()) {
        if (it->getRight() == 1000 || it->getBottom() == 600) {
            it = cars.erase(it);
        }
        else {
            ++it;
        }
    }

}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{ 

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE:
        // Part 3
        SetTimer(hWnd,             // handle to main window
            1,            // timer identifier
            10000,                 // 10-second interval
            (TIMERPROC)NULL);     // no timer callback

        SetTimer(hWnd,             // handle to main window
            2,            // timer identifier
            1000,                 // 1-second interval
            (TIMERPROC)NULL);     // no timer callback

        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HBRUSH hBrush;
        HGDIOBJ hOrg;

        drawRoad(hdc);
        drawCars(hdc);

        // Part 1
        switch (lightState) {
        case 0:
            // Part 1
            SelectObject(hdc, hBlackBrush);
            Rectangle(hdc, 100, 0, 200, 299);
          
            SelectObject(hdc, hRedBrush);
            Ellipse(hdc, 105, 0, 195, 100);

            SelectObject(hdc, hGreyBrush);
            Ellipse(hdc, 105, 100, 195, 200);
            Ellipse(hdc, 105, 199, 195, 299);

        // Part 2
            SelectObject(hdc, hBlackBrush);
            Rectangle(hdc, 710, 0, 810, 299);

            SelectObject(hdc, hGreenBrush);
            Ellipse(hdc, 715, 199, 805, 299);

            SelectObject(hdc, hGreyBrush);
            Ellipse(hdc, 715, 100, 805, 200);
            Ellipse(hdc, 715, 0, 805, 100);
            break;
        case 1:
            // Part 1
            SelectObject(hdc, hBlackBrush);
            Rectangle(hdc, 100, 0, 200, 299);

            SelectObject(hdc, hRedBrush);
            Ellipse(hdc, 105, 0, 195, 100);

            SelectObject(hdc, hYellowBrush);
            Ellipse(hdc, 105, 100, 195, 200);

            SelectObject(hdc, hGreyBrush);
            Ellipse(hdc, 105, 199, 195, 299);

            // Part 2
            SelectObject(hdc, hBlackBrush);
            Rectangle(hdc, 710, 0, 810, 299);

            SelectObject(hdc, hYellowBrush);
            Ellipse(hdc, 715, 100, 805, 200);

            SelectObject(hdc, hGreyBrush);
            Ellipse(hdc, 715, 0, 805, 100);
            Ellipse(hdc, 715, 199, 805, 299);
            break;
        case 2:
            // Part 1
            SelectObject(hdc, hBlackBrush);
            Rectangle(hdc, 100, 0, 200, 299);

            SelectObject(hdc, hGreenBrush);
            Ellipse(hdc, 105, 199, 195, 299);

            SelectObject(hdc, hGreyBrush);
            Ellipse(hdc, 105, 0, 195, 100);
            Ellipse(hdc, 105, 100, 195, 200);

            // Part 2
            SelectObject(hdc, hBlackBrush);
            Rectangle(hdc, 710, 0, 810, 299);

            SelectObject(hdc, hRedBrush);
            Ellipse(hdc, 715, 0, 805, 100);

            SelectObject(hdc, hGreyBrush);
            Ellipse(hdc, 715, 100, 805, 200);
            Ellipse(hdc, 715, 199, 805, 299);
            break;
        case 3:
            // Part 1
            SelectObject(hdc, hBlackBrush);
            Rectangle(hdc, 100, 0, 200, 299);

            SelectObject(hdc, hYellowBrush);
            Ellipse(hdc, 105, 100, 195, 200);

            SelectObject(hdc, hGreyBrush);
            Ellipse(hdc, 105, 0, 195, 100);
            Ellipse(hdc, 105, 199, 195, 299);

            //Part 2
            SelectObject(hdc, hBlackBrush);
            Rectangle(hdc, 710, 0, 810, 299);

            SelectObject(hdc, hRedBrush);
            Ellipse(hdc, 715, 0, 805, 100);

            SelectObject(hdc, hYellowBrush);
            Ellipse(hdc, 715, 100, 805, 200);

            SelectObject(hdc, hGreyBrush);
            Ellipse(hdc, 715, 199, 805, 299);
            break;
        default: 
                break;
        }
        EndPaint(hWnd, &ps);

    }
    break;
    case WM_LBUTTONDOWN:
        /* previous part 1
        lightState = (lightState + 1) % 4; // Cycle through the states 0-1-2-3-0...
        InvalidateRect(hWnd, NULL, TRUE); // Redraw the window to reflect the new state
         */
        // Part 4
        makeCar(1);
        break;

        //Part 3 WM_TIMER case which uses previous on mouse click logic
    case WM_TIMER:
        double val, val1;
        switch (wParam)
        {
        case 1:
            // Part 1
            lightState = (lightState + 1) % 4; // Cycle through the states 0-1-2-3-0...
            InvalidateRect(hWnd, NULL, false); // Redraw the window to reflect the new state
            break;
        case 2:
            updateCars();
            val = (double)rand() / RAND_MAX;
            if (val < sannsynligNord) {
                makeCar(1);
            }
            val1 = (double)rand() / RAND_MAX;
            if (val1 < sannsynligVest) {
                makeCar(0);
            }
            InvalidateRect(hWnd, NULL, false);
            break;
        default: break;
        }
        break;
    case WM_DESTROY:
        DeleteObject(hBlackBrush);
        DeleteObject(hGreyBrush);
        DeleteObject(hRedBrush);
        DeleteObject(hYellowBrush);
        DeleteObject(hGreenBrush);
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}