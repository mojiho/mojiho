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
	// ����Ž��
	// 1. ���ĵǾ��ִ� �����Ϳ� ���� ����

	// ����Ž��Ʈ��
	// 1. ����Ž���� ��� �� �� �ְ� ��ȵ� ����Ʈ��
	// 2. ������ �Է� �� 0(logN) ȿ��
	// 3. Ž�� ȿ���� 0(logN)
	// 4. Ʈ���� ��� �뷱���� �������� ������ ����ε� Ž�� ȿ���� ������ �ʴ´�.
	// �� �ڰ����� ��� �ʿ� (AVL, Res/Black)
	//set ���� ���� Red/Black Tree ���� ������ 2�� Ʈ������
	// �޸𸮴� Heap �޸𸮿� �����Ҵ� �� �޸𸮸� ��� 
	// �ڷ� ���� ���´� Tree ���¸� ���Ѵ�.
	// ���� � ������ �����Ϳ� ������ �Ϸ� �ϴ��� Loot node �� �˰� �ȴٸ� ������ �����ϴ�.

	// set���� map �� �ַ� ��� �ϰ� �ȴ�.

	const wchar_t* pStr = L"���ڿ�";


	map<const wchar_t*, tStdInfo> mapData;

	tStdInfo info(L"ȫ�浿", 18 ,MAN);
	tStdInfo info2(L"������", 25, WOMAN);

	mapData.insert(make_pair(L"ȫ�浿", info));
	mapData.insert(make_pair(L"������", info2));

	map<const wchar_t*, tStdInfo>::iterator mapiter;
	mapiter = mapData.find(L"ȫ�浿");

	_wsetlocale(LC_ALL, L"korean");

	//ã�� ���������
	if (mapiter == mapData.end())
	{
		wcout << L"�����͸� ã�� �� ����." << endl;
	}
	else
	{
		wcout << L"�̸� : " << mapiter->second.szName << endl;
		wcout << L"���� : " << mapiter->second.age << endl;
		wcout << L"���� : ";
		if (MAN == mapiter->second.gender)
		{
			wcout << L"����" << endl;
		}
		else if (WOMAN == mapiter->second.gender)
		{
			wcout << L"����" << endl;
		}
		else
		{
			wcout << L"�� �� ����." << endl;
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
	//mapData.find(L"ȫ�浿");

	return 0;
}
