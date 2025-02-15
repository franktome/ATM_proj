#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#define _MAX_(x, y) ((x > y) ? x : y) //MAX �Լ�
#define _MIN_(x, y) ((x < y) ? x : y) //MIN �Լ�
using namespace std;

class bank;
class atm;
class account;

vector <bank*> banklist; //bank list global variable
vector <account*> accountlist; // account list global variable
vector <atm*> atmlist; //atm list global variable


class card { // card Ŭ���� //    ���°� ������� �� �ڵ����� �Ϲݿ� construct �ҷ����Ѵ�. atm�� ������������� admin ī���ȣ�� �ҷ����� �ǰ�.>> ���¼� ������
private:
	bool admin; // admin ���� �ƴ���.
	bank* card_bank; // ī�尡 ��� bank ����
	int card_num; // ī�� ��ȣ
	atm* mas_atm; // ������ ������ �� �� �ִ� atm
	static int static_card_number;
public:
	card(bank* b, atm* master_atm); // admin �� constructor 
	card(bank* b); // �Ϲݿ� constructor
	bool get_ad() { return admin; } // admin���� �ƴ���
	bank* get_bank() { return card_bank; } // bank �ּ� �޾� ���°� 
	int get_cardnum() { return card_num; } // card number �޾� ���°�
};

int card::static_card_number = 1000;


class bank { // ����
private:
	string bankname; // ���� �̸�
	vector<account*> user_acc; // ���µ��� ��� ���� vector
	account* curr_account; // ���� ���೻���� ����ϰ� �ִ� ���� �ּ�
	account* src_account; // ���� ��ü�� ����� �ּ�(�� �� ������ �ʿ� ����!)
	account* dest_account; // ���� ���� ���� �ּ�
	vector <atm*> atm_list; // atm�� ��Ƴ��� ����Ʈ
public:
	bank(string b_name); //bank ������
	string get_bankname() { return bankname; } //���� �̸� �޾ƿ��� ��
	vector<account*> get_user_acc(); // ���� ��ī��Ʈ ����
	bool bool_get_account(int card_num);// ī�� ��ȣ�� ������ ���� ã�� ��.
	account* account_get_account(int card_num);// ī�� ��ȣ�� ������ ���� ã�� ��.	
	account* account_get_accountnum(long long int account_num);// ���� ��ȣ�� ���� ã�� ��.	
	bool authorize_bank(int card_num, int password);//  card_num�� password�� ������ Ȯ���ϱ�		
	bool ac_enough_money_in_acc_bank(long long int acc_num, int m);// �Ű������� ���� ���¹�ȣ�� ���� ����� �ִ��� Ȯ���ϱ�
	bool cn_enough_money_in_acc_bank(int card_num, int m);// �Ű������� ���� ī���ȣ�� ���� ����� �ִ��� Ȯ���ϱ�
	void account_transfer_bank(long long int src_acc_num, bank* dest_bank, long long int dest_acc_num, int m, int fee);// ���¿��� ���·� ������ü�ϱ��
	void cash_transfer_bank(bank* dest_bank, long long int dest_acc_num, int m);// �������� ���¿� ������ü�ϱ�
	void withdrawl_bank(int cardnum, int m);//����Լ� ��ũ��
	void deposit_bank(int cardnum, int m);   //ī���ȣ�� �����ؼ� �� �ֱ�
	bool bool_accountnumber_in_bank(long long int accountnumber);// ���¹�ȣ�� �� ���࿡ �����ϴ��� bool�� Ȯ��
	bank* bank_accountnumber_in_bank(int accountnumber);// ���¹�ȣ�� �� ���࿡ �����Ѵٸ� ���� �ּ� ��ȯ
	void make_multi_uni_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, int serialnumber);
	void make_multi_bi_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, int serialnumber);
	void make_single_uni_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, int serialnumber);
	void make_single_bi_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, int serialnumber);
	void make_account(long long int ac_n, string user_name, int password, int cash);
	vector <atm*> get_atm_list();
};
class transaction_history {
protected:
	int transaction_ID;
	static int transaction_ID_counter;
	int card_number;
	string tr_type; //���, �Ա�, ������ü
	int amount; //��
	bank* tr_bank;//���� �۾��� ����
	account* tr_account; // �۾��� ����
	int curr_acc_money;
public:
	transaction_history(int cardnum, string type, int am, bank* bank, account* account); //������
	virtual void print_history(); //print �ϴ� �Լ�.
	virtual void korean_print_history();
	virtual string print_txt();
	virtual string korean_print_txt();
};
int transaction_history::transaction_ID_counter = 1;

class transfer_acc_transaction_history : public transaction_history {
private:
	bank* end_bank;
	account* end_account;
public:
	transfer_acc_transaction_history(int cardnum, string type, int am, bank* start_bank, bank* end_bank, account* start_account, account* end_account);
	void print_history();
	void korean_print_history();
	string print_txt();
	string korean_print_txt();
};

class account { //����
private:
	bank* bank_adress; //���� �ּ�
	string user_name; // ��� �̸�
	long long int acc_number;// ���� ��ȣ 6�ڸ�
	int acc_cash; // ���� ��
	int password; // ���� ��й�ȣ
	int card_num; // ī�� ��ȣ 1001 ���� ����.
	card* account_card; // ���¿� ����� ī��
	vector<transaction_history*> account_tr_his;//account�� �ŷ����
public:
	account(long long int acc_n, bank* b_adress, string u_name, int pd, int a_cash); // constructor
	bank* get_bank_adress() { return bank_adress; } // ���� �ּ� �޾ƿ���
	string get_user_name() { return user_name; } // ��� �̸� �޾� ����
	int get_account_password() { return password; } // ���� ��й�ȣ �޾ƿ���
	long long int get_acc_number() { return acc_number; } // ���� ��ȣ �޾ƿ���
	int get_acc_cash() { return acc_cash; }//���¿� ���� �� �ִ� �� �˾ƿ���
	int get_cardnum() { return card_num; }// ī�� ��ȣ �޾ƿ���
	void change_acc_cash(int ch_cash) { acc_cash = ch_cash; } // ���� �ȿ� �ִ� �� �����ϱ�
	void change_account_tr(transaction_history* tr) {
		account_tr_his.push_back(tr);
	}
	void print_account_history();
	void korean_print_account_history();
};

class atm {
private:
	int serial_number; //atm �Ϸù�ȣ
	int atm_cash[4] = { 0,0,0,0 }; // atm �ȿ� ���� �ִ� �� // 1000,5000,10000,50000
	int card_insert_slot;//���� ���� card�� ��ȣ
	int session = 0; //session
	int number_of_withdrawl = 0; //��� Ƚ��
	int limit_of_number_of_withdral = 3;//��� Ƚ�� ����
	int get_money = 0;//����� ��
	int limit_of_get_money;//����� ���� ����
	int single_or_multi; //single�� 0, multi�� 1
	int uni_or_bi;//uni�� 0, bi�� 1
	bank* primary_bank;//atm�� �� ����
	//static-������>>�ø���ѹ�>>bank make atm �����ϱ�.
	card* admin_card;
	vector<transaction_history*> temp_tr_his;
	vector<transaction_history*> atm_tr_his;
public:
	atm(int single_or_multi, int uni_or_bi, int one_thou, int five_thou, int ten_thou, int fifty_thou, bank* pb, int serialnumber);//constructor
	int get_serial_number() { return serial_number; } //serial number ��� ���� ��
	int get_atm_cash() { //atm�� ���� �� �ִ� �� ��� ���� �Լ�.
		int cash = 1000 * atm_cash[0] + 5000 * atm_cash[1] + 10000 * atm_cash[2] + 50000 * atm_cash[3];
		return cash;
	}
	int get_1000() { return atm_cash[0]; }
	int get_5000() { return atm_cash[1]; }
	int get_10000() { return atm_cash[2]; }
	int get_50000() { return atm_cash[3]; }
	int get_card_insert_slot() { return card_insert_slot; } //���� ���� card�� ��ȣ�� �������� ���� �Լ�
	void set_card_insert_slot(int card_num) { card_insert_slot = card_num; }
	void increase_session() { session += 1; } // ������ 1 �߰��ϴ� �Լ�.
	void decrease_session() { session -= 1; } // ������ 1 �����ϴ� �Լ�.
	void increase_number_of_withdrawl() { number_of_withdrawl += 1; } //��� Ƚ���� 1 �����ϴ� �Լ�.
	void zero_number_of_withdrawl() { number_of_withdrawl = 0; } // ��� Ƚ���� 0���� �ʱ�ȭ�ϴ� �Լ�.
	void increase_get_money(int money) { get_money += money; }//����� ���� �����ϴ� �Լ�
	bank* find_cardnumber_bank(int cardnumber); //ī�� ��ȣ�� ���� ã��
	bool valid_cardnumber_account(int cardnumber); //ī���ȣ�� �ش��ϴ� ���°� �ִ� �� ������
	bank* find_bank_by_account(long long int accountnumber);//���¹�ȣ�� ����ã��
	bool valid_find_bank_by_account(long long int accountnumber); //���¹�ȣ�� �ش��ϴ� ���°� �ִ��� ������
	virtual void transfer(); //������ü
	virtual void deposit(); // �Ա�
	virtual void withdrawl();//���
	virtual void koreaninterface();//�������̽�(�ѱ��� ����)
	virtual void interface();//�������̽�
	virtual void koreantransfer(); //������ü
	virtual void koreandeposit(); // �Ա�
	virtual void koreanwithdrawl();//���
	//virtual void koreaninterface();//�������̽�
	bool check_password(int cardnumber, int password);
	void print_session_history(); //���¼� ���� ���� main �߰�
	void print_atm_history();
	virtual void admin_interface();
	virtual void admin_interface_korean();
	virtual void mini_interface();
	bank* get_primary_bank() { return primary_bank; }
	void change_atm_cash(int a, int b, int c, int d) {
		atm_cash[0] += a;
		atm_cash[1] += b;
		atm_cash[2] += c;
		atm_cash[3] += d;
	}
	vector <transaction_history*> get_temp() {
		return temp_tr_his;
	}
	vector <transaction_history*> get_atm_tr() {
		return atm_tr_his;
	}
	void change_temp(transaction_history* tr) {
		temp_tr_his.push_back(tr);
	}
	void change_atm_tr(transaction_history* tr) {
		atm_tr_his.push_back(tr);
	}
	void clear_temp() {
		temp_tr_his.clear();
	}
	void change_transfer_temp(transfer_acc_transaction_history* tr) {
		temp_tr_his.push_back(tr);
	}
	void set_number_of_withdrawl(int number) {
		number_of_withdrawl = number;
	}
	int get_number_of_withdrawl() { return number_of_withdrawl; }
	int get_limit_of_number_of_withdral() { return limit_of_number_of_withdral; }
	void korean_print_atm_history();
};

class single_uni_atm : public atm {
private:
public:
	single_uni_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, bank* pb, int serialnumber) :atm(type, language_type, one_thou, five_thou, ten_thou, fifty_thou, pb, serialnumber) {};
	void transfer(); //������ü
	void deposit(); // �Ա�
	void withdrawl();//���
	void interface();//�������̽�	
};
class multi_uni_atm : public atm {
private:
public:
	multi_uni_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, bank* pb, int serialnumber) :atm(type, language_type, one_thou, five_thou, ten_thou, fifty_thou, pb, serialnumber) {};
	void transfer(); //������ü
	void deposit(); // �Ա�
	void withdrawl();//���
	void interface();//�������̽�	
};
class single_bi_atm : public atm {
private:
public:
	single_bi_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, bank* pb, int serialnumber) :atm(type, language_type, one_thou, five_thou, ten_thou, fifty_thou, pb, serialnumber) {};
	void transfer(); //������ü
	void deposit(); // �Ա�
	void withdrawl();//���
	void interface();//�������̽�
	void koreantransfer(); //������ü
	void koreandeposit(); // �Ա�
	void koreanwithdrawl();//���
	void koreaninterface();//�������̽�
	void korean_print_session_history();
};
class multi_bi_atm : public atm {
private:
public:
	multi_bi_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, bank* pb, int serialnumber) :atm(type, language_type, one_thou, five_thou, ten_thou, fifty_thou, pb, serialnumber) {};
	void transfer(); //������ü
	void deposit(); // �Ա�
	void withdrawl();//���
	void interface();//�������̽�
	void koreantransfer(); //������ü
	void koreandeposit(); // �Ա�
	void koreanwithdrawl();//���
	void koreaninterface();//�������̽�
	void korean_print_session_history();
};
//ī�� �κ�				 
card::card(bank* b, atm* master_atm) { //admin�� constructor
	admin = true; //admin�̴�
	card_bank = b; //bank �����
	card_num = 7777777; // admin�� card_num�� 7777777�̴�.
	mas_atm = master_atm; //atm ���� ������ ī��
	std::cout << "admin card�� �����Ǿ����ϴ�. ī���� ������" << b->get_bankname() << "�Դϴ�." << endl;
}
card::card(bank* b) //�Ϲݿ� constructor
{
	static_card_number++;
	admin = false; //admin �� �ƴϴ�.
	card_bank = b; // bank �ּ�
	card_num = static_card_number; // cardnumber
	std::cout << "card�� �����Ǿ����ϴ�. ī���� ������ " << b->get_bankname() << "�̸� " << "ī�� ��ȣ�� " << card_num << "�Դϴ�." << endl;
}

//���� �κ�

bank::bank(string b_name) {  // ���� ������
	bankname = b_name;
	banklist.push_back(this);
	std::cout << "������ �����Ǿ����ϴ�. ���� �̸��� " << bankname << "�Դϴ�." << endl;
}

vector<account*> bank::get_user_acc() { return user_acc; }

bool bank::bool_get_account(int card_num) {  // ī�� ��ȣ�� ������ ���� ã�� ��.//�� ����
	for (int i = 0; i < user_acc.size(); i++) { // user acc ���� ����.
		if (user_acc[i]->get_cardnum() == card_num) { //���� ī�� ��ȣ�� ã�Ҵ�. 
			curr_account = user_acc[i]; //���� ���¹�ȣ�� ����
			return true; //return true ���ش�.
		}
	}
	return false;
}
account* bank::account_get_account(int card_num) { // ī�� ��ȣ�� ������ ���� ã�� ��.
	int count = 0;
	for (int i = 0; i < user_acc.size(); i++)
	{ // user acc ���� ����.
		count += 1;
		if (user_acc[i]->get_cardnum() == card_num)
		{ //���� ī�� ��ȣ�� ã�Ҵ�. 
			account* ac = user_acc[i]; //���� ���¹�ȣ�� ����
			return ac;// ���� ���� �ν��Ͻ��� ��ȯ 
		}
	}
}
account* bank::account_get_accountnum(long long int account_num) { // ���� ��ȣ�� ���� ã�� ��. 
	int count = 0;
	for (int i = 0; i < user_acc.size(); i++) {
		count += 1;
		if (user_acc[i]->get_acc_number() == account_num) { // ���� ���� ��ȣ�� ã�Ҵ�.
			curr_account = user_acc[i]; //���� ���¹�ȣ�� ����
			return curr_account;  // ���� ���� �ν��Ͻ��� ��ȯ	
		}
	}
}
bool bank::authorize_bank(int card_num, int password) { //  card_num�� password�� ������ Ȯ���ϱ�
	if (this->account_get_account(card_num)->get_account_password() == password) // card_num�� ������ password �޾ƿͼ� ���� password�� ���ϱ� 
		return true; // ������ true
	else return false; // �ٸ��� false
}
bool bank::ac_enough_money_in_acc_bank(long long int acc_num, int m) { // �Ű������� ���� ���¹�ȣ�� ���� ����� �ִ��� Ȯ���ϱ�
	account* acc = account_get_accountnum(acc_num);  // ���¹�ȣ�� ���� �ν��Ͻ� ã��
	if (acc->get_acc_cash() < m) { return false; }        // ������ �ܿ��ݾ��� m���� �۴ٸ� false�� ��ȯ
	else { return true; }                              // �ƴ϶�� true�� ��ȯ
}
bool bank::cn_enough_money_in_acc_bank(int card_num, int m) {   // �Ű������� ���� ī���ȣ�� ���� ����� �ִ��� Ȯ���ϱ�
	account* acc = account_get_account(card_num);  // ī���ȣ�� ���� �ν��Ͻ� ã��
	if (acc->get_acc_cash() < m) { return false; }        // ������ �ܿ��ݾ��� m���� �۴ٸ� false�� ��ȯ
	else { return true; }                              // �ƴ϶�� true�� ��ȯ
}
void bank::account_transfer_bank(long long int src_acc_num, bank* dest_bank, long long int dest_acc_num, int m, int fee) {// ���¿��� ���·� ������ü�ϱ��
	account* src_account = account_get_accountnum(src_acc_num);  // ���� ���� ���¸� src_account�� ������
	src_account->change_acc_cash(src_account->get_acc_cash() - m - fee);          // src_account ���¿��� m���� ��
	dest_account = dest_bank->account_get_accountnum(dest_acc_num); // ���� �޴� ���¸� dest_account�� ������
	dest_account->change_acc_cash(dest_account->get_acc_cash() + m);        // dest_account ���¿��� m���� ����
}
void bank::cash_transfer_bank(bank* dest_bank, long long int dest_acc_num, int m) {  // �������� ���¿� ������ü�ϱ�
	dest_account = dest_bank->account_get_accountnum(dest_acc_num); // ���� �޴� ���¸� dest_account�� ������
	dest_account->change_acc_cash(dest_account->get_acc_cash() + m);        // dest_account ���¿��� m���� ����
}
void bank::withdrawl_bank(int cardnum, int m) { //����Լ� ��ũ��
	account* acc = account_get_account(cardnum); //ī������� ��ī��Ʈ
	acc->change_acc_cash(acc->get_acc_cash() - m);
}
void bank::deposit_bank(int cardnum, int m) {   //ī���ȣ�� �����ؼ� �� �ֱ�
	account* acc = account_get_account(cardnum);
	acc->change_acc_cash(acc->get_acc_cash() + m);
}
bool bank::bool_accountnumber_in_bank(long long int accountnumber) { // ���¹�ȣ�� �� ���࿡ �����ϴ��� bool�� Ȯ��
	int number_of_account = user_acc.size();
	for (int i = 0; i < number_of_account; i++) {
		if (accountnumber == user_acc[i]->get_acc_number()) { return true; }
	}
	return false;
}
bank* bank::bank_accountnumber_in_bank(int accountnumber) { // ���¹�ȣ�� �� ���࿡ �����Ѵٸ� ���� �ּ� ��ȯ
	int number_of_account = user_acc.size();
	for (int i = 0; i < number_of_account; i++) {
		if (accountnumber == user_acc[i]->get_acc_number()) { return this; }
	}
}
void bank::make_multi_uni_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, int serial_number) {
	atm_list.push_back(new multi_uni_atm(type, language_type, one_thou, five_thou, ten_thou, fifty_thou, this, serial_number));
}

void bank::make_multi_bi_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, int serial_number) {
	atm_list.push_back(new multi_bi_atm(type, language_type, one_thou, five_thou, ten_thou, fifty_thou, this, serial_number));
}

void bank::make_single_uni_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, int serial_number) {
	atm_list.push_back(new single_uni_atm(type, language_type, one_thou, five_thou, ten_thou, fifty_thou, this, serial_number));
}

void bank::make_single_bi_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, int serial_number) {
	atm_list.push_back(new single_bi_atm(type, language_type, one_thou, five_thou, ten_thou, fifty_thou, this, serial_number));
}

void bank::make_account(long long int ac_n, string user_name, int password, int cash) {
	user_acc.push_back(new account(ac_n, this, user_name, password, cash));
}
vector <atm*> bank::get_atm_list() { return atm_list; }

//transaction_history
transaction_history::transaction_history(int cardnum, string type, int am, bank* bank, account* account) {
	transaction_ID = transaction_ID_counter++;
	card_number = cardnum;
	tr_type = type;
	amount = am;
	tr_bank = bank;
	tr_account = account;
	curr_acc_money = tr_account->get_acc_cash();
}
void transaction_history::print_history() {
	std::cout << "transaction ID: " << transaction_ID << endl;
	std::cout << "transcaction type: " << tr_type << endl;
	std::cout << "card number: " << card_number << endl;
	std::cout << tr_type << " amount: " << amount << "won" << endl;
	std::cout << tr_type << " account -> " << tr_bank->get_bankname() << " bank: " << tr_account->get_acc_number() << ", owner: " << tr_account->get_user_name() << endl;
	std::cout << "account cash after transaction: " << curr_acc_money << "won" << endl << endl;
}
void transaction_history::korean_print_history() {
	std::cout << "�ŷ� ID: " << transaction_ID << endl;
	std::cout << "�ŷ� ����: " << tr_type << endl;
	std::cout << "ī�� ��ȣ: " << card_number << endl;
	std::cout << tr_type << " �ݾ�: " << amount << "��" << endl;
	std::cout << tr_type << " ���� -> " << tr_bank->get_bankname() << "����: " << tr_account->get_acc_number() << ", ������: " << tr_account->get_user_name() << endl;
	std::cout << "�ŷ� �� �ݾ�: " << curr_acc_money << "��" << endl << endl;
}
string transaction_history::print_txt() {
	string t = "transaction ID: " + to_string(transaction_ID) + '\n'
		+ "transcaction type: " + tr_type + '\n'
		+ "card number: " + to_string(card_number) + '\n'
		+ tr_type + " amount: " + to_string(amount) + "won" + '\n'
		+ tr_type + " account -> " + tr_bank->get_bankname() + " bank: " + to_string(tr_account->get_acc_number()) + ", owner: " + tr_account->get_user_name() + '\n'
		+ "account cash after transaction: " + to_string(curr_acc_money) + "won" + '\n' + '\n';
	return t;
}
string transaction_history::korean_print_txt() {
	string t = "�ŷ� ID: " + to_string(transaction_ID) + '\n'
		+ "�ŷ� ����: " + tr_type + '\n'
		+ "ī�� ��ȣ: " + to_string(card_number) + '\n'
		+ tr_type + " �ݾ�: " + to_string(amount) + "��" + '\n'
		+ tr_type + " ���� -> " + tr_bank->get_bankname() + "����: " + to_string(tr_account->get_acc_number()) + ", ������: " + tr_account->get_user_name() + '\n'
		+ "�ŷ� �� �ݾ�: " + to_string(curr_acc_money) + "��" + '\n' + '\n';
	return t;
}

transfer_acc_transaction_history::transfer_acc_transaction_history(int cardnum, string type, int am, bank* start_bank, bank* eb, account* start_account, account* ea) : transaction_history(cardnum, type, am, start_bank, start_account) {
	end_bank = eb;
	end_account = ea;
}
void transfer_acc_transaction_history::print_history() {
	std::cout << "transaction ID: " << transaction_ID << endl;
	std::cout << "transcaction type: " << tr_type << endl;
	std::cout << "card number: " << card_number << endl;
	std::cout << tr_type << "amount: " << amount << "won" << endl;
	std::cout << "source account -> " << tr_bank->get_bankname() << "bank: " << tr_account->get_acc_number() << ", owner: " << tr_account->get_user_name() << endl;
	std::cout << "destination account -> " << end_bank->get_bankname() << " bank: " << end_account->get_acc_number() << ", owner: " << end_account->get_user_name() << endl;
	std::cout << "account cash after transaction: " << curr_acc_money << "won" << endl << endl;
}
void transfer_acc_transaction_history::korean_print_history() {
	std::cout << "�ŷ� ID: " << transaction_ID << endl;
	std::cout << "�ŷ� ����: " << tr_type << endl;
	std::cout << "ī�� ��ȣ: " << card_number << endl;
	std::cout << tr_type << "�ݾ�: " << amount << "��" << endl;
	std::cout << "��ü ���� -> " << tr_bank->get_bankname() << "����: " << tr_account->get_acc_number() << ", ������: " << tr_account->get_user_name() << endl;
	std::cout << "�Ա� ���� -> " << end_bank->get_bankname() << "����: " << end_account->get_acc_number() << ", ������: " << end_account->get_user_name() << endl;
	std::cout << "�ŷ� �� �ݾ�: " << curr_acc_money << "��" << endl << endl;
}
string transfer_acc_transaction_history::print_txt() {
	string t = "transaction ID: " + to_string(transaction_ID) + '\n'
		+ "transcaction type: " + tr_type + '\n'
		+ "card number: " + to_string(card_number) + '\n'
		+ tr_type + " amount: " + to_string(amount) + "won" + '\n'
		+ "source account -> " + tr_bank->get_bankname() + "bank: " + to_string(tr_account->get_acc_number()) + ", owner: " + tr_account->get_user_name() + '\n'
		+ "destination account -> " + end_bank->get_bankname() + " bank: " + to_string(end_account->get_acc_number()) + ", owner: " + end_account->get_user_name() + '\n'
		+ "account cash after transaction: " + to_string(curr_acc_money) + "won" + '\n' + '\n';
	return t;
}
string transfer_acc_transaction_history::korean_print_txt() {
	string t = "�ŷ� ID: " + to_string(transaction_ID) + '\n'
		+ "�ŷ� ����: " + tr_type + '\n'
		+ "ī�� ��ȣ: " + to_string(card_number) + '\n'
		+ tr_type + " �ݾ�: " + to_string(amount) + "��" + '\n'
		+ "��ü ���� -> " + tr_bank->get_bankname() + "����: " + to_string(tr_account->get_acc_number()) + ", ������: " + tr_account->get_user_name() + '\n'
		+ "�Ա� ���� -> " + end_bank->get_bankname() + "����: " + to_string(end_account->get_acc_number()) + ", ������: " + end_account->get_user_name() + '\n'
		+ "�ŷ� �� �ݾ�: " + to_string(curr_acc_money) + "��" + '\n' + '\n';
	return t;
}



//���ºκ�
account::account(long long int acc_n, bank* b_adress, string u_name, int pd, int a_cash) {
	bank_adress = b_adress; //bank �ּ�
	user_name = u_name; // ���� �̸�
	acc_number = acc_n; // ���� ��ȣ
	acc_cash = a_cash; // ���� ��
	password = pd; // ��й�ȣ
	account_card = new card(this->get_bank_adress());
	card_num = account_card->get_cardnum();
	accountlist.push_back(this);
	std::cout << "���°� �����Ǿ����ϴ�. ������ ������ " << bank_adress->get_bankname() << "�̸� " << "���� �̸��� " << user_name << "�Դϴ�. ���¹�ȣ�� " << acc_number << "�Դϴ�." << endl;
}
void account::print_account_history() {
	for (int i = 0; i < account_tr_his.size(); i++) {
		account_tr_his[i]->print_history();
	}
}
void account::korean_print_account_history() {
	for (int i = 0; i < account_tr_his.size(); i++) {
		account_tr_his[i]->korean_print_history();
	}
}
atm::atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, bank* pb, int serialnumber) { //constructor 
	single_or_multi = type;
	uni_or_bi = language_type;
	serial_number = serialnumber;
	atm_cash[0] = one_thou;
	atm_cash[1] = five_thou;
	atm_cash[2] = ten_thou;
	atm_cash[3] = fifty_thou;
	primary_bank = pb;
	admin_card = new card(this->primary_bank, this);
	atmlist.push_back(this);

	std::cout << "atm �� �����Ǿ����ϴ�. atm�� serial number�� " << serial_number << "�Դϴ�." << "���� ���� " << get_atm_cash() << "�����ϰ� �ֽ��ϴ�." << endl;
}
bool atm::valid_cardnumber_account(int cardnumber) { //ī�� ��ȣ�� �ش��ϴ� ���°� �ִ� �� ������
	int banknum = banklist.size();
	for (int i = 0; i < banknum; i++)
	{
		if ((banklist[i]->bool_get_account(cardnumber)) == true)
		{
			return true;
		}
	}
	return false;
}
bank* atm::find_cardnumber_bank(int cardnumber) { // ī�� ��ȣ�� �ش��ϴ� ������ ã��
	int banknum = banklist.size();
	for (int i = 0; i < banknum; i++) {
		if ((banklist[i]->bool_get_account(cardnumber)) == true)
		{
			return banklist[i];
		}
	}
}
bool atm::valid_find_bank_by_account(long long int accountnumber) { //���¹�ȣ�� ��ȿ������ true false return
	int banknum = banklist.size();
	for (int i = 0; i < banknum; i++) {
		if ((banklist[i]->bool_accountnumber_in_bank(accountnumber)) == true)
		{
			return true;
		}
	}
	return false;
}

bank* atm::find_bank_by_account(long long int accountnumber) { //���¹�ȣ�� ���� ã��
	int banknum = banklist.size();
	for (int i = 0; i < banknum; i++) {
		if ((banklist[i]->bool_accountnumber_in_bank(accountnumber)) == true)
		{
			return banklist[i];
		}
	}
}
bool atm::check_password(int cardnumber, int password) {
	return find_cardnumber_bank(cardnumber)->authorize_bank(cardnumber, password);
}
void atm::print_session_history() {
	for (int i = 0; i < temp_tr_his.size(); i++) {
		temp_tr_his[i]->print_history();
		atm_tr_his.push_back(temp_tr_his[i]);
	}
	temp_tr_his.clear();//������ ã�ƺ��� ���� �ʱ�ȭ
}
void atm::print_atm_history() {
	std::ofstream outFile("atm_history.txt", std::ios::trunc);
	for (int i = 0; i < atm_tr_his.size(); i++) {
		atm_tr_his[i]->print_history();
		outFile.write(atm_tr_his[i]->print_txt().c_str(), atm_tr_his[i]->print_txt().size());
	}
	outFile.close();
}

void atm::transfer() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int money = 0;
	int money_sum = 0;
	int c1;
	int fee;
	bank* curr_bank = find_cardnumber_bank(card_insert_slot);
	long long int dest_account;
	bank* dest_bank;
	long long int src_account;
	bank* src_bank;
	string u_name = curr_bank->account_get_account(card_insert_slot)->get_user_name();

	while (1) {
		std::cout << "Please choose between 1 Cash transfer or 2 Account transfer : ";
		std::cin >> c1;
		if (c1 == 1) {       //���� ������ü
			std::cout << "Please enter the Destination Account : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			std::cout << "Please enter the cash to send: ";
			std::cin >> money;
			std::cout << "Please enter the amount you wish to send along with the 5,000 won fee." << endl;
			std::cout << "How many 1,000 won would you like to put in? ";
			std::cin >> one_thou;
			std::cout << "How many 5,000 won would you like to put in? ";
			std::cin >> five_thou;
			std::cout << "How many 10,000 won would you like to put in? ";
			std::cin >> ten_thou;
			std::cout << "How many 50,000 won would you like to put in?";
			std::cin >> fifty_thou;
			money_sum = one_thou * 1000 + five_thou * 5000 + ten_thou * 10000 + fifty_thou * 50000;
			if (money_sum != money + 5000) {
				std::cout << "The money you requested and the money you entered are different." << endl;
				return;
			};
			change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
			curr_bank->cash_transfer_bank(dest_bank, dest_account, money);
			std::cout << "Cash transfer has been completed." << endl;
			transaction_history* th = new transaction_history(card_insert_slot, "����_������ü", money, dest_bank, dest_bank->account_get_accountnum(dest_account));
			temp_tr_his.push_back(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);

			return;
		}

		else if (c1 == 2) {    // ���³��� ������ü
			std::cout << "Please enter the Destination Account : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			std::cout << "Enter your account number : ";
			std::cin >> src_account;
			src_bank = find_bank_by_account(src_account);
			if (!valid_find_bank_by_account(src_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}

			if (u_name != src_bank->account_get_accountnum(src_account)->get_user_name() or src_bank != curr_bank) {
				std::cout << "You cannot send money from this account. Please insert another bank card!" << endl;
				return;
			}

			if (src_bank == primary_bank && dest_bank == primary_bank) { fee = 2000; }
			else if ((src_bank != primary_bank && dest_bank == primary_bank) or (src_bank == primary_bank && dest_bank != primary_bank)) { fee = 3000; }
			else if (src_bank != primary_bank && dest_bank != primary_bank) { fee = 4000; }

			std::cout << "Please enter the cash to send : ";
			std::cin >> money;
			if (!curr_bank->cn_enough_money_in_acc_bank(card_insert_slot, money + fee)) {
				std::cout << "There is not enough money in the account.";
				return;
			}
			curr_bank->account_transfer_bank(src_account, dest_bank, dest_account, money, fee);
			std::cout << money << "+" << fee << " Won was deducted from your account." << endl;
			cout << "The account transfer has been completed." << endl << endl;
			transfer_acc_transaction_history* th = new transfer_acc_transaction_history(card_insert_slot, "����_������ü", money, src_bank, dest_bank, src_bank->account_get_accountnum(src_account), dest_bank->account_get_accountnum(dest_account));
			temp_tr_his.push_back(th);
			src_bank->account_get_accountnum(src_account)->change_account_tr(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}
		else {
			std::cout << "Choose between 1 and 2!!!" << endl;
			return;
		}
	}
}

void atm::deposit() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	long long int check_sum = 0;
	int withdrawl_count = 0;
	bank* b = find_cardnumber_bank(card_insert_slot);
	if (b != primary_bank) {
		std::cout << "Ÿ ���� �Աݿ��� ������ 1000���� �߻��մϴ�. �����Ḧ �����ؼ� �Ա��ϼ���" << endl;
	}
	while (true) {
		std::cout << "õ���� �� �� �����ðڽ��ϴϱ�? (�����Ϸ��� -1�� �Է��ϼ���)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "��õ���� �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> five_thou;
		std::cout << "������ �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> ten_thou;
		std::cout << "�������� �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "������ ���� �� ���� �ʰ��ϼ̽��ϴ�." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "��ǥ�� �� �� �����ðڽ��ϱ�? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "������ ��ǥ �� ���� �ʰ��ϼ̽��ϴ�." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 �϶� ����
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "��° ��ǥ�� �ݾ��� ���Դϱ�? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100�� �̻��� �Ұ�
				flag = 1; //flag = 1�̸� ���� �߻�
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "�߸��� ��ǥ �ݾ� �Դϴ�." << endl;
		}
		else { break; }
	}
	atm_cash[0] += one_thou;
	atm_cash[1] += five_thou;
	atm_cash[2] += ten_thou;
	atm_cash[3] += fifty_thou;
	long long int m = 1000 * one_thou + 5000 * five_thou + 10000 * ten_thou + 50000 * fifty_thou + check_sum;
	if (b != primary_bank) { m = m - 1000; }
	b->deposit_bank(card_insert_slot, m);
	std::cout << m << "���� �ԱݵǾ����ϴ�." << endl;
	std::cout << "�ܾ��� " << b->account_get_account(card_insert_slot)->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(card_insert_slot, "�Ա�", m, b, b->account_get_account(card_insert_slot));
	temp_tr_his.push_back(th);
	b->account_get_account(card_insert_slot)->change_account_tr(th);
	return;
}

void atm::withdrawl() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	int flag = 0;
	bank* b = find_cardnumber_bank(card_insert_slot);
	if (number_of_withdrawl >= limit_of_number_of_withdral) { //withdrawl �ʱ�ȭ �ʿ�. atm�� ī�带 ������ withdrawl�� �ʱ�ȭ �������.
		std::cout << "�� ����Ͻ÷��� ������ �ʱ�ȭ �ϼž� �մϴ�." << endl;
		return;
	}
	if (b != primary_bank) {
		std::cout << "Ÿ ���� ��ݿ��� ������ 2000���� �߻��մϴ�." << endl;
		flag = 1;
	}
	else {
		std::cout << "�� ���� ��ݿ��� ������ 1000���� �߻��մϴ�." << endl;
	}
	while (1) {
		std::cout << "�󸶸� ����Ͻðڽ��ϱ�?(������ ����) (�����Ϸ��� -1�� �Է��ϼ���)";
		std::cin >> check_num;
		if (check_num == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//��� ������ �ݾ��� �ʰ��Ͽ����� ����
			std::cout << "��� �����Ͻ� �ݾ��� �ʰ��ϼ̽��ϴ�" << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "ȭ�� �ּ� ������ 1000�� �Դϴ�" << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "ATM�� ����� ������ �������� �ʽ��ϴ�" << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(card_insert_slot, (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "���¿� ���� ������� �ʽ��ϴ�" << endl;
		}
		else {
			int m = check_num;
			fifty_thou = _MIN_((m / 50000), atm_cash[3]);
			m = m - 50000 * fifty_thou;
			ten_thou = _MIN_((m / 10000), atm_cash[2]);
			m = m - 10000 * ten_thou;
			five_thou = _MIN_((m / 5000), atm_cash[1]);
			m = m - 5000 * five_thou;
			one_thou = _MIN_((m / 1000), atm_cash[0]);
			m = m - 1000 * one_thou;

			if (m != 0) {
				std::cout << "ATM�� ������ ������ �����մϴ�" << endl;
			}
			else {
				//atm_cash ����
				atm_cash[0] -= one_thou;
				atm_cash[1] -= five_thou;
				atm_cash[2] -= ten_thou;
				atm_cash[3] -= fifty_thou;
				b->withdrawl_bank(card_insert_slot, (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "���࿡�� " << (flag ? (check_num + 2000) : (check_num + 1000)) << "���� ��ݵǾ��� �������� ������ �� " << check_num << "�� �Դϴ�." << endl;
				std::cout << "õ�� " << one_thou << "�� / " << "��õ�� " << five_thou << "�� / " << "���� " << ten_thou << "�� / " << "������ " << fifty_thou << "��" << endl;
				std::cout << "�ܾ��� " << b->account_get_account(card_insert_slot)->get_acc_cash() << endl;
				number_of_withdrawl += 1;
				return;
				transaction_history* th;
				th = new transaction_history(card_insert_slot, "���", check_num, b, b->account_get_account(card_insert_slot));
				temp_tr_his.push_back(th);
				b->account_get_account(card_insert_slot)->change_account_tr(th);
				return;
			}
		}
	}
}
void atm::korean_print_atm_history() {
	std::ofstream outFile("atm_history.txt", std::ios::trunc);
	for (int i = 0; i < get_atm_tr().size(); i++) {
		get_atm_tr()[i]->korean_print_history();
		outFile.write(get_atm_tr()[i]->print_txt().c_str(), get_atm_tr()[i]->print_txt().size());
	}
	outFile.close();
}


void atm::interface() {
	increase_session();
	set_number_of_withdrawl(0);
	int password;
	int work_type;
	int password_count = 0;
	int number;//�߰�
main1:
	std::cout << "Please insert the card.(Please enter the card number). If you want to exit, enter -1.";
	std::cin >> number; //����
	set_card_insert_slot(number);//�߰�
	if (get_card_insert_slot() == -1) {
		return;
	}
	if (get_card_insert_slot() == 7777777) {
		admin_interface();
		goto end;
	}
	if (valid_cardnumber_account(get_card_insert_slot()) == 0) {
		cout << "Invalid card number." << endl;
		goto main1;
	}
	while (1) {
		if (password_count == 3) {
			break;
		}
		std::cout << "Please enter the password." << endl;
		std::cin >> password;
		if (check_password(get_card_insert_slot(), password)) {
			break;
		}
		cout << "Wrong Password" << endl;
		password_count++;
	}
	if (password_count == 3) {
		cout << "You got the password wrong 3 times. " << endl;
		goto end;//�μ� end
	}
	while (1) {
		std::cout << "What kind of work would you like to do? Please enter the number." << endl;
		std::cout << "1. deposit " << "2. withdrawl " << "3. transfer " << "4. end session:  ";
		std::cin >> work_type;
		if (work_type == 1) { deposit(); }
		else if (work_type == 2) { withdrawl(); }
		else if (work_type == 3) { transfer(); }
		else if (work_type == 4) { break; }
		else { std::cout << "You have selected the wrong menu." << endl; }
	}
end:
	cout << "Ends the session." << endl;
	print_session_history();
	decrease_session();
}

void atm::koreaninterface() {
	increase_session();
	number_of_withdrawl = 0;
	int password;
	int work_type;
	int password_count = 0;
main1:
	std::cout << "ī�带 �־��ּ���.(ī���ȣ�� �Է����ּ���). �����ϰ� �����ø� -1�� �Է����ּ���.";
	std::cin >> card_insert_slot;
	if (card_insert_slot == -1) {
		return;
	}
	if (card_insert_slot == 7777777) {
		admin_interface_korean();
		goto end;
	}
	if (valid_cardnumber_account(card_insert_slot) == 0) {
		cout << "��ȿ���� ���� ī�� ��ȣ�Դϴ�. " << endl;
		goto main1;
	}
	while (1) {
		if (password_count == 3) {
			break;
		}
		std::cout << "��й�ȣ�� �Է����ּ���." << endl;
		std::cin >> password;
		if (check_password(card_insert_slot, password)) {
			break;
		}
		cout << "�߸��� ��й�ȣ�Դϴ�." << endl;
		password_count++;
	}
	if (password_count == 3) {
		cout << "��й�ȣ�� 3�� Ʋ���̽��ϴ�. " << endl;
		goto end;//�μ� end
	}
	while (1) {
		std::cout << "� ������ �Ͻðڽ��ϱ�? ��ȣ�� �����Ͽ� �ּ���." << endl;
		std::cout << "1. �Ա� " << "2. ��� " << "3. ��ü " << "4. ���� ���� :  ";
		std::cin >> work_type;
		if (work_type == 1) { deposit(); }
		else if (work_type == 2) { withdrawl(); }
		else if (work_type == 3) { transfer(); }
		else if (work_type == 4) { break; }
		else { std::cout << "�߸��� �޴��� �����ϼ̽��ϴ�." << endl; }
	}
end:
	cout << "������ �����մϴ�." << endl;
	print_session_history();
	decrease_session();
}

void atm::admin_interface() {
	int number;
	cout << "Choose the menu" << endl;
	cout << "Menu" << endl;
	cout << "1. Transcation History" << endl;
menu1:
	cin >> number;
	if (number != 1) {
		cout << "Wrong number. Please enter it one more time." << endl;
		goto menu1;
	}
	else {
		print_atm_history();
		// transaction_history ������ټ� �� ����?
		//������ ����ٰ� fileout
	}
	return;
}

void atm::admin_interface_korean() {
	int number;
	cout << "�����Ͻ� �޴��� ����ּ���." << endl;
	cout << "�޴�" << endl;
	cout << "1. �ŷ� ���" << endl;
menu1:
	cin >> number;
	if (number != 1) {
		cout << "�߸��� ��ȣ�� �Է��ϼ̽��ϴ�. �ٽ� �Է����ּ���." << endl;
		goto menu1;
	}
	else {
		korean_print_atm_history();
		// transaction_history ������ټ� �� ����?
		//������ ����ٰ� fileout
	}
	return;
}

void show_information() {
	int account_number = accountlist.size();
	int atm_number = atmlist.size();
	for (int i = 0; i < account_number; i++) {
		cout << "[Account " << i << "] Balance:" << accountlist[i]->get_acc_cash() << endl;
	}
	for (int i = 0; i < atm_number; i++) {
		cout << "[ATM " << i << "]";
		cout << "Remaining Cash: " << atmlist[i]->get_atm_cash();
		if (atmlist[i]->get_atm_cash() != 0) {
			cout << "(";
		}
		if (atmlist[i]->get_1000() >= 1) {
			cout << "1000*" << atmlist[i]->get_1000() << "�� ";
		}
		if (atmlist[i]->get_5000() >= 1) {
			cout << "5000*" << atmlist[i]->get_5000() << "�� ";
		}
		if (atmlist[i]->get_10000() >= 1) {
			cout << "10000*" << atmlist[i]->get_10000() << "�� ";
		}
		if (atmlist[i]->get_50000() >= 1) {
			cout << "50000*" << atmlist[i]->get_50000() << "�� ";
		}
		if (atmlist[i]->get_atm_cash() != 0) {
			cout << ")";
		}
		cout << endl;
	}
}
void atm::mini_interface() {
	int number;
	if (uni_or_bi == 0) {
		interface();
		return;
	}
	else if (uni_or_bi == 1) {
	menu1:
		cout << "� �� ���ðڽ��ϱ�? Which language do you prefer?" << endl;
		cout << "0. ���� \t 1. �ѱ���" << endl;
		cin >> number;
		if (number == 0) {
			interface();
		}
		else if (number == 1) {
			koreaninterface();
		}
		else {
			cout << "�߸��� ��ȣ�� �Է��ϼ̽��ϴ�" << endl;
			goto menu1;
		}
	}
}

void atm::koreantransfer() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int money = 0;
	int money_sum = 0;
	int c1;
	int fee;
	bank* curr_bank = find_cardnumber_bank(card_insert_slot);
	long long int dest_account;
	bank* dest_bank;
	long long int src_account;
	bank* src_bank;
	string u_name = curr_bank->account_get_account(card_insert_slot)->get_user_name();

	while (1) {
		std::cout << "� ������ �Ͻðڽ��ϱ�? ��ȣ�� �������ּ���. 1. ���� ��ü 2. ���� ��ü : ";
		std::cin >> c1;
		if (c1 == 1) {       //���� ������ü
			std::cout << "�������� ���� ��ü�Ͻðڽ��ϱ�? �۱� ���� ���¸� �Է����ּ���. : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "�ش��ϴ� ������´� �������� �ʽ��ϴ�." << endl;
				return;
			}
			std::cout << "�󸶸� �۱��Ͻðڽ��ϱ�?: ";
			std::cin >> money;
			std::cout << "5000���� �����ᰡ ���, �����Ḧ �����ؼ� ������ �־��ּ���" << endl;
			std::cout << "1000�� ¥�� ���� �� �� �ְڽ��ϱ�? ";
			std::cin >> one_thou;
			std::cout << "5000�� ¥�� ���� �� �� �ְڽ��ϱ�? ";
			std::cin >> five_thou;
			std::cout << "10000�� ¥�� ���� �� �� �ְڽ��ϱ�? ";
			std::cin >> ten_thou;
			std::cout << "50000�� ¥�� ���� �� �� �ְڽ��ϱ�? ";
			std::cin >> fifty_thou;
			money_sum = one_thou * 1000 + five_thou * 5000 + ten_thou * 10000 + fifty_thou * 50000;
			if (money_sum != money + 5000) {
				std::cout << "����� �Է��� ���� ���� ���� �ٸ��ϴ�." << endl;
				return;
			}
			atm_cash[0] += one_thou;
			atm_cash[1] += five_thou;
			atm_cash[2] += ten_thou;
			atm_cash[3] += fifty_thou;
			curr_bank->cash_transfer_bank(dest_bank, dest_account, money);
			std::cout << "���� ��ü�� �Ϸ�Ǿ����ϴ�." << endl << endl;
			return;
		}

		else if (c1 == 2) {    // ���³��� ������ü
			std::cout << "���� ���� ������ ���¹�ȣ�� �Է��ϼ��� : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "�ش� ���´� ��ȿ���� �ʽ��ϴ�." << endl; //�̰� �¾�?
				return;
			}
			std::cout << "����� ���¹�ȣ�� �Է����ּ��� : ";
			std::cin >> src_account;
			src_bank = find_bank_by_account(src_account);
			if (!valid_find_bank_by_account(src_account)) {
				std::cout << "�ش��ϴ� ���°� �����ϴ�." << endl;
				return;
			}

			if (src_bank == primary_bank && dest_bank == primary_bank) { fee = 2000; }
			else if ((src_bank != primary_bank && dest_bank == primary_bank) or (src_bank == primary_bank && dest_bank != primary_bank)) { fee = 3000; }
			else if (src_bank != primary_bank && dest_bank != primary_bank) { fee = 4000; }

			if (u_name != src_bank->account_get_accountnum(src_account)->get_user_name() or src_bank != curr_bank) {
				std::cout << "�ش� ���¿��� ���� ��ü�� �� �����ϴ�." << endl;
				return;
			}

			std::cout << "�󸶳� ���� �� �Է��ϼ���. : ";
			std::cin >> money;
			if (!curr_bank->cn_enough_money_in_acc_bank(card_insert_slot, money + fee)) {
				std::cout << "���¿� ����� ���� ���� �ʽ��ϴ�.";
				return;
			}
			curr_bank->account_transfer_bank(src_account, dest_bank, dest_account, money, fee);
			std::cout << money << "+" << fee << "���� ���¿��� �����ϴ�." << endl;
			cout << "���� ��ü�� �Ϸ�Ǿ����ϴ�." << endl << endl;
			return;
		}
		else {
			std::cout << "1���� 2�� ���̿� ���ÿ�." << endl;
			return;
		}
	}
}




void atm::koreandeposit() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	long long int check_sum = 0;
	int withdrawl_count = 0;
	bank* b = find_cardnumber_bank(card_insert_slot);
	if (b != primary_bank) {
		std::cout << "Ÿ ���� �Աݿ��� ������ 1000���� �߻��մϴ�. �����Ḧ �����ؼ� �Ա��ϼ���" << endl;
	}
	while (true) {
		std::cout << "õ���� �� �� �����ðڽ��ϴϱ�? (�����Ϸ��� -1�� �Է��ϼ���)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "��õ���� �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> five_thou;
		std::cout << "������ �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> ten_thou;
		std::cout << "�������� �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "������ ���� �� ���� �ʰ��ϼ̽��ϴ�." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "��ǥ�� �� �� �����ðڽ��ϱ�? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "������ ��ǥ �� ���� �ʰ��ϼ̽��ϴ�." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 �϶� ����
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "��° ��ǥ�� �ݾ��� ���Դϱ�? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) {
				flag = 1; //flag = 1�̸� ���� �߻�
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "�߸��� ��ǥ �ݾ� �Դϴ�." << endl;
		}
		else { break; }
	}
	atm_cash[0] += one_thou;
	atm_cash[1] += five_thou;
	atm_cash[2] += ten_thou;
	atm_cash[3] += fifty_thou;
	long long int m = 1000 * one_thou + 5000 * five_thou + 10000 * ten_thou + 50000 * fifty_thou + check_sum;
	if (b != primary_bank) { m = m - 1000; }
	b->deposit_bank(card_insert_slot, m);
	std::cout << m << "���� �ԱݵǾ����ϴ�." << endl;
	std::cout << "�ܾ��� " << b->account_get_account(card_insert_slot)->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(card_insert_slot, "�Ա�", m, b, b->account_get_account(card_insert_slot));
	temp_tr_his.push_back(th);
	b->account_get_account(card_insert_slot)->change_account_tr(th);
	return;
}


void atm::koreanwithdrawl() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	int flag = 0;
	bank* b = find_cardnumber_bank(card_insert_slot);
	if (number_of_withdrawl > limit_of_number_of_withdral) { //withdrawl �ʱ�ȭ �ʿ�. atm�� ī�带 ������ withdrawl�� �ʱ�ȭ �������.
		std::cout << "�� ����Ͻ÷��� ������ �ʱ�ȭ �ϼž� �մϴ�." << endl;
		return;
	}
	if (b != primary_bank) {
		std::cout << "Ÿ ���� ��ݿ��� ������ 2000���� �߻��մϴ�." << endl;
		flag = 1;
	}
	else {
		std::cout << "�� ���� ��ݿ��� ������ 1000���� �߻��մϴ�." << endl;
	}
	while (1) {
		std::cout << "õ���� �� �� �����ðڽ��ϴϱ�? (�����Ϸ��� -1�� �Է��ϼ���)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//��� ������ �ݾ��� �ʰ��Ͽ����� ����
			std::cout << "��� �����Ͻ� �ݾ��� �ʰ��ϼ̽��ϴ�" << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "ȭ�� �ּ� ������ 1000�� �Դϴ�" << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "ATM�� ����� ������ �������� �ʽ��ϴ�" << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(card_insert_slot, (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "���¿� ���� ������� �ʽ��ϴ�" << endl;
		}
		else {
			int m = check_num;
			fifty_thou = _MIN_((m / 50000), atm_cash[3]);
			m = m - 50000 * fifty_thou;
			ten_thou = _MIN_((m / 10000), atm_cash[2]);
			m = m - 10000 * ten_thou;
			five_thou = _MIN_((m / 5000), atm_cash[1]);
			m = m - 5000 * five_thou;
			one_thou = _MIN_((m / 1000), atm_cash[0]);
			m = m - 1000 * one_thou;

			if (m != 0) {
				std::cout << "ATM�� ������ ������ �����մϴ�" << endl;
			}
			else {
				//atm_cash ����
				atm_cash[0] -= one_thou;
				atm_cash[1] -= five_thou;
				atm_cash[2] -= ten_thou;
				atm_cash[3] -= fifty_thou;
				b->withdrawl_bank(card_insert_slot, (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "���࿡�� " << (flag ? (check_num + 2000) : (check_num + 1000)) << "���� ��ݵǾ��� �������� ������ �� " << check_num << "�� �Դϴ�." << endl;
				std::cout << "õ�� " << one_thou << "�� / " << "��õ�� " << five_thou << "�� / " << "���� " << ten_thou << "�� / " << "������ " << fifty_thou << "��" << endl;
				std::cout << "�ܾ��� " << b->account_get_account(card_insert_slot)->get_acc_cash() << endl;
				number_of_withdrawl += 1;
				transaction_history* th;
				th = new transaction_history(card_insert_slot, "���", check_num, b, b->account_get_account(card_insert_slot));
				temp_tr_his.push_back(th);
				b->account_get_account(card_insert_slot)->change_account_tr(th);
				return;
			}
		}
	}
}
//������ ����

// single_uni

void single_uni_atm::transfer() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int money = 0;
	int money_sum = 0;
	int c1;
	int fee;
	bank* curr_bank = find_cardnumber_bank(get_card_insert_slot());
	long long int dest_account;
	bank* dest_bank;
	long long int src_account;
	bank* src_bank;
	string u_name = curr_bank->account_get_account(get_card_insert_slot())->get_user_name();

	while (1) {
		std::cout << "Please choose between 1 Cash transfer or 2 Account transfer : ";
		std::cin >> c1;
		if (c1 == 1) {       //���� ������ü
			std::cout << "Please enter the Destination Account : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (dest_bank != get_primary_bank()) {
				cout << "This is not primary bank account." << endl;
				return;
			}
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			std::cout << "Please enter the cash to send: ";
			std::cin >> money;
			std::cout << "Please enter the amount you wish to send along with the 5,000 won fee." << endl;
			std::cout << "How many 1,000 won would you like to put in? ";
			std::cin >> one_thou;
			std::cout << "How many 5,000 won would you like to put in? ";
			std::cin >> five_thou;
			std::cout << "How many 10,000 won would you like to put in? ";
			std::cin >> ten_thou;
			std::cout << "How many 50,000 won would you like to put in?";
			std::cin >> fifty_thou;
			money_sum = one_thou * 1000 + five_thou * 5000 + ten_thou * 10000 + fifty_thou * 50000;
			if (money_sum != money + 5000) {
				std::cout << "The money you requested and the money you entered are different." << endl;
				return;
			}
			change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
			curr_bank->cash_transfer_bank(dest_bank, dest_account, money);
			std::cout << "Cash transfer has been completed." << endl;
			transaction_history* th = new transaction_history(get_card_insert_slot(), "cash_transfer", money, dest_bank, dest_bank->account_get_accountnum(dest_account));
			change_temp(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}

		else if (c1 == 2) {    // ���³��� ������ü
			std::cout << "Please enter the Destination Account : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			if (dest_bank != get_primary_bank()) {
				cout << "This is not primary bank account." << endl;
				return;
			}
			std::cout << "Enter your account number : ";
			std::cin >> src_account;
			src_bank = find_bank_by_account(src_account);
			if (!valid_find_bank_by_account(src_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			if (src_bank != get_primary_bank()) {
				cout << "This is not primary bank account." << endl;
				return;
			}
			if (u_name != src_bank->account_get_accountnum(src_account)->get_user_name() or src_bank != curr_bank) {
				std::cout << "You cannot send money from this account. Please insert another bank card!" << endl;
				return;
			}

			if (src_bank == get_primary_bank() && dest_bank == get_primary_bank()) { fee = 2000; }
			else if ((src_bank != get_primary_bank() && dest_bank == get_primary_bank()) or (src_bank == get_primary_bank() && dest_bank != get_primary_bank())) { fee = 3000; }
			else if (src_bank != get_primary_bank() && dest_bank != get_primary_bank()) { fee = 4000; }

			std::cout << "Please enter the cash to send : ";
			std::cin >> money;
			if (!curr_bank->cn_enough_money_in_acc_bank(get_card_insert_slot(), money + fee)) {
				std::cout << "There is not enough money in the account.";
				return;
			}
			curr_bank->account_transfer_bank(src_account, dest_bank, dest_account, money, fee);
			std::cout << money << "+" << fee << " Won was deducted from your account." << endl;
			cout << "The account transfer has been completed." << endl << endl;
			transfer_acc_transaction_history* th = new transfer_acc_transaction_history(get_card_insert_slot(), "account_transfer", money, src_bank, dest_bank, src_bank->account_get_accountnum(src_account), dest_bank->account_get_accountnum(dest_account));
			change_transfer_temp(th);
			src_bank->account_get_accountnum(src_account)->change_account_tr(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}
		else {
			std::cout << "Choose between 1 and 2!!!" << endl;
			return;
		}
	}
}

void single_uni_atm::deposit() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	long long int check_sum = 0;
	int withdrawl_count = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (b != get_primary_bank()) {
		std::cout << "A fee of 1,000 won will be charged for deposits to other banks. Deposit including fees" << endl;
	}
	while (true) {
		std::cout << "How many 1,000 won would you like to put in? (Enter -1 to exit)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "How many 5,000 won would you like to put in? ";
		std::cin >> five_thou;
		std::cout << "How many 1,0000 won would you like to put in? ";
		std::cin >> ten_thou;
		std::cout << "How many 5,0000 won would you like to put in? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "You have exceeded the number of cash cards available." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "How many checks would you like to write? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "You have exceeded the number of checks available." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 �϶� ����
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "th check, how much? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100�� �̻��� �Ұ�
				flag = 1; //flag = 1�̸� ���� �߻�
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "The check amount is incorrect." << endl;
		}
		else { break; }
	}
	change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
	long long int m = 1000 * one_thou + 5000 * five_thou + 10000 * ten_thou + 50000 * fifty_thou + check_sum;
	if (b != get_primary_bank()) { m = m - 1000; }
	b->deposit_bank(get_card_insert_slot(), m);
	std::cout << m << "Won has been deposited." << endl;
	std::cout << "The balance is " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(get_card_insert_slot(), "deposit", m, b, b->account_get_account(get_card_insert_slot()));
	change_temp(th);
	b->account_get_account(get_card_insert_slot())->change_account_tr(th);
	return;
}

void single_uni_atm::withdrawl() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	int flag = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (get_number_of_withdrawl() >= get_limit_of_number_of_withdral()) {
		std::cout << "If you want to withdraw more, you must reset your session." << endl;
		return;
	}
	if (b != get_primary_bank()) {
		std::cout << "A fee of 2,000 won is charged for withdrawals from other banks." << endl;
		flag = 1;
	}
	else {
		std::cout << "There is a fee of 1,000 won for withdrawal." << endl;
	}
	while (1) {
		std::cout << "How much do you want to withdraw (excluding fees)(Enter -1 to exit) : ";
		std::cin >> check_num;
		if (check_num == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//��� ������ �ݾ��� �ʰ��Ͽ����� ����
			std::cout << "You have exceeded the amount you can withdraw." << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "The minimum unit of currency is 1,000 won." << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "There is not enough cash in the ATM." << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "There is not enough money in the account" << endl;
		}
		else {
			int m = check_num;
			fifty_thou = _MIN_((m / 50000), get_50000());
			m = m - 50000 * fifty_thou;
			ten_thou = _MIN_((m / 10000), get_10000());
			m = m - 10000 * ten_thou;
			five_thou = _MIN_((m / 5000), get_5000());
			m = m - 5000 * five_thou;
			one_thou = _MIN_((m / 1000), get_1000());
			m = m - 1000 * one_thou;

			if (m != 0) {
				std::cout << "There are not enough bills in the ATM" << endl;
			}
			else {
				//atm_cash ����
				change_atm_cash(-1 * (one_thou), -1 * (five_thou), -1 * (ten_thou), -1 * (fifty_thou));
				b->withdrawl_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "In the bank " << (flag ? (check_num + 2000) : (check_num + 1000)) << "won was withdrawn,  Total cash to bring is " << check_num << "Won." << endl;
				std::cout << "1000 Won " << one_thou << "sheets / " << "5000Won " << five_thou << "sheets / " << "10000 Won " << ten_thou << "sheets / " << "50000 Won " << fifty_thou << "sheets." << endl;
				std::cout << "The balance is " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
				set_number_of_withdrawl(get_number_of_withdrawl() + 1);
				transaction_history* th = new transaction_history(get_card_insert_slot(), "Withdrawl", check_num, b, b->account_get_account(get_card_insert_slot()));
				change_temp(th);
				b->account_get_account(get_card_insert_slot())->change_account_tr(th);
				return;
			}
		}
	}
}

void single_uni_atm::interface() {
	increase_session();
	set_number_of_withdrawl(0);
	int password;
	int work_type;
	int password_count = 0;
	int number;//�߰�
main1:
	std::cout << "Please insert the card.(Please enter the card number). If you want to exit, enter -1.";
	std::cin >> number; //����
	set_card_insert_slot(number);//�߰�
	if (get_card_insert_slot() == -1) {
		return;
	}
	if (get_card_insert_slot() == 7777777) {
		admin_interface();
		goto end;
	}
	if (find_cardnumber_bank(get_card_insert_slot()) != get_primary_bank()) {
		cout << " This is not primary bank account." << endl;
		return;
	}
	if (valid_cardnumber_account(get_card_insert_slot()) == 0) {
		cout << "Invalid card number." << endl;
		goto main1;
	}
	while (1) {
		if (password_count == 3) {
			break;
		}
		std::cout << "Please enter the password." << endl;
		std::cin >> password;
		if (check_password(get_card_insert_slot(), password)) {
			break;
		}
		cout << "Wrong Password" << endl;
		password_count++;
	}
	if (password_count == 3) {
		cout << "You got the password wrong 3 times. " << endl;
		goto end;//�μ� end
	}
	while (1) {
		std::cout << "What kind of work would you like to do? Please enter the number." << endl;
		std::cout << "1. deposit " << "2. withdrawl " << "3. transfer " << "4. end session:  ";
		std::cin >> work_type;
		if (work_type == 1) { deposit(); }
		else if (work_type == 2) { withdrawl(); }
		else if (work_type == 3) { transfer(); }
		else if (work_type == 4) { break; }
		else { std::cout << "You have selected the wrong menu." << endl; }
	}
end:
	cout << "Ends the session." << endl;
	print_session_history();
	decrease_session();
}



// multi_uni
void multi_uni_atm::transfer() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int money = 0;
	int money_sum = 0;
	int c1;
	int fee;
	bank* curr_bank = find_cardnumber_bank(get_card_insert_slot());
	long long int dest_account;
	bank* dest_bank;
	long long int src_account;
	bank* src_bank;
	string u_name = curr_bank->account_get_account(get_card_insert_slot())->get_user_name();

	while (1) {
		std::cout << "Please choose between 1 Cash transfer or 2 Account transfer : ";
		std::cin >> c1;
		if (c1 == 1) {       //���� ������ü
			std::cout << "Please enter the Destination Account : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			std::cout << "Please enter the cash to send: ";
			std::cin >> money;
			std::cout << "Please enter the amount you wish to send along with the 5,000 won fee." << endl;
			std::cout << "How many 1,000 won would you like to put in? ";
			std::cin >> one_thou;
			std::cout << "How many 5,000 won would you like to put in? ";
			std::cin >> five_thou;
			std::cout << "How many 10,000 won would you like to put in? ";
			std::cin >> ten_thou;
			std::cout << "How many 50,000 won would you like to put in?";
			std::cin >> fifty_thou;
			money_sum = one_thou * 1000 + five_thou * 5000 + ten_thou * 10000 + fifty_thou * 50000;
			if (money_sum != money + 5000) {
				std::cout << "The money you requested and the money you entered are different." << endl;
				return;
			}
			change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
			curr_bank->cash_transfer_bank(dest_bank, dest_account, money);
			std::cout << "Cash transfer has been completed." << endl;
			transaction_history* th = new transaction_history(get_card_insert_slot(), "cash_transfer", money, dest_bank, dest_bank->account_get_accountnum(dest_account));
			change_temp(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}

		else if (c1 == 2) {    // ���³��� ������ü
			std::cout << "Please enter the Destination Account : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			std::cout << "Enter your account number : ";
			std::cin >> src_account;
			src_bank = find_bank_by_account(src_account);
			if (!valid_find_bank_by_account(src_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			if (u_name != src_bank->account_get_accountnum(src_account)->get_user_name() or src_bank != curr_bank) {
				std::cout << "You cannot send money from this account. Please insert another bank card!" << endl;
				return;
			}

			if (src_bank == get_primary_bank() && dest_bank == get_primary_bank()) { fee = 2000; }
			else if ((src_bank != get_primary_bank() && dest_bank == get_primary_bank()) or (src_bank == get_primary_bank() && dest_bank != get_primary_bank())) { fee = 3000; }
			else if (src_bank != get_primary_bank() && dest_bank != get_primary_bank()) { fee = 4000; }

			std::cout << "Please enter the cash to send : ";
			std::cin >> money;
			if (!curr_bank->cn_enough_money_in_acc_bank(get_card_insert_slot(), money + fee)) {
				std::cout << "There is not enough money in the account.";
				return;
			}
			curr_bank->account_transfer_bank(src_account, dest_bank, dest_account, money, fee);
			std::cout << money << "+" << fee << " Won was deducted from your account." << endl;
			cout << "The account transfer has been completed." << endl << endl;
			transfer_acc_transaction_history* th = new transfer_acc_transaction_history(get_card_insert_slot(), "account_transfer", money, src_bank, dest_bank, src_bank->account_get_accountnum(src_account), dest_bank->account_get_accountnum(dest_account));
			change_transfer_temp(th);
			src_bank->account_get_accountnum(src_account)->change_account_tr(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}
		else {
			std::cout << "Choose between 1 and 2!!!" << endl;
			return;
		}
	}
}

void multi_uni_atm::deposit() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	long long int check_sum = 0;
	int withdrawl_count = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (b != get_primary_bank()) {
		std::cout << "A fee of 1,000 won will be charged for deposits to other banks. Deposit including fees" << endl;
	}
	while (true) {
		std::cout << "How many 1,000 won would you like to put in? (Enter -1 to exit)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "How many 5,000 won would you like to put in? ";
		std::cin >> five_thou;
		std::cout << "How many 1,0000 won would you like to put in? ";
		std::cin >> ten_thou;
		std::cout << "How many 5,0000 won would you like to put in? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "You have exceeded the number of cash cards available." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "How many checks would you like to write? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "You have exceeded the number of checks available." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 �϶� ����
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "th check, how much? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100�� �̻��� �Ұ�
				flag = 1; //flag = 1�̸� ���� �߻�
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "The check amount is incorrect." << endl;
		}
		else { break; }
	}
	change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
	long long int m = 1000 * one_thou + 5000 * five_thou + 10000 * ten_thou + 50000 * fifty_thou + check_sum;
	if (b != get_primary_bank()) { m = m - 1000; }
	b->deposit_bank(get_card_insert_slot(), m);
	std::cout << m << "Won has been deposited." << endl;
	std::cout << "The balance is " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(get_card_insert_slot(), "deposit", m, b, b->account_get_account(get_card_insert_slot()));
	change_temp(th);
	b->account_get_account(get_card_insert_slot())->change_account_tr(th);
	return;
}

void multi_uni_atm::withdrawl() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	int flag = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (get_number_of_withdrawl() >= get_limit_of_number_of_withdral()) {
		std::cout << "If you want to withdraw more, you must reset your session." << endl;
		return;
	}
	if (b != get_primary_bank()) {
		std::cout << "A fee of 2,000 won is charged for withdrawals from other banks." << endl;
		flag = 1;
	}
	else {
		std::cout << "There is a fee of 1,000 won for withdrawal." << endl;
	}
	while (1) {
		std::cout << "How much do you want to withdraw (excluding fees)(Enter -1 to exit) : ";
		std::cin >> check_num;
		if (check_num == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//��� ������ �ݾ��� �ʰ��Ͽ����� ����
			std::cout << "You have exceeded the amount you can withdraw." << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "The minimum unit of currency is 1,000 won." << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "There is not enough cash in the ATM." << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "There is not enough money in the account" << endl;
		}
		else {
			int m = check_num;
			fifty_thou = _MIN_((m / 50000), get_50000());
			m = m - 50000 * fifty_thou;
			ten_thou = _MIN_((m / 10000), get_10000());
			m = m - 10000 * ten_thou;
			five_thou = _MIN_((m / 5000), get_5000());
			m = m - 5000 * five_thou;
			one_thou = _MIN_((m / 1000), get_1000());
			m = m - 1000 * one_thou;

			if (m != 0) {
				std::cout << "There are not enough bills in the ATM" << endl;
			}
			else {
				//atm_cash ����
				change_atm_cash(-1 * (one_thou), -1 * (five_thou), -1 * (ten_thou), -1 * (fifty_thou));
				b->withdrawl_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "In the bank " << (flag ? (check_num + 2000) : (check_num + 1000)) << "won was withdrawn,  Total cash to bring is " << check_num << "Won." << endl;
				std::cout << "1000 Won " << one_thou << "sheets / " << "5000Won " << five_thou << "sheets / " << "10000 Won " << ten_thou << "sheets / " << "50000 Won " << fifty_thou << "sheets." << endl;
				std::cout << "The balance is " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
				set_number_of_withdrawl(get_number_of_withdrawl() + 1);
				transaction_history* th = new transaction_history(get_card_insert_slot(), "Withdrawl", check_num, b, b->account_get_account(get_card_insert_slot()));
				change_temp(th);
				b->account_get_account(get_card_insert_slot())->change_account_tr(th);
				return;
			}
		}
	}
}
void multi_uni_atm::interface() {
	increase_session();
	set_number_of_withdrawl(0);
	int password;
	int work_type;
	int password_count = 0;
	int number;//�߰�
main1:
	std::cout << "Please insert the card.(Please enter the card number). If you want to exit, enter -1.";
	std::cin >> number; //����
	set_card_insert_slot(number);//�߰�
	if (get_card_insert_slot() == -1) {
		return;
	}
	if (get_card_insert_slot() == 7777777) {
		admin_interface();
		goto end;
	}
	if (valid_cardnumber_account(get_card_insert_slot()) == 0) {
		cout << "Invalid card number." << endl;
		goto main1;
	}
	while (1) {
		if (password_count == 3) {
			break;
		}
		std::cout << "Please enter the password." << endl;
		std::cin >> password;
		if (check_password(get_card_insert_slot(), password)) {
			break;
			cout << "Wrong Password" << endl;
		}
		password_count++;
	}
	if (password_count == 3) {
		cout << "You got the password wrong 3 times. " << endl;
		goto end;//�μ� end
	}
	while (1) {
		std::cout << "What kind of work would you like to do? Please enter the number." << endl;
		std::cout << "1. deposit " << "2. withdrawl " << "3. transfer " << "4. end session:  ";
		std::cin >> work_type;
		if (work_type == 1) { deposit(); }
		else if (work_type == 2) { withdrawl(); }
		else if (work_type == 3) { transfer(); }
		else if (work_type == 4) { break; }
		else { std::cout << "You have selected the wrong menu." << endl; }
	}
end:
	cout << "Ends the session." << endl;
	print_session_history();
	decrease_session();
}





//���¼� ����

void multi_bi_atm::transfer() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int money = 0;
	int money_sum = 0;
	int c1;
	int fee;
	bank* curr_bank = find_cardnumber_bank(get_card_insert_slot());
	long long int dest_account;
	bank* dest_bank;
	long long int src_account;
	bank* src_bank;
	string u_name = curr_bank->account_get_account(get_card_insert_slot())->get_user_name();

	while (1) {
		std::cout << "Please choose between 1 Cash transfer or 2 Account transfer : ";
		std::cin >> c1;
		if (c1 == 1) {       //���� ������ü
			std::cout << "Please enter the Destination Account : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			std::cout << "Please enter the cash to send: ";
			std::cin >> money;
			std::cout << "Please enter the amount you wish to send along with the 5,000 won fee." << endl;
			std::cout << "How many 1,000 won would you like to put in? ";
			std::cin >> one_thou;
			std::cout << "How many 5,000 won would you like to put in? ";
			std::cin >> five_thou;
			std::cout << "How many 10,000 won would you like to put in? ";
			std::cin >> ten_thou;
			std::cout << "How many 50,000 won would you like to put in?";
			std::cin >> fifty_thou;
			money_sum = one_thou * 1000 + five_thou * 5000 + ten_thou * 10000 + fifty_thou * 50000;
			if (money_sum != money + 5000) {
				std::cout << "The money you requested and the money you entered are different." << endl;
				return;
			}
			change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
			curr_bank->cash_transfer_bank(dest_bank, dest_account, money);
			std::cout << "Cash transfer has been completed." << endl;
			transaction_history* th = new transaction_history(get_card_insert_slot(), "cash_transfer", money, dest_bank, dest_bank->account_get_accountnum(dest_account));
			change_temp(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}

		else if (c1 == 2) {    // ���³��� ������ü
			std::cout << "Please enter the Destination Account : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			std::cout << "Enter your account number : ";
			std::cin >> src_account;
			src_bank = find_bank_by_account(src_account);
			if (!valid_find_bank_by_account(src_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			if (u_name != src_bank->account_get_accountnum(src_account)->get_user_name() or src_bank != curr_bank) {
				std::cout << "You cannot send money from this account. Please insert another bank card!" << endl;
				return;
			}

			if (src_bank == get_primary_bank() && dest_bank == get_primary_bank()) { fee = 2000; }
			else if ((src_bank != get_primary_bank() && dest_bank == get_primary_bank()) or (src_bank == get_primary_bank() && dest_bank != get_primary_bank())) { fee = 3000; }
			else if (src_bank != get_primary_bank() && dest_bank != get_primary_bank()) { fee = 4000; }

			std::cout << "Please enter the cash to send : ";
			std::cin >> money;
			if (!curr_bank->cn_enough_money_in_acc_bank(get_card_insert_slot(), money + fee)) {
				std::cout << "There is not enough money in the account.";
				return;
			}
			curr_bank->account_transfer_bank(src_account, dest_bank, dest_account, money, fee);
			std::cout << money << "+" << fee << " Won was deducted from your account." << endl;
			cout << "The account transfer has been completed." << endl << endl;
			transfer_acc_transaction_history* th = new transfer_acc_transaction_history(get_card_insert_slot(), "account_transfer", money, src_bank, dest_bank, src_bank->account_get_accountnum(src_account), dest_bank->account_get_accountnum(dest_account));
			change_transfer_temp(th);
			src_bank->account_get_accountnum(src_account)->change_account_tr(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}
		else {
			std::cout << "Choose between 1 and 2!!!" << endl;
			return;
		}
	}
}

void multi_bi_atm::deposit() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	long long int check_sum = 0;
	int withdrawl_count = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (b != get_primary_bank()) {
		std::cout << "A fee of 1,000 won will be charged for deposits to other banks. Deposit including fees" << endl;
	}
	while (true) {
		std::cout << "How many 1,000 won would you like to put in? (Enter -1 to exit)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "How many 5,000 won would you like to put in? ";
		std::cin >> five_thou;
		std::cout << "How many 1,0000 won would you like to put in? ";
		std::cin >> ten_thou;
		std::cout << "How many 5,0000 won would you like to put in? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "You have exceeded the number of cash cards available." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "How many checks would you like to write? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "You have exceeded the number of checks available." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 �϶� ����
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "th check, how much? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100�� �̻��� �Ұ�
				flag = 1; //flag = 1�̸� ���� �߻�
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "The check amount is incorrect." << endl;
		}
		else { break; }
	}
	change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
	long long int m = 1000 * one_thou + 5000 * five_thou + 10000 * ten_thou + 50000 * fifty_thou + check_sum;
	if (b != get_primary_bank()) { m = m - 1000; }
	b->deposit_bank(get_card_insert_slot(), m);
	std::cout << m << "Won has been deposited." << endl;
	std::cout << "The balance is " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(get_card_insert_slot(), "deposit", m, b, b->account_get_account(get_card_insert_slot()));
	change_temp(th);
	b->account_get_account(get_card_insert_slot())->change_account_tr(th);
	return;
}

void multi_bi_atm::withdrawl() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	int flag = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (get_number_of_withdrawl() >= get_limit_of_number_of_withdral()) {
		std::cout << "If you want to withdraw more, you must reset your session." << endl;
		return;
	}
	if (b != get_primary_bank()) {
		std::cout << "A fee of 2,000 won is charged for withdrawals from other banks." << endl;
		flag = 1;
	}
	else {
		std::cout << "There is a fee of 1,000 won for withdrawal." << endl;
	}
	while (1) {
		std::cout << "How much do you want to withdraw (excluding fees)(Enter -1 to exit) : ";
		std::cin >> check_num;
		if (check_num == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//��� ������ �ݾ��� �ʰ��Ͽ����� ����
			std::cout << "You have exceeded the amount you can withdraw." << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "The minimum unit of currency is 1,000 won." << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "There is not enough cash in the ATM." << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "There is not enough money in the account" << endl;
		}
		else {
			int m = check_num;
			fifty_thou = _MIN_((m / 50000), get_50000());
			m = m - 50000 * fifty_thou;
			ten_thou = _MIN_((m / 10000), get_10000());
			m = m - 10000 * ten_thou;
			five_thou = _MIN_((m / 5000), get_5000());
			m = m - 5000 * five_thou;
			one_thou = _MIN_((m / 1000), get_1000());
			m = m - 1000 * one_thou;

			if (m != 0) {
				std::cout << "There are not enough bills in the ATM" << endl;
			}
			else {
				//atm_cash ����
				change_atm_cash(-1 * (one_thou), -1 * (five_thou), -1 * (ten_thou), -1 * (fifty_thou));
				b->withdrawl_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "In the bank " << (flag ? (check_num + 2000) : (check_num + 1000)) << "won was withdrawn,  Total cash to bring is " << check_num << "Won." << endl;
				std::cout << "1000 Won " << one_thou << "sheets / " << "5000Won " << five_thou << "sheets / " << "10000 Won " << ten_thou << "sheets / " << "50000 Won " << fifty_thou << "sheets." << endl;
				std::cout << "The balance is " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
				set_number_of_withdrawl(get_number_of_withdrawl() + 1);
				transaction_history* th = new transaction_history(get_card_insert_slot(), "Withdrawl", check_num, b, b->account_get_account(get_card_insert_slot()));
				change_temp(th);
				b->account_get_account(get_card_insert_slot())->change_account_tr(th);
				return;
			}
		}
	}
}

void multi_bi_atm::interface() {
	increase_session();
	set_number_of_withdrawl(0);
	int password;
	int work_type;
	int password_count = 0;
	int number;//�߰�
main1:
	std::cout << "Please insert the card.(Please enter the card number). If you want to exit, enter -1.";
	std::cin >> number; //����
	set_card_insert_slot(number);//�߰�
	if (get_card_insert_slot() == -1) {
		return;
	}
	if (get_card_insert_slot() == 7777777) {
		admin_interface();
		goto end;
	}
	if (valid_cardnumber_account(get_card_insert_slot()) == 0) {
		cout << "Invalid card number." << endl;
		goto main1;
	}
	while (1) {
		if (password_count == 3) {
			break;
		}
		std::cout << "Please enter the password." << endl;
		std::cin >> password;
		if (check_password(get_card_insert_slot(), password)) {
			break;
		}
		cout << "Wrong Password" << endl;
		password_count++;
	}
	if (password_count == 3) {
		cout << "You got the password wrong 3 times. " << endl;
		goto end;//�μ� end
	}
	while (1) {
		std::cout << "What kind of work would you like to do? Please enter the number." << endl;
		std::cout << "1. deposit " << "2. withdrawl " << "3. transfer " << "4. end session:  ";
		std::cin >> work_type;
		if (work_type == 1) { deposit(); }
		else if (work_type == 2) { withdrawl(); }
		else if (work_type == 3) { transfer(); }
		else if (work_type == 4) { break; }
		else { std::cout << "You have selected the wrong menu." << endl; }
	}
end:
	cout << "Ends the session." << endl;
	print_session_history();
	decrease_session();
}


void multi_bi_atm::koreantransfer() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int money = 0;
	int money_sum = 0;
	int c1;
	int fee;
	bank* curr_bank = find_cardnumber_bank(get_card_insert_slot());
	long long int dest_account;
	bank* dest_bank;
	long long int src_account;
	bank* src_bank;
	string u_name = curr_bank->account_get_account(get_card_insert_slot())->get_user_name();

	while (1) {
		std::cout << "��ȣ�� ���ÿ�. " << "1. ���� ��ü 2. ���� ��ü: ";
		std::cin >> c1;
		if (c1 == 1) {       //���� ������ü
			std::cout << "���� �ް��� �ϴ� ������ ���¹�ȣ�� �Է��Ͻÿ�. : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "����� �Է��� ���¹�ȣ�� �������� �ʽ��ϴ�." << endl;
				return;
			}
			std::cout << "���� �󸶳� �����ڽ��ϱ�? ";
			std::cin >> money;
			std::cout << "5000���� �����ᰡ ���, �����Ḧ �����ؼ� ������ �־��ּ���" << endl;
			std::cout << "1000�� �� �� �ְڽ��ϱ�? ";
			std::cin >> one_thou;
			std::cout << "5000�� �� �� �ְڽ��ϱ�? ";
			std::cin >> five_thou;
			std::cout << "10000�� �� �� �ְڽ��ϱ�? ";
			std::cin >> ten_thou;
			std::cout << "50000�� �� �� �ְڽ��ϱ�? ";
			std::cin >> fifty_thou;
			money_sum = one_thou * 1000 + five_thou * 5000 + ten_thou * 10000 + fifty_thou * 50000;
			if (money_sum != money + 5000) {
				std::cout << "��û�Ͻ� �ݾװ� �����Ͻ� ���� �׼��� �ٸ��ϴ�." << endl;
				return;
			}
			change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
			curr_bank->cash_transfer_bank(dest_bank, dest_account, money);
			std::cout << "��ü�� �Ϸ�Ǿ����ϴ�." << endl;
			transaction_history* th = new transaction_history(get_card_insert_slot(), "����_������ü", money, dest_bank, dest_bank->account_get_accountnum(dest_account));
			change_temp(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}

		else if (c1 == 2) {    // ���³��� ������ü
			std::cout << "���� ���� ������ ���¹�ȣ�� �Է��ϼ���: ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "�Է��Ͻ� ���¹�ȣ�� ���°� �������� �ʽ��ϴ�." << endl;
				return;
			}
			std::cout << "������ ���¹�ȣ�� �Է��ϼ���: ";
			std::cin >> src_account;
			src_bank = find_bank_by_account(src_account);
			if (!valid_find_bank_by_account(src_account)) {
				std::cout << "�Է��Ͻ� ���¹�ȣ�� ���°� �������� �ʽ��ϴ�." << endl;
				return;
			}
			if (u_name != src_bank->account_get_accountnum(src_account)->get_user_name() or src_bank != curr_bank) {
				std::cout << "�ش� ���¿��� ���� ��ü�� �� �����ϴ�." << endl;
				return;
			}

			if (src_bank == get_primary_bank() && dest_bank == get_primary_bank()) { fee = 2000; }
			else if ((src_bank != get_primary_bank() && dest_bank == get_primary_bank()) or (src_bank == get_primary_bank() && dest_bank != get_primary_bank())) { fee = 3000; }
			else if (src_bank != get_primary_bank() && dest_bank != get_primary_bank()) { fee = 4000; }

			std::cout << "�۱��Ͻ� �ݾ��� �˷��ּ���: ";
			std::cin >> money;
			if (!curr_bank->cn_enough_money_in_acc_bank(get_card_insert_slot(), money + fee)) {
				std::cout << "���¿� ���� ������� �ʽ��ϴ�.";
				return;
			}
			curr_bank->account_transfer_bank(src_account, dest_bank, dest_account, money, fee);
			std::cout << money << "+" << fee << " ���� ���¿��� ��ݵǾ����ϴ�." << endl;
			cout << "������ü�� �Ϸ�Ǿ����ϴ�." << endl << endl;
			transfer_acc_transaction_history* th = new transfer_acc_transaction_history(get_card_insert_slot(), "����_������ü", money, src_bank, dest_bank, src_bank->account_get_accountnum(src_account), dest_bank->account_get_accountnum(dest_account));
			change_transfer_temp(th);
			src_bank->account_get_accountnum(src_account)->change_account_tr(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}
		else {
			std::cout << "1�� 2�߿� ������." << endl;
			return;
		}
	}
}
void multi_bi_atm::koreanwithdrawl() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	int flag = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (get_number_of_withdrawl() >= get_limit_of_number_of_withdral()) {
		std::cout << "�� ����Ͻ÷��� ������ �ʱ�ȭ �ϼž� �մϴ�." << endl;
		return;
	}
	if (b != get_primary_bank()) {
		std::cout << "Ÿ ���� ��ݿ��� ������ 2000���� �߻��մϴ�." << endl;
		flag = 1;
	}
	else {
		std::cout << "�� ���� ��ݿ��� ������ 1000���� �߻��մϴ�." << endl;
	}
	while (1) {
		std::cout << "�󸶸� ����Ͻðڽ��ϱ�?(������ ����) (�����Ϸ��� -1�� �Է��ϼ���)";
		std::cin >> check_num;
		if (check_num == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//��� ������ �ݾ��� �ʰ��Ͽ����� ����
			std::cout << "��� �����Ͻ� �ݾ��� �ʰ��ϼ̽��ϴ�" << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "ȭ�� �ּ� ������ 1000�� �Դϴ�" << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "ATM�� ����� ������ �������� �ʽ��ϴ�" << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "���¿� ���� ������� �ʽ��ϴ�" << endl;
		}
		else {
			int m = check_num;
			fifty_thou = _MIN_((m / 50000), get_50000());
			m = m - 50000 * fifty_thou;
			ten_thou = _MIN_((m / 10000), get_10000());
			m = m - 10000 * ten_thou;
			five_thou = _MIN_((m / 5000), get_5000());
			m = m - 5000 * five_thou;
			one_thou = _MIN_((m / 1000), get_1000());
			m = m - 1000 * one_thou;

			if (m != 0) {
				std::cout << "ATM�� ������ ������ �����մϴ�" << endl;
			}
			else {
				//atm_cash ����
				change_atm_cash(-1 * (one_thou), -1 * (five_thou), -1 * (ten_thou), -1 * (fifty_thou));
				b->withdrawl_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "���࿡�� " << (flag ? (check_num + 2000) : (check_num + 1000)) << "���� ��ݵǾ��� �������� ������ �� " << check_num << "�� �Դϴ�." << endl;
				std::cout << "õ�� " << one_thou << "�� / " << "��õ�� " << five_thou << "�� / " << "���� " << ten_thou << "�� / " << "������ " << fifty_thou << "��" << endl;
				std::cout << "�ܾ��� " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
				set_number_of_withdrawl(get_number_of_withdrawl() + 1);
				transaction_history* th = new transaction_history(get_card_insert_slot(), "���", check_num, b, b->account_get_account(get_card_insert_slot()));
				change_temp(th);
				b->account_get_account(get_card_insert_slot())->change_account_tr(th);
				return;
			}
		}
	}
}
void multi_bi_atm::koreandeposit() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	long long int check_sum = 0;
	int withdrawl_count = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (b != get_primary_bank()) {
		std::cout << "Ÿ ���� �Աݿ��� ������ 1000���� �߻��մϴ�. �����Ḧ �����ؼ� �Ա��ϼ���" << endl;
	}
	while (true) {
		std::cout << "õ���� �� �� �����ðڽ��ϴϱ�? (�����Ϸ��� -1�� �Է��ϼ���)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "��õ���� �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> five_thou;
		std::cout << "������ �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> ten_thou;
		std::cout << "�������� �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "������ ���� �� ���� �ʰ��ϼ̽��ϴ�." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "��ǥ�� �� �� �����ðڽ��ϱ�? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "������ ��ǥ �� ���� �ʰ��ϼ̽��ϴ�." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 �϶� ����
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "��° ��ǥ�� �ݾ��� ���Դϱ�? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100�� �̻��� �Ұ�
				flag = 1; //flag = 1�̸� ���� �߻�
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "�߸��� ��ǥ �ݾ� �Դϴ�." << endl;
		}
		else { break; }
	}
	change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
	long long int m = 1000 * one_thou + 5000 * five_thou + 10000 * ten_thou + 50000 * fifty_thou + check_sum;
	if (b != get_primary_bank()) { m = m - 1000; }
	b->deposit_bank(get_card_insert_slot(), m);
	std::cout << m << "���� �ԱݵǾ����ϴ�." << endl;
	std::cout << "�ܾ��� " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(get_card_insert_slot(), "�Ա�", m, b, b->account_get_account(get_card_insert_slot()));
	change_temp(th);
	b->account_get_account(get_card_insert_slot())->change_account_tr(th);
	return;
}

void multi_bi_atm::korean_print_session_history() {
	for (int i = 0; i < get_temp().size(); i++) {
		get_temp()[i]->korean_print_history();
		change_atm_tr(get_temp()[i]);
	}
	clear_temp();//������ ã�ƺ��� ���� �ʱ�ȭ
}

void multi_bi_atm::koreaninterface() {
	increase_session();
	set_number_of_withdrawl(0);
	int password;
	int work_type;
	int password_count = 0;
	int number;//�߰�
main1:
	std::cout << "ī�带 �־��ּ���.(ī���ȣ�� �Է����ּ���). �����ϰ� �����ø� -1�� �Է����ּ���.";
	std::cin >> number; //����
	set_card_insert_slot(number);//�߰�
	if (get_card_insert_slot() == -1) {
		return;
	}
	if (get_card_insert_slot() == 7777777) {
		admin_interface_korean();
		goto end;
	}
	if (valid_cardnumber_account(get_card_insert_slot()) == false) {
		cout << "��ȿ���� ���� ī�� ��ȣ�Դϴ�. " << endl;
		goto main1;
	}
	while (1) {
		if (password_count == 3) {
			break;
		}
		std::cout << "��й�ȣ�� �Է����ּ���." << endl;
		std::cin >> password;
		if (check_password(get_card_insert_slot(), password)) {
			break;
		}
		cout << "��й�ȣ�� Ʋ���ϴ�." << endl;
		password_count++;
	}
	if (password_count == 3) {
		cout << "��й�ȣ�� 3�� Ʋ���̽��ϴ�. " << endl;
		goto end;//�μ� end
	}
	while (1) {
		std::cout << "� ������ �Ͻðڽ��ϱ�? ��ȣ�� �����Ͽ� �ּ���." << endl;
		std::cout << "1. �Ա� " << "2. ��� " << "3. ��ü " << "4. ���� ���� :  ";
		std::cin >> work_type;
		if (work_type == 1) { koreandeposit(); }
		else if (work_type == 2) { koreanwithdrawl(); }
		else if (work_type == 3) { koreantransfer(); }
		else if (work_type == 4) { break; }
		else { std::cout << "�߸��� �޴��� �����ϼ̽��ϴ�." << endl; }
	}
end:
	cout << "������ �����մϴ�." << endl;
	korean_print_session_history();
	decrease_session();
}




//single bi atm

void single_bi_atm::transfer() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int money = 0;
	int money_sum = 0;
	int c1;
	int fee;
	bank* curr_bank = find_cardnumber_bank(get_card_insert_slot());
	long long int dest_account;
	bank* dest_bank;
	long long int src_account;
	bank* src_bank;
	string u_name = curr_bank->account_get_account(get_card_insert_slot())->get_user_name();

	while (1) {
		std::cout << "Please choose between 1 Cash transfer or 2 Account transfer : ";
		std::cin >> c1;
		if (c1 == 1) {       //���� ������ü
			std::cout << "Please enter the Destination Account : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (dest_bank != get_primary_bank()) {
				cout << "This is not primary bank account." << endl;
				return;
			}
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			std::cout << "Please enter the cash to send: ";
			std::cin >> money;
			std::cout << "Please enter the amount you wish to send along with the 5,000 won fee." << endl;
			std::cout << "How many 1,000 won would you like to put in? ";
			std::cin >> one_thou;
			std::cout << "How many 5,000 won would you like to put in? ";
			std::cin >> five_thou;
			std::cout << "How many 10,000 won would you like to put in? ";
			std::cin >> ten_thou;
			std::cout << "How many 50,000 won would you like to put in?";
			std::cin >> fifty_thou;
			money_sum = one_thou * 1000 + five_thou * 5000 + ten_thou * 10000 + fifty_thou * 50000;
			if (money_sum != money + 5000) {
				std::cout << "The money you requested and the money you entered are different." << endl;
				return;
			}
			change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
			curr_bank->cash_transfer_bank(dest_bank, dest_account, money);
			std::cout << "Cash transfer has been completed." << endl;
			transaction_history* th = new transaction_history(get_card_insert_slot(), "cash_transfer", money, dest_bank, dest_bank->account_get_accountnum(dest_account));
			change_temp(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}

		else if (c1 == 2) {    // ���³��� ������ü
			std::cout << "Please enter the Destination Account : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			if (dest_bank != get_primary_bank()) {
				cout << "This is not primary bank account." << endl;
				return;
			}
			std::cout << "Enter your account number : ";
			std::cin >> src_account;
			src_bank = find_bank_by_account(src_account);
			if (!valid_find_bank_by_account(src_account)) {
				std::cout << "There is no account you entered." << endl;
				return;
			}
			if (src_bank != get_primary_bank()) {
				cout << "This is not primary bank account." << endl;
				return;
			}
			if (u_name != src_bank->account_get_accountnum(src_account)->get_user_name() or src_bank != curr_bank) {
				std::cout << "You cannot send money from this account. Please insert another bank card!" << endl;
				return;
			}

			if (src_bank == get_primary_bank() && dest_bank == get_primary_bank()) { fee = 2000; }
			else if ((src_bank != get_primary_bank() && dest_bank == get_primary_bank()) or (src_bank == get_primary_bank() && dest_bank != get_primary_bank())) { fee = 3000; }
			else if (src_bank != get_primary_bank() && dest_bank != get_primary_bank()) { fee = 4000; }

			std::cout << "Please enter the cash to send : ";
			std::cin >> money;
			if (!curr_bank->cn_enough_money_in_acc_bank(get_card_insert_slot(), money + fee)) {
				std::cout << "There is not enough money in the account.";
				return;
			}
			curr_bank->account_transfer_bank(src_account, dest_bank, dest_account, money, fee);
			std::cout << money << "+" << fee << " Won was deducted from your account." << endl;
			cout << "The account transfer has been completed." << endl << endl;
			transfer_acc_transaction_history* th = new transfer_acc_transaction_history(get_card_insert_slot(), "account_transfer", money, src_bank, dest_bank, src_bank->account_get_accountnum(src_account), dest_bank->account_get_accountnum(dest_account));
			change_transfer_temp(th);
			src_bank->account_get_accountnum(src_account)->change_account_tr(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}
		else {
			std::cout << "Choose between 1 and 2!!!" << endl;
			return;
		}
	}
}

void single_bi_atm::deposit() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	long long int check_sum = 0;
	int withdrawl_count = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (b != get_primary_bank()) {
		std::cout << "A fee of 1,000 won will be charged for deposits to other banks. Deposit including fees" << endl;
	}
	while (true) {
		std::cout << "How many 1,000 won would you like to put in? (Enter -1 to exit)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "How many 5,000 won would you like to put in? ";
		std::cin >> five_thou;
		std::cout << "How many 1,0000 won would you like to put in? ";
		std::cin >> ten_thou;
		std::cout << "How many 5,0000 won would you like to put in? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "You have exceeded the number of cash cards available." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "How many checks would you like to write? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "You have exceeded the number of checks available." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 �϶� ����
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "th check, how much? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100�� �̻��� �Ұ�
				flag = 1; //flag = 1�̸� ���� �߻�
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "The check amount is incorrect." << endl;
		}
		else { break; }
	}
	change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
	long long int m = 1000 * one_thou + 5000 * five_thou + 10000 * ten_thou + 50000 * fifty_thou + check_sum;
	if (b != get_primary_bank()) { m = m - 1000; }
	b->deposit_bank(get_card_insert_slot(), m);
	std::cout << m << "Won has been deposited." << endl;
	std::cout << "The balance is " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(get_card_insert_slot(), "deposit", m, b, b->account_get_account(get_card_insert_slot()));
	change_temp(th);
	b->account_get_account(get_card_insert_slot())->change_account_tr(th);
	return;
}

void single_bi_atm::withdrawl() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	int flag = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (get_number_of_withdrawl() >= get_limit_of_number_of_withdral()) {
		std::cout << "If you want to withdraw more, you must reset your session." << endl;
		return;
	}
	if (b != get_primary_bank()) {
		std::cout << "A fee of 2,000 won is charged for withdrawals from other banks." << endl;
		flag = 1;
	}
	else {
		std::cout << "There is a fee of 1,000 won for withdrawal." << endl;
	}
	while (1) {
		std::cout << "How much do you want to withdraw (excluding fees)(Enter -1 to exit) : ";
		std::cin >> check_num;
		if (check_num == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//��� ������ �ݾ��� �ʰ��Ͽ����� ����
			std::cout << "You have exceeded the amount you can withdraw." << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "The minimum unit of currency is 1,000 won." << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "There is not enough cash in the ATM." << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "There is not enough money in the account" << endl;
		}
		else {
			int m = check_num;
			fifty_thou = _MIN_((m / 50000), get_50000());
			m = m - 50000 * fifty_thou;
			ten_thou = _MIN_((m / 10000), get_10000());
			m = m - 10000 * ten_thou;
			five_thou = _MIN_((m / 5000), get_5000());
			m = m - 5000 * five_thou;
			one_thou = _MIN_((m / 1000), get_1000());
			m = m - 1000 * one_thou;

			if (m != 0) {
				std::cout << "There are not enough bills in the ATM" << endl;
			}
			else {
				//atm_cash ����
				change_atm_cash(-1 * (one_thou), -1 * (five_thou), -1 * (ten_thou), -1 * (fifty_thou));
				b->withdrawl_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "In the bank " << (flag ? (check_num + 2000) : (check_num + 1000)) << "won was withdrawn,  Total cash to bring is " << check_num << "Won." << endl;
				std::cout << "1000 Won " << one_thou << "sheets / " << "5000Won " << five_thou << "sheets / " << "10000 Won " << ten_thou << "sheets / " << "50000 Won " << fifty_thou << "sheets." << endl;
				std::cout << "The balance is " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
				set_number_of_withdrawl(get_number_of_withdrawl() + 1);
				transaction_history* th = new transaction_history(get_card_insert_slot(), "Withdrawl", check_num, b, b->account_get_account(get_card_insert_slot()));
				change_temp(th);
				b->account_get_account(get_card_insert_slot())->change_account_tr(th);
				return;
			}
		}
	}
}
void single_bi_atm::korean_print_session_history() {
	for (int i = 0; i < get_temp().size(); i++) {
		get_temp()[i]->korean_print_history();
		change_atm_tr(get_temp()[i]);
	}
	clear_temp();//������ ã�ƺ��� ���� �ʱ�ȭ
}





void single_bi_atm::interface() {
	increase_session();
	set_number_of_withdrawl(0);
	int password;
	int work_type;
	int password_count = 0;
	int number;//�߰�
main1:
	std::cout << "Please insert the card.(Please enter the card number). If you want to exit, enter -1.";
	std::cin >> number; //����
	set_card_insert_slot(number);//�߰�
	if (get_card_insert_slot() == -1) {
		return;
	}
	if (get_card_insert_slot() == 7777777) {
		admin_interface();
		goto end;
	}

	if (find_cardnumber_bank(get_card_insert_slot()) != get_primary_bank()) {
		cout << " This is not primary bank account." << endl;
		return;
	}
	if (valid_cardnumber_account(get_card_insert_slot()) == 0) {
		cout << "Invalid card number." << endl;
		goto main1;
	}
	while (1) {
		if (password_count == 3) {
			break;
		}
		std::cout << "Please enter the password." << endl;
		std::cin >> password;
		if (check_password(get_card_insert_slot(), password)) {
			break;
		}
		cout << "Wrong Password" << endl;
		password_count++;
	}
	if (password_count == 3) {
		cout << "You got the password wrong 3 times. " << endl;
		goto end;//�μ� end
	}
	while (1) {
		std::cout << "What kind of work would you like to do? Please enter the number." << endl;
		std::cout << "1. deposit " << "2. withdrawl " << "3. transfer " << "4. end session:  ";
		std::cin >> work_type;
		if (work_type == 1) { deposit(); }
		else if (work_type == 2) { withdrawl(); }
		else if (work_type == 3) { transfer(); }
		else if (work_type == 4) { break; }
		else { std::cout << "You have selected the wrong menu." << endl; }
	}
end:
	cout << "Ends the session." << endl;
	print_session_history();
	decrease_session();
}


void single_bi_atm::koreantransfer() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int money = 0;
	int money_sum = 0;
	int c1;
	int fee;
	bank* curr_bank = find_cardnumber_bank(get_card_insert_slot());
	long long int dest_account;
	bank* dest_bank;
	long long int src_account;
	bank* src_bank;
	string u_name = curr_bank->account_get_account(get_card_insert_slot())->get_user_name();

	while (1) {
		std::cout << "��ȣ�� ���ÿ�. " << "1. ���� ��ü 2. ���� ��ü: ";
		std::cin >> c1;
		if (c1 == 1) {       //���� ������ü
			std::cout << "���� �ް��� �ϴ� ������ ���¹�ȣ�� �Է��Ͻÿ�. : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (dest_bank != get_primary_bank()) {
				cout << "�� ������ �� ���� ���°� �ƴմϴ�." << endl;
				return;
			}
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "����� �Է��� ���¹�ȣ�� �������� �ʽ��ϴ�." << endl;
				return;
			}
			std::cout << "���� �󸶳� �����ðڽ��ϱ�? " << endl;
			std::cin >> money;
			std::cout << "5000���� �����ᰡ ���, �����Ḧ �����ؼ� ������ �־��ּ���" << endl;
			std::cout << "1000�� �� �� �ְڽ��ϱ�? ";
			std::cin >> one_thou;
			std::cout << "5000�� �� �� �ְڽ��ϱ�? ";
			std::cin >> five_thou;
			std::cout << "10000�� �� �� �ְڽ��ϱ�? ";
			std::cin >> ten_thou;
			std::cout << "50000�� �� �� �ְڽ��ϱ�? ";
			std::cin >> fifty_thou;
			money_sum = one_thou * 1000 + five_thou * 5000 + ten_thou * 10000 + fifty_thou * 50000;
			if (money_sum != money + 5000) {
				std::cout << "��û�Ͻ� �ݾװ� �����Ͻ� ���� �׼��� �ٸ��ϴ�." << endl;
				return;
			}
			change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
			curr_bank->cash_transfer_bank(dest_bank, dest_account, money);
			std::cout << "��ü�� �Ϸ�Ǿ����ϴ�." << endl;
			transaction_history* th = new transaction_history(get_card_insert_slot(), "����_������ü", money, dest_bank, dest_bank->account_get_accountnum(dest_account));
			change_temp(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}

		else if (c1 == 2) {    // ���³��� ������ü
			std::cout << "���� ���� ������ ���¹�ȣ�� �Է��ϼ���: ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "�Է��Ͻ� ���¹�ȣ�� ���°� �������� �ʽ��ϴ�." << endl;
				return;
			}
			if (dest_bank != get_primary_bank()) {
				cout << "�� ���� ���°� �ƴմϴ�." << endl;
				return;
			}
			std::cout << "������ ���¹�ȣ�� �Է��ϼ���: ";
			std::cin >> src_account;
			src_bank = find_bank_by_account(src_account);
			if (!valid_find_bank_by_account(src_account)) {
				std::cout << "�Է��Ͻ� ���¹�ȣ�� ���°� �������� �ʽ��ϴ�." << endl;
				return;
			}
			if (src_bank != get_primary_bank()) {
				cout << "�ְ��� ������ �ƴմϴ�." << endl;
				return;
			}
			if (u_name != src_bank->account_get_accountnum(src_account)->get_user_name() or src_bank != curr_bank) {
				std::cout << "�ش� ���¿��� ���� ��ü�� �� �����ϴ�." << endl;
				return;
			}

			if (src_bank == get_primary_bank() && dest_bank == get_primary_bank()) { fee = 2000; }
			else if ((src_bank != get_primary_bank() && dest_bank == get_primary_bank()) or (src_bank == get_primary_bank() && dest_bank != get_primary_bank())) { fee = 3000; }
			else if (src_bank != get_primary_bank() && dest_bank != get_primary_bank()) { fee = 4000; }

			std::cout << "�۱��Ͻ� �ݾ��� �˷��ּ���: ";
			std::cin >> money;
			if (!curr_bank->cn_enough_money_in_acc_bank(get_card_insert_slot(), money + fee)) {
				std::cout << "���¿� ���� ������� �ʽ��ϴ�.";
				return;
			}
			curr_bank->account_transfer_bank(src_account, dest_bank, dest_account, money, fee);
			std::cout << money << "+" << fee << " ���� ���¿��� ��ݵǾ����ϴ�." << endl;
			cout << "������ü�� �Ϸ�Ǿ����ϴ�." << endl << endl;
			transfer_acc_transaction_history* th = new transfer_acc_transaction_history(get_card_insert_slot(), "����_������ü", money, src_bank, dest_bank, src_bank->account_get_accountnum(src_account), dest_bank->account_get_accountnum(dest_account));
			change_transfer_temp(th);
			src_bank->account_get_accountnum(src_account)->change_account_tr(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}
		else {
			std::cout << "1�� 2�߿� ������." << endl;
			return;
		}
	}
}
void single_bi_atm::koreanwithdrawl() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	int flag = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (get_number_of_withdrawl() >= get_limit_of_number_of_withdral()) {
		std::cout << "�� ����Ͻ÷��� ������ �ʱ�ȭ �ϼž� �մϴ�." << endl;
		return;
	}
	if (b != get_primary_bank()) {
		std::cout << "Ÿ ���� ��ݿ��� ������ 2000���� �߻��մϴ�." << endl;
		flag = 1;
	}
	else {
		std::cout << "�� ���� ��ݿ��� ������ 1000���� �߻��մϴ�." << endl;
	}
	while (1) {
		std::cout << "�󸶸� ����Ͻðڽ��ϱ�?(������ ����) (�����Ϸ��� -1�� �Է��ϼ���)";
		std::cin >> check_num;
		if (check_num == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//��� ������ �ݾ��� �ʰ��Ͽ����� ����
			std::cout << "��� �����Ͻ� �ݾ��� �ʰ��ϼ̽��ϴ�" << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "ȭ�� �ּ� ������ 1000�� �Դϴ�" << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "ATM�� ����� ������ �������� �ʽ��ϴ�" << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "���¿� ���� ������� �ʽ��ϴ�" << endl;
		}
		else {
			int m = check_num;
			fifty_thou = _MIN_((m / 50000), get_50000());
			m = m - 50000 * fifty_thou;
			ten_thou = _MIN_((m / 10000), get_10000());
			m = m - 10000 * ten_thou;
			five_thou = _MIN_((m / 5000), get_5000());
			m = m - 5000 * five_thou;
			one_thou = _MIN_((m / 1000), get_1000());
			m = m - 1000 * one_thou;

			if (m != 0) {
				std::cout << "ATM�� ������ ������ �����մϴ�" << endl;
			}
			else {
				//atm_cash ����
				change_atm_cash(-1 * (one_thou), -1 * (five_thou), -1 * (ten_thou), -1 * (fifty_thou));
				b->withdrawl_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "���࿡�� " << (flag ? (check_num + 2000) : (check_num + 1000)) << "���� ��ݵǾ��� �������� ������ �� " << check_num << "�� �Դϴ�." << endl;
				std::cout << "õ�� " << one_thou << "�� / " << "��õ�� " << five_thou << "�� / " << "���� " << ten_thou << "�� / " << "������ " << fifty_thou << "��" << endl;
				std::cout << "�ܾ��� " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
				set_number_of_withdrawl(get_number_of_withdrawl() + 1);
				transaction_history* th = new transaction_history(get_card_insert_slot(), "���", check_num, b, b->account_get_account(get_card_insert_slot()));
				change_temp(th);
				b->account_get_account(get_card_insert_slot())->change_account_tr(th);
				return;
			}
		}
	}
}
void single_bi_atm::koreandeposit() {
	int one_thou = 0;
	int five_thou = 0;
	int ten_thou = 0;
	int fifty_thou = 0;
	int check_num = 0;
	long long int check_sum = 0;
	int withdrawl_count = 0;
	bank* b = find_cardnumber_bank(get_card_insert_slot());
	if (b != get_primary_bank()) {
		std::cout << "Ÿ ���� �Աݿ��� ������ 1000���� �߻��մϴ�. �����Ḧ �����ؼ� �Ա��ϼ���" << endl;
	}
	while (true) {
		std::cout << "õ���� �� �� �����ðڽ��ϴϱ�? (�����Ϸ��� -1�� �Է��ϼ���)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "��õ���� �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> five_thou;
		std::cout << "������ �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> ten_thou;
		std::cout << "�������� �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "������ ���� �� ���� �ʰ��ϼ̽��ϴ�." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "��ǥ�� �� �� �����ðڽ��ϱ�? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "������ ��ǥ �� ���� �ʰ��ϼ̽��ϴ�." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 �϶� ����
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "��° ��ǥ�� �ݾ��� ���Դϱ�? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100�� �̻��� �Ұ�
				flag = 1; //flag = 1�̸� ���� �߻�
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "�߸��� ��ǥ �ݾ� �Դϴ�." << endl;
		}
		else { break; }
	}
	change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
	long long int m = 1000 * one_thou + 5000 * five_thou + 10000 * ten_thou + 50000 * fifty_thou + check_sum;
	if (b != get_primary_bank()) { m = m - 1000; }
	b->deposit_bank(get_card_insert_slot(), m);
	std::cout << m << "���� �ԱݵǾ����ϴ�." << endl;
	std::cout << "�ܾ��� " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(get_card_insert_slot(), "�Ա�", m, b, b->account_get_account(get_card_insert_slot()));
	change_temp(th);
	b->account_get_account(get_card_insert_slot())->change_account_tr(th);
	return;
}

void single_bi_atm::koreaninterface() {
	increase_session();
	set_number_of_withdrawl(0);
	int password;
	int work_type;
	int password_count = 0;
	int number;//�߰�
main1:
	std::cout << "ī�带 �־��ּ���.(ī���ȣ�� �Է����ּ���). �����ϰ� �����ø� -1�� �Է����ּ���.";
	std::cin >> number; //����
	set_card_insert_slot(number);//�߰�
	if (get_card_insert_slot() == -1) {
		return;
	}
	if (get_card_insert_slot() == 7777777) {
		admin_interface_korean();
		goto end;
	}
	if (find_cardnumber_bank(get_card_insert_slot()) != get_primary_bank()) {                                       // �������� �߰��� �κ�(������ Ȯ���ʿ�)
		cout << " ������ ���°� �ƴմϴ�." << endl;
		return;
	}
	if (valid_cardnumber_account(get_card_insert_slot()) == 0) {
		cout << "��ȿ���� ���� ī�� ��ȣ�Դϴ�. " << endl;
		goto main1;
	}
	while (1) {
		if (password_count == 3) {
			break;
		}
		std::cout << "��й�ȣ�� �Է����ּ���." << endl;
		std::cin >> password;
		if (check_password(get_card_insert_slot(), password)) {
			break;
		}
		cout << "��й�ȣ�� Ʋ���ϴ�." << endl;
		password_count++;
	}
	if (password_count == 3) {
		cout << "��й�ȣ�� 3�� Ʋ���̽��ϴ�. " << endl;
		goto end;//�μ� end
	}
	while (1) {
		std::cout << "� ������ �Ͻðڽ��ϱ�? ��ȣ�� �����Ͽ� �ּ���." << endl;
		std::cout << "1. �Ա� " << "2. ��� " << "3. ��ü " << "4. ���� ���� :  ";
		std::cin >> work_type;
		if (work_type == 1) { koreandeposit(); }
		else if (work_type == 2) { koreanwithdrawl(); }
		else if (work_type == 3) { koreantransfer(); }
		else if (work_type == 4) { break; }
		else { std::cout << "�߸��� �޴��� �����ϼ̽��ϴ�." << endl; }
	}
end:
	cout << "������ �����մϴ�." << endl;
	korean_print_session_history();
	decrease_session();
}


bool check_atm_serial_number(int number) { // atm serial number ����� �� �ִ��� Ȯ���ϴ� �Լ�
	int size = atmlist.size();
	for (int i = 0; i < size; i++) {
		if (number == atmlist[i]->get_serial_number()) {
			return false; //�̹� �����ϸ� false
		}
	}
	return true; //�������������� ����Ҽ� ������ true
}

int main() {
	int number; // ����
	int size; // ����
	int banknumber; // ���� ��� �����ߴ���.
	string bank_name; // ���� �̸�
	string user_name; // ���� �̸�
	int initial_money = 0; // �� �ʱ�ȭ
	int password; //��й�ȣ
	int type; //Ÿ�� ��ȣ
	int language_type;// ��� Ÿ�� ��ȣ
	int cardnumber; //ī�� ��ȣ
	int password_count = 0;//��й�ȣ�� �󸶳� Ʋ�ȴ���.
	long long int accountnumber;
	int count = 0;
	int number_of_atm;
	int test_num;
	int atm_serial_number;
main1:	//���� 1 ���ุ��� or ���� �����ϱ�
	std::cout << "� ������ �Ͻðڽ��ϱ�? ��ȣ�� �����Ͽ� �ּ���." << endl;
	std::cout << "1. ���� �����" << " " << "2. ���� �����ϱ�" << " " << "3. ���� ����ϱ� :  ";
	std::cin >> number;
	if (number == 1) {
		std::cout << "<���� �����>" << endl;
		std::cout << "���� �̸��� �����Ͽ� �ּ���. ���� ȭ������ ���ư����� �ϸ� back�� �Է����ּ��� :  ";
		std::cin >> bank_name;
		if (bank_name == "back") { goto main1; } // ���� ȭ�� ���ư���
		else {
			new bank(bank_name);
			goto main1;
		}
	}
	else if (number == 2) {
		std::cout << "<���� �����ϱ�>" << endl;
		std::cout << "������ �ϴ� ���� ��ȣ�� �Է����ּ���. ���� ȭ������ ���ư����� �ϸ� -1�� �Է����ּ���." << endl;
		size = banklist.size();
		for (int i = 0; i < size; i++) {
			std::cout << i << "." << banklist[i]->get_bankname() << " "; // ex) 0. �뱸����
		}
		std::cout << endl;
	sub_main_1_2:
		std::cin >> banknumber; // banklist�� index�� �Է� ���� ����.
		if (banknumber == -1) { goto main1; } // ���� ȭ�� ���ư���
		else if (0 <= banknumber and banknumber < size) {
			goto main2;
		}
		else {
			cout << "�߸��� ��ȣ�� �Է��ϼ̽��ϴ�. �ٽ� �Է����ּ���. " << endl;
			goto sub_main_1_2;
		} //
	}
	else if (number == 3) {
		show_information(); goto main1;
	}
	else {
		std::cout << "��ȣ�� �߸� �Է��ϼ̽��ϴ�." << endl;
		goto main1;
	}
main2: // ���� ���� ����� or  ATM ����� or ATM �����ϱ�
	std::cout << "� ������ �Ͻðڽ��ϱ�? ��ȣ�� �����Ͽ� �ּ���." << endl;
	std::cout << "1. ���� ���� �����  " << "2. ATM �����  " << "3. ATM �̿��ϱ�  " << "4. ���� ����ϱ�  " << "5. ���� ���� ����" << "6. ���� ȭ������ ���ư���  :  ";
	std::cin >> number;
	if (number == 1) {
	sub_main2_0://���� ���� �����
		std::cout << "<���� ���� �����>" << endl;
		std::cout << "����� �̸��� �Է����ּ��� :  ";
		std::cin >> user_name;
		std::cout << "���¹�ȣ�� �Է����ּ��� : ";
		std::cin >> accountnumber;
		if ((accountnumber < 99999999999) or (accountnumber > 1000000000000)) {
			cout << "���¹�ȣ ����� �߸��Ǿ����ϴ�. 12�ڸ��� �Է����ּ���." << endl;
			goto sub_main2_0;
		}
		for (int i = 0; i < banklist.size(); i++) {
			if (banklist[i]->bool_accountnumber_in_bank(accountnumber) == true) {
				cout << "�̹� �ش� ���¹�ȣ�� �����մϴ�! " << endl;
				goto sub_main2_0;
			}
		}
		//������ banklist[banknumber] ��.
		std::cout << "���¿� ������ ���� �������� �Է����ּ���  :  ";
		std::cin >> initial_money;
		std::cout << "������ ��й�ȣ�� �������� ���� �Է����ּ���  :  ";
		std::cin >> password;
		banklist[banknumber]->make_account(accountnumber, user_name, password, initial_money);
		goto main2;
	}
	else if (number == 2) { //ATM �����
		std::cout << "<ATM �����>" << endl;
	sub_main_2_1:
		std::cout << "ATM�� SERIAL NUMBER�� �Է����ּ���  ";
		cin >> atm_serial_number;
		if (atm_serial_number < 100000 or atm_serial_number>999999) {
			cout << "�߸��� ATM SERIAL NUMBER�� �����Դϴ�. 6�ڸ��� �Է����ּ���." << endl;
			goto sub_main_2_1;
		}
		if (!check_atm_serial_number(atm_serial_number)) {
			cout << "�̹� �����ϴ� ATM SERIAL NUMBER �Դϴ�. �ٽ� �Է����ּ���." << endl;
			goto sub_main_2_1;
		}
	sub_main_2_2:
		std::cout << "ATM type�� �ش��ϴ� ��ȣ�� �Է����ּ���" << endl;
		std::cout << "0. Single Bank ATM ";
		std::cout << "1. Multi-Bank ATM   ";
		std::cin >> type;
		if (type != 1 and type != 0) {
			cout << "�߸��� ��ȣ�Դϴ�." << endl;
			goto sub_main_2_2;
		}
	sub_main_2_3:
		std::cout << "ATM�� ��� ���� ���ο� �ش��ϴ� ��ȣ�� �Է����ּ���" << endl;
		std::cout << "0. Unilingual ";
		std::cout << "1. Bilingual  ";
		std::cin >> language_type;
		if (language_type != 1 and language_type != 0) {
			cout << "�߸��� ��ȣ�Դϴ�." << endl;
			goto sub_main_2_3;
		}
		std::cout << "���� �󸶳� ����ְ� �ұ��?" << endl;
		int one_thou = 0;
		int five_thou = 0;
		int ten_thou = 0;
		int fifty_thou = 0;
		int check_num = 0;
		std::cout << "õ���� �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> one_thou;
		std::cout << "��õ���� �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> five_thou;
		std::cout << "������ �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> ten_thou;
		std::cout << "�������� �� �� �����ðڽ��ϴϱ�? ";
		std::cin >> fifty_thou;
		if (type == 1 && language_type == 1)  //multi_bi	
			banklist[banknumber]->make_multi_bi_atm(1, 1, one_thou, five_thou, ten_thou, fifty_thou, atm_serial_number);
		else if (type == 1 && language_type == 0)  //multi_uni	
			banklist[banknumber]->make_multi_uni_atm(1, 0, one_thou, five_thou, ten_thou, fifty_thou, atm_serial_number);
		else if (type == 0 && language_type == 1)  //single_bi	
			banklist[banknumber]->make_single_bi_atm(0, 1, one_thou, five_thou, ten_thou, fifty_thou, atm_serial_number);
		else if (type == 0 && language_type == 0)  //single_uni	
			banklist[banknumber]->make_single_uni_atm(0, 0, one_thou, five_thou, ten_thou, fifty_thou, atm_serial_number);
		goto main2;
	}
	else if (number == 3) {// ATM �����ϱ�
		// atm �����ϱ�. � atm�� �����Ͻðڽ��ϱ�? ���� �ʿ�.
	sub_main2:
		std::cout << "<ATM �����ϱ�>" << endl;
		std::cout << "�����ϰ� ���� atm�� ��ȣ�� �������ּ���. �������� ���� �����ø� -1�� �Է����ּ���." << endl;
		size = banklist[banknumber]->get_atm_list().size();
		for (int i = 0; i < size; i++)
		{
			cout << i << ". atm serial number : " << banklist[banknumber]->get_atm_list()[i]->get_serial_number() << endl;
		}
		cin >> number_of_atm;
		if (number_of_atm == -1) {
			goto main2;
		}


		if (number_of_atm >= 0 and size > number_of_atm) {
			goto main3;
		}
		else {
			cout << "�߸��� ATM ��ȣ�Դϴ�. �ٽ� �Է����ּ���." << endl;
			goto sub_main2;
		}

	}
	else if (number == 4) {
		show_information(); goto main2;
	}
	else if (number == 5) {
		std::cout << "���¹�ȣ�� �Է����ּ��� : ";
		std::cin >> accountnumber;
		if (banklist[banknumber]->bool_accountnumber_in_bank(accountnumber)) {
			banklist[banknumber]->account_get_accountnum(accountnumber)->korean_print_account_history();
		}
		else {
			cout << "���¹�ȣ�� ���࿡ �������� �ʽ��ϴ�." << endl;
		}
		goto main2;
	}
	else if (number == 6) {
		goto main1;
	}
	else {
		cout << "��ȣ�� �߸� �Է��ϼ̽��ϴ�." << endl;
		goto main2;
	}
main3://ATM �̿��ϱ�
	(banklist[banknumber]->get_atm_list())[number_of_atm]->mini_interface();
	goto main1;
}