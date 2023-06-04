#include "CServerManager.h"

#include <stdint.h>

const CServerInstance::CServerInstanceEncrypted g_sEncryptedAddresses[MAX_SERVERS] = {
	// сантроп

	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 227), 16, 5125, false), // 1
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 228), 16, 5125, false), // 2
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 229), 16, 5125, false), // 3
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 230), 16, 5125, true), // 4 войс
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 231), 16, 5125, true), // 5 войс
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 232), 16, 5125, true), // 6 войс

	CServerInstance::create(NUM_TO_STR_IP(51, 83, 146, 10), 16, 7777, false), // 1
	CServerInstance::create(NUM_TO_STR_IP(51, 83, 146, 10), 16, 8888, true), // тест войс
	CServerInstance::create(NUM_TO_STR_IP(51, 77, 32, 196), 16, 7777, false), // 2
	CServerInstance::create(NUM_TO_STR_IP(51, 83, 146, 11), 16, 7777, false), // 3
	CServerInstance::create(NUM_TO_STR_IP(54, 38, 142, 49), 16, 7777, true) , // 4 войс
	CServerInstance::create(NUM_TO_STR_IP(54, 38, 142, 50), 16, 7777, true), // 5 войс
	CServerInstance::create(NUM_TO_STR_IP(54, 38, 142, 51), 16, 7777, true), // 6 войс
	CServerInstance::create(NUM_TO_STR_IP(92, 63, 104, 218), 16, 7777,  false), // тест 2
	// барвиха
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 220), 16, 5125,  true), // 1 войс - реакт
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 221), 16, 5125,  true), // 2 войс - реакт
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 222), 16, 5125,  true), // 3 войс - реакт
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 225), 16, 5125,  true), // 4 войс - реакт
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 226), 16, 5125,  true), // 5 войс - реакт
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 235), 16, 5125,  true), // 7 войс - реакт

	CServerInstance::create(NUM_TO_STR_IP(51, 83, 165, 232), 16, 7777,  true), // 1 войс
	CServerInstance::create(NUM_TO_STR_IP(51, 83, 165, 233), 16, 7777,  true), // 2 войс
	CServerInstance::create(NUM_TO_STR_IP(51, 83, 165, 234), 16, 7777,  true), // 3 войс
	CServerInstance::create(NUM_TO_STR_IP(51, 83, 165, 235), 16, 7777,  true), // 4 войс
	CServerInstance::create(NUM_TO_STR_IP(51, 77,  32, 197), 16, 7777,  true), // 5 войс
	CServerInstance::create(NUM_TO_STR_IP(80, 66, 82, 217), 16, 5125,  true), // 6 войс
	CServerInstance::create(NUM_TO_STR_IP(51, 77, 238, 92), 16, 7777,   false), // тест 1
	CServerInstance::create(NUM_TO_STR_IP(213, 32, 120, 112), 16, 7777, false), // тест 2

	CServerInstance::create(NUM_TO_STR_IP(212, 80, 54, 78), 16, 28850, false), // кырыл
	CServerInstance::create(NUM_TO_STR_IP(83, 220, 173, 239), 16, 7777,  false), // рома новый сэрвэр
	CServerInstance::create(NUM_TO_STR_IP(92, 63, 104, 218), 16, 8888,  true) // рома новый сэрвэр
}; 