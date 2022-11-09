#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <climits>
#include <algorithm>

using namespace std;

map<int, int> DaysInMonth{{1,31},{2,28},{3,31},{4,30},{5,31},{6,30},{7,31},{8,31},{9,30},
{10,31},{11,30},{12,31}};

int ReturnDaysInMonth(int month);
int ReturnLastId();
void DisplayTasks(char sort);
class Date
{
	private:
		int Day;
		int Month;
		int Year;
		int DateArr[3];
		int* pDateArr(string aDate);
	public:
		bool SetDate(string aDate);
		string ToString();
};

class Task
{
	private:
		int Id;
		string Description;
		int Priority;
		Date Due;
		string CompletionNote;
	public:
		Task(string aTaskStr);
		void CreateTask(string aDescription, string aPriority, Date aDue);
		void RemoveTask(int aId);
		bool CompleteTask(int aId, string aCompletionNote);
		void ToString(string aTaskStr);
};
int main()
{
	while(true)
	{
		cout << "---------- Project Manager ----------" << std::endl;
		cout << "." << std::endl << "." << std::endl << "." << std::endl;
		cout << "What would you like to do?" << endl;
		cout << "1. View Tasks" << endl;
		cout << "2. Create Task" << endl;
		cout << "3. Complete Task" << endl;
		cout << "4. Cancel Task" << endl;
		cout << "5. Exit" << endl;
		char input;
		cin >> input;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		switch(input)
		{
			case '1':
			cout << "\nSpecify sort: \n1: Creation Date \n2: Due Date \n3: Priority \n";
			char sort;
			cin >> sort;
			DisplayTasks(sort);
			break;
			
			case '2':
			{
				cout << "Task Description: ";
				string aDescription;
				getline(cin, aDescription);
				cout << "Specify Priority (0-100): ";
				string aPrio;
				getline(cin, aPrio);
				cout << "Specify Due Date (dd-mm-yyyy) (defaults to today): ";
				string due;
				getline(cin, due);
				Date aDue;
				aDue.SetDate(due);
				Task t;
				t.CreateTask(aDescription, aPrio, aDue);
			}
			break;
		
			case '5':
			return 0;
		}
	}
	return 0;
}
int* Date::pDateArr(string DateStr)
{
	try
	{	
		size_t delimiter1 = 0;
		size_t  delimiter2 = 0;
		int index = 0;
		do
		{
			delimiter2 = DateStr.find('-', delimiter2+1);
			DateArr[index] = stoi(DateStr.substr(delimiter1, delimiter2-delimiter1));
			delimiter1 = delimiter2 + 1;
			index++;
		}
		while(delimiter2 != string::npos);
		
		return &DateArr[0];
	}
	catch(const std::out_of_range& oor)
	{
		cout << "Using default date (today)" << endl;
		DateArr[0] = 1;
		DateArr[1] = 1;
		DateArr[2] = 2022;
		return &DateArr[0];
	}
}
bool Date::SetDate(string aDate)
{
	//call conversion function to get Date as int array, not string
	//technically think this is fine to be done without pointers
	//but for the sake of learning, im going to use pointers
	int* pDate = this -> pDateArr(aDate);
	cout << *pDate << "   " << *(pDate + 1) << "   " << *(pDate + 2) << endl;
	int DateArr[] = {*pDate, *(pDate + 1), *(pDate + 2)};		
	if(2022 <= DateArr[2] <= 3000)
	{
		if(1 <= DateArr[1] <= 12)
		{
			if(1 <= DateArr[0] <= ReturnDaysInMonth(DateArr[1]))
			{
				Day = DateArr[0];
				Month = DateArr[1];
				Year = DateArr[2];
				return true;
			}
		}
	}
	return false;
}

string Date::ToString()
{
	stringstream ss;
	ss << Day << '-' << Month << '-' << Year;
	return ss.str();
}

void Task::CreateTask(string aDescription, string aPriority, Date aDue)
{
	try
	{

		Id = ReturnLastId() + 1;
	}
	catch(const invalid_argument& ia)
	{
		cout << "Invalid argument for: 'ID' - check file." << endl;
		return;
	}
	replace(aDescription.begin(), aDescription.end(), ',', '.');
	Description = aDescription;
	Priority = stoi(aPriority);
	Due = aDue;
	
	ofstream file;
	file.open("manager.txt", ios::app);
	if(file.is_open())
	{
		file <<  Id << ',' << Description << ',' << Priority << ',' << aDue.ToString() << endl;
	}
	file.close();
}

void Task::RemoveTask(int aId)
{

}

bool Task::CompleteTask(int aId, string aCompletionNotice)
{
	return true;
}

Task::Task(string aTaskStr)
{
	size_t delimiter1 = 0;
	size_t delimiter2 = -1;
	for(int i = 0; i < 4; i++)
	{
		delimiter2 = aTaskStr.find(',', delimiter2+1);
		string extractedValue = aTaskStr.substr(delimiter1, delimiter2-delimiter1);
		delimiter1 = delimiter2 + 1;
		switch(i)
		{
			case 0:
				Id = stoi(extractedValue);
				break;
			case 1:
				Description = extractedValue;
				break;
			case 2:
				Priority = stoi(extractedValue);
				break;
			case 3:
				Date aDue;
				aDue.SetDate(extractedValue);
				Due = aDue;
				break;
		}
	}
	cout << "ID: " << Id << " DESC: " << Description << " PRIO: " << Priority << " DUE: " << Due.ToString() << endl;
}
void Task::ToString(string aTaskStr)		
{
	//0,the description of the task, 59, 23-05-2022\n
	string header[] = {"Id: ", "Description: ", "priority: ", "Due: "};
	size_t delimiter1 = 0;
	size_t delimiter2 = -1;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << ".										." << endl;
	cout << ".										." << endl;		
	cout << ".										." << endl;
	for(int i = 0; i < 4; i++)
	{
		delimiter2 = aTaskStr.find(',', delimiter2+1);						//find(char to find, pos to start));
		cout << "	" << header[i] << aTaskStr.substr(delimiter1, delimiter2-delimiter1) << endl;	//substr(position, length)
		delimiter1 = delimiter2 + 1;
	}
	cout << ".										." << endl;
	cout << ".										." << endl;
	cout << ".										." << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
}
int ReturnDaysInMonth(int month)
{
	return DaysInMonth[month];
}

int ReturnLastId()
{
	
	ifstream file;
	string taskId;
	file.open("manager.txt", ios::binary | ios::ate);
	if(!file.is_open())
	{
		return 0;
	}
	streampos size = file.tellg();
	char ch;	
	for(int i = 3; i <= size; i++)
	{
		file.seekg(-i, ios_base::end);
		file.get(ch);
		if(ch == '\n')
		{
			break;
		}
	}
	do
	{
		taskId += ch;
		file.get(ch);
	}
	while(ch != ',');
	file.close();
	return stoi(taskId);
}

void DisplayTasks(char sort)
{
	ifstream file;
	file.open("manager.txt", ios::binary);
	char ch;
	if(file.is_open())
	{
		do
		{
			string task;
			do
			{
				file.get(ch);
				task += ch;
			}
			while(ch != '\n');
			Task t(task);
			t.ToString(task);
		}
		while(file.peek() != EOF);
	}
	else
	{
		cout << "You have no active tasks! Congratulations!" << endl;
		return;
	}
}
