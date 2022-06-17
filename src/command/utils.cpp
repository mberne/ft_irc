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
