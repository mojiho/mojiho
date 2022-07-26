#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "PlayerManager.h"
#include "AccountManager.h"


int main()
{
	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "Player ThenAccount" << endl;
				GPlayerManager.PlayerThenAccount();
				this_thread::sleep_for(100ms);
			}
		});

	GThreadManager->Launch([=]
	{
		while (true)
		{
			cout << "AccountThenPlayer" << endl;
			GPlayerManager.PlayerThenAccount();
			this_thread::sleep_for(100ms);
		}
	});
}
