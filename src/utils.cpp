#include "ircserv.hpp"

void	parseArg(std::string cmdArg, std::vector<std::string>& argList)
{
	size_t						commaPos = cmdArg.find(',');
	size_t						tmp = 0;

	while (commaPos != std::string::npos)
	{
		argList.push_back(cmdArg.substr(tmp, commaPos - tmp));
		tmp = commaPos + 1;
		commaPos = cmdArg.find(',', tmp);
	}
	argList.push_back(cmdArg.substr(tmp, cmdArg.size() - tmp));
}

std::vector<std::string>	vectorizator(std::string arg1, std::string arg2, std::string arg3)
{
	std::vector<std::string> vec;

	vec.push_back(arg1);
	vec.push_back(arg2);
	if (!arg3.empty())
		vec.push_back(arg3);
	return vec;
}

