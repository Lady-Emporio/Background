#include "pch.h"
#include "Settings.h"

std::string Settings::getTodayFileName() {
	SYSTEMTIME data;
	//GetSystemTime(&data);
	GetLocalTime(&data);
	int todayDayNomber = data.wDayOfWeek;
	switch (todayDayNomber)
	{
	case 1:
		return Settings::Monday + ".json";
	case 2:
		return Settings::Tuesday + ".json";
	case 3:
		return Settings::Wednesday + ".json";
	case 4:
		return Settings::Thursday + ".json";
	case 5:
		return Settings::Friday + ".json";
	case 6:
		return Settings::Saturday + ".json";
	case 7:
		return Settings::Sunday + ".json";
	}
	Settings::MessageBoxAndExit("wDayOfWeek not in 1-7", Settings::WonderError);
	exit(1);
}

std::vector<std::map<std::string, std::string>> Settings::getListImage(std::string FileName)
{
	std::ifstream fileRead(FileName);
	if (!existFile(FileName)) {
		std::string textMessage = "Today file name = " + FileName + " not exist.";
		Settings::MessageBoxAndExit(textMessage.c_str(), Settings::ErrorWithTodayFile);
	}
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::Value value;
	std::string errorDefaultValue = "error not value.ReturnDefaultValue";
	std::string errs;
	bool ok = parseFromStream(builder, fileRead, &value, &errs);
	if (!ok) {
		std::string textMessage = "Today file name = ";
		textMessage += FileName;
		textMessage += "\n";
		textMessage += errs;
		Settings::MessageBoxAndExit(textMessage.c_str(), Settings::ErrorWithTodayFile);

	};
	fileRead.close();
	Json::Value todayListWithImage = value.get(Settings::NameArrayInJsonWithImage, errorDefaultValue);
	if (todayListWithImage == errorDefaultValue) {
		std::string textMessage = "Not array with name = ";
		textMessage += NameArrayInJsonWithImage;
		Settings::MessageBoxAndExit(textMessage, Settings::ErrorWithTodayFile);
	};
	if (!todayListWithImage.isArray()) {
		std::string textMessage = NameArrayInJsonWithImage;
		textMessage += " not array.";
		Settings::MessageBoxAndExit(textMessage, Settings::ErrorWithTodayFile);
	}

	std::vector<std::map<std::string, std::string>> returnVestor;
	for (int i = 0; i != todayListWithImage.size(); ++i) {
		Json::Value imageObject = todayListWithImage.get(i, errorDefaultValue);
		if (imageObject == errorDefaultValue) {
			std::string textMessage = "Element in index: ";
			textMessage+= std::to_string(i);
			textMessage += " not exist.\n In try get his, return default value.";
			Settings::MessageBoxAndExit(textMessage, Settings::WonderError);
		}
		if (!imageObject.isObject()) {
			std::string textMessage = "Element in index: ";
			textMessage += std::to_string(i);
			textMessage += " not object";
			Settings::MessageBoxAndExit(textMessage, Settings::ErrorWithTodayFile);
		}
		Json::Value path= imageObject.get(Settings::Name_Path_ToBMPInArray, errorDefaultValue);
		Json::Value time = imageObject.get(Settings::Name_Time_ToBMPInArray, errorDefaultValue);
		if (path == errorDefaultValue) {
			std::string textMessage = "Element in index: "+ std::to_string(i)+std::string(".\nNot have: ")+ Settings::Name_Path_ToBMPInArray;
			textMessage +=".\n In try get his, return default value.";
			Settings::MessageBoxAndExit(textMessage, Settings::ErrorWithTodayFile);
		}
		if (time == errorDefaultValue) {
			std::string textMessage = "Element in index: " + std::to_string(i) + std::string(".\nNot have: ") + Settings::Name_Time_ToBMPInArray;
			textMessage += ".\n In try get his, return default value.";
			Settings::MessageBoxAndExit(textMessage, Settings::ErrorWithTodayFile);
		}
		if (!path.isString()) {
			std::string textMessage = "Element in index: " + std::to_string(i) + std::string(".\nNot string: ") + Settings::Name_Path_ToBMPInArray;
			textMessage += path.asCString();
			Settings::MessageBoxAndExit(textMessage, Settings::ErrorWithTodayFile);
		}
		if (!time.isString()) {
			std::string textMessage = "Element in index: " + std::to_string(i) + std::string(".\nNot string: ") + Settings::Name_Time_ToBMPInArray;
			textMessage += time.asCString();
			Settings::MessageBoxAndExit(textMessage, Settings::ErrorWithTodayFile);
		}
		std::map<std::string, std::string > map;
		map.insert( std::pair<std::string, std::string>(Settings::Name_Path_ToBMPInArray, path.asCString()) );
		map.insert( std::pair<std::string, std::string>(Settings::Name_Time_ToBMPInArray, time.asCString()) );
		returnVestor.push_back(map);
	};
	return returnVestor;
}

void Settings::MessageBoxAndExit(std::string text, std::string header)
{
	std::wstring w_text(text.begin(), text.end());
	std::wstring w_header(header.begin(), header.end());
	LPCWSTR  LPCWSTR_text = w_text.c_str();
	LPCWSTR  LPCWSTR_header = w_header.c_str();
	MessageBox(
		NULL,
		LPCWSTR_text,
		LPCWSTR_header,
		0
	);
	exit(1);
}

bool Settings::existFile(std::string path)
{
	std::ifstream fileRead(path);
	return fileRead.good();
}

void Settings::createDefaultFile(std::string fileName){
	Json::Value root;
	Json::Value jsonArray;
	for (int i = 0; i != 3; ++i) {
		Json::Value image;
		image[Settings::Name_Path_ToBMPInArray] = "set path in this template to .bmp file.";
		image[Settings::Name_Time_ToBMPInArray] = "set time in this template in format: %H:%M:%S . You control enum time DESC";
		jsonArray.append(image);
	}
	root[NameArrayInJsonWithImage]= jsonArray;

	std::ofstream outfile(fileName);
	outfile << root;
	outfile.close();
}


