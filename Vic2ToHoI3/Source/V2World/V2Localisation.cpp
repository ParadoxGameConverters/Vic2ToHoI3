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



#include "V2Localisation.h"
#include <Windows.h>
#include <fstream>
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "Object.h"



const std::array<std::string, V2Localisation::numLanguages> V2Localisation::languages = 
	{ "english", "french", "german", "spanish" };

void V2Localisation::SetTag(const std::string& newTag)
{
	tag = newTag;
}


void V2Localisation::SetPartyKey(size_t partyIndex, const std::string& partyKey)
{
	if (parties.size() <= partyIndex)
	{
		parties.resize(partyIndex + 1);
	}
	parties[partyIndex].key = partyKey;
}

void V2Localisation::SetPartyName(size_t partyIndex, const std::string& language, const std::string& name)
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

void V2Localisation::WriteToStream(std::ostream& out) const
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

std::string V2Localisation::convertCountryFileName(const std::string countryFileName) const
{
	return Utils::convertUTF8ToWin1252(countryFileName);
}


void V2Localisation::ReadFromFile(const std::string& fileName)
{
	std::ifstream in(fileName);

	std::string line;			// the line being processed

	// Subsequent lines are 'KEY: "Text"'
	while (!in.eof())
	{
		std::string line;
		getline(in, line);

		if ((line[0] == '#') || (line.size() < 3)) // BE: The 3 here is arbitrary.
		{
			continue;
		}

		int division = line.find_first_of(';');
		std::string key = line.substr(0, division);

		unsigned length = division;
		for (auto language : languages)
		{
			int frontDivision = division + 1;
			division = line.find_first_of(';', frontDivision);
			localisations[key][language] = line.substr(frontDivision, division - frontDivision);

			// dash characters other than 0x2D break HoI3
			int dash = localisations[key][language].find_first_of('�');									// the first (if any) dask in the output name
			while (dash != std::string::npos)
			{
				localisations[key][language].replace(dash, 1, "-");
				dash = localisations[key][language].find_first_of('�');
			}
		}
	}
}


void V2Localisation::ReadFromAllFilesInFolder(const std::string& folderPath)
{
	std::set<std::string> fileNames;
	Utils::GetAllFilesInFolder(folderPath, fileNames);

	// Read all these files.
	for (const auto& fileName : fileNames)
	{
		ReadFromFile(folderPath + '\\' + fileName);
	}
}

const std::string& V2Localisation::GetText(const std::string& key, const std::string& language) const
{
	static const std::string noLocalisation = "";	// used if there's no localisation

	const auto keyFindIter = localisations.find(key);	// the localisations for this key
	if (keyFindIter == localisations.end())
	{
		return noLocalisation;
	}
	const auto& localisationsByLanguage = keyFindIter->second;	// the localisations for this language
	const auto languageFindIter = localisationsByLanguage.find(language);	// the localisation we want
	if (languageFindIter == localisationsByLanguage.end())
	{
		return noLocalisation;
	}

	return languageFindIter->second;
}

const std::map<std::string, std::string>& V2Localisation::GetTextInEachLanguage(const std::string& key) const
{
	static const std::map<std::string, std::string> noLocalisation;	// used if there's no localisation

	const auto keyFindIter = localisations.find(key);	// the localisation we want
	if (keyFindIter == localisations.end())
	{
		return noLocalisation;
	}

	return keyFindIter->second;
}

