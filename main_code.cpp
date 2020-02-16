#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <conio.h>
#include <cstdio>

using namespace std;

bool check_the_string(string str) {
	for (int i = 0; i < str.length(); ++i) { if ( !(isalpha(str[i]) ) ) { return false; } }
	return true;
}

bool check_the_number(string str) {
	for (int i = 0; i < str.length(); ++i) { if ( !(isdigit(str[i]) ) ) { return false; } }
	return true;
}

const int MAX_VALID_YR = 9999; 
const int MIN_VALID_YR = 1800;

int customer_menu(); // Menu for Customer

int technician_menu(); // Menu for Technician

int validate_number(int, bool);
// Function for checking the input bus_number is valid or not

bool isValidDate(int, int, int);

bool isLeap(int);

int* tommorow(int, int, int);

fstream fileVar;

class Bus {
	
	protected:

		string driver;
		
		long int bus_number;

		string seat[32];

		string type;
		
		string fc,tc;

		string city[10];

		int distance, duration;

		struct time { int hr, min; string h, m; } dep, arr;

		struct date { int dd, mm, yyyy; } start, end;

		float price;

	public:
		
		void set_from_to()
		{	
			city[0] = "Coimbatore";
			city[1] = "Chennai";
			city[2] = "Bangalore";
			city[3] = "Hyderabad";
			city[4] = "Mumbai";
			city[5] = "Nagpur";
			city[6] = "Jaipur";
			city[7] = "Kolkata";
			city[8] = "Allahabad";
			city[9] = "Delhi";

			cout <<"\nChoose a city from the options:\n";
			for (int i = 0; i < 10; ++i) {
				cout <<"-> "<< city[i] << endl;
			}
			re_fc: cout <<"\nEnter the 'From' city name -->> ";
			cin >> fc;

			if( !(check_the_string(fc)) ) { cout <<"\nInvalid Input"<< endl; goto re_fc; }

			int city_flag = 0;
			for (int i = 0; i < 10; ++i) {
				if ( fc == city[i] ) { city_flag = 1; break; }
			}

			if ( city_flag != 1 ) { cout <<"\nIncorrect City Name, Re-enter"<< endl; goto re_fc; }

			cout <<"\nChoose a city from the options:\n";
			for (int i = 0; i < 10; ++i) {
				if ( city[i] != fc ) { cout <<"-> "<< city[i] << endl; }
			}
			re_tc: cout <<"\nEnter the 'To' city name -->> ";
			cin >> tc;

			if( !(check_the_string(tc)) ) { cout <<"\nInvalid Input"<< endl; goto re_tc; }

			if ( fc == tc ) { cout <<"\nError: From and To are same"<< endl; goto re_tc; }

			city_flag = 0;	
			for (int i = 0; i < 10; ++i) {
				if ( tc == city[i] ) { city_flag = 1; break; }
			}

			if ( city_flag != 1 ) { cout <<"\nIncorrect City Name, Re-enter"<< endl; goto re_tc; }

			int indexf = 0, indext = 0;
			for (int i = 0; i < 10; ++i)
			{ 
				if( fc == city[i] ) { indexf = i; }
				if( tc == city[i] ) { indext = i; }
			}

			distance = (indext - indexf)*230;
			if ( distance < 0 ) { distance = -1*distance; }
			duration = distance/70;

			char col;
			re_dep: cout <<"\nEnter the departure time from "<< fc <<" (24 hour format, input in HH:MM) = ";
			cin >> dep.hr >> col >> dep.min;

			if( col != ':' ) { cout <<"\nInvalid format"<< endl; goto re_dep; }

			if( dep.hr > 23 || dep.hr < 0 || dep.min > 59 || dep.min < 0 ) { 
				cout <<"\nInvalid Time"<< endl; goto re_dep;
			}

			if ( dep.hr < 10 ) { dep.h = '0' + to_string(dep.hr); } else { dep.h = to_string(dep.hr); }
			
			if ( dep.min < 10 ) { dep.m = '0' + to_string(dep.min); } else { dep.m = to_string(dep.min); }

			int th = dep.hr + duration;
			if ( th >= 24 ) { arr.hr = th - 24; } else { arr.hr = th; }
			if ( arr.hr >= 24 ) { arr.hr -= 24; }
			arr.min = dep.min;

			cout <<"\nThe distance is "<< distance <<"-km and duration is "<< duration <<"-hours"<<endl;
			
			if ( arr.hr < 10 ) { arr.h = '0' + to_string(arr.hr); } else { arr.h = to_string(arr.hr); }
			
			if ( arr.min < 10 ) { arr.m = '0' + to_string(arr.min); } else { arr.m = to_string(arr.min); }
			
			cout <<"\nThe arrival time at "<< tc <<" is "<< arr.h <<":"<< arr.m << endl;
		}

		void set_dates()
		{
			char temp1, temp2;
			
			re_Date: cout <<"\nEnter the departure date (dd/mm/yyyy): ";
			cin >> start.dd >> temp1 >> start.mm >> temp2 >> start.yyyy;

			if( temp1 != '/' || temp2 != '/' ) { cout <<"\nInvalid format"<< endl; goto re_Date; }

			if( !(isValidDate(start.dd, start.mm, start.yyyy)) ) { cout <<"\nInvalid Date\n"; goto re_Date; }

			if ( (dep.hr > arr.hr) || ( dep.hr < arr.hr && duration >= 24 ) ) { 
				int *array = (int*)malloc(3*sizeof(int));
				array = tommorow(start.dd, start.mm, start.yyyy);
				end.dd = *(array + 0); end.mm = *(array + 1); end.yyyy = *(array + 2);
			} else {
				end.dd = start.dd; end.mm = start.mm; end.yyyy = start.yyyy;
			}

			//cout <<"\nStart Date is: "<< start.dd <<"-"<< start.mm <<"-"<< start.yyyy << endl;
			cout <<"\nThe arrival date is: "<< end.dd <<"/"<< end.mm <<"/"<< end.yyyy << endl;
		}

		void set_price(int t) {
			if ( t == 1 ) { price = (2.5)*distance; } else { price = (1.7)*distance; }
		}

		void print_the_ticket(string temp, int seat_num, int f, string G) 
		{
			string file_name = to_string(bus_number) + temp + to_string(seat_num) + ".txt";

			fileVar.open(file_name,ios::out);

			fileVar <<"\t\n --- KBSS Travels . Ticket Booked  --- "<< endl;

			if ( f == 1 ) { fileVar <<"\nSleeper Bus"<< endl; } else { fileVar <<"\nSemi-Sleeper Bus"<< endl; }

			fileVar <<"\nPassenger name: "<< temp << "\t\tGender: "<< G <<"\n\nBus number: "<< bus_number << endl;

			fileVar <<"\nBus Type: "<< type <<"\t\tSeat.No: "<< seat_num << endl;

			fileVar <<"\nFrom - "<< fc <<" to - "<< tc << endl;

			fileVar <<"\nFare: ₹ "<< price << endl;

			fileVar <<"\nDeparture time: "<< dep.h <<":"<< dep.m <<"\tArrival time: "<< arr.h <<":"<< arr.m << endl;

			fileVar <<"\nDeparture date: "<< start.dd <<"-"<< start.mm <<"-"<< start.yyyy << endl;

			fileVar <<"\nArrival date: "<< end.dd <<"-"<< end.mm <<"-"<< end.yyyy << endl;

			fileVar.close();
		}

		void show_ticket(string temp, int seat_num, int f, string G) {
			fstream fi;

			string file_name = to_string(bus_number) + temp + to_string(seat_num) + ".txt";

			fi.open(file_name,ios::in);

			string r;
			while(!fi.eof()){
				getline(fi, r);
				cout << r << endl;
			}
		}

		// for getting the bus data from technician and storing it in object 
		virtual void getData_tech() = 0;
		/* this is pure virtual because thise should be defined in the derived class
		and no object must be created of the "Bus" class */ 
};

class Sleeper: public Bus {
	
public:

	Sleeper() { bus_number = -1; } // Constructor for making all the bus_number = -1 default

	friend int validate_number(int, bool);
	// For checking whether the bus entered number is valid or not 

	int return_bus_num() { return bus_number; }
	// as "bus_number" is protected data member we require public member function to know it's value

	bool check_from_to(string a, string b) { return( a == fc && b == tc); }

	void edit_seat(string t, int n) { seat[n-1] = t; }

	void getData_tech() {
		
		int temp = 0;
		string sn;
		type_number: cout <<"\nBus_number: ";
		cin >> sn;

		if ( check_the_number(sn) ) { temp = stoi(sn); } else { 
			cout <<"\nExpected number input"<< endl; 
			goto type_number; 
		}

		if ( temp < 1 ) { cout <<"\nInvalid Input"<< endl; goto type_number; }

		int r = validate_number(temp, false);

		if ( r != -1 ) {
			cout <<"\nBus with the number "<< temp <<" already exist, enter new number" << endl;
			goto type_number;
		} else {
			bus_number = temp;
		}

		re_driver: cout <<"\nDriver name: ";
		cin >> driver;

		if( !(check_the_string(driver)) ) { cout <<"\nExpected String input"<< endl; goto re_driver; }

		for (int i = 0; i < 32; ++i) { seat[i] = "Available"; }
		
		type = "A/C";

		Bus::set_from_to(); // Calling the set_from_to() function present in "Bus" class

		Bus::set_dates();

		Bus::set_price(1);
	}

	bool check_seat(int x) { return (seat[x-1] == "Available"); } 

	void display(bool with_seats) {

		cout <<"\nBus number: "<< bus_number << endl;
		
		cout <<"\nDriver name: "<< driver <<"  Bus-type: "<< type <<"\n\nFrom "<< fc <<" to "<< tc << endl;
		
		cout <<"\nDeparture time: "<< dep.h <<":"<< dep.m <<" Arrival time: "<< arr.h <<":"<< arr.m << endl;

		cout <<"\nDeparture data: "<< start.dd <<"-"<< start.mm <<"-"<< start.yyyy << endl;

		cout <<"\nArrival data: "<< end.dd <<"-"<< end.mm <<"-"<< end.yyyy << endl;

		cout <<"\nFare: Rupees "<< price << endl;

		if (with_seats) {
			cout <<"\nSeats of the Bus:\n"<< endl;
			int x = 0;
			for (int i = 0; i < 8; ++i) {
				for (int j = 0; j < 4; ++j) {
					string temp = "Available";
					if(seat[x] != "Available") { temp = "Not Available"; }
					temp.resize(20,' ');
					if ( x + 1 < 10 ) {	
						cout <<"  Seat[0"<< x + 1 <<"]  "<< temp;
					} else {
						cout <<"  Seat["<< x + 1 <<"]  "<< temp;
					}
					x++;
				}
				cout << endl << endl;
			}
		}
	}

} sleeper[25];

class Semi_sleeper: public Bus {
	
public:

	Semi_sleeper() { bus_number = -1; } // Constructor for making all the bus_number = -1 default

	friend int validate_number(int, bool);
	// For checking whether the bus entered number is valid or not

	int return_bus_num() { return bus_number; }
	// as "bus_number" is protected data member we require public member function to know it's value

	bool check_from_to(string a, string b) { return( a == fc && b == tc); }

	void edit_seat(string t, int n) { seat[n-1] = t; }

	void getData_tech() {
		
		int temp = 0;
		string sn;
		type_number: cout <<"\nBus_number: ";
		cin >> sn;

		if ( check_the_number(sn) ) { temp = stoi(sn); } else { 
			cout <<"\nExpected number input"<< endl; 
			goto type_number; 
		}

		if ( temp < 1 ) { cout <<"\nInvalid Input"<< endl; goto type_number; }

		int r = validate_number(temp, false);

		if ( r != -1 ) {
			cout <<"\nBus with the number "<< temp <<" already exist, enter new number" << endl;
			goto type_number;
		} else {
			bus_number = temp;
		}

		re_driver: cout <<"\nDriver name: ";
		cin >> driver;

		if( !(check_the_string(driver)) ) { cout <<"\nExpected String input"<< endl; goto re_driver; }

		for (int i = 0; i < 32; ++i) { seat[i] = "Available"; }
		
		string st;
		int v;
		choose_type: cout <<"\nType: [1] A/C  [2] Normal -> ";
		cin >> st;

		if ( check_the_number(st) ) { v = stoi(st); } else { 
			cout <<"\nExpected number input"<< endl; 
			goto choose_type; 
		}

		if ( v == 1 ) { type = "A/C"; }

		else if ( v == 2 ) { type = "Normal"; }

		else { cout <<"\nInvalid entry\n"; goto choose_type; }

		Bus::set_from_to(); // Calling the set_from_to() function present in "Bus" class
	
		Bus::set_dates();

		Bus::set_price(2);
	}

	bool check_seat(int x) { return (seat[x-1] == "Available"); }

	void display(bool with_seats) {
		
		cout <<"\nBus number: "<< bus_number << endl;
		
		cout <<"\nDriver name: "<< driver <<"  Bus-type: "<< type <<"\n\nFrom "<< fc <<" to "<< tc << endl;
		
		cout <<"\nDeparture time: "<< dep.h <<":"<< dep.m <<" Arrival time: "<< arr.h <<":"<< arr.m << endl;

		cout <<"\nDeparture data: "<< start.dd <<"-"<< start.mm <<"-"<< start.yyyy << endl;

		cout <<"\nArrival data: "<< end.dd <<"-"<< end.mm <<"-"<< end.yyyy << endl;

		cout <<"\nFare: ₹ "<< price << endl;

		if (with_seats) {
			cout <<"\nSeats of the Bus:\n"<< endl;
			int x = 0;
			for (int i = 0; i < 8; ++i) {
				for (int j = 0; j < 4; ++j) {
					string temp = "Available";
					if ( seat[x] != "Available" ) { temp = "Not Available"; }
					temp.resize(20,' ');
					if ( x + 1 < 10 ) {	
						cout <<"  Seat[0"<< x + 1 <<"]  "<< temp;
					} else {
						cout <<"  Seat["<< x + 1 <<"]  "<< temp;
					}
					x++;
				}
				cout << endl << endl;
			}
		}
	}

} semi_sleeper[25];

int validate_number(int number, bool del)
{
	int flag_1 = 0, flag_2 = 0, index1 = -1, index2 = -1;
	
	for (int i = 0; i < 25; ++i) {
		if ( sleeper[i].bus_number == number ) {
			flag_1 = 1;
			index1 = i;
			break;
		}
		if ( semi_sleeper[i].bus_number == number ) {
			flag_2 = 1;
			index2 = i;
			break;
		}	
	}

	if ( flag_1 == 0 && flag_2 == 0 ) {
		
		return -1;
	
	} else if ( flag_1 == 1 ) { 
		
		if ( del ) { sleeper[index1].bus_number = -1; }

		return index1;

	} else { 
		
		if ( del ) { semi_sleeper[index1].bus_number = -1; }		

		return index2; 
	}
}

void add_Bus()
{
	string st;
	int v, index = 0;
	Again: cout <<"\nEnter: [1] Sleeper  [2] Semi_sleeper  --> ";
	cin >> st;

	if ( check_the_number(st) ) { v = stoi(st); } else { 
		cout <<"\nExpected number input"<< endl; 
		goto Again; 
	}

	switch (v) {
		
		case 1: for (int i = 0; i < 25; ++i) {
					if ( sleeper[i].return_bus_num() == -1 ) {
						sleeper[i].getData_tech();
						break;
					}
				}
				break;
		
		case 2: for (int i = 0; i < 25; ++i) {
					if ( semi_sleeper[i].return_bus_num() == -1 ) {
						semi_sleeper[i].getData_tech();
						break;
					}
				}
				break;
		
		default: cout <<"\nInvalid Input"<< endl; goto Again;
	}

	system("CLS");
}

void delete_Bus()
{
	string st;
	int num = 0;
	re_bn: cout <<"\nEnter the bus number to delete: ";
	cin >> st;

	if ( check_the_number(st) ) { num = stoi(st); } else { 
		cout <<"\nExpected number input"<< endl; 
		goto re_bn; 
	}

	int r = validate_number(num, true);

	if ( r == -1 ) {
		system("CLS");
		cout <<"\nThe bus_number "<< num <<" doesn't exist, Invalid bus_number\n"<< endl;
	}
}

void all_buses()
{
	cout <<"\nALL THE AVAILABLE BUSES ARE: "<< endl;

	for (int i = 0; i < 25; ++i) {
		if ( sleeper[i].return_bus_num() != -1 ) {
			cout <<"\n********  Sleeper Bus  ********"<<endl;
			sleeper[i].display(false);
		}
		if (semi_sleeper[i].return_bus_num() != -1) {
			cout <<"\n********  Semi_sleeper Bus  ********"<<endl;
			semi_sleeper[i].display(false);
		}
	}

	cout << endl;
}

void particular_bus()
{
	string st; 
	int bn;
	re_BBNN: cout <<"\nEnter the bus number: ";
	cin >> st;

	if ( check_the_number(st) ) { bn = stoi(st); } else { 
		cout <<"\nExpected number input"<< endl; 
		goto re_BBNN; 
	}

	if ( bn < 1 ) {
		cout <<"\nInvalid input, exited from function\n"<< endl;
		return;
	}

	int flag = 0, index = 0;
	for (int i = 0; i < 25; ++i) {
		if ( sleeper[i].return_bus_num() == bn ) {
			flag = 1;
			index = i;
			break;
		}
		if ( semi_sleeper[i].return_bus_num() == bn ) {
			flag = 2;
			index = i;
			break;	
		}	
	}

	if ( flag == 1 ) {
		cout <<"\n********  Sleeper Bus  ********"<<endl;
		sleeper[index].display(true);
	} else if ( flag == 2 ) {
		cout <<"\n********  Semi_sleeper Bus  ********"<<endl;
		semi_sleeper[index].display(true);
	} else {
		cout <<"\nBus with number "<< bn <<" is not available\n"<< endl;
	}
}

void book_ticket()
{
	string a, b, city[10];
	
	city[0] = "Coimbatore";
	city[1] = "Chennai";
	city[2] = "Bangalore";
	city[3] = "Hyderabad";
	city[4] = "Mumbai";
	city[5] = "Nagpur";
	city[6] = "Jaipur";
	city[7] = "Kolkata";
	city[8] = "Allahabad";
	city[9] = "Delhi";

	re_city: cout <<"\nChoose a city from the options:\n";
	for (int i = 0; i < 10; ++i) {
		cout <<"-> "<< city[i] << endl;
	}
	re_a: cout <<"\nEnter the 'From' city name -->> ";
	cin >> a;

	if( !(check_the_string(a)) ) { cout <<"\nInvalid Input"<< endl; goto re_a; }

	int city_flag = 0;
	for (int i = 0; i < 10; ++i) {
		if ( a == city[i] ) { city_flag = 1; break; }
	}

	if ( city_flag != 1 ) { cout <<"\nIncorrect City Name, Re-enter"<< endl; goto re_a; }

	cout <<"\nChoose a city from the options:\n";
	for (int i = 0; i < 10; ++i) {
		if ( city[i] != a ) { cout <<"-> "<< city[i] << endl; }
	}
	re_b: cout <<"\nEnter the 'To' city name -->> ";
	cin >> b;

	if( !(check_the_string(b)) ) { cout <<"\nInvalid Input"<< endl; goto re_b; }

	if ( a == b ) { cout <<"\nError: From and To are same"<< endl; goto re_b; }

	city_flag = 0;	
	for (int i = 0; i < 10; ++i) {
		if ( b == city[i] ) { city_flag = 1; break; }
	}

	if ( city_flag != 1 ) { cout <<"\nIncorrect City Name, Re-enter"<< endl; goto re_b; }

	cout << endl;

	bool check1 = false, check2 = false;

	for (int i = 0; i < 25; ++i) {
		if ( sleeper[i].return_bus_num() != -1 ) {
			if ( sleeper[i].check_from_to(a, b) ) {
				sleeper[i].display(false);
				check1 = true;
			}
		}
		if ( semi_sleeper[i].return_bus_num() != -1 ) {
			if ( semi_sleeper[i].check_from_to(a, b) ) {
				semi_sleeper[i].display(false);
				check2 = true;
			}
		}
	}

	if ( !check1 && !check2 ) { 
		system("CLS"); 
		cout <<"\nNo buses are available from "<< a <<" to "<< b << endl; 
		return;
	}

	string st;
	int book_num;
	re_bnbn: cout <<"\nEnter the bus number, in which you want to bookticket: ";
	cin >> st;

	if ( check_the_number(st) ) { book_num = stoi(st); } else { 
		cout <<"\nExpected number input"<< endl; 
		goto re_bnbn; 
	}


	if ( validate_number(book_num,false) == -1 || book_num <= 0 ) {
		cout <<"\nInvalid Input\n"<< endl;
		goto re_bnbn;
	}

	int flag = 0, index = 0;
	for (int i = 0; i < 25; ++i) {
		if ( sleeper[i].return_bus_num() == book_num ) {
			flag = 1;
			index = i;
			break;
		}
		if ( semi_sleeper[i].return_bus_num() == book_num ) {
			flag = 2;
			index = i;
			break;	
		}
	}

	if ( flag == 1 ) {
		cout <<"\n********  Sleeper Bus  ********"<<endl;
		sleeper[index].display(true);
	} else if ( flag == 2 ) {
		cout <<"\n********  Semi_sleeper Bus  ********"<<endl;
		semi_sleeper[index].display(true);
	} else {
		cout <<"\nBus with number "<< book_num <<" is not available\n"<< endl;
		goto re_bnbn;
	}

	string nsns;
	int nos_num = 0;
	re_num_seats: cout <<"\nNumber of seats to book: ";
	cin >> nsns;

	if ( check_the_number(nsns) ) { nos_num = stoi(nsns); } else { 
		cout <<"\nExpected number input"<< endl; 
		goto re_num_seats; 
	}

	for (int nos = 0; nos < nos_num; nos++) {
		string sn1;
		int seat_num;
		book_seat_again: cout <<"\nEnter the seat number for booking: ";
		cin >> sn1;

		if ( check_the_number(sn1) ) { seat_num = stoi(sn1); } else { 
			cout <<"\nExpected number input"<< endl; 
			goto book_seat_again; 
		}

		if ( seat_num < 1 || seat_num > 32) { cout <<"\nInvalid seat number"<< endl; goto book_seat_again; }

		if ( flag == 1 ) {
			if ( !(sleeper[index].check_seat(seat_num)) ) {
				cout <<"\nThe Seat is not available, already reserved for a passenger" << endl;
				cout <<"\nTry to book another seat"<< endl;
				goto book_seat_again;
			}
		} else if ( flag == 2 ) {
			if ( !(semi_sleeper[index].check_seat(seat_num)) ) {
				cout <<"\nThe Seat is not available, already reserved for a passenger" << endl;
				cout <<"\nTry to book another seat"<< endl;
				goto book_seat_again;
			}
		}

		string temp;
		re_name: cout <<"\nEnter the name of passenger: ";
		cin >> temp;

		if( !(check_the_string(temp)) ) { cout <<"\nExpected String input"<< endl; goto re_name; }

		int gi;
		string g, sg;
		re_gender: cout <<"\nGender:  [1] Male  [2] Female  [3] Other -> ";
		cin >> sg;

		if ( check_the_number(sg) ) { gi = stoi(sg); } else { 
			cout <<"\nExpected number input"<< endl; 
			goto re_gender; 
		}

		if ( gi == 1 ) { g = "Male"; } else if ( gi == 2 ) { g = "Female"; } else if ( gi == 3 ) { g = "Other"; }
		else {
			cout <<"\nInvalid Input"<< endl;
			goto re_gender;
		}

		if ( flag == 1 ) {
			sleeper[index].edit_seat(temp, seat_num);
			sleeper[index].print_the_ticket(temp, seat_num, flag, g);
			system("CLS");
			int y = 0;
			cout <<"\nDisplay ticket: [1] Yes  [2] No -> ";
			cin >> y;
			if ( y == 1 ) { sleeper[index].show_ticket(temp, seat_num, flag, g); }
		} else if ( flag == 2 ) {
			semi_sleeper[index].edit_seat(temp, seat_num);
			semi_sleeper[index].print_the_ticket(temp, seat_num, flag, g);
			system("CLS");
			int y = 0;
			cout <<"\nDisplay ticket: [1] Yes  [2] No -> ";
			cin >> y;
			if ( y == 1 ) { semi_sleeper[index].show_ticket(temp, seat_num, flag, g); }
		}

	}	
}

void cancel_ticket()
{
	string snb;
	int book_num;
	re_ctbn: cout <<"\nEnter the bus number, in which you want to cancel the ticket: ";
	cin >> snb;

	if ( check_the_number(snb) ) { book_num = stoi(snb); } else { 
		cout <<"\nExpected number input"<< endl; 
		goto re_ctbn; 
	}

	if ( validate_number(book_num,false) == -1 || book_num <= 0 ) {
		cout <<"\nInvalid bus number\n"<< endl;
		return;
	}

	int flag = 0, index = 0;
	for (int i = 0; i < 25; ++i) {
		if ( sleeper[i].return_bus_num() == book_num ) {
			flag = 1;
			index = i;
			break;
		}
		if ( semi_sleeper[i].return_bus_num() == book_num ) {
			flag = 2;
			index = i;
			break;	
		}
	}

	if ( flag == 1 ) {
		cout <<"\n********  Sleeper Bus  ********"<<endl;
		sleeper[index].display(true);
	} else if ( flag == 2 ) {
		cout <<"\n********  Semi_sleeper Bus  ********"<<endl;
		semi_sleeper[index].display(true);
	} else {
		cout <<"\nBus with number "<< book_num <<" is not available\n"<< endl;
	}

	string ctsn;
	int seat_num;
	book_seat_again: cout <<"\nEnter the seat number for cancelling the ticket: ";
	cin >> ctsn;

	if ( check_the_number(ctsn) ) { seat_num = stoi(ctsn); } else { 
		cout <<"\nExpected number input"<< endl; 
		goto book_seat_again; 
	}

	if ( seat_num < 1 || seat_num > 32) { cout <<"\nInvalid seat number"<< endl; goto book_seat_again; }

	if ( flag == 1 ) {
		if ( !(sleeper[index].check_seat(seat_num)) ) { 
			sleeper[index].edit_seat("Available", seat_num);
			cout <<"\nTicket is cancelled"<< endl;
		} else {
			cout <<"\nSeat is not reserved"<< endl;
			goto book_seat_again;
		}
	} else if ( flag == 2 ) {
		if ( !(semi_sleeper[index].check_seat(seat_num)) ) {
			semi_sleeper[index].edit_seat("Available", seat_num);
			system("CLS");
			cout <<"\nTicket is cancelled"<< endl;
		} else {
			cout <<"\nSeat is not reserved"<< endl;
			return;
		}
	}
}

int main()
{
	string temp;
	int c = 0;
	Choice: cout <<"\nMain Menu"<< endl;
	cout << "\nPress:  [1] Customer    [2] Technician    [3] Exit  --> ";
	cin >> temp;

	int i = 0;

	char pwd[5], ch;

	if ( check_the_number(temp) ) { c = stoi(temp); } 

	else { cout <<"\nInvalid Input, Please enter again"<< endl; goto Choice; }
	
	string id, password;

	switch (c) {

		case 1: customer_menu();
				break;

		case 2: cout <<"\nEnter the ID: ";
				cin >> id;

				i = 0;	
				if(id == "Tech.101.102")
           		{
                	pass: cout<<"\nEnter Password: ";
                	while(i<4) {
		                ch = getch();
		                if(ch==13) { break; }
		                else if(ch=='\b'&&i>0) {
		                    cout<<"\b \b";
		                    i--;
		                } else {
		                    pwd[i]=ch;
		                    cout<<"*";
		                    i++;
		                }
		            }
                	pwd[i]='\0';

                	if ( pwd[0] == 'K' && pwd[1] == 'B' && pwd[2] == 'S' && pwd[3] == 'S') {
						technician_menu();
					} else {
						cout <<"\n\nWrong password, re-try"<< endl; 
					}

				} else { cout <<"\nId "<< id <<" is wrong"<< endl; }

				break;

		case 3: cout << "\nProgram terminated\n" << endl;
				return 0;

		default: cout << "\nInvalid Input\n" << endl;
	}

	goto Choice;
}

int customer_menu()
{
	string temp;
	int c = 0;
	cout <<"\nCustomer Menu"<< endl;
	Choice: cout << "\nPress: [1] Book-Ticket  [2] Cancel-Ticket  [3] Buses Available  [4] Show a Bus  [5] Main Menu\n--> ";
	cin >> temp;

	if ( check_the_number(temp) ) { c = stoi(temp); } 

	else { cout <<"\nInvalid Input, Please enter again"<< endl; goto Choice; }

	switch (c) {

		case 1: book_ticket();
				break;

		case 2: cancel_ticket();
				break;

		case 3: all_buses();
				break;

		case 4: particular_bus();
				break;

		case 5: system("CLS");
				cout <<"\nExited from Customer Menu\n" << endl;
				return 0;

		default: cout << "\nInvalid Input\n" << endl;
	}

	goto Choice;	
}

int technician_menu()
{
	system("CLS");
	
	string temp;
	int c = 0;
	cout <<"\nTechnician Menu"<< endl;
	Choice: cout << "\nPress: [1] Add Bus  [2] Delete Bus  [3] Main Menu --> ";
	cin >> temp;

	if ( check_the_number(temp) ) { c = stoi(temp); }

	else { cout <<"\nInvalid Input, Please enter again"<< endl; goto Choice; }

	switch (c) {

		case 1: add_Bus();
				break;

		case 2: delete_Bus();
				break;

		case 3: system("CLS");
				cout <<"\nExited from Technician Menu\n" << endl;
				return 0;

		default: cout <<"\nInvalid Input\n"<< endl;
	}

	goto Choice;	
}

bool isValidDate(int d, int m, int y) {
	
    if (y > MAX_VALID_YR ||  y < MIN_VALID_YR) { return false; }

    if (m < 1 || m > 12) { return false; }

    if (d < 1 || d > 31) { return false; }

    if (m == 2) {
        if (isLeap(y)) { return (d <= 29); } else { return (d <= 28); } 
    }

    if (m == 4 || m == 6 || m == 9 || m == 11) { return (d <= 30); }

    return true; 
}

bool isLeap(int year)  { return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)); }

int* tommorow(int d, int m, int y) {
	
	if ((m == 4 || m == 6 || m == 9 || m == 11) && d == 30) { 
		d = 1;
		m++; 
	} else if ((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10) && d == 31) {	
		d = 1;
		m++;
	} else if( m == 2) {
		if (isLeap(y) && d == 29) { d = 1; m++; } else if(isLeap(y) == 0 && d == 28) { d = 1; m++; } else { d++; }
	} else if( m == 12 && d == 31) {
		d = 1;
		m = 1;
		y++;
	} else { d++; }

	int *array = (int*)malloc(3*sizeof(int));
	*(array + 0) = d;
	*(array + 1) = m;
	*(array + 2) = y; 

	return array;
}
