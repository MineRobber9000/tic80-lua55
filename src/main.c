#include "core/core.h"

static bool examplecore_init(tic_mem *tic, const char *code) {
	return true;
}

static void examplecore_close(tic_mem *tic) {
	return;
}

static void examplecore_boot(tic_mem *tic) {
	return;
}

static void examplecore_tick(tic_mem *tic) {
	tic_core *core = (tic_core*)tic;

	core->api.cls(tic, 0);
	#define PRINT(s, x, y) core->api.print(tic, s, x, y, 12, false, 1, false);
	PRINT("This is a TIC-80 script core compiled outside", 0, 0);
	PRINT("of the TIC-80 repository. In an actual scripting", 0, 7);
	PRINT("core, this is where your code would run.", 0, 14);
	#undef PRINT
}

static void examplecore_callback(tic_mem *tic, s32 arg, void *data) {
	// scanline (SCN) and border (BDR) give the row, menu gives the index
	// this core doesn't implement *any* of them but we still have to give
	// a valid function pointer or we get segfault
	return;
}

// Built by tool/generate_demo_cart.py
static const u8 DemoRom[] = {
	#include "democart.dat"
};

// For a more feature-filled tic_script, see any of the ones in the actual
// repository. This is just the bare minimum to get TIC-80 working.
TIC_EXPORT const tic_script EXPORT_SCRIPT(ExampleCore) = {
	.id = 0xFC,
	.name = "examplecore",
	.fileExtension = ".exm",
	.projectComment = "//",
	.init = examplecore_init,
	.close = examplecore_close,
	.tick = examplecore_tick,
	.boot = examplecore_boot,
	.callback = {
		.scanline = examplecore_callback,
		.border = examplecore_callback,
		.menu = examplecore_callback
	},
	.singleComment = "//",
	.demo = {DemoRom, sizeof DemoRom}
};
