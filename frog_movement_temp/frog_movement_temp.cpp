#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include <stdlib.h>
#include <time.h>

void collision_check(int item_index);
void item_print(HDC hdc_f, HDC memdc_f, int r_f, HBITMAP Items, HBITMAP Items_B);

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
	// UNICODE 사용시 wWinMain() 형태 	
	// hPrevInstance 이전 인스턴스 항상 0값
	// lpszCmdLine > 외부에서 (내부로) 입력받는 변수
	// nCmdShow 윈도우 출력 형태에 관련한 값
{
	HWND     hwnd;
	MSG		 msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;	//height, vertical redraw
	WndClass.lpfnWndProc = WndProc;		// Proc 설정
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);	// 형변환
	WndClass.lpszMenuName = MAKEINTRESOURCE(109);
	// resource.h 파일 에서 메뉴이름 확인 109번
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);		// WndClass 등록
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("2018253004_박상현"),	// 타이틀바, 학번이름 체크
		WS_OVERLAPPEDWINDOW,		// 윈도우 스타일
		600, 400,					// 창출력좌표 x, y 
		1050, 650,					// 창크기 x, y축
		NULL,						// 부모 윈도우
		NULL,						// 메뉴바 핸들
		hInstance,					// 인스턴스
		NULL						// 여분, NULL
	);
	ShowWindow(hwnd, nCmdShow);		// 윈도우 출력, WM_PAINT 출력내용 가져옴
	UpdateWindow(hwnd);				// WM_PAINT 출력내용 발생해서 출력하도록
									// hwnd 핸들을 통해 보여주고 갱신

	//ShowWindow(hwnd, SW_SHOW);	// 위와 같음
	//UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))	// 메시지 큐의 메시지를 가져옴
	{
		TranslateMessage(&msg);		// 키입력에 반응하는 메시지 변환, WM_KEYDOWN (키가 눌릴때) WM_CHAR 메시지 발생
		DispatchMessage(&msg);		// WndProc() 함수 호출과 WndProc()으로 메세지 전달
	}								// 종료는 WM_QUIT 발생할때 FALSE 리턴하면서 종료
	return (int)msg.wParam;			// wParam, lParam 윈도우 크기가 어떻게 변했는지, 변경된 클라이언트, 키보드, 마우스 값

}

static int speed, score;
int life;
static bool reverse, stunn, protection;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
	//	WinDef.h 에서 정의
	//	wPram > unsigned ptr, lParam > long ptr 
{
	HDC hdc, memdc, memdc1, memdc2;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap13, hBitmap46, hBitmap79, hBitmap1012, hBitmap_TONGUE_L1_L, hBitmap_TONGUE_L1_R,
		hBitmap_GameOver, hBitmap13B, hBitmap46B, hBitmap79B, hBitmap1012B, hBitmap_TONGUE_L1_LB,
		hBitmap_TONGUE_L1_RB, hBitmap_GameOverB;
	BITMAP bit;
	static HBITMAP old_bitmap, life_bitmap;
	static int x = 100, y = 200;
	static bool flag[5];
	static bool item = FALSE;
	static char direct = 'n', space = 'n', old_direct = 'n';
	static int xPos;

	static HBITMAP RunBit_L[3], RunBit_R[3], RunBit_LB[3], RunBit_RB[3];
	static int count;
	int  i;
	int bx, by, ix, iy;

	static HBITMAP hBit1, hBit2, oldBit1, oldBIt2, Item_List[10], Item_List_B[10];

	static RECT box1, box2, temp;
	static RECT rc;

	static bool get = FALSE;

	static int item_x, item_y, item_num, r;
	static BOOL drop = FALSE;

	static HBITMAP old_item[10], old_item_B[10];
	TCHAR str[128];
	int delay_counter = 0;

	switch (iMsg) {
	case WM_CREATE:
		speed = 10, score = 0;
		life = 5;
		reverse = FALSE, stunn = FALSE, protection = FALSE;
		r = -1;
		srand(time(NULL));

		for (i = 0; i < 10; i++) {
			Item_List[i] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_ITEM_ALCHOL + (i * 2)));
			Item_List_B[i] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_ITEM_ALCHOL_B + (i * 2)));
			old_item[i] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_ITEM_ALCHOL + (i * 2)));
			old_item_B[i] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_ITEM_ALCHOL_B + (i * 2)));
		}

		item_x = rand() % 400;
		item_y = 0;
		item_num = rand() % 10;

		x = 100;
		y = 450;

		RunBit_L[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_1L_B));
		RunBit_L[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_2L_B));
		RunBit_L[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_3L_B));
		RunBit_R[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_1R_B));
		RunBit_R[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_2R_B));;
		RunBit_R[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_3R_B));

		RunBit_LB[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_1L));
		RunBit_LB[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_2L));
		RunBit_LB[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_3L));
		RunBit_RB[0] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_1R));
		RunBit_RB[1] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_2R));
		RunBit_RB[2] = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_MOVE_3R));

		hBitmap13 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE13_B));
		hBitmap46 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE46_B));
		hBitmap79 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE79_B));
		hBitmap1012 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE1012_B));
		hBitmap_TONGUE_L1_R = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_TONGUE_R1_B));
		hBitmap_TONGUE_L1_L = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_TONGUE_L1_B));
		hBitmap_GameOver = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_GameOver));

		hBitmap13B = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE13));
		hBitmap46B = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE46));
		hBitmap79B = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE79));
		hBitmap1012B = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_LIFE1012));
		hBitmap_TONGUE_L1_RB = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_TONGUE_R1));
		hBitmap_TONGUE_L1_LB = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_TONGUE_L1));
		hBitmap_GameOverB = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(BIT_GameOver_B));

		SetTimer(hwnd, 1, 50, NULL);
		break;
	case WM_TIMER:
		item_y += rand() % 15;
		InvalidateRgn(hwnd, NULL, true);
		return 0;

	case WM_PAINT:
		if (life > 12)
			life = 12;
		else if (life < 0)
			life = 0;
		
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		memdc1 = CreateCompatibleDC(memdc);
		memdc2 = CreateCompatibleDC(hdc);

		wsprintf(str, TEXT("life : %d"), life);

		TextOut(hdc, 10, 10, str, lstrlen(str));

		//아이템 출력
		if (drop == FALSE) {
			r = 0;
			r = rand() % 10;
		}

		switch (r)
		{
		case 0:
			SetTimer(hwnd, 1, 300, NULL);
			GetObject(Item_List[0], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			rc = { item_x, item_y, item_x + bx, item_y + by };
			//Rectangle(hdc, item_x, item_y, item_x + bx, item_y + by);
			SelectObject(memdc1, Item_List_B[0]);
			StretchBlt(hdc, item_x, item_y, bx * 0.75, by * 0.75, memdc1, 0, 0, bx, by, SRCAND);
			SelectObject(memdc1, Item_List[0]);
			StretchBlt(hdc, item_x, item_y, bx * 0.75, by * 0.75, memdc1, 0, 0, bx, by, SRCPAINT);
			drop = TRUE;
			DeleteDC(memdc1);
			break;
		case 1:
			SetTimer(hwnd, 2, 300, NULL);
			GetObject(Item_List[1], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			rc = { item_x, item_y, item_x + bx, item_y + by };
			//Rectangle(hdc, item_x, item_y, item_x + bx, item_y + by);
			SelectObject(memdc1, Item_List_B[1]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCAND);
			SelectObject(memdc1, Item_List[1]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCPAINT);
			drop = TRUE;
			DeleteDC(memdc1);
			break;
		case 2:
			SetTimer(hwnd, 3, 300, NULL);
			GetObject(Item_List[2], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			rc = { item_x, item_y, item_x + bx, item_y + by };
			//Rectangle(hdc, item_x, item_y, item_x + bx, item_y + by);
			SelectObject(memdc1, Item_List_B[2]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCAND);
			SelectObject(memdc1, Item_List[2]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCPAINT);
			drop = TRUE;
			DeleteDC(memdc1);
			break;
		case 3:
			SetTimer(hwnd, 4, 300, NULL);
			GetObject(Item_List[3], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			rc = { item_x, item_y, item_x + bx, item_y + by };
			//Rectangle(hdc, item_x, item_y, item_x + bx, item_y + by);
			SelectObject(memdc1, Item_List_B[3]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCAND);
			SelectObject(memdc1, Item_List[3]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCPAINT);
			drop = TRUE;
			DeleteDC(memdc1);
			break;
		case 4:
			SetTimer(hwnd, 5, 300, NULL);
			GetObject(Item_List[4], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			rc = { item_x, item_y, item_x + bx, item_y + by };
			//Rectangle(hdc, item_x, item_y, item_x + bx, item_y + by);
			SelectObject(memdc1, Item_List_B[4]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCAND);
			SelectObject(memdc1, Item_List[4]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCPAINT);
			drop = TRUE;
			DeleteDC(memdc1);
			break;
		case 5:
			SetTimer(hwnd, 6, 300, NULL);
			GetObject(Item_List[5], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			rc = { item_x, item_y, item_x + bx, item_y + by };
			//Rectangle(hdc, item_x, item_y, item_x + bx, item_y + by);
			SelectObject(memdc1, Item_List_B[5]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCAND);
			SelectObject(memdc1, Item_List[5]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCPAINT);
			drop = TRUE;
			DeleteDC(memdc1);
			break;
		case 6:
			SetTimer(hwnd, 7, 300, NULL);
			GetObject(Item_List[6], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			rc = { item_x, item_y, item_x + bx, item_y + by };
			//Rectangle(hdc, item_x, item_y, item_x + bx, item_y + by);
			SelectObject(memdc1, Item_List_B[6]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCAND);
			SelectObject(memdc1, Item_List[6]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCPAINT);
			drop = TRUE;
			DeleteDC(memdc1);
			break;
		case 7:
			SetTimer(hwnd, 8, 300, NULL);
			GetObject(Item_List[7], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			rc = { item_x, item_y, item_x + bx, item_y + by };
			//Rectangle(hdc, item_x, item_y, item_x + bx, item_y + by);
			SelectObject(memdc1, Item_List_B[7]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCAND);
			SelectObject(memdc1, Item_List[7]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCPAINT);
			drop = TRUE;
			DeleteDC(memdc1);
			break;
		case 8:
			SetTimer(hwnd, 9, 300, NULL);
			GetObject(Item_List[8], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			rc = { item_x, item_y, item_x + bx, item_y + by };
			//Rectangle(hdc, item_x, item_y, item_x + bx, item_y + by);
			SelectObject(memdc1, Item_List_B[8]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCAND);
			SelectObject(memdc1, Item_List[8]);
			StretchBlt(hdc, item_x, item_y, bx * 0.5, by * 0.5, memdc1, 0, 0, bx, by, SRCPAINT);
			drop = TRUE;
			DeleteDC(memdc1);
			break;
		case 9:
			SetTimer(hwnd, 10, 300, NULL);
			GetObject(Item_List[9], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			rc = { item_x, item_y, item_x + bx, item_y + by };
			//Rectangle(hdc, item_x, item_y, item_x + bx, item_y + by);
			SelectObject(memdc1, Item_List_B[9]);
			StretchBlt(hdc, item_x, item_y, bx * 0.75, by * 0.75, memdc1, 0, 0, bx, by, SRCAND);
			SelectObject(memdc1, Item_List[9]);
			StretchBlt(hdc, item_x, item_y, bx * 0.75, by * 0.75, memdc1, 0, 0, bx, by, SRCPAINT);
			drop = TRUE;
			DeleteDC(memdc1);
			break;
		}
		wsprintf(str, TEXT("case : %d"), r);

		TextOut(hdc, 10, 30, str, lstrlen(str));
		

		// 생명력에 따라 다른 비트맵 이미지 출력.
		if (life >= 1 && life <= 3) {
			GetObject(hBitmap13, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap13);
		}
		else if (life >= 4 && life <= 6) {
			GetObject(hBitmap46, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap46);
		}
		else if (life >= 7 && life <= 9) {
			GetObject(hBitmap79, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap79);
		}
		else if (life >= 10 && life <= 12) {
			GetObject(hBitmap1012, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap1012);
		}
		else {
			GetObject(hBitmap_GameOver, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap_GameOver);
		}

		
		

		// 방향키 좌측일 때와 우측일 때 비트맵 출력 두 조건 모두 실행될 필요가 있어서
		// if로만 구현.
		if (direct == 'L') {
			old_direct = 'L';
			GetObject(RunBit_L[count], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			SelectObject(memdc, RunBit_L[count]);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCAND);
			SelectObject(memdc, RunBit_LB[count]);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCPAINT);
			//dest, w/h, src
			DeleteDC(memdc);
			count++;
			count = count % 3;
			direct = 'n';
		}
		if (direct == 'R') {
			old_direct = 'R';
			GetObject(RunBit_R[count], sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			SelectObject(memdc, RunBit_R[count]);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCAND);
			SelectObject(memdc, RunBit_RB[count]);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCPAINT);
			//dest, w/h, src
			DeleteDC(memdc);
			count++;
			count = count % 3;
			direct = 'n';
		}

		// 스페이스바가 눌렸을 때 좌, 우측에 따른 비트맵 출력.
		if (space == 'O') {
			if (old_direct == 'L') {
				box2 = { x - 110, y, x - 60, y + 50 };

				GetObject(hBitmap_TONGUE_L1_L, sizeof(BITMAP), &bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				SelectObject(memdc, hBitmap_TONGUE_L1_L);
				StretchBlt(hdc, x - 80, y, 140, 55, memdc, 0, 0, bx, by, SRCAND);
				SelectObject(memdc, hBitmap_TONGUE_L1_LB);
				StretchBlt(hdc, x - 80, y, 140, 55, memdc, 0, 0, bx, by, SRCPAINT);
				//dest, w/h, src

				if (IntersectRect(&temp, &rc, &box2)) {
					collision_check(r);
					SelectObject(memdc1, Item_List[r]);
					DeleteObject(Item_List[r]);
					SelectObject(memdc1, Item_List_B[r]);
					DeleteObject(Item_List_B[r]);					
					get = TRUE;

				}
				DeleteDC(memdc);
			}
			else if (old_direct == 'R' || old_direct == 'n') {
				box1 = { x + 120, y, x + 170, y + 50 };

				GetObject(hBitmap_TONGUE_L1_R, sizeof(BITMAP), &bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;
				SelectObject(memdc, hBitmap_TONGUE_L1_R);
				StretchBlt(hdc, x, y, 140, 55, memdc, 0, 0, bx, by, SRCAND);
				SelectObject(memdc, hBitmap_TONGUE_L1_RB);
				StretchBlt(hdc, x, y, 140, 55, memdc, 0, 0, bx, by, SRCPAINT);
				//dest, w/h, src

				if (IntersectRect(&temp, &rc, &box1)) {					
					collision_check(r);
					SelectObject(memdc1, Item_List[r]);
					DeleteObject(Item_List[r]);
					SelectObject(memdc1, Item_List_B[r]);
					DeleteObject(Item_List_B[r]);
					get = TRUE;
				}
				DeleteDC(memdc);
			}
		}
		wsprintf(str, TEXT("get : %d"), get);

		TextOut(hdc, 10, 90, str, lstrlen(str));
		//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
		if (get) {

			item_print(hdc, memdc2, r, old_item[r], old_item_B[r]);
			DeleteDC(memdc2);
		}

		bx = bit.bmWidth;
		by = bit.bmHeight;

		if (life >= 1 && life <= 3) {
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCAND);
			GetObject(hBitmap13B, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap13B);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCPAINT);
			DeleteDC(memdc);
		}

		else if (life >= 4 && life <= 6) {
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCAND);
			GetObject(hBitmap46B, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap46B);
			StretchBlt(hdc, x, y, 55, 55, memdc, 0, 0, bx, by, SRCPAINT);
			DeleteDC(memdc);
		}
		else if (life >= 7 && life <= 9) {
			StretchBlt(hdc, x, y, 60, 55, memdc, 0, 0, bx, by, SRCAND);
			GetObject(hBitmap79B, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap79B);
			StretchBlt(hdc, x, y, 60, 55, memdc, 0, 0, bx, by, SRCPAINT);
			DeleteDC(memdc);
		}

		else if (life >= 10 && life <= 12) {
			StretchBlt(hdc, x, y, 65, 60, memdc, 0, 0, bx, by, SRCAND);
			GetObject(hBitmap1012B, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap1012B);
			StretchBlt(hdc, x, y, 65, 60, memdc, 0, 0, bx, by, SRCPAINT);
			DeleteDC(memdc);
		}
		else {
			StretchBlt(hdc, x, y, 80, 80, memdc, 0, 0, bx, by, SRCAND);
			GetObject(hBitmap_GameOverB, sizeof(BITMAP), &bit);
			SelectObject(memdc, hBitmap_GameOverB);
			StretchBlt(hdc, x, y, 80, 80, memdc, 0, 0, bx, by, SRCPAINT);
			DeleteDC(memdc);
		}
		wsprintf(str, TEXT("stunn : %d"), stunn);
		TextOut(hdc, 10, 60, str, lstrlen(str));
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:	// 키를 눌렀을 때.
		switch (wParam) {
		case VK_LEFT:
			direct = 'L';
			space = 'x';
			if (stunn == TRUE) {
				x += speed;
			}
			x -= speed;
			break;

		case VK_RIGHT:
			direct = 'R';
			space = 'x';
			if (stunn == TRUE) {
				x -= speed;
			}
			x += speed;
			break;

		case VK_SPACE:
			space = 'O';			
			if (stunn == TRUE) {
				space = 'n';
			}
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_KEYUP:		//키를 떼었을 때.
		switch (wParam) {
		case VK_SPACE:
			space = 'X';
			break;
		case VK_LEFT:
			direct = 'n';
			break;
		case VK_RIGHT:
			direct = 'n';
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		for (int i = 1; i <= 10; i++) {
			KillTimer(hwnd, i);
		}

		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void collision_check(int item_index) {
	switch (item_index) {
	// 0:alchol, 1:bat, 2:cricket, 3:dna, 4:dopping, 5:fly, 6:rock, 7:slime, 8:snake, 9:worm 
	case 0:
		reverse = TRUE;
		break;
	case 1:
		life -= 2;
		break;
	case 2:
		life += 3;
		break;
	case 3:
		//혀 길이 증가.
		break;
	case 4:
		if (speed > 30)
			speed = 30;
		else
			speed += 10;
		break;
	case 5:
		life += 1;
		score += 1;
		break;
	case 6:
		life -= 1;
		stunn = TRUE;
		break;
	case 7:
		protection = TRUE;
		break;
	case 8:
		life -= 3;
		break;
	case 9:
		life += 2;
		break;
	}
}

void item_print(HDC hdc_f, HDC memdc_f, int r_f, HBITMAP Items, HBITMAP Items_B) {
	int bx_f, by_f;
	BITMAP bit_f;
	GetObject(Items, sizeof(BITMAP), &bit_f);
	bx_f = bit_f.bmWidth;
	by_f = bit_f.bmHeight;

	switch (r_f) {
	case 0:
		SelectObject(memdc_f, Items_B);
		StretchBlt(hdc_f, 0, 50, bx_f, by_f, memdc_f, 0, 0, bx_f, by_f, SRCAND); 
		SelectObject(memdc_f, Items);
		StretchBlt(hdc_f, 0, 50, bx_f, by_f, memdc_f, 0, 0, bx_f, by_f, SRCPAINT); //alchol
		break;
	case 3:
		SelectObject(memdc_f, Items_B);
		StretchBlt(hdc_f, 0, 50, bx_f, by_f, memdc_f, 0, 0, bx_f, by_f, SRCAND); //dna
		SelectObject(memdc_f, Items);
		StretchBlt(hdc_f, 0, 50, bx_f, by_f, memdc_f, 0, 0, bx_f, by_f, SRCPAINT); //dna
		break;
	case 4:
		SelectObject(memdc_f, Items_B);
		StretchBlt(hdc_f, 50, 50, bx_f, by_f, memdc_f, 0, 0, bx_f, by_f, SRCAND);   //dopping
		SelectObject(memdc_f, Items);
		StretchBlt(hdc_f, 50, 50, bx_f, by_f, memdc_f, 0, 0, bx_f, by_f, SRCPAINT);   //dopping
		break;
	case 6:
		SelectObject(memdc_f, Items_B);
		StretchBlt(hdc_f, 50, 100, bx_f, by_f, memdc_f, 0, 0, bx_f, by_f, SRCAND);   //rock
		SelectObject(memdc_f, Items);
		StretchBlt(hdc_f, 50, 100, bx_f, by_f, memdc_f, 0, 0, bx_f, by_f, SRCPAINT);   //rock
		break;
	case 7:
		SelectObject(memdc_f, Items_B);
		StretchBlt(hdc_f, 100, 50, bx_f, by_f, memdc_f, 0, 0, bx_f, by_f, SRCAND);   //slime
		SelectObject(memdc_f, Items);
		StretchBlt(hdc_f, 100, 50, bx_f, by_f, memdc_f, 0, 0, bx_f, by_f, SRCPAINT);   //slime
		break;
	}

}