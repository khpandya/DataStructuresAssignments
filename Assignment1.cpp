#include<iostream>
#include<forward_list>
#include<vector>
#include<fstream>
#include<algorithm>
using namespace std;

bool contains(vector<std::string> list,std::string toFind){
    if(std::find(list.begin(),list.end(),toFind)==list.end()){
        return false;
    }
    return true;
}

class countryStats{
    public:
    std::string name;
    int numCitiesInList;
    int totalPopulationInList;

    countryStats(){
        name="default";
        numCitiesInList=-1;
        totalPopulationInList=-1;
    }

    countryStats(std::string countryName,int numCitiesInCountry,int totalPopulationInCountry){
        name=countryName;
        numCitiesInList=numCitiesInCountry;
        totalPopulationInList=totalPopulationInCountry;
    }

    countryStats (const countryStats &old_obj){
        name=old_obj.name;
        numCitiesInList=old_obj.numCitiesInList;
        totalPopulationInList=old_obj.totalPopulationInList;
    }

    friend ostream& operator<<(ostream& os, const countryStats& cs);

    ~countryStats(){

    }

};

ostream& operator<<(ostream& os, const countryStats& cs){
    os << "Country: "<<cs.name<<", "<<"Number of Cities in List: "<<cs.numCitiesInList<<", "<<"Total Population In List: "<<cs.totalPopulationInList<<endl;
    return os;
}

struct cityNode{
    string cityName;
    string countryName;
    int population;
};

forward_list<cityNode> read_record()
{
	// File pointer
	fstream fin;

	// Open the file
	fin.open("WorldCities.csv", ios::in);

	// Read the Data from the file
    int counter=1;
	string city, country, population;
    // list to store data in
    forward_list<cityNode> cityList;
    // there are 25590 rows in the file
	while (counter<25591) {
	
		getline(fin, city,',');
		getline(fin, country,',');
		getline(fin, population,'\n');

        // cout<<city<<endl;
		// cout<<country<<endl;
		// cout<<population<<endl;

        cityNode cN;
        cN.cityName=city;
        cN.countryName=country;
        cN.population=std::stoi(population);

        cityList.push_front(cN);

        counter++;
	}
    return cityList;
}

/*
city not found message if not found
default - get country where the city is located then cout country name endl each city in country with commas endl
if allcountries true - do this for every country with a city named 'city' instead of just the first one you find
if alpha true - cities in each country should be in alpha order
if both true - countries should appear in order. In each country cities should appear in order. 
*/
std::vector<countryStats> printAllInSameCountry(std::string city, bool allCountries, bool alphabetical){
    // each row (city, country, population) in one node. 
    // search list for city, if not found print message.
    // if found, add country to list of countries. if allcountries true, keep searching and add the other countries with the city (if not already in the list)
    // if alpha false - just go through the list again for each country in the list and print the cities everytime you encounter one
    // figure out alphabetical sorting
    // to get your return data - just go over the list of countries you made again and count the number of cities and total population
    // in each country, make an object of type countryStats with the right data and push it to the vector you want to return
    vector<countryStats> countryList;
    vector<std::string> countryListToPrint;
    forward_list<cityNode> cityList;
	cityList=read_record();
    bool cityFound=false;
    
    for(cityNode&cityNode1:cityList){
        // if city is found
        if(cityNode1.cityName==city){
            // add the country of that city to templist
            countryListToPrint.push_back(cityNode1.countryName);
            // if all countries is true go through LL again to find all countries with that city
            if(allCountries==true){
                for(cityNode&cityNode2:cityList){
                    // check that the country doesn't already exist in the templist
                    if(cityNode2.cityName==city && !contains(countryListToPrint,cityNode2.countryName)){
                        countryListToPrint.push_back(cityNode2.countryName);
                    }
                }
            }
            if(alphabetical==true){
                std::sort(countryListToPrint.begin(),countryListToPrint.end());
            }
            //vector of vector of strings. each vector contains list of cities in a country e.g. 1st vector of cities
            //corresponds to in 1st country in countryListToPrint and so on
            vector<vector<std::string>> citiesInEachCountry;
            for(std::string countryName : countryListToPrint){
                vector<std::string> cities;
                for(cityNode&cityNode3:cityList){
                    if(cityNode3.countryName==countryName){
                        cities.push_back(cityNode3.cityName);
                    }
                }
                citiesInEachCountry.push_back(cities);
            }
            //if alphabetical true sort each city list as well
            if (alphabetical==true){
                for(vector<string> cities : citiesInEachCountry){
                    std::sort(cities.begin(),cities.end());
                }
            }
            //do the printing
            for (int countryIndex = 0; countryIndex < countryListToPrint.size(); countryIndex++){
                cout<<countryListToPrint[countryIndex]<<endl;
                for(int cityIndex=0; cityIndex<citiesInEachCountry[countryIndex].size(); cityIndex++){
                    if(cityIndex==citiesInEachCountry[countryIndex].size()-1){
                        cout<<citiesInEachCountry[countryIndex][cityIndex]<<endl;
                    }
                    else{
                        cout<<citiesInEachCountry[countryIndex][cityIndex]<<",";
                    }
                }
                cout<<endl;
            }
            //make the countryStats objects
            for (int countryIndex = 0; countryIndex < countryListToPrint.size(); countryIndex++){
                std::string countryName=countryListToPrint[countryIndex];
                int numOfCitiesInList=citiesInEachCountry[countryIndex].size();
                int population=0;
                for(cityNode&cityNode4:cityList){
                    if(cityNode4.countryName==countryListToPrint[countryIndex]){
                        population+=cityNode4.population;
                    }
                }
                countryStats country=countryStats(countryName,numOfCitiesInList,population);
                countryList.push_back(country);
            }
            cityFound=true;
            break;
        }
    }

    if(!cityFound){
        cout<<"This city is not in the list"<<endl;
    }

    return countryList;
}

int main(){
    vector<countryStats> countryList=printAllInSameCountry("Paris",true,true);    
    return 0;
}
