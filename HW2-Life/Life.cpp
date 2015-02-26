#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>

using namespace std;


struct element {
	int x;
	int y;
};

// result of iteration
struct stats {
	int borned;
	int died;
	int population;
	stats(int p1, int p2, int p3)
	{
		borned = p1;
		died = p2;
		population = p3;
	}
    const stats operator+(const stats & b)
	{
		return stats(b.borned + borned, b.died + died, b.population + population);
	}
};

class pole {	
 private:	 
    element * list;
    int minx = 0;
    int miny = 0;
    int maxx = 0;
    int maxy = 0;
    int count;
    void update();
    int ** genpole(int y, int x, bool cl); // generate pole size of y * x, cl = 1 - clear, not otherwise
 public:
    pole(int x, int y, int number, int random, vector<element> list);
	pole() {};
	int print(int y0,int x0, int camerax, int cameray);
	void startvision();
	stats nextstep();
};

void pole::update()
{
	if(count == 0)
	{
		minx = 0;
		miny = 0;
		maxx = 0;
		maxy = 0;
		return;
	}
	minx = list[0].x;
	maxx = list[0].x;
	miny = list[0].y;
	maxy = list[0].y;
	for(int i = 1;i < count;i++)
	{
		minx = std::min(list[i].x, minx);
	    maxx = std::max(list[i].x, maxx);
	    miny = std::min(list[i].y, miny);
		maxy = std::max(list[i].y, maxy);
	}
	for(int i = 0;i < count;i++)
	{
		list[i].x -= minx;
		list[i].y -= miny;
	}
	maxx -= minx;
	maxy -= miny;
	minx = 0;
	miny = 0;
	maxx++;
	maxy++;

}

pole::pole(int y, int x, int number, int random, vector<element> startlist)
{
	int ** map = genpole(y, x, 1);
	list = new element[number];
	if(random == 0)
		for(int i = 0;i < number;i++)
		{
			int curx = rand()%x;
			int cury = rand()%y;
			while(map[cury][curx])
			{
				curx = rand()%x;
				cury = rand()%y;
			}
			map[cury][curx] = 1;
			list[i].x = curx;
			list[i].y = cury;		
		}
	else
	{
		for(int i = 0;i < startlist.size();i++)
		{
			list[i].x = startlist[i].x;
			list[i].y = startlist[i].y;
		}
	}
	
	count =  number;
	update();	
	delete map;
}

int ** pole::genpole(int y,int x,bool cl)
{
	int ** map = new int * [y];
	for(int i = 0;i < y;i++)
	{
		map[i] = new int[x];
		for(int j = 0;j < x;j++)
			map[i][j] = 0;
	}
	if(!cl)
		for(int i = 0 ;i < count;i++)
		{
			map[list[i].y][list[i].x] = 1;
		}
	return map;
}

int pole::print(int y0, int x0, int cameray, int camerax)
{
	int ** map = genpole(maxy, maxx, 0);
	if(camerax > maxx)
		camerax = maxx;
	if(cameray > maxy)
		cameray = maxy;
	
	if(y0 + cameray > maxy || x0 + camerax > maxx || y0 < 0 || x0 < 0)
		return -1;
	
		
	for(int i = y0;i < y0 + cameray;i++)
	{
		for(int j = x0;j < x0 + camerax;j++)
		{
			if(map[i][j] == 0)
				cout << ' ';
			else
				cout << '#';
		}
		cout << endl;
	}
	
	delete map;
	return 0;
}

void pole::startvision()
{
	if(count == 0)
	{
		cout << "All dieds"; 
	}
	int curx = 0, cury = 0, camera = 20;
	char p;
	cout << "Write W to move up, S to down, A to left, D to right, E to exit:" << endl;
	print(0,0,camera,camera);
	while(1)
	{
		int nextx = curx, nexty = cury;
		
		cin >> p;
		if(p == 'E')
		 break;
		if(p == 'W')
		 nexty -= 1;
		if(p == 'S')
		 nexty += 1;
		if(p == 'A')
		 nextx -= 1;
		if(p == 'D')
		 nextx += 1;
		if(print(nexty, nextx, camera, camera) != -1)
		{
			curx = nextx;
			cury = nexty;
		} 
		else
		{
			cout << "You are on the border of this direction" << endl;
		}
	}
}


stats pole::nextstep()
{
	int ** map = genpole(maxy, maxx, 0);
	int ** next = genpole(maxy + 2, maxx + 2, 1);
	stats result(0,0,0);
	count = 0;
	for(int i = -1; i < maxy + 1; i++)
		for(int j = -1; j < maxx + 1; j++)
		{
			int zas = 0;
			for(int x = -1;x <= 1;x++)
				for(int y = -1;y <= 1;y++)
					if(x != 0 || y != 0)
					{
						if(i + y >= 0 && i + y < maxy && j + x >= 0 && j + x < maxx)
						{
							zas += map[i + y][j + x];
						}
					}
			int cur = 0;
			if(i >= 0 && i < maxy && j >= 0 && j < maxx)
			{
				cur = map[i][j];
			}
			else
			{
				cur = 0;
			}
			if(cur == 0 && zas == 3 || cur == 1 && (zas == 2 || zas == 3))
			{
				next[i + 1][j + 1] = 1;
				count++;
			}
			else
			{
				next[i + 1][j + 1] = 0;
			}
		}
	
	for(int i = 0;i < maxy + 2;i++)
		for(int j = 0;j < maxx + 2;j++)
		{
			if(next[i][j] == 1)
			{
				result.population++;
				if(i >= 1 && i <= maxy && j >= 1 && j <= maxx)
					if(map[i - 1][j - 1] == 0)
						result.borned++;
			}
			else
			{
				if(i >= 1 && i <= maxy && j >= 1 && j <= maxx)
					if(map[i - 1][j - 1] == 1)
						result.died++;
			}
		}
	delete map;
	delete list;
	list = new element[count];
	int cur = 0;
	for(int i = 0;i < maxy + 2;i++)
		for(int j = 0;j < maxx + 2;j++)
			if(next[i][j] == 1 && cur < count)
			{
				list[cur].x = j;
				list[cur].y = i;
				cur++;
			}		
	
	update();
	delete next;
	return result;
}   


int main() {
	srand(time(NULL));
	int x, y, n, t;
	cout << "Height, width, start count, number of iterations: " << endl;
	cin >> x >> y >> n >> t;
	if(x * y > 1e6 || t * x * y > 1e7)
	{
		cout << "It is very big data, program will work long";
		return 0;
	}
	if(n > x*y)
	{
		cout << "Impossible situation";
		return 0;
	}
	cout << "Generate random pole(0 - no, 1 - yes):" << endl;
	int r;
	cin >> r;
	pole myp;
	if(r == 1)
		myp = pole(x,y,n,0,vector<element>(0));
	else
	{
		vector<element> list(n);
		cout << "Write list of coordinates(y, x):" << endl;
		for(int i = 0; i < n;i++)
			cin >> list[i].y >> list[i].x;
		myp = pole(x,y,n,1,list);	
	}
	myp.print(0,0,10000,10000); // it is a trying to print all pole 
	stats res(0,0,0), cur(0,0,0), last(-1,-1,-1);
	vector<int> unchange;
	for(int i = 0;i < t; i++)
	{
		cur = myp.nextstep();
		if(cur.population == last.population)
			unchange.push_back(i);
		res = res + cur;
		
		if((i + 1)%10 == 0)
		{
			cout << i + 1 << " - iteration: " << endl;	
			cout << "    " << cur.population << endl;
			if(res.died != 0)
				cout << "    " << (double)res.borned / res.died << endl;
			else
				cout << "    No died" << endl;
			if(unchange.size() > 0)
			{
				cout << "    Unchanged iterations: ";
				for(int j = 0;j < unchange.size();j++)
					cout << unchange[j] << " ";
				cout << endl;
				res = stats(0,0,0);
				unchange.clear();
			}
		}
		last = cur;
	}
	cout << " !---------! " << endl;
	cout << "Population increasing: " << cur.population - n << endl;
	myp.startvision(); 
	return 0;
}
