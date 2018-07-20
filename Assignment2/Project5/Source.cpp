#include<iostream>
#include<fstream>
#include<strstream>
using namespace std;
const int size = 122;
struct Student
{
	char Name[51];
	char GPA[4];
	char Address[51];
	char Mobile[12];
	char Department[3];
};
struct HashIndex
{
	char Name[51];
	int offset;
};
istream & operator>>(istream & is, Student & s)
{
	cout << "Please Enter The Following Data " << endl;
	cout << "Name : ";
	is.getline(s.Name, 51);
	cout << "GPA : ";
	cin >> s.GPA;
	cin.ignore();
	cout << "Address : ";
	is.getline(s.Address, 51);
	cout << "Mobile : ";
	is.getline(s.Mobile, 12);
	cout << "Department : ";
	is.getline(s.Department, 3);
	return is;
}
ostream & operator<<(ostream & of, Student& s)
{
	of << "Name of Student : " << s.Name << endl;
	of << "GPA Of Student  : " << s.GPA << endl;
	of << "Address Of Student: " << s.Address << endl;
	of << "Mobile Of Student : " << s.Mobile << endl;
	of << "Deprtment Of Student : " << s.Department << endl;
	return of;
}
int H1(char*name)
{
	return (((int)(name[0]))*((int)(name[1]))) % 101;
}
int H2(char*name)
{
	return (((int)(name[2]))*((int)(name[3]))) % 23;
}
HashIndex hashed[101][3];
void m1_Insert(fstream & f, Student s)
{
	char c = '&';
	f.seekp(0, ios::end);
	int ofset = f.tellg();
	char a[size];
	short len;
	strcpy_s(a, s.Name);
	strcat_s(a, "|");
	strcat_s(a, s.GPA);
	strcat_s(a, "|");
	strcat_s(a, s.Address);
	strcat_s(a, "|");
	strcat_s(a, s.Mobile);
	strcat_s(a, "|");
	strcat_s(a, s.Department);
	strcat_s(a, "|");
	len = strlen(a);
	f.write((char*)&c, sizeof(c));
	f.write((char*)&len, sizeof(len));
	f.write(a, len);

	int x = H1(s.Name);
	//cout << "ofset " << ofset << endl;
	for (int i = x; i < 101; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (hashed[i][j].offset < 0)
			{
				strcpy_s(hashed[i][j].Name, s.Name);
				hashed[i][j].offset = ofset;
				return;
			}
		}
	}
}
bool m1_ReadStudent(fstream & f, Student & s, int ofset)
{
	short length;
	char x;
	f.seekg(ofset, ios::beg);
	f.read((char*)&x, sizeof(x));
	if (x == '*')
		return false;
	f.read((char*)&length, sizeof(length));
	char*b = new char[length];
	f.read(b, length);
	istrstream strb(b);
	strb.getline(s.Name, 50, '|');
	strb.getline(s.GPA, 3, '|');
	strb.getline(s.Address, 50, '|');
	strb.getline(s.Mobile, 11, '|');
	strb.getline(s.Department, 2, '|');
	//cout << s;
	delete b;
	return true;
}
int m1_search(fstream & f, Student & s, char name[], int t)
{
	int counter = 0;
	int x = H1(name);
	int ofset;
	for (int i = x; i < 101; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			counter++;
			if (strcmp(hashed[i][j].Name, name) == 0)
			{
				ofset = hashed[i][j].offset;
				m1_ReadStudent(f, s, ofset);
				t = counter;
				return ofset;
			}
			else if (hashed[i][j].offset == -1)
			{
				t = counter;
				return -1;
			}
		}
	}
	return -1;
}
void m1_Delete(fstream & f, Student &s, char nameofstudent[])
{
	int x = H1(nameofstudent);
	int n = 0;
	int t = m1_search(f, s, nameofstudent, n);
	char c = '*';
	if (t == -1)
		cout << "Data Not Found " << endl;
	else
	{
		f.seekp(t, ios::beg);
		f.write((char*)&c, sizeof(c));
		for (int i = x; i < 101; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (strcmp(hashed[i][j].Name, nameofstudent) == 0)
				{
					strcpy_s(hashed[i][j].Name, "*");
					cout << "Data Deleted " << endl;
					return;
				}
			}
		}
	}
}
void m1_loadIndexFile(fstream & f)
{
	if (!f)
		return;
	f.read((char*)hashed, sizeof hashed);
}

void m1_SaveIndexFile(fstream & f)
{
	f.write((char*)hashed, sizeof hashed);
}

/////////////////////////////////////////////////////////method 2 double hashing
HashIndex hashe[101][2];
void m2_Insert(fstream &f, Student s)
{
	f.seekp(0, ios::end);
	int ofset = f.tellg();
	char a[size];
	short len;
	strcpy_s(a, s.Name);
	strcat_s(a, "|");
	strcat_s(a, s.GPA);
	strcat_s(a, "|");
	strcat_s(a, s.Address);
	strcat_s(a, "|");
	strcat_s(a, s.Mobile);
	strcat_s(a, "|");
	strcat_s(a, s.Department);
	strcat_s(a, "|");
	len = strlen(a);
	f.write((char*)&len, sizeof(len));
	f.write(a, len);
	int x = H1(s.Name);
	int y = H2(s.Name);
	int i = 0;
	for (int i = x; i < 101; i += y)
	{
		for (int j = 0; j < 2; j++)
		{
			if (hashed[i][j].offset < 0)
			{
				strcpy_s(hashed[i][j].Name, s.Name);
				hashed[i][j].offset = ofset;
				return;
			}
		}
	}
}
bool m2_ReadStudent(fstream & f, Student & s, int ofset)
{
	short length;
	char x;
	f.seekg(ofset, ios::beg);
	f.read((char*)&x, sizeof(x));
	if (x == '*')
		return false;
	f.read((char*)&length, sizeof(length));
	char*b = new char[length];
	f.read(b, length);
	istrstream strb(b);
	strb.getline(s.Name, 50, '|');
	strb.getline(s.GPA, 3, '|');
	strb.getline(s.Address, 50, '|');
	strb.getline(s.Mobile, 11, '|');
	strb.getline(s.Department, 2, '|');
	//cout << s;
	delete b;
	return true;
}
int m2_search(fstream & f, Student &s, char name[], int q)
{
	int counter = 0;
	int x = H1(name);
	int y = H2(name);
	int ofset;
	for (int i = x; i < 101; i += y)
	{
		for (int j = 0; j < 2; j++)
		{
			counter++;
			if (hashed[i][j].offset >= 0 && strcmp(hashed[i][j].Name, name) == 0)
			{
				q = counter;
				ofset = hashed[i][j].offset;
				return ofset;
			}
		}
	}
	return -1;
}
void m2_Delete(fstream & f, Student &s, char name[])
{
	int x = H1(name);
	int y = H2(name);
	int q = 0;
	char c = '*';
	int t = m2_search(f, s, name, q);
	if (t == -1)
		cout << "Data Not Found" << endl;
	else
	{
		f.seekg(t, ios::beg);
		f.write((char*)&c, sizeof (c));
		for (int i = x; i < 101; i += y)
		{
			for (int j = 0; j < 2; j++)
			{
				if (strcmp(hashed[i][j].Name, name) == 0)
				{
					strcpy_s(hashed[i][j].Name, "*");
					cout << "Data Deleted " << endl;
					return;
				}
			}
		}
	}
}

void m2_loadIndexFile(fstream & f)
{
	if (!f)
		return;
	f.read((char*)hashed, sizeof hashed);
}
void m2_SaveIndexFile(fstream & f)
{
	f.write((char*)hashed, sizeof hashed);
}
///////////////////////////////////////////////////method3 scatter table
int arr[101];
void m3_Insert(fstream & f, Student s)
{
	char c = '&';
	int x = H1(s.Name);
	f.seekp(0, ios::end);
	int ofset = f.tellg();
	char a[size];
	short len;
	strcpy_s(a, s.Name);
	strcat_s(a, "|");
	strcat_s(a, s.GPA);
	strcat_s(a, "|");
	strcat_s(a, s.Address);
	strcat_s(a, "|");
	strcat_s(a, s.Mobile);
	strcat_s(a, "|");
	strcat_s(a, s.Department);
	strcat_s(a, "|");
	len = strlen(a);
	f.write((char*)&c, sizeof(c));
	f.write((char*)&len, sizeof(len));
	f.write(a, len);
	//	cout << "ofset " << ofset << endl;
	int t = arr[x];
	cout << t << endl;
	f.write((char*)&t, sizeof(t));
	arr[x] = ofset;

}
bool m3_ReadStudent(fstream & f, Student & s, int ofset)
{
	short length;
	char x = ' ';
	if (ofset < 0)
		return false;
	f.seekg(ofset, ios::beg);
	while (!f.eof() && f.tellg() >= 0 && x != '&')
	{
		f >> x;
		if (f.eof() || f.tellg() < 0)
			return false;
	}
	if (f.eof() || f.tellg() < 0)
		return false;
	f.read((char*)&x, sizeof(x));

	if (x == '*')
		return false;
	f.read((char*)&length, sizeof(length));
	char*b = new char[length];
	f.read(b, length);
	istrstream strb(b);
	strb.getline(s.Name, 50, '|');
	strb.getline(s.GPA, 3, '|');
	strb.getline(s.Address, 50, '|');
	strb.getline(s.Mobile, 11, '|');
	strb.getline(s.Department, 2, '|');
	//cout << s;
	delete b;
	return true;
}
int m3_search(fstream & f, Student &s, char name[], int i)
{
	int x = H1(name);
	if (arr[x] == -1)
		return -1;
	int y = arr[x];
	cout << x << " " << arr[x] << endl;
	while (y != -1 && m3_ReadStudent(f, s, y))
	{
		if (strcmp(s.Name, name) == 0){
			cout << s.Name << " " << name << endl;
			return y;
		}
		else
			f.read((char*)&y, sizeof(y));
	}
	return -1;
}
void m3_Delete(fstream & f, Student & s, char name[])
{
	int p, cur, next;
	int x = H1(name);
	char c = '*';
	if (arr[x] == -1)
		return;
	p = cur = arr[x];
	m3_ReadStudent(f, s, cur);
	f.read((char*)&next, sizeof(next));
	cout << p << " " << cur << " " << next << endl;
	if (strcmp(s.Name, name) == 0)
	{
		f.seekg(cur, ios::beg);
		f.write((char*)&c, sizeof(c));
		arr[x] = next;
		return;
	}
	p = cur;
	cur = next;
	while (cur != -1 && m3_ReadStudent(f, s, cur))
	{


		cout << p << " " << cur << " " << next << endl;

		f.read((char*)&next, sizeof(next));
		if (strcmp(s.Name, name) == 0)
		{
			f.seekg(cur, ios::beg);
			f.write("*", sizeof(char));
			m3_ReadStudent(f, s, p);
			f.write((char*)&next, sizeof next);
			return;
		}

		p = cur;
		cur = next;
	}
}
void m3_loadIndexFile(fstream & f)
{
	f.read((char*)arr, sizeof(arr));
}
void m3_SaveIndexFile(fstream & f)
{
	f.write((char*)arr, sizeof(arr));
}

int main()
{
	for (int i = 0; i < 101; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			hashed[i][j].offset = -1;
			strcpy_s(hashed[i][j].Name, "-4");
		}
	}

	for (int i = 0; i < 101; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			hashe[i][j].offset = -1;
			strcpy_s(hashe[i][j].Name, "-4");
		}
	}
	for (int i = 0; i < 101; i++)
		arr[i] = -1;
	int q;
	cout << "1)Linear Probing \n2)Double Hashing \n3)Scatter Table\n 4)Comparison" << endl;
	cout << "Enter Your Choise" << endl;
	cin >> q;
	if (q == 1)
	{
		fstream f, f1;
		Student s;
		int c;
		int u = 0;
		char name[51];
		f.open("Method1Data.txt", ios::out | ios::in | ios::trunc);
		f1.open("Method1IndexFile.txt", ios::out);
		while (true)
		{
			cout << "1) inset data \n2)serach data \n3) delete data \n4)load file \n5) Exit " << endl;
			cin >> c;
			if (c == 1)
			{
				cin.ignore();
				cin >> s;
				m1_Insert(f, s);
			}
			else if (c == 2)
			{
				cin.ignore();
				cout << "Enter name to search with it : ";

				cin.getline(name, 50);
				if (m1_search(f, s, name, u) == -1)
					cout << "NOT Found " << endl;
				else
				{
					cout << "Found " << endl;
					cout << s << endl;
				}
			}
			else if (c == 3)
			{
				cin.ignore();
				cout << "Enter name to delete its data : ";
				cin.getline(name, 50);
				m1_Delete(f, s, name);
			}
			else if (c == 4)
			{
				m1_loadIndexFile(f1);
			}
			else if (c == 5)
			{
				m1_SaveIndexFile(f1);
				f.close();
				f1.close();
				system("pause");
				return 0;
			}
		}
	}
	else if (q == 2)
	{
		cin.ignore();
		fstream f2, f3;
		Student t;
		int c;
		int u = 0;
		char name[51];
		f2.open("Method2Data.txt", ios::out);// | ios::in | ios::trunc);
		f3.open("Method2IndexFile.txt", ios::out);
		while (true)
		{
			cout << "1) inset data \n2)serach data \n3) delete data \n4)load file \n5) Exit " << endl;
			cin >> c;
			if (c == 1)
			{
				cin.ignore();
				cin >> t;
				m2_Insert(f2, t);
			}
			else if (c == 2)
			{
				cin.ignore();
				cout << "Enter name to search with it : ";

				cin.getline(name, 50);
				if (m2_search(f2, t, name, u) == -1)
					cout << "NOT Found " << endl;
				else
				{
					cout << "Found " << endl;
					cout << t << endl;
				}
			}
			else if (c == 3)
			{
				cin.ignore();
				cout << "Enter name to delete its data : ";
				cin.getline(name, 50);
				m2_Delete(f2, t, name);
			}
			else if (c == 4)
			{
				m2_loadIndexFile(f3);
			}
			else if (c == 5)
			{
				m2_SaveIndexFile(f3);
				f2.close();
				f3.close();
				system("pause");
				return 0;
			}
		}
	}
	else if (q == 3)
	{
		cin.ignore();
		fstream f2, f3;
		Student t;
		int c;
		int u = 0;
		char name[51];
		f2.open("Method3Data.txt", ios::out);// | ios::in | ios::trunc);
		f3.open("Method3IndexFile.txt", ios::out);
		while (true)
		{
			cout << "1) inset data \n2)serach data \n3) delete data \n4)load file \n5) Exit " << endl;
			cin >> c;
			if (c == 1)
			{
				cin.ignore();
				cin >> t;
				m3_Insert(f2, t);
			}
			else if (c == 2)
			{
				cin.ignore();
				cout << "Enter name to search with it : ";

				cin.getline(name, 50);
				if (m3_search(f2, t, name, u) == -1)
					cout << "NOT Found " << endl;
				else
				{
					cout << "Found " << endl;
					cout << t << endl;
				}
			}
			else if (c == 3)
			{
				cin.ignore();
				cout << "Enter name to delete its data : ";
				cin.getline(name, 50);
				m3_Delete(f2, t, name);
			}
			else if (c == 4)
			{
				m3_loadIndexFile(f3);
			}
			else if (c == 5)
			{
				m3_SaveIndexFile(f3);
				f2.close();
				f3.close();
				system("pause");
				return 0;
			}
		}
	}
	else if (q == 4)
	{
		cin.ignore();
		Student s;
		fstream f1, f2, f3, f4, f5, f6;
		fstream f7, f8, f9, f10,f11;//data 
		f1.open("Method1Data.txt", ios::out|ios::in| ios::trunc);
		f2.open("Method1IndexFile.txt", ios::out);

		f7.open("add1.txt", ios::out | ios::in);
		f8.open("search1.txt", ios::out | ios::in);
		f9.open("delete.txt", ios::out | ios::in);
		f10.open("add2.txt", ios::out | ios::in);
		f11.open("search2.txt", ios::out | ios::in);

		while (!f7.eof())
		{
			if (f7.eof())
				break;
			f7.getline(s.Name, 51); 
			f7.getline(s.GPA, 4);
		    f7.getline(s.Address, 51);
			f7.getline(s.Mobile, 12);
			f7.getline(s.Department, 3);
		f7.seekg(2, ios::cur);
		if (f7.eof() || f7.tellg() < 0) 
			break;
		m1_Insert(f1, s);
		}
		int y = 0;
		int z = 0;
		while (!f8.eof())
		{
			if (f8.eof())
				break;
			f8.getline(s.Name, 51);
			if (f8.eof())
				break;
			m1_search(f1, s, s.Name, y);
			z += y;
		}
		while (!f9.eof())
		{
			if (f9.eof())
				break;
			f9.getline(s.Name, 51);
			if (f9.eof())
				break;
			m1_Delete(f1, s, s.Name);
		}
		while (!f10.eof())
		{
			if (f10.eof())
				break;
			f10.getline(s.Name, 51);
			f10.getline(s.GPA, 4);
			f10.getline(s.Address, 51);
			f10.getline(s.Mobile, 12);
			f10.getline(s.Department, 3);
			f10.seekg(2, ios::cur);
			if (f10.eof() || f10.tellg() < 0)
				break;
			m1_Insert(f1, s);
		}
		int m = 0;
		int n = 0;
		while (!f11.eof())
		{
			if (f11.eof())
				break;
			f11.getline(s.Name, 51);
			m1_search(f1, s, s.Name, m);
			n += m;
		}
		cout << "Data In Case 1 is " << (n / 15) + (z / 27) << endl;
		f1.close();
		f2.close();
		f7.close();
		f8.close();
		f9.close();
		f10.close();
		f11.close();
		//////////////////////////////////////////////////////////////
		f3.open("Method2Data.txt", ios::out| ios::in |ios::trunc);
		f4.open("Method2IndexFile.txt", ios::out);
		f7.open("add1.txt", ios::out | ios::in);
		f8.open("search1.txt", ios::out | ios::in);
		f9.open("delete.txt", ios::out | ios::in);
		f10.open("add2.txt", ios::out | ios::in);
		f11.open("search2.txt", ios::out | ios::in);

		while (!f7.eof())
		{
			if (f7.eof())
				break;
			f7.getline(s.Name, 51);
			f7.getline(s.GPA, 4);
			f7.getline(s.Address, 51);
			f7.getline(s.Mobile, 12);
			f7.getline(s.Department, 3);
			f7.seekg(2, ios::cur);
			if (f7.eof() || f7.tellg() < 0)
				break;
			m2_Insert(f1, s);
		}
		 y = 0;
		 z = 0;
		while (!f8.eof())
		{
			if (f8.eof())
				break;
			f8.getline(s.Name, 51);
			if (f8.eof())
				break;
			m2_search(f1, s, s.Name, y);
			z += y;
		}
		while (!f9.eof())
		{
			if (f9.eof())
				break;
			f9.getline(s.Name, 51);
			if (f9.eof())
				break;
			m2_Delete(f1, s, s.Name);
		}
		while (!f10.eof())
		{
			if (f10.eof())
				break;
			f10.getline(s.Name, 51);
			f10.getline(s.GPA, 4);
			f10.getline(s.Address, 51);
			f10.getline(s.Mobile, 12);
			f10.getline(s.Department, 3);
			f10.seekg(2, ios::cur);
			if (f10.eof() || f10.tellg() < 0)
				break;
			m2_Insert(f1, s);
		}
		 m = 0;
		 n = 0;
		while (!f11.eof())
		{
			if (f11.eof())
				break;
			f11.getline(s.Name, 51);
			m2_search(f1, s, s.Name, m);
			n += m;
		}
		cout << "Data In Case 2 is " << (n / 15) + (z / 27) << endl;
		f1.close();
		f2.close();
		f7.close();
		f8.close();
		f9.close();
		f10.close();
		f11.close();

		/////////////////////////case 3 
		f5.open("Method3Data.txt", ios::out | ios::in | ios::trunc);
		f6.open("Method3IndexFile.txt", ios::out);
		f7.open("add1.txt", ios::out | ios::in);
		f8.open("search1.txt", ios::out | ios::in);
		f9.open("delete.txt", ios::out | ios::in);
		f10.open("add2.txt", ios::out | ios::in);
		f11.open("search2.txt", ios::out | ios::in);

		while (!f7.eof())
		{
			if (f7.eof())
				break;
			f7.getline(s.Name, 51);
			f7.getline(s.GPA, 4);
			f7.getline(s.Address, 51);
			f7.getline(s.Mobile, 12);
			f7.getline(s.Department, 3);
			f7.seekg(2, ios::cur);
			if (f7.eof() || f7.tellg() < 0)
				break;
			m3_Insert(f1, s);
		}
		 y = 0;
		 z = 0;
		while (!f8.eof())
		{
			if (f8.eof())
				break;
			f8.getline(s.Name, 51);
			if (f8.eof())
				break;
			m3_search(f1, s, s.Name, y);
			z += y;
		}
		while (!f9.eof())
		{
			if (f9.eof())
				break;
			f9.getline(s.Name, 51);
			if (f9.eof())
				break;
			m3_Delete(f1, s, s.Name);
		}
		while (!f10.eof())
		{
			if (f10.eof())
				break;
			f10.getline(s.Name, 51);
			f10.getline(s.GPA, 4);
			f10.getline(s.Address, 51);
			f10.getline(s.Mobile, 12);
			f10.getline(s.Department, 3);
			f10.seekg(2, ios::cur);
			if (f10.eof() || f10.tellg() < 0)
				break;
			m3_Insert(f1, s);
		}
		 m = 0;
		 n = 0;
		while (!f11.eof())
		{
			if (f11.eof())
				break;
			f11.getline(s.Name, 51);
			m3_search(f1, s, s.Name, m);
			n += m;
		}
		cout << "Data In Case 3 is " << (n / 15) + (z / 27)<<endl;
		f1.close();
		f2.close();
		f7.close();
		f8.close();
		f9.close();
		f10.close();
		f11.close();
	}
}