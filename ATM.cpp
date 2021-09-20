//--heder files
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <fstream>

using namespace std;

void showMenu();

//--cash unit structure
typedef struct cdm_cashunit
{
	int Type; //Cash unit type i.e. {BILL=0, REJECT=1, RETRACT=2}.
	char UnitID[5];
	short Denomination;
	int Count;
	int Maximum;
	short Status;
} CASHUNIT, * LPCASHUNIT;

typedef struct cdm_cu_info
{
	int Count; //number of cash units
	LPCASHUNIT* lppList; //Pointer to an array of pointers to CASHUNIT
	vector<cdm_cashunit> cashunits;
} CUINFO, * LPCUINFO;

//--Global value for cas units
vector<cdm_cu_info> thecdm_cu_info;

//--cash unit type enum
enum cash_unit_type
{
	BILL,
	REJECT,
	RETRACT
};

//--cash unit status enum
enum cash_unit_status
{
	ONLINE,
	INOP
};

//--Get Data from file
void setCashUnitValue()
{
	fstream fio;
	string line;
	try
	{
		fio.open("config.txt");
		if (!fio)
		{
			cout << "\n**************************************************************************";
			cout << "\nATM is Out Of Service! ";
			cout << "\nThere is no Config file! ";
			cout << "\nPlease set config file according to format written in ReadMe.txt File.";
			cout << "\n***************************************************************************";
			exit(1);
		}
		cdm_cu_info cu;
		int row = 0;
		vector<cdm_cashunit> thecdm_cashunit;
		cdm_cashunit u;
		while (fio) {
			getline(fio, line);

			rsize_t strmax = sizeof line;
			const char* delim = "|";
			char* next_token;
			char* token = strtok_s((char*)line.c_str(), delim, &next_token);
			int col = 0;

			cdm_cu_info* lcu;

			while (token) {

				switch (col) {
				case 0:
					row = atol(token);
					//cout << "row="<<row;
					break;
				case 1:
					u.Type = atol(token);
					//cout << "type="<<u.Type;
					break;
				case 2:
					sprintf_s(u.UnitID, token);
					//cout << "id="<<u.UnitID;
					break;
				case 3:

					u.Denomination = atol(token);
					//cout << "dom="<<u.Denomination;
					break;
				case 4:
					u.Count = atol(token);
					//cout << "count ="<<u.Count;
					break;
				case 5:
					u.Maximum = atol(token);
					//cout << "max="<<u.Maximum;
					break;
				case 6:
					u.Status = atol(token);
					//cout << "status="<<u.Status;
					break;

				}
				col++;
				token = strtok_s(NULL, delim, &next_token);

			}
			thecdm_cashunit.push_back(u);
		}

		cu.Count = row;
		cu.cashunits = thecdm_cashunit;
		thecdm_cu_info.push_back(cu);
		fio.close();
	}
	catch (...)
	{
			cout << "\nException in serCashUnitValue.";
			return;
	}
	return;
}
//--void displayCashUnit()
void displayCashUnit()
{
	int thePassword = 2021, theInputPassword;
	try {
		cout << "\nPlease Enter password For displaying cash units ";
		cin >> theInputPassword;
		if (theInputPassword == thePassword) {
			for (int i = 0; i < thecdm_cu_info.size(); i++)
			{
				cout << "\n***************************  CASH UNIT : " << thecdm_cu_info[i].Count << "  ****************************\n";
				for (int j = 0; j < thecdm_cu_info[i].cashunits.size(); j++)
				{
					cout << "Unit Id : " << thecdm_cu_info[i].cashunits[j].UnitID << "  ";
					cout << "Denomination : " << thecdm_cu_info[i].cashunits[j].Denomination << "  ";
					cout << "Count : " << thecdm_cu_info[i].cashunits[j].Count << "  ";
					cout << "Maximum : " << thecdm_cu_info[i].cashunits[j].Maximum << "  ";
					cout << "Status : " << thecdm_cu_info[i].cashunits[j].Status << "  ";
					cout << "\n";
				}

			}
			cout << "\n**************************************************************************\n";
		}
		else
		{
			cout << "\nYour password is wrong( try current year number) ";
			showMenu();
		}
	}
	catch (...)
	{
		cout << "\nError in  displaycashunit ";
	}
}

//--displayDenomination()  
void displayDenomination()
{
	for (int i = 0; i < thecdm_cu_info.size(); i++)
	{
		cout << "\n***************************  Bill Denominations : " << thecdm_cu_info[i].Count << "  ****************************\n";
		for (int j = 0; j < thecdm_cu_info[i].cashunits.size(); j++)
		{
			cout << "Denomination : " << thecdm_cu_info[i].cashunits[j].Denomination << "  ";
			cout << "\n";
		}
	}
	cout << "\n**************************************************************************\n";
}
//--increaseRetract
void increaseretrac()
{
	if (thecdm_cu_info[0].cashunits[5].Status != cash_unit_status::INOP)
		thecdm_cu_info[0].cashunits[5].Count++;
	//increase in file : TODO
	displayCashUnit();
}
//--Greedy Algorithm
void selectGreedyBill(int amount)
{
	vector<int> ans;
	for (int i = thecdm_cu_info[0].cashunits.size()-1; i >= 0; i--) {
		// Find denominations
		if(thecdm_cu_info[0].cashunits[i].Denomination>0)
        
		while (amount >= thecdm_cu_info[0].cashunits[i].Denomination) {
			amount -= thecdm_cu_info[0].cashunits[i].Denomination;
			ans.push_back(thecdm_cu_info[0].cashunits[i].Denomination);
		}
	}
	if (amount > 0)
	{
		cout << "\nErroe : Your amount is not dispansable";
		displayDenomination();
		return;
	}
	
	for (int i = 0; i < ans.size(); i++)
		cout <<ans[i] /100<< "  ";
	char ch;
	cout << "\n Take your money (Press y key) ";
	cin >> ch;

	if (ch != 'y')
		increaseretrac();
	else
		cout << "Here you are!!!";
	
}
//input Money
void inputMoney()
{
	try {
		int theAmount = 0;
		cout << "\nEnter your amount in Euro ";
		cin >> theAmount;
		selectGreedyBill(theAmount * 100);
	}
	catch (...)
	{
		cout << "\nError in inputMoney";
	}

}
//--Main menu
void showMenu()
{
	int choice;
	cout << "\n1-Get Money ";
	cout << "\n2-Display Units ";
	cout << "\n3-Exit ";
	cout << "\nSelect a choice : ";
	cin >> choice;
	if (choice == 1)
		inputMoney();
	else if (choice == 2)
		displayCashUnit();
	else if (choice == 3)
		exit(1);
	else
		cout << "Invalid choice number!";
	showMenu();
}
//--main
int main()
{
	setCashUnitValue();
	showMenu();
}

