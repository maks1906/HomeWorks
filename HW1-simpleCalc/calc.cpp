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
		char priority[255]; // "+", "-" - 2, "*", "/", "%" - 1  
		int calculatesegment(int left,int right); // result of expression between left and right inclusive. 
		int operation(int x, int y, char o); // return (x o y)
	public:
		int getexpr(); // read expresion from input
		string checkcorrect(); // check the corretc of expression structure 
		void calculate(); // calculate expression 
		int getresult(); // return result
};

static class errors
{
	public:
	 int errorfound;
	 string message;
} err;

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
	return expr[0].number;
}


int expression::operation(int x, int y, char o)
{
	if(o == '+')
		return x + y;
	if(o == '-')
		return x - y;
	if(o == '*')
		return x * y;
	if(o == '/')
		if(y == 0)
		{
			err.errorfound = 1;
			err.message = "division by zero";
			return 0;
		}
		else
			return x / y;
	if(o == '%')
		if(y == 0)
		{
			err.errorfound = 1;
			err.message = "division by zero";
			return 0;
		}
		else
			return x % y;
		
}


void expression::calculate()
{
	expr[0].number = calculatesegment(0,expr.size() - 1);
	expr[0].type = 0;
}


int expression::calculatesegment(int left,int right)
{
	if(left > right)
		return 1;
	vector<int> number; // stack analog
	vector<int> oper; // stack analog
	bool nwp = 0;
	for(int i = left;i <= right;i++)
	{
		nwp = 0;
		if(expr[i].type == 1)
		{
			if(expr[i].symbol == '(')
			{
				int balance = 1;
				int j = i;
				while(balance > 0)
				{
					j++;
					if(expr[j].type == 1)
					{
						if(expr[j].symbol == '(')
							balance++;
						if(expr[j].symbol == ')')
							balance--;
					}
				}
				number.push_back(calculatesegment(i + 1,j - 1));
				nwp = 1;
				i = j;
			}
			else
			{
				oper.push_back(expr[i].symbol);
			}
		}
		else
		{
			number.push_back(expr[i].number);
			nwp = 1;
		}
		if(nwp)
		while(number.size() >= 2 &&
			 (oper.size() >= 2 && priority[oper[oper.size() - 1]] < priority[oper[oper.size() - 2]] ||
		      oper.size() >= 1 && priority[oper[oper.size() - 1]] == 1))
		      {
		      	int res = operation(number[number.size() - 2],number[number.size() - 1],oper[oper.size() - 1]);
				number.pop_back();
		      	number.pop_back();
		      	oper.pop_back();
		      	number.push_back(res);
		      }
		nwp = 0;    
	}
	while(number.size() >= 2 && oper.size() >= 1)
	{
		int res = operation(number[number.size() - 2],number[number.size() - 1],oper[oper.size() - 1]);
		number.pop_back();
      	number.pop_back();
	   	oper.pop_back();
	   	number.push_back(res);
	}
	if(number.size() != 1 || oper.size() != 0)
		err.errorfound = 1;
	return number[0];	
	
}


string expression::checkcorrect()
{
	for(int i = 1;i < expr.size(); i++)
		if(expr[i].type == 0 && expr[i - 1].type == 0 ||
		   expr[i].type == 1 && expr[i - 1].type == 1 && 
		  (expr[i].symbol != ')' && expr[i].symbol != '(' && expr[i - 1].symbol == '(' ||
		   expr[i - 1].symbol != '(' && expr[i - 1].symbol != ')' && expr[i].symbol == ')' || 
		   expr[i].symbol != '(' && expr[i].symbol != ')' && expr[i - 1].symbol != ')' && expr[i - 1].symbol != '(' ||
		   expr[i].symbol == '(' && expr[i - 1].symbol == ')') || 
		   expr[i - 1].type == 0 && expr[i].symbol == '(' || 
		   expr[i].type == 0 && expr[i - 1].symbol == ')')
		   
		{
			err.errorfound = 1;
			err.message = "Invalid structure";
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
		err.errorfound = 1;
		err.message = "Brackets missed or extra";
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
			err.errorfound = 1;
			err.message = "Invalid symbol";
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
						err.errorfound = 1;
						err.message = "Missing spaces";
						return 0;
					}
				}
				buffer.type = 0;
				buffer.number = buffer.number * 10 + (int)(current - '0') * signum;
			}
			else
			{
				if(last != ' ')
				{
					err.errorfound = 1;
					err.message = "Missing spaces";
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
	priority['+'] = 2;
	priority['+'] = 2;
	priority['%'] = 1;
	priority['/'] = 1;
	priority['*'] = 1;
	return 1;
}



int main() {
	//freopen("input.txt","r",stdin);
	expression mye;
    mye.getexpr();
    string check = mye.checkcorrect();
	if(err.errorfound == 0)
    {
    	mye.calculate();
    	if(err.errorfound == 0)
			cout << mye.getresult() << endl;
		else
			cout << err.message;
	}
	else
	{
		cout << err.message;
	}
    return 0;
}
