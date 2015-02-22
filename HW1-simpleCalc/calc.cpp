#include <iostream>
#include <vector>
using namespace std;


// element of expression, 1 - symbol, 2 - number
struct object {
	int type = -1;
	int number = 0;
	char symbol = 0;
	void clear()
	{
		type = -1;
		number = 0;
		symbol = 0;
	}
};


// expression
class expression {
	private:	
		vector<object> expr; // given expression		
		int errorfound = 0;
		string errormessage;
		void toleft(int from,int to); // move expression to the left 
		int calculatesegment(int left,int right); // result of expression between left and right inclusive. 
		int closebracketindex(int left); // index of close bracket of given open bracket.
	public:
		int getexpr(); // read expresion from input
		string checkcorrect(); // check the corretc of expression structure 
		void calculate(); // calculate expression 
		int getresult(); // return result
};

bool isvalid(char str); // check the validity of symbol



bool isvalid(char str)
{
	if(!('0' <= str && str <= '9' 
		|| str == '+'
		|| str == '-' 
		|| str == '*' 
		|| str == '/' 
		|| str == '%' 
		|| str == '(' 
		|| str == ')' 
		|| str == ' '))
			return 0;
	return 1;
}


int expression::getresult()
{
	if(expr[0].type == 0)
		return expr[0].number;
}


void expression::calculate()
{
	expr[0].number = calculatesegment(0,expr.size() - 1);
	expr[0].type = 0;
}


int expression::calculatesegment(int left,int right)
{
	
	if(left > right)
		return 0;
	// find brackets and replace them their results	
	for(int i = left;i <= right;i++)
		if(expr[i].symbol == '(')
		{
			int close = closebracketindex(i);
			if(close == i + 1)
			{
				expr[i].number = 1;
				toleft(i + 2,i + 1);
				right -= 1;
			}
			else
			{
				expr[i].number = calculatesegment(i + 1,close - 1); 
				toleft(i + 3,i + 1);
				right -= close - i;
			}
			expr[i].type = 0;
		}
	// now we don't have brackets
	// find all operation in priority order : / , % , * , - , +
	for(int i = left;i <= right;i++)
		if(expr[i].symbol == '/')
		{
			expr[i - 1].number = expr[i - 1].number / expr[i + 1].number;
			toleft(i + 2,i);
			i--;
			right -= 2;
		}
	for(int i = left;i <= right;i++)
		if(expr[i].symbol == '%')
		{
			expr[i - 1].number = expr[i - 1].number % expr[i + 1].number;
			toleft(i + 2,i);
			i--;
			right -= 2;
		}
	for(int i = left;i <= right;i++)
		if(expr[i].symbol == '*')
		{
			expr[i - 1].number = expr[i - 1].number * expr[i + 1].number;
			toleft(i + 2,i);
			i--;
			right -= 2;
		}
	for(int i = left;i <= right;i++)
		if(expr[i].symbol == '-')
		{
			expr[i - 1].number = expr[i - 1].number - expr[i + 1].number;
			toleft(i + 2,i);
			i--;
			right -= 2;
		}
	for(int i = left;i <= right;i++)
		if(expr[i].symbol == '+')
		{
			expr[i - 1].number = expr[i - 1].number + expr[i + 1].number;	
			toleft(i + 2,i);
			i--;
			right -= 2;
		}
	
	return expr[left].number;	
}

int expression::closebracketindex(int left)
{
	int balance = 1;
	int result = left;
	while(balance != 0 && result < expr.size())
	{
		result++;
		if(expr[result].type == 1)
		{
			if(expr[result].symbol == '(')
				balance++;
			if(expr[result].symbol == ')')
				balance--;
		}
	}
	return result;
}

string expression::checkcorrect()
{
	if(errorfound)
	{
		return errormessage;
	}
	for(int i = 1;i < expr.size() - 1;i++)
		if(expr[i].type == 0 && expr[i - 1].type == 0 ||
		   expr[i].type == 1 && expr[i - 1].type == 1 && 
		  (expr[i].symbol != ')' && expr[i].symbol != '(' && expr[i - 1].symbol == '(' ||
		   expr[i - 1].symbol != '(' && expr[i - 1].symbol != ')' && expr[i].symbol == ')' || 
		   expr[i].symbol != '(' && expr[i].symbol != ')' && expr[i - 1].symbol != ')' && expr[i - 1].symbol != '(') || 
		   expr[i - 1].type == 0 && expr[i].symbol == '(' || 
		   expr[i].type == 0 && expr[i - 1].symbol == ')')
		   
		{
			return "Invalid structure";
		}
	
	int balance = 0;
	for(int i = 0;i < expr.size();i++)
	{
		if(expr[i].symbol == '(')
			balance++;
		if(expr[i].symbol == ')')
			balance--;
		if(balance < 0)
			break;
	}
	if(balance)
	{
		return "Brackets missed or extra";
	}
	return "";
}

int expression::getexpr()
{
	char current, last = ' ';
	object buffer;
	int signum = 1;
	while((current = getchar()) != '\n')
	{
		if(!isvalid(current))
		{
			errorfound = 1;
			errormessage = "Invalid symbol";
			return 0; 
		}
		if(current != ' ')
		{
			if('0' <= current && current <= '9')
			{
				if(buffer.type != 0 && last != ' ')
				{
					if(last == '-')
					{
						signum = -1;
					}
					else
					{
						errorfound = 1;
						errormessage = "Missing spaces";
						return 0;
					}
				}
				buffer.clear();
				buffer.type = 0;
				buffer.number = buffer.number * 10 + (int)(current - '0') * signum;
			}
			else
			{
				if(last != ' ')
				{
					errorfound = 1;
					errormessage = "Missing spaces";
					return 0;
				}
				buffer.type = 1;
				buffer.symbol = current;	
			}
		}
		else
		{
			if(buffer.type >= 0)
			{
				expr.push_back(buffer);
				buffer.clear();
				signum = 1;
			}
		}
		last = current;
	}
	if(buffer.type >= 0)
		expr.push_back(buffer);
	buffer.clear();
	buffer.type = 1;
	buffer.symbol = '#';
	expr.push_back(buffer);	
	return 1;
}

void expression::toleft(int from,int to)
{
	while(from < expr.size())
	{
		expr[to] = expr[from];
		to++;
		from++;
	}
	expr.resize(expr.size() - (from - to));
}


int main() {
	//freopen("input.txt","r",stdin);
    expression mye;
    mye.getexpr();
    string check = mye.checkcorrect();
	if(check.empty())
    {
    	mye.calculate();
		cout << mye.getresult() << endl;
	}
	else
	{
		cout << check;
	}
    return 0;
}
