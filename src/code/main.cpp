#include "MieAyam/src/mieayam_window.h"

int main()
{
	// Inisialisasi Pustaka MieAyam
	MieAyam_Init();

	// Definisikan atribut pada window
	mieayam_window_attributes window;
	window.width = 800;
	window.height = 600;
	window.title = "Amazing";

	// Buat window
	MieAyam_CreateWindow(&window, 1);

	// Tampilkan window, 0 untuk single window
	MieAyam_ShowWindow(0);

	// Jalankan proses per frame
	while (MieAyam_RunProccess())
	{

	}

	return 0;
}