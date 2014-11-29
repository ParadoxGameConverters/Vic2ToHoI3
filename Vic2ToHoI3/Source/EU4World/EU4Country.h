/*Copyright(c) 2014 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */


#ifndef EU4COUNTRY_H_
#define EU4COUNTRY_H_



#include "EU4Army.h"
#include "..\Color.h"
#include "..\Date.h"

class EU4Province;
class EU4Relations;
class EU4Loan;
class EU4Leader;


class EU4Country
{
	public:
		EU4Country(Object* obj, map<string, int> armyInvIdeas, map<string, int> commerceInvIdeas, map<string, int> cultureInvIdeas, map<string, int> industryInvIdeas, map<string, int> navyInvIdeas);

		// Some countries are created by EU4 as placeholders to be filled as the game 
		// progresses (colonial nations in particular). This method returns true if
		// this country is one of these unused countries.
		bool isUnusedCountry() const;

		// Add any additional information available from the specified country file.
		void readFromCommonCountry(const string& fileName, Object*);

		void setLocalisationName(const string& language, const string& name);
		void setLocalisationAdjective(const string& language, const string& adjective);

		void						addProvince(EU4Province*);
		void						addCore(EU4Province*);
		bool						hasModifier(string) const;
		int						hasNationalIdea(string) const;
		bool						hasFlag(string) const ;
		void						resolveRegimentTypes(const RegimentTypeMap& map);
		int						getManufactoryCount() const;
		void						eatCountry(EU4Country* target);

		string						getTag()										const { return tag; };
		vector<EU4Province*>		getProvinces()								const { return provinces; };
		vector<EU4Province*>		getCores()									const { return cores; };
		int							getCapital()								const { return capital; };
		int							getNationalFocus()						const { return nationalFocus; };
		string						getTechGroup()								const { return techGroup; };
		string						getPrimaryCulture()						const { return primaryCulture; };
		vector<string>				getAcceptedCultures()					const { return acceptedCultures; };
		string						getCulturalUnion()						const { return culturalUnion; };
		string						getReligion()								const { return religion; };
		double						getScore()									const { return score; };
		double						getStability()								const { return stability; };
		double						getAdmTech()								const { return admTech; };
		double						getDipTech()								const { return dipTech; };
		double						getMilTech()								const { return milTech; };
		double						getArmyInvestment()						const { return armyInvestment; };
		double						getNavyInvestment()						const { return navyInvestment; };
		double						getCommerceInvestment()					const { return commerceInvestment; };
		double						getIndustryInvestment()					const { return industryInvestment; };
		double						getCultureInvestment()					const { return cultureInvestment; };
		bool							getPossibleDaimyo()						const { return possibleDaimyo; };
		string						getGovernment()							const { return government; };
		vector<EU4Relations*>	getRelations()								const { return relations; };
		vector<EU4Army*>			getArmies()									const { return armies; };
		vector<EU4Leader*>		getLeaders()								const { return leaders; };

		string getName() const { return name; }
		string getName(const string& language) const;
		string getAdjective(const string& language) const;
		Color getColor() const { return color; }

	private:
		void							determineInvestments(Object* obj, map<string, int> armyInvIdeas, map<string, int> commerceInvIdeas, map<string, int> cultureInvIdeas, map<string, int> industryInvIdeas, map<string, int> navyInvIdeas);
		void							determineFlagsAndModifiers(Object* obj);
		void							clearProvinces();
		void							clearCores();

		string						tag;						// the tag for the EU4 nation
		vector<EU4Province*>		provinces;				// the EU4 provinces this nations holds
		vector<EU4Province*>		cores;					// the EU4 provinces this nation has cores on
		int							capital;					// the EU4 province that is this nation's capital
		int							nationalFocus;			// the location of this country's national focus
		string						techGroup;				// the tech group for this nation
		string						primaryCulture;		// the primary EU4 culture of this nation
		vector<string>				acceptedCultures;		// the accepted EU4 cultures for this nation
		string						culturalUnion;			// the culture group this nation is a union for
		string						religion;				// the accepted religion of this country
		double						score;					// this country's EU4 score
		double						stability;				// the stability of this nation
		double						admTech;					// the admin tech of this nation
		double						dipTech;					// the diplo tech of this nation
		double						milTech;					// the mil tech of this nation
		double						armyInvestment;		// the amount of 'investment' into army tech (determined by ideas)
		double						navyInvestment;		// the amount of 'investment' into navy tech (determined by ideas)
		double						commerceInvestment;	// the amount of 'investment' into commerce tech (determined by ideas)
		double						industryInvestment;	// the amount of 'investment' into industry tech (determined by ideas)
		double						cultureInvestment;	// the amount of 'investment' into culture tech (determined by ideas)
		map<string, bool>			flags;					// any flags set for this country
		map<string, bool>			modifiers;				// any modifiers set for this country
		bool							possibleDaimyo;		// if this country is possibly a daimyo
		vector<EU4Leader*>		leaders;					// the military leaders in this country
		string						government;				// the government type
		vector<EU4Relations*>	relations;				// the relations with other nations
		vector<EU4Army*>			armies;					// this nation's armies and navies
		map<string, int>			nationalIdeas;			// the national ideas for this country
		double						legitimacy;				// the legitimacy of this nation

		// Localisation attributes
		string name;			// the name of this country
		string adjective;		// the adjective for this country
		Color color;			// the color of this country

		map<string, string> namesByLanguage;		// the names of this country in different localisations
		map<string, string> adjectivesByLanguage;	// the adjectives for this country in different localisations
};


#endif // EU4COUNTRY_H_