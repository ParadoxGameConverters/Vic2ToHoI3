/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#include "HoI3Localisation.h"

#include <Windows.h>
#include <fstream>

#include "..\V2World\V2Country.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"

const std::array<std::string, HoI3Localisation::numLanguages> HoI3Localisation::languages = 
	{ "english", "french", "german", "spanish" };

void HoI3Localisation::SetTag(const std::string& newTag)
{
	tag = newTag;
}

void HoI3Localisation::ReadFromCountry(const V2Country& source)
{
	for (size_t i = 0; i < numLanguages; ++i)
	{
		if (!languages[i].empty())
		{
			name[i] = source.getName(languages[i]);
			adjective[i] = source.getAdjective(languages[i]);
		}
	}
}

void HoI3Localisation::SetPartyKey(size_t partyIndex, const std::string& partyKey)
{
	if (parties.size() <= partyIndex)
	{
		parties.resize(partyIndex + 1);
	}
	parties[partyIndex].key = partyKey;
}

void HoI3Localisation::SetPartyName(size_t partyIndex, const std::string& language, const std::string& name)
{
	if (parties.size() <= partyIndex)
	{
		parties.resize(partyIndex + 1);
	}
	auto languageIter = std::find(languages.begin(), languages.end(), language);
	if (languageIter != languages.end())
	{
		size_t languageIndex = std::distance(languages.begin(), languageIter);
		parties[partyIndex].name[languageIndex] = name;
	}
}

void HoI3Localisation::WriteToStream(std::ostream& out) const
{
	out << Utils::convertUTF8ToWin1252(tag);
	for (const auto& localisedName : name)
	{
		out << ';' << Utils::convertUTF8ToWin1252(localisedName);
	}
	out << "x\n";

	out << Utils::convertUTF8ToWin1252(tag) << "_ADJ";
	for (const auto& localisedAdjective : adjective)
	{
		out << ';' << Utils::convertUTF8ToWin1252(localisedAdjective);
	}
	out << "x\n";

	for (const auto& party : parties)
	{
		out << Utils::convertUTF8ToWin1252(party.key);
		for (const auto& localisedPartyName : party.name)
		{
			out << ';' << Utils::convertUTF8ToWin1252(localisedPartyName);
		}
		out << "x\n";
	}
}

std::string HoI3Localisation::convertCountryFileName(const std::string countryFileName) const
{
	return Utils::convertUTF8ToWin1252(countryFileName);
}