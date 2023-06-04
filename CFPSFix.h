#pragma once

#include <list>
#include <thread>
#include <mutex>

class CFPSFix
{
private:
	void Routine();

	std::mutex m_Mutex;
	std::list<pid_t> m_Threads;
public:
	CFPSFix();
	~CFPSFix();

	void PushThread(pid_t tid);
};

