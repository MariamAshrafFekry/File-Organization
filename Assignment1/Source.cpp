#include<iostream>
#include<fstream>
#include <strstream>
#include<string>
using namespace std;
const int size = 125;

struct  NoteBook
{
	char ID[21];
	char Brand[21];
	char ProcessorType[51];
	char ProcessorSpeed[5];
	char OperatingSystem[21];
};
NoteBook arrNotebook[1000000];

istream & operator>>(istream & is, NoteBook & n)
{
	cout << "Enter the ID : ";
	is >> n.ID;
	cout << "Enter the Brand : ";
	cin.ignore();
	is.getline(n.Brand, 21);
	cout << "Enter the Processor Type : ";
	is.getline(n.ProcessorType, 51);
	cout << "Enter the Processor Speed : ";
	is >> n.ProcessorSpeed;
	cout << "Enter the Operating System : ";
	cin.ignore();
	is.getline(n.OperatingSystem, 21);
	return is;
}
ostream& operator<<(ostream & os, NoteBook &n)
{
	os << "The ID is : " << n.ID << endl;
	os << "The Brand is : " << n.Brand << endl;
	os << "The Processor Type is : " << n.ProcessorType << endl;
	os << "The Processor Speed is : " << n.ProcessorSpeed << endl;
	os << "The Operating System is : " << n.OperatingSystem << endl;
	return os;
}
void writeNoteBook(fstream & f, NoteBook& n)
{
	char b[size];
	short len;
	char y = '&';
	strcpy_s(b, n.ID);
	strcat_s(b, "|");
	strcat_s(b, n.Brand);
	strcat_s(b, "|");
	strcat_s(b, n.ProcessorType);
	strcat_s(b, "|");
	strcat_s(b, n.ProcessorSpeed);
	strcat_s(b, "|");
	strcat_s(b, n.OperatingSystem);
	strcat_s(b, "|");
	len = strlen(b);
	f.write(&y, sizeof(y));
	f.write((char*)&len, sizeof(len));
	f.write(b, len);
}
bool readNoteBook(fstream& f, NoteBook &n)
{
	f.seekg(-1, ios::cur);     
	short len;
	char x;
	x = '*';
	while (x != '&'&&!f.eof())
	{
		f.read(&x, sizeof(x));

	}
	if (f.eof())
		return false;
	f.read((char*)&len, sizeof(len));

	char*b = new char[len];
	f.read(b, len);
	istrstream strb(b);
	strb.getline(n.ID, 20, '|');
	strb.getline(n.Brand, 20, '|');
	strb.getline(n.ProcessorType, 50, '|');
	strb.getline(n.ProcessorSpeed, 4, '|');
	strb.getline(n.OperatingSystem, 20, '|');
	//cout << n;
	delete b;
	return true;
}
int SearchNoteBook(fstream &f, NoteBook &n, char*id)
{
	char x = ' ';
	int offset;
	f.seekg(4, ios::beg);
	offset = f.tellg();
	while (!f.eof())
	{
		f.read(&x, sizeof(x));
		if (x == '&')
		{
			f.seekg(-1, ios::cur);
			offset = f.tellg();
			readNoteBook(f, n);
			if (strcmp(n.ID, id) == 0)
			{
				return offset;
			}
		}
	}
	return -1;
}
void DeleteNoteBook(fstream &f, char*id)
{
	NoteBook n;
	char t = '*';
	short s;
	int i;//availlist
	f.seekg(0, ios::beg);
	f.read((char*)&i, sizeof(i));
	int x = SearchNoteBook(f, n, id);
	if (x != -1)
	{
		f.seekp(x, ios::beg);
		f.write(&t, sizeof t);
		f.seekg(x + 1, ios::beg);
		f.read((char*)&s, sizeof(s));
		f.seekp(x + 1, ios::beg);
		f.write((char*)&i, sizeof(i));
		f.write((char*)&s, sizeof(s));
		f.seekp(0, ios::beg);
		f.write((char*)&x, sizeof x);
	}
	else
		cout << "the record not found " << endl;
}

void UpdateNoteBook(fstream &f, NoteBook & n, char*id)
{
	NoteBook m;
	short len;
	int x = SearchNoteBook(f, m, id);
	if (x == -1)
		return;
	f.seekg(x + 1, ios::beg);
	f.read((char*)&len, sizeof(len));

	f.seekp(x, ios::beg);

	char b[size];
	short length;
	char y = '&';
	strcpy_s(b, n.ID);
	strcat_s(b, "|");
	strcat_s(b, n.Brand);
	strcat_s(b, "|");
	strcat_s(b, n.ProcessorType);
	strcat_s(b, "|");
	strcat_s(b, n.ProcessorSpeed);
	strcat_s(b, "|");
	strcat_s(b, n.OperatingSystem);
	strcat_s(b, "|");
	length = strlen(b);
	if (len >= length)
	{
		f.seekg(x, ios::beg);
		writeNoteBook(f, n);
	}
	else
	{
		DeleteNoteBook(f, id);
		f.seekp(0, ios::end);
		writeNoteBook(f, n);
	}
}
void CompactFile(fstream & f)
{
	int counter = 0;
	f.seekg(4, ios::beg);
	while (readNoteBook(f, arrNotebook[counter]) && counter < 1000000)
	{
		counter++;
	}
	f.close();
	f.open("notebooks.txt", ios::in | ios::out | ios::trunc);
	int n = -1;
	f.write((char*)&n, sizeof(n));
	for (int i = 0; i < counter; i++)
	{
		writeNoteBook(f, arrNotebook[i]);
	}
}
int main()
{
	fstream f;
	int Mychoose;
	NoteBook Note;
	char id[21];
	//int n = -1;
	/*f.open("NoteBooks.txt",ios::out);
	f.close();*/
	f.open("NoteBooks.txt", ios::in | ios::out);
	//f.write((char*)&n, sizeof(n));
	/*cin >> Note;
	writeNoteBook(f, Note);
	cin >> Note;
	writeNoteBook(f, Note);
	cin >> Note;
	writeNoteBook(f, Note);
	cin >> Note;
	writeNoteBook(f, Note);
	cin >> Note;
	writeNoteBook(f, Note);
	cin >> Note;
	writeNoteBook(f, Note);
	f.close();*/

	cout << " 1) Add a notebook : " << " 2) Update a notebook : " << "3) Delete a notebook : " <<
		" 4) Display a notebook : " << " 5) Compact the file : " << " 6) Exit : " << endl;
	cout << "Enter your Choise From 1 to 6 : ";

	cin >> Mychoose;
	do
	{
		if (Mychoose == 1) 
		{
		    
			cin >> Note;
			f.seekp(0,ios::end);
			writeNoteBook(f, Note);
		}
		else if (Mychoose == 2)
		{
			
			cout << "enter id you want to update its data : ";
			cin >> id;
			cout << "Enter new data : "<<endl;
			cin.ignore();
			cin >> Note;
			UpdateNoteBook(f,Note,id);
		}
		else if (Mychoose == 3)
		{
			cout << "enter id you want to delete its data : ";
			cin>>id;
			DeleteNoteBook(f,id);
		}
		else if (Mychoose == 4)
		{
			cout << "enter id you want to search its data : ";
			cin>>id;
			if (SearchNoteBook(f, Note, id) != -1)
				cout << Note << endl;
			else
			cout << "the record not found " << endl;
		}
		else if (Mychoose == 5)
		{
			CompactFile(f);
		}
		else if (Mychoose == 6)
		{
			f.close();
			return 0;
		}
	} 
	while (cin >> Mychoose);
	system("pause");
	return 0;
}