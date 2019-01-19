#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <algorithm>

using namespace std;

const int MAXTRIENODE = 1000000,ALPHABET = 26 + 1;
const int root = 0;
const int MAXWORDLIST = 100;
const int MAXSELECT = 10,MAXPOSSIBLE = 10000;
const int MAXLEFT = 3;

int ctoi(char c) {return (int)(c - 'a' + 1);}

char itoc(int i) {return (char)(i - 1 + 'a');}

struct trienode
{
	string str;
	int to[ALPHABET];
	bool isword;
} t[MAXTRIENODE];
int tot = 0;

void addword(string s)
{
	int len = s.length(),now = root;
	for (int i = 0;i < len;i++)
	{
		if(!t[now].to[ctoi(s[i])])
		{
			t[now].to[ctoi(s[i])] = ++tot;
			t[tot].str = t[now].str + s[i];
		}
		now = t[now].to[ctoi(s[i])];
	}
	t[now].isword = true;
}

typedef struct
{
	int l[ALPHABET];
} vec;

vec tovec(string s)
{
	vec res;
	for (int i = 1;i < ALPHABET;i++)
		res.l[i] = 0;
	int len = s.length();
	for (int i = 0;i < len;i++)
		res.l[ctoi(s[i])]++;
	return res;
}

string wordlist[MAXWORDLIST];
int listtot = 0;

void dfs1(int now,vec left)
{
	if (t[now].isword)
		wordlist[++listtot] = t[now].str;
	for (int i = 1;i < ALPHABET;i++)
		if (t[now].to[i] && left.l[i] > 0)
		{
			left.l[i]--;
			dfs1(t[now].to[i],left);
			left.l[i]++;
		}
}

void printwordlist()
{
	cout << endl << endl << "possible word list:" << endl;
	for (int i = 1;i <= listtot;i++)
		cout << wordlist[i] << endl;
	cout << endl << endl;
}

void loaddictionary()
{
	fstream file("dictionary.txt");
	string word;
	while (file >> word && !word.empty())
		addword(word);
	file.close();
}

typedef struct
{
	int cnt;
	string select[MAXSELECT];
	int left[ALPHABET];
	int lefttot;
} choice;

choice possible[MAXPOSSIBLE];
int posstot = 0;

void printchoice(choice res)
{
	for (int i = 1;i <= res.cnt;i++)
		cout << res.select[i] << " ";
	cout << "\tselected " << res.cnt << " words,";
	if (res.lefttot == 0)
	{
		cout << " LEFT LETTER = 0!" << endl;
		return;
	}
	cout << " left letter" << (res.lefttot == 1 ? "" : "s") << ": ";
	for (int i = 1;i < ALPHABET;i++)
		if (res.left[i] > 0)
			for (int j = 1;j <= res.left[i];j++)
				cout << itoc(i) << " ";
	cout << endl;
}

void record(choice c)
{
	if (c.lefttot > MAXLEFT) return; 
	possible[++posstot] = c;
}

choice tochoice(string s)
{
	choice res;
	res.cnt = 0;
	for (int i = 1;i < ALPHABET;i++)
		res.left[i] = 0;
	int len = s.length();
	res.lefttot = len;
	for (int i = 0;i < len;i++)
		res.left[ctoi(s[i])]++;
	return res;
}

bool contain(choice now,int i)
{
	int len = wordlist[i].length();
	for (int j = 0;j < len;j++)
	{
		now.left[ctoi(wordlist[i][j])]--;
		if (now.left[ctoi(wordlist[i][j])] < 0)
			return false;
	}
	return true;
}

bool cmp(choice a,choice b)
{
	return a.lefttot < b.lefttot;
}

void dfs2(int pos,choice now)
{
	bool flag = true;
	for (int i = pos;i <= listtot;i++)
		if (contain(now,i))
		{
			flag = false;
			choice tmp = now;
			tmp.select[++tmp.cnt] = wordlist[i];
			int len = wordlist[i].length();
			for (int j = 0;j < len;j++)
				tmp.left[ctoi(wordlist[i][j])]--;
			tmp.lefttot -= len;
			dfs2(i,tmp);
		}
	if (flag) record(now);
}

int main()
{
	cout << "loading dictionary..." << endl;
	loaddictionary();
	cout << "loading complete" << endl;
	cout << "input pinyin: ";
	string pinyin;
	cin >> pinyin;
	dfs1(root,tovec(pinyin));
	printwordlist();
	dfs2(1,tochoice(pinyin));
	sort(possible + 1,possible + posstot + 1,cmp);
	for (int i = 1;i <= posstot;i++)
		printchoice(possible[i]);
	system("pause");
	return 0;
}
