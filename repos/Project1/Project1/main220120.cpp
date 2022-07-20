#include <iostream>

#include <map>
#include <set>

#include <string>
#include "CBST.h"

using std::wcout;
using std::endl;

using std::map;
using std::make_pair;

using std::set;

using std::wstring;

#define MAN 1
#define WOMAN 2

struct tStdInfo
{
	wchar_t szName[20];
	unsigned char age;
	unsigned char gender;

	tStdInfo()
		: szName{}
		, age(0)
		, gender(0)
	{
		
	}
	tStdInfo(const wchar_t* _pName, unsigned char _age, unsigned _gender)
		: szName{}
		, age(_age)
		, gender(_gender)
	{
		wcscpy_s(szName, _pName);
	}

};

int main()
{
	// 이진탐색
	// 1. 정렬되어있는 데이터에 적용 가능

	// 이진탐색트리
	// 1. 이진탐색을 사용 할 수 있게 고안된 이진트리
	// 2. 데이터 입력 시 0(logN) 효율
	// 3. 탐색 효율은 0(logN)
	// 4. 트리의 모양 밸런스가 유지되지 않으면 제대로된 탐색 효율이 나오지 않는다.
	// ㄴ 자가균형 기능 필요 (AVL, Res/Black)
	//set 같은 경우는 Red/Black Tree 같은 균형적 2진 트리구조
	// 메모리는 Heap 메모리에 동적할당 된 메모리를 사용 
	// 자료 관리 형태는 Tree 형태를 취한다.
	// 따라서 어떤 순서의 데이터에 접근을 하려 하더라도 Loot node 만 알게 된다면 접근이 가능하다.

	// set보단 map 을 주로 사용 하게 된다.

	const wchar_t* pStr = L"문자열";


	map<const wchar_t*, tStdInfo> mapData;

	tStdInfo info(L"홍길동", 18 ,MAN);
	tStdInfo info2(L"이지혜", 25, WOMAN);

	mapData.insert(make_pair(L"홍길동", info));
	mapData.insert(make_pair(L"이지혜", info2));

	map<const wchar_t*, tStdInfo>::iterator mapiter;
	mapiter = mapData.find(L"홍길동");

	_wsetlocale(LC_ALL, L"korean");

	//찾지 못했을경우
	if (mapiter == mapData.end())
	{
		wcout << L"데이터를 찾을 수 없다." << endl;
	}
	else
	{
		wcout << L"이름 : " << mapiter->second.szName << endl;
		wcout << L"나이 : " << mapiter->second.age << endl;
		wcout << L"성별 : ";
		if (MAN == mapiter->second.gender)
		{
			wcout << L"남자" << endl;
		}
		else if (WOMAN == mapiter->second.gender)
		{
			wcout << L"여자" << endl;
		}
		else
		{
			wcout << L"알 수 없음." << endl;
		}
	}

	map<wstring, tStdInfo> mapStdInfo;

	wstring str;
	str = L"abcdef";
	str += L"hijk";
	str += L"lmnop";

	CBST<int, int> bstint;



	//mapiter->first;
	//mapiter->second;
	//mapData.find(L"홍길동");

	return 0;
}
