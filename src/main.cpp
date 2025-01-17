#include "common.h"
#include "main.h"
#include "program.h"

extern "C" void QuickTextDisplay(const char *text)
{
	OSScreenInit();
	OSScreenEnableEx(1, 1);
	OSScreenClearBufferEx(1, 0);
	OSScreenSetBufferEx(1, (void*)0xF4000000);
	OSScreenPutFontEx(1, 0, 0, text);
	OSScreenFlipBuffersEx(1);
}

extern "C" int InitThread(int argc, char** argv) {
	// Create new thread
	log_printf("Creating new thread..\n");

	uint8_t *stack = (uint8_t*)MEM2_alloc(0x1000, 0x10);
	OSThread *thread = (OSThread*)MEM2_alloc(0x1000, 0x10);

	bool ret = OSCreateThread(thread, MainThread, 0, NULL, (uint32_t)stack + 0x1000, 0x1000, 0, ((1 << 2) | 8 | 16));
	if(!ret)
	{
		log_printf("Can't create new thread..\n"); // Error message log
		QuickTextDisplay("Can't create new thread.."); // Error message display
		os_sleep(2);
		return 0;
	}

	if(OSGetTitleID() == 0x000500101004A100 || OSGetTitleID() == 0x000500101004A000 || OSGetTitleID() == 0x000500101004A200)
	{
		log_print("Error to launch title games."); // Error to load titleID
		SYSLaunchMenu(); // Start the Wii U Menu
		return -3;
	}

	// Name fo the thread
	OSSetThreadName(thread, "WiiUHacking");
	OSResumeThread(thread);

	return -3;
}
