#pragma once
namespace Settings{

const std::string Monday = "Monday";
const std::string Tuesday = "Tuesday";
const std::string Wednesday = "Wednesday";
const std::string Thursday = "Thursday";
const std::string Friday = "Friday";
const std::string Saturday = "Saturday";
const std::string Sunday = "Sunday";
const std::string NameArrayInJsonWithImage = "Images";
const std::string Name_Path_ToBMPInArray = "path";
const std::string Name_Time_ToBMPInArray = "time";
const std::string WonderError = "Wonder Error";
const std::string ErrorWithTodayFile = "Error with today file.";
std::string getTodayFileName();

std::vector<std::map<std::string, std::string>> getListImage(std::string FileName);

void MessageBoxAndExit(std::string text,std::string header);

bool existFile(std::string path);
void createDefaultFile(std::string fileName);
}