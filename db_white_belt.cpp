#include <string>
#include <iostream>
#include <map>
#include <set>
#include <ostream>
#include <sstream>
#include <iomanip>
using namespace std;

class Date {
public:
	Date(int new_year, int new_month, int new_day){
        if (new_month < 1 || new_month > 12)
        {
            throw out_of_range("Month value is invalid: " + to_string(new_month));
        } else if (new_day < 1 || new_day > 31)
        {
            throw out_of_range("Day value is invalid: " + to_string(new_day));
        }
        year = new_year;
        month = new_month;
        day = new_day;
	}

	int GetYear() const{
		return year;
	}
	int GetMonth() const{
		return month;
	}
	int GetDay() const{
		return day;
	}
private:
	int year;
	int month;
	int day;
};

/////////////////////////////////////////////////////////////////////

//istream operator>>(istream& stream, Date& date){
//	if(stream){
//		int year,month,day;
//		bool
//		stream >> year;
//		if()
//	}
//}

ostream& operator<<(ostream& stream, const Date date){
	stream << setfill('0') << setw(4) << to_string(date.GetYear());
	stream << '-';
	stream << setfill('0') << setw(2) << to_string(date.GetMonth());
	stream << '-';
	stream << setfill('0') << setw(2) << to_string(date.GetDay());
	return stream;
}

bool operator<(const Date& lhs, const Date& rhs){
	if(lhs.GetYear() == rhs.GetYear()){
		if(lhs.GetMonth() == rhs.GetMonth()){
			return lhs.GetDay() < rhs.GetDay();
		}
		else return lhs.GetMonth() < rhs.GetMonth();
	}
	else return lhs.GetYear() < rhs.GetYear();
}

Date Parse(const string date_str){
    stringstream str(date_str);
    bool flag = true;

    int year;
    flag = flag && (str >> year);
    flag = flag && (str.peek() == '-');
    str.ignore(1);

    int month;
    flag = flag && (str >> month);
    flag = flag && (str.peek() == '-');
    str.ignore(1);

    int day;
    flag = flag && (str >> day);
    flag = flag && str.eof();

    if (!flag) {
		throw logic_error("Wrong date format: " + date_str);
    }
    return Date(year, month, day);
}

////////////////////////////////////////////////////////////////////////

class Database {
public:
  void AddEvent(const Date& date, const string& event){
	  db[date].insert(event);
  }
  bool DeleteEvent(const Date& date, const string& event){
	  if(db[date].count(event)>0){
		  db[date].erase(event);
		  return true;
	  }
	  else return false;
  }
  int  DeleteDate(const Date& date){
	  int events_count = 0;
	  if(db.count(date)>0){
		  events_count = db[date].size();
		  db.erase(date);
	  }
	  return events_count;
  }

  void Find(const Date& date) const{
	  if(db.count(date)>0){
		  for(const string& a : db.at(date)){
			  cout << a << endl;
		  }
	  }
  }

  void Print() const{
	  for(auto& b : db){
		  for(auto& c : b.second){
			  cout << b.first << " " << c << endl;
		  }
	  }
  }
private:
  map<Date,set<string>> db;
};

int main() {
	try{
	  Database db;
	  string command;
	  while (getline(cin, command)) {
		  if(command == "") continue;

		  string action;
		  stringstream input(command);
		  input >> action;

		  if(action == "Add"){
			  string date_str, event;
			  input >> date_str >> event;
			  const Date date = Parse(date_str);
			  db.AddEvent(date,event);
		  }
		  else if(action == "Del"){
			  string date_str, event;
			  input >> date_str>> event;/////проверка на конец строки???
			  const Date date = Parse(date_str);
			  if(event == ""){
				  cout << "Deleted " << db.DeleteDate(date) << " events" << endl;
			  }
			  else{
				  if(db.DeleteEvent(date,event)){
					  cout << "Deleted successfully"<<endl;
				  }
				  else{
					  cout << "Event not found" << endl;
				  }
			  }
		  }
		  else if(action == "Find"){
			  string date_str, event;
			  input >> date_str;
			  const Date date = Parse(date_str);
			  db.Find(date);
		  }
		  else if(action == "Print"){
			  db.Print();
		  }
		  else cout << "Unknown command: " << action << endl;
		// Считайте команды с потока ввода и обработайте каждую
	  }
	}
	catch(exception& e){
		cout << e.what() << endl;
	}

  return 0;
}
