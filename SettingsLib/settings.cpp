#include<iostream>
#include<string>
#include<cstdio>
#include<map>
#include<vector>


using namespace std;
class settings {
	public:
		class param {
			private:
				std::string data;
				param(param const &);
			public:
				param();
				operator std::string() const;
				operator int() const;
				operator bool() const;
				operator double() const;
 
				param & operator=(std::string const &);
				param & operator=(int);
				param & operator=(bool);
				param & operator=(double);
 
				param & operator+=(std::string const &);
				param & operator+=(int);
				param & operator+=(double);
 
				param & operator-=(int);
				param & operator-=(double);
 
				param & operator*=(int);
				param & operator*=(double);
 
				param & operator/=(int);
				param & operator/=(double);
 
				param & operator|=(bool);
				param & operator&=(bool);
				bool is_empty() const;
		};
 		
				
		
		// Main functions
 
		/**
		 * Construct settings store 
		 * and load data from file (if exists)
		 * \param filename Path to file with settings
		 */
		settings(std::string const & filename);
		/**
		 * Get setting value
		 * \param name Setting unique identifier
		 * \param def Default setting value
		 * \return Stored value for given name or default value
		 */
		std::string const & get(std::string const & name, 
			std::string const & def = "") const;
		/**
		 * Set or replace setting value and save changes to file
		 * \param name Setting unique identifier
		 * \param value New setting value
		 */
		void set(std::string const & name, 
			std::string const & value);
		/**
		 * Reset all settings
		 */
		void reset();		
		/**
		 * Reload all settings from file
		 */	
		void reload();
	private:
		string file;
		int count = 0;
		param buf;
		map<string, string> sets;
		void write();
		void getset();
};



settings::param::param()
{
	data = "";	
}

settings::param::operator std::string() const
{
	return data;
}

settings::param::operator int() const
{
	if(is_empty())
		return 0;
	int res = 0;
	int sign = 1;
	int i = 0;
	if(data[0] == '-')
		sign = -1, i = 1;
	while(i < data.size() && data[i] != '.')
		res = res * 10 + (data[i++] - '0'); 
	return sign * res;	
}	

settings::param::operator double() const
{
	if(is_empty())
		return 0;
	double res = 0;
	int sign = 1;
	long long raz = 1;
	int i = 0;
	if(data[0] == '-')
		sign = -1, i = 1;
	int pos = data.size() - 1;
	while(i < data.size())
		if(data[i] != '.')
			res = res * 10 + (data[i++] - '0');
	 	else
	 		pos = i++;
	for(int i = 0;i < data.size() - 1 - pos;i++)
		raz *= 10;
	return (double)sign * res / raz;	
}

settings::param::operator bool() const
{
	if(int(*this) != 0) 
		return true;
	else
		return false;	
}

settings::param& settings::param::operator=(std::string const & arg)
{
	data = arg;
}

settings::param& settings::param::operator=(int arg)
{
	char szbuff[255];
    string Y;
    sprintf(szbuff, "%d", arg);
    this->data = szbuff;
}

settings::param& settings::param::operator=(double arg)
{
	char szbuff[255];
    string Y;
    sprintf(szbuff, "%.9g", arg);
    this->data = szbuff;
}

settings::param& settings::param::operator=(bool arg)
{
	if(arg == 0)
		this->data = "0";
	if(arg == 1)
		this->data = "1";
}

settings::param& settings::param::operator+=(std::string const & arg)
{
	this->data += arg;
}

settings::param& settings::param::operator+=(int arg)
{
	param s;
	s = double(*this) + arg; 
	this->data = s.data;
}

settings::param& settings::param::operator+=(double arg)
{
	param s;
	s = double(*this) + arg; 
	this->data = s.data;
}

settings::param& settings::param::operator-=(int arg)
{
	param s;
	s = double(*this) - arg; 
	this->data = s.data;
}

settings::param& settings::param::operator-=(double arg)
{
	param s;
	s = double(*this) - arg; 
	this->data = s.data;
}

settings::param& settings::param::operator*=(int arg)
{
	param s;
	s = double(*this) * arg; 
	this->data = s.data;
}

settings::param& settings::param::operator*=(double arg)
{
	param s;
	s = double(*this) * arg; 
	this->data = s.data;
}

settings::param& settings::param::operator/=(int arg)
{
	param s;
	s = double(*this) / arg; 
	this->data = s.data;
}

settings::param& settings::param::operator/=(double arg)
{
	param s;
	s = double(*this) / arg; 
	this->data = s.data;
}

settings::param& settings::param::operator|=(bool arg)
{
	param s;
	s = bool(*this) | arg; 
	this->data = s.data;
}

settings::param& settings::param::operator&=(bool arg)
{
	param s;
	s = bool(*this) & arg; 
	this->data = s.data;
}

bool settings::param::is_empty() const
{
	return data.size() == 0;
}

settings::param::param(param const & arg)
{
	//it's unnecessary
}

void settings::getset() 
{
	char * fname = new char(file.size());
	for(int i = 0;i < file.size(); i++)
		fname[i] = file[i];
	FILE * input = fopen(fname, "r");
	string s1, s2;
	param p;
	while(1)
	{
		char c = 0;
		fscanf(input,"%c",&c);
		if(c == 0)
			break;
		while(c != ' ') 
		{
			s1 = s1 + c;
			fscanf(input,"%c",&c);
		}
		fscanf(input,"%c",&c);
		while(c != ';') 
		{
			s2 = s2 + c;
			fscanf(input,"%c",&c);
		}
		sets[s1] = s2;
		count++; 
		if(s2 == "undefined")
			p = "";
		else
			p = s2;
		s2 = "";
		s1 = "";
	}
	fclose(input);
}
settings::settings(std::string const & filename)
{
	file = filename;
	getset();
}

string const & settings::get(std::string const & name, 
			std::string const & def) const
{
	if(sets.count(name) >= 1)
		return (*sets.find(name)).second;
	else
		return def;
}

void settings::set(std::string const & name, 
			std::string const & value)
{
	sets[name] = value;
	write(); 
}

void settings::write()
{

	char * fname = new char(file.size());
	for(int i = 0;i < file.size(); i++)
		fname[i] = file[i];
	FILE * output = fopen(fname, "w");
	
	for(map<string,string>::iterator it = sets.begin();it != sets.end();it++)
	{
		
		const char *	s1 = (*it).first.c_str();
		const char *	s2 = (*it).second.c_str();
		
		if((*it).second != "")
		{
			fprintf(output, "%s ",s1 );
			fprintf(output, "%s",s2 );
			fprintf(output, "%c",';');
			
		}
		else	
			fprintf(output, "%s undefined\n",s1);
	}
	fclose(output);
}

void settings::reset()
{
	sets.clear();
//	for(map<string,string>::iterator it = sets.begin();it != sets.end();it++)
//	{
//		(*it).second = string("");
//	}
	write();
}

void settings::reload()
{
	getset();
}

/*
int main() {
	settings my("asd.txt");
	string s = "asd";
	my.set(s, "mye");
	my.set("ert", "ertertert");
	my.reset();
	my.set("ert", "erteasdasdrtert");
	
	cout << my.get("ad","des") << endl;

	return 0;
}

*/
