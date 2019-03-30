// Background.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "Settings.h"
#include <iomanip>//get_time
#include <thread>         // std::this_thread::sleep_until
#include <chrono>         // std::chrono::system_clock
#include <ctime>          // std::time_t, std::tm, std::localtime, std::mktime
#include <tchar.h>

struct Image {
	std::string path;
	struct tm todayIn;
};
int main()
{
	HWND hConsole = GetConsoleWindow();
	ShowWindow(hConsole, SW_HIDE);
	std::cout << "Hello World!" << std::endl;
	std::string pathFile = Settings::getTodayFileName();
	if (!Settings::existFile(pathFile)) {
		std::string messageText = "File: \"" + pathFile + "\" not exist.\nYes - Create default template.\nNo-exit and close.";
		std::wstring w_text(messageText.begin(), messageText.end());
		LPCWSTR  LPCWSTR_text = w_text.c_str();
		int choose = MessageBox(NULL,
			LPCWSTR_text,
			L"Error with file settings.",
			MB_YESNO
		);
		if (choose == IDYES) {
			Settings::createDefaultFile(pathFile);
		}
		else {
			exit(1);
		};
	};
	std::vector<std::map<std::string, std::string>> TodayImage = Settings::getListImage(Settings::getTodayFileName());
	std::vector<std::map<std::string, std::string>> workVector;
	boolean error = false;
	//Проверка на правильность даты и существования image
	for (int i = 0; i != TodayImage.size(); ++i) {
		std::map<std::string, std::string > nextimage = TodayImage.at(i);
		std::string pathtoimage = nextimage.at(Settings::Name_Path_ToBMPInArray);
		std::string timetoimage = nextimage.at(Settings::Name_Time_ToBMPInArray);

		if (!Settings::existFile(pathtoimage)) {
			error = true;
			std::cout << i << " not exist path: \"" << pathtoimage << "\"" << std::endl;
		};
		struct std::tm tm_to_start;
		std::istringstream ss(timetoimage.c_str());
		ss >> std::get_time(&tm_to_start, "%H:%M:%S");
		if (ss.fail()) {
			error = true;
			std::cout << i << " not true time format: %H:%M:%S : \"" << timetoimage << "\"" << std::endl;
		};
	};
	std::string fileImageOpen = "C:\\Users\\al\\Desktop\\TestHabr\\hot-air-balloon-3648832.bmp";


	std::string text= "D:\\orig.bmp";
	

	if (error) {
		ShowWindow(hConsole, SW_SHOW);
		MessageBox(NULL, L"Watch consol", L"Error with file settings.", 0);
		exit(1);
	}
	for (int i = 0; i != TodayImage.size(); ++i) {
		std::map<std::string, std::string > nextimage = TodayImage.at(i);
		std::string pathtoimage = nextimage.at(Settings::Name_Path_ToBMPInArray);
		std::string timetoimage = nextimage.at(Settings::Name_Time_ToBMPInArray);


		struct std::tm tm_to_start;//Заполняем структуру часом,мин и сек из json
		std::istringstream ss(timetoimage.c_str());
		ss >> std::get_time(&tm_to_start, "%H:%M:%S");

		std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		struct tm todayIn;
		localtime_s(&todayIn, &tt);//В эту структуру надо запихнуть текущий день, год и прочее.
		todayIn.tm_hour = tm_to_start.tm_hour;
		todayIn.tm_min = tm_to_start.tm_min;
		todayIn.tm_sec = tm_to_start.tm_sec;

		std::this_thread::sleep_until(std::chrono::system_clock::from_time_t(mktime(&todayIn)));
		std::wstring w_text(pathtoimage.begin(), pathtoimage.end());
		LPCWSTR  LPCWSTR_text = w_text.c_str();

		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)LPCWSTR_text, SPIF_SENDWININICHANGE);
	}
}