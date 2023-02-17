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

static double sannsynligVest = 0.10;
static double sannsynligNord = 0.10;

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

void drawRoad(HDC hdc) { // Tegner veien
	HGDIOBJ	hOrg = SelectObject(hdc, hGreyBrush);
	Rectangle(hdc, 0, 300, 1500, 400);
	Rectangle(hdc, 600, 0, 700, 700);;
}

void drawCars(HDC hdc) { // Tegner alle bilene i vektoren
	HGDIOBJ	hOrg = SelectObject(hdc, hBlackBrush);
	for (Car& c : cars) {
		Rectangle(hdc, c.getLeft(), c.getTop(), c.getRight(), c.getBottom());
	}

}

bool checkAvailable(Car c1) { //Sjekker om det er ledig plass til ny bil 
	for (auto& testPos : available) {
		if (c1.getDirection() == 0) {
			if ((c1.getLeft() + 50) == testPos.second.second) {
				return false;
			}
		}
		else {
			if (c1.getBottom() + 50 == testPos.second.second) {
				return false;
			}
		}
	}
	return true;
}

void makeCar(int direction) { // Funksjon som legger en bil inn i Car vectoren enten fra retning nord eller vest
	Car car = Car(carNr);
	if (direction == 0) {
		car.setLeft(0);
		car.setTop(360);
		car.setRight(40);
		car.setBottom(340);
		car.setDirection(0);
	}
	if (direction == 1) {
		car.setLeft(620);
		car.setTop(0);
		car.setRight(640);
		car.setBottom(40);
		car.setDirection(1);
	}
	cars.push_back(car);
	carNr++;
}

void updateCars() { // Funksjon som oppdaterer posisjonen til bilene
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
		SetTimer(hWnd,
			1,            // timer id
			10000,        // 10 sekunds intervall
			(TIMERPROC)NULL);

		SetTimer(hWnd,
			2,            // timer id
			100,        // 1 sekunds intervall
			(TIMERPROC)NULL);

		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HBRUSH hBrush;
		HGDIOBJ hOrg;

		drawRoad(hdc); // Tegner veien
		drawCars(hdc); // Tegner bilene

		// Part 1
		switch (lightState) { // Switch som bestemmer fargen på trafikklysene
		case 0:
			// Lys 1
			SelectObject(hdc, hBlackBrush);
			Rectangle(hdc, 100, 0, 200, 299);

			SelectObject(hdc, hRedBrush);
			Ellipse(hdc, 105, 0, 195, 100);

			SelectObject(hdc, hGreyBrush);
			Ellipse(hdc, 105, 100, 195, 200);
			Ellipse(hdc, 105, 199, 195, 299);

			// Lys 2
			SelectObject(hdc, hBlackBrush);
			Rectangle(hdc, 710, 0, 810, 299);

			SelectObject(hdc, hGreenBrush);
			Ellipse(hdc, 715, 199, 805, 299);

			SelectObject(hdc, hGreyBrush);
			Ellipse(hdc, 715, 100, 805, 200);
			Ellipse(hdc, 715, 0, 805, 100);

			break;
		case 1:
			// Lys 1
			SelectObject(hdc, hBlackBrush);
			Rectangle(hdc, 100, 0, 200, 299);

			SelectObject(hdc, hRedBrush);
			Ellipse(hdc, 105, 0, 195, 100);

			SelectObject(hdc, hYellowBrush);
			Ellipse(hdc, 105, 100, 195, 200);

			SelectObject(hdc, hGreyBrush);
			Ellipse(hdc, 105, 199, 195, 299);

			// Lys 2
			SelectObject(hdc, hBlackBrush);
			Rectangle(hdc, 710, 0, 810, 299);

			SelectObject(hdc, hYellowBrush);
			Ellipse(hdc, 715, 100, 805, 200);

			SelectObject(hdc, hGreyBrush);
			Ellipse(hdc, 715, 0, 805, 100);
			Ellipse(hdc, 715, 199, 805, 299);
			break;
		case 2:
			// Lys 1
			SelectObject(hdc, hBlackBrush);
			Rectangle(hdc, 100, 0, 200, 299);

			SelectObject(hdc, hGreenBrush);
			Ellipse(hdc, 105, 199, 195, 299);

			SelectObject(hdc, hGreyBrush);
			Ellipse(hdc, 105, 0, 195, 100);
			Ellipse(hdc, 105, 100, 195, 200);

			// Lys 2
			SelectObject(hdc, hBlackBrush);
			Rectangle(hdc, 710, 0, 810, 299);

			SelectObject(hdc, hRedBrush);
			Ellipse(hdc, 715, 0, 805, 100);

			SelectObject(hdc, hGreyBrush);
			Ellipse(hdc, 715, 100, 805, 200);
			Ellipse(hdc, 715, 199, 805, 299);
			break;
		case 3:
			// Lys 1
			SelectObject(hdc, hBlackBrush);
			Rectangle(hdc, 100, 0, 200, 299);

			SelectObject(hdc, hYellowBrush);
			Ellipse(hdc, 105, 100, 195, 200);

			SelectObject(hdc, hGreyBrush);
			Ellipse(hdc, 105, 0, 195, 100);
			Ellipse(hdc, 105, 199, 195, 299);

			//Lys 2
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
		makeCar(0); // Skal lage en bil fra vest
		break;

	case WM_RBUTTONDOWN:
		makeCar(1); // Skal lage en bil fra nord
		break;

		//Part 3 WM_TIMER case which uses previous on mouse click logic
	case WM_TIMER:
		double val, val1; //Lager 2 verdier
		switch (wParam)
		{
		case 1:
			// Part 1
			lightState = (lightState + 1) % 4; // Går gjennom de 4 forsjellige lys "statene" hver gang timer 1 "proccer"
			InvalidateRect(hWnd, NULL, false); // Tegner vidunet på nytt
			break;
		case 2:
			updateCars(); // Oppdaterer posisjonen til bilene
			val = (double)rand() / RAND_MAX; //gir val en tilfeldig verdi mellom 0 og 1
			if (val < sannsynligNord) { // Dersom verdien er mindre enn sannsynet for bil fra nord blir det laget en bil fra nord
				makeCar(1);
			}
			val1 = (double)rand() / RAND_MAX;
			if (val1 < sannsynligVest) { // samme som val men med vest
				makeCar(0);
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		default: break;
		}
		break;
	case WM_DESTROY:
		// Rydder opp i noen brushes
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