#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#define _MAX_(x, y) ((x > y) ? x : y) //MAX 함수
#define _MIN_(x, y) ((x < y) ? x : y) //MIN 함수
using namespace std;

class bank;
class atm;
class account;

vector <bank*> banklist; //bank list global variable
vector <account*> accountlist; // account list global variable
vector <atm*> atmlist; //atm list global variable


class card { // card 클래스 //    계좌가 만들어질 때 자동으로 일반용 construct 불러야한다. atm이 만들어질때마다 admin 카드번호가 불러져야 되고.>> 강승수 구현함
private:
	bool admin; // admin 인지 아닌지.
	bank* card_bank; // 카드가 어느 bank 인지
	int card_num; // 카드 번호
	atm* mas_atm; // 관리자 역할을 할 수 있는 atm
	static int static_card_number;
public:
	card(bank* b, atm* master_atm); // admin 용 constructor 
	card(bank* b); // 일반용 constructor
	bool get_ad() { return admin; } // admin인지 아닌지
	bank* get_bank() { return card_bank; } // bank 주소 받아 오는거 
	int get_cardnum() { return card_num; } // card number 받아 오는거
};

int card::static_card_number = 1000;


class bank { // 은행
private:
	string bankname; // 은행 이름
	vector<account*> user_acc; // 계좌들을 담아 놓은 vector
	account* curr_account; // 현재 은행내에서 사용하고 있는 계좌 주소
	account* src_account; // 계좌 이체시 사용할 주소(꼭 이 은행일 필요 없음!)
	account* dest_account; // 돈을 보낼 계좌 주소
	vector <atm*> atm_list; // atm을 담아놓은 리스트
public:
	bank(string b_name); //bank 생성자
	string get_bankname() { return bankname; } //은행 이름 받아오는 거
	vector<account*> get_user_acc(); // 유저 어카운트 벡터
	bool bool_get_account(int card_num);// 카드 번호를 가지고 계좌 찾는 거.
	account* account_get_account(int card_num);// 카드 번호를 가지고 계좌 찾는 거.	
	account* account_get_accountnum(long long int account_num);// 계좌 번호로 계좌 찾는 거.	
	bool authorize_bank(int card_num, int password);//  card_num과 password가 같은지 확인하기		
	bool ac_enough_money_in_acc_bank(long long int acc_num, int m);// 매개변수로 받은 계좌번호에 돈이 충분히 있는지 확인하기
	bool cn_enough_money_in_acc_bank(int card_num, int m);// 매개변수로 받은 카드번호에 돈이 충분히 있는지 확인하기
	void account_transfer_bank(long long int src_acc_num, bank* dest_bank, long long int dest_acc_num, int m, int fee);// 계좌에서 계좌로 계좌이체하기기
	void cash_transfer_bank(bank* dest_bank, long long int dest_acc_num, int m);// 현금으로 계좌에 계좌이체하기
	void withdrawl_bank(int cardnum, int m);//출금함수 뱅크용
	void deposit_bank(int cardnum, int m);   //카드번호로 접근해서 돈 넣기
	bool bool_accountnumber_in_bank(long long int accountnumber);// 계좌번호가 이 은행에 존재하는지 bool로 확인
	bank* bank_accountnumber_in_bank(int accountnumber);// 계좌번호가 이 은행에 존재한다면 은행 주소 반환
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
	string tr_type; //출금, 입금, 계좌이체
	int amount; //양
	bank* tr_bank;//현재 작업한 은행
	account* tr_account; // 작업한 계좌
	int curr_acc_money;
public:
	transaction_history(int cardnum, string type, int am, bank* bank, account* account); //생성자
	virtual void print_history(); //print 하는 함수.
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

class account { //계좌
private:
	bank* bank_adress; //은행 주소
	string user_name; // 사람 이름
	long long int acc_number;// 계좌 번호 6자리
	int acc_cash; // 계좌 돈
	int password; // 계좌 비밀번호
	int card_num; // 카드 번호 1001 부터 시작.
	card* account_card; // 계좌와 연결된 카드
	vector<transaction_history*> account_tr_his;//account의 거래기록
public:
	account(long long int acc_n, bank* b_adress, string u_name, int pd, int a_cash); // constructor
	bank* get_bank_adress() { return bank_adress; } // 은행 주소 받아오기
	string get_user_name() { return user_name; } // 사람 이름 받아 오기
	int get_account_password() { return password; } // 계좌 비밀번호 받아오기
	long long int get_acc_number() { return acc_number; } // 계좌 번호 받아오기
	int get_acc_cash() { return acc_cash; }//계좌에 돈이 얼마 있는 지 알아오기
	int get_cardnum() { return card_num; }// 카드 번호 받아오기
	void change_acc_cash(int ch_cash) { acc_cash = ch_cash; } // 계좌 안에 있는 돈 갱신하기
	void change_account_tr(transaction_history* tr) {
		account_tr_his.push_back(tr);
	}
	void print_account_history();
	void korean_print_account_history();
};

class atm {
private:
	int serial_number; //atm 일련번호
	int atm_cash[4] = { 0,0,0,0 }; // atm 안에 남아 있는 돈 // 1000,5000,10000,50000
	int card_insert_slot;//현재 꽂힌 card의 번호
	int session = 0; //session
	int number_of_withdrawl = 0; //출금 횟수
	int limit_of_number_of_withdral = 3;//출금 횟수 제한
	int get_money = 0;//출금한 돈
	int limit_of_get_money;//출금한 돈의 제한
	int single_or_multi; //single은 0, multi는 1
	int uni_or_bi;//uni는 0, bi는 1
	bank* primary_bank;//atm의 주 은행
	//static-정세훈>>시리얼넘버>>bank make atm 구현하기.
	card* admin_card;
	vector<transaction_history*> temp_tr_his;
	vector<transaction_history*> atm_tr_his;
public:
	atm(int single_or_multi, int uni_or_bi, int one_thou, int five_thou, int ten_thou, int fifty_thou, bank* pb, int serialnumber);//constructor
	int get_serial_number() { return serial_number; } //serial number 얻어 오는 거
	int get_atm_cash() { //atm에 돈이 얼마 있는 지 얻어 오는 함수.
		int cash = 1000 * atm_cash[0] + 5000 * atm_cash[1] + 10000 * atm_cash[2] + 50000 * atm_cash[3];
		return cash;
	}
	int get_1000() { return atm_cash[0]; }
	int get_5000() { return atm_cash[1]; }
	int get_10000() { return atm_cash[2]; }
	int get_50000() { return atm_cash[3]; }
	int get_card_insert_slot() { return card_insert_slot; } //현재 꽂힌 card의 번호를 가직지고 오는 함수
	void set_card_insert_slot(int card_num) { card_insert_slot = card_num; }
	void increase_session() { session += 1; } // 세션을 1 추가하는 함수.
	void decrease_session() { session -= 1; } // 세션을 1 감소하는 함수.
	void increase_number_of_withdrawl() { number_of_withdrawl += 1; } //출금 횟수를 1 증가하는 함수.
	void zero_number_of_withdrawl() { number_of_withdrawl = 0; } // 출금 횟수를 0으로 초기화하는 함수.
	void increase_get_money(int money) { get_money += money; }//출금한 돈을 증가하는 함수
	bank* find_cardnumber_bank(int cardnumber); //카드 번호로 은행 찾기
	bool valid_cardnumber_account(int cardnumber); //카드번호에 해당하는 계좌가 있는 지 없는지
	bank* find_bank_by_account(long long int accountnumber);//계좌번호로 은행찾기
	bool valid_find_bank_by_account(long long int accountnumber); //계좌번호에 해당하는 계좌가 있는지 없는지
	virtual void transfer(); //계좌이체
	virtual void deposit(); // 입금
	virtual void withdrawl();//출금
	virtual void koreaninterface();//인터페이스(한국어 버전)
	virtual void interface();//인터페이스
	virtual void koreantransfer(); //계좌이체
	virtual void koreandeposit(); // 입금
	virtual void koreanwithdrawl();//출금
	//virtual void koreaninterface();//인터페이스
	bool check_password(int cardnumber, int password);
	void print_session_history(); //강승수 세센 종료 main 추가
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
	void transfer(); //계좌이체
	void deposit(); // 입금
	void withdrawl();//출금
	void interface();//인터페이스	
};
class multi_uni_atm : public atm {
private:
public:
	multi_uni_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, bank* pb, int serialnumber) :atm(type, language_type, one_thou, five_thou, ten_thou, fifty_thou, pb, serialnumber) {};
	void transfer(); //계좌이체
	void deposit(); // 입금
	void withdrawl();//출금
	void interface();//인터페이스	
};
class single_bi_atm : public atm {
private:
public:
	single_bi_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, bank* pb, int serialnumber) :atm(type, language_type, one_thou, five_thou, ten_thou, fifty_thou, pb, serialnumber) {};
	void transfer(); //계좌이체
	void deposit(); // 입금
	void withdrawl();//출금
	void interface();//인터페이스
	void koreantransfer(); //계좌이체
	void koreandeposit(); // 입금
	void koreanwithdrawl();//출금
	void koreaninterface();//인터페이스
	void korean_print_session_history();
};
class multi_bi_atm : public atm {
private:
public:
	multi_bi_atm(int type, int language_type, int one_thou, int five_thou, int ten_thou, int fifty_thou, bank* pb, int serialnumber) :atm(type, language_type, one_thou, five_thou, ten_thou, fifty_thou, pb, serialnumber) {};
	void transfer(); //계좌이체
	void deposit(); // 입금
	void withdrawl();//출금
	void interface();//인터페이스
	void koreantransfer(); //계좌이체
	void koreandeposit(); // 입금
	void koreanwithdrawl();//출금
	void koreaninterface();//인터페이스
	void korean_print_session_history();
};
//카드 부분				 
card::card(bank* b, atm* master_atm) { //admin용 constructor
	admin = true; //admin이다
	card_bank = b; //bank 어딘지
	card_num = 7777777; // admin의 card_num은 7777777이다.
	mas_atm = master_atm; //atm 마다 마스터 카드
	std::cout << "admin card가 생성되었습니다. 카드의 은행은" << b->get_bankname() << "입니다." << endl;
}
card::card(bank* b) //일반용 constructor
{
	static_card_number++;
	admin = false; //admin 이 아니다.
	card_bank = b; // bank 주소
	card_num = static_card_number; // cardnumber
	std::cout << "card가 생성되었습니다. 카드의 은행은 " << b->get_bankname() << "이며 " << "카드 번호는 " << card_num << "입니다." << endl;
}

//은행 부분

bank::bank(string b_name) {  // 은행 생성자
	bankname = b_name;
	banklist.push_back(this);
	std::cout << "은행이 생성되었습니다. 은행 이름은 " << bankname << "입니다." << endl;
}

vector<account*> bank::get_user_acc() { return user_acc; }

bool bank::bool_get_account(int card_num) {  // 카드 번호를 가지고 계좌 찾는 거.//참 거짓
	for (int i = 0; i < user_acc.size(); i++) { // user acc 내를 돈다.
		if (user_acc[i]->get_cardnum() == card_num) { //같은 카드 번호를 찾았다. 
			curr_account = user_acc[i]; //현재 계좌번호에 넣음
			return true; //return true 해준다.
		}
	}
	return false;
}
account* bank::account_get_account(int card_num) { // 카드 번호를 가지고 계좌 찾는 거.
	int count = 0;
	for (int i = 0; i < user_acc.size(); i++)
	{ // user acc 내를 돈다.
		count += 1;
		if (user_acc[i]->get_cardnum() == card_num)
		{ //같은 카드 번호를 찾았다. 
			account* ac = user_acc[i]; //현재 계좌번호에 넣음
			return ac;// 현재 계좌 인스턴스를 반환 
		}
	}
}
account* bank::account_get_accountnum(long long int account_num) { // 계좌 번호로 계좌 찾는 거. 
	int count = 0;
	for (int i = 0; i < user_acc.size(); i++) {
		count += 1;
		if (user_acc[i]->get_acc_number() == account_num) { // 같은 계좌 번호를 찾았다.
			curr_account = user_acc[i]; //현재 계좌번호에 넣음
			return curr_account;  // 현재 계좌 인스턴스를 반환	
		}
	}
}
bool bank::authorize_bank(int card_num, int password) { //  card_num과 password가 같은지 확인하기
	if (this->account_get_account(card_num)->get_account_password() == password) // card_num의 계좌의 password 받아와서 받은 password랑 비교하기 
		return true; // 같으면 true
	else return false; // 다르면 false
}
bool bank::ac_enough_money_in_acc_bank(long long int acc_num, int m) { // 매개변수로 받은 계좌번호에 돈이 충분히 있는지 확인하기
	account* acc = account_get_accountnum(acc_num);  // 계좌번호로 계좌 인스턴스 찾기
	if (acc->get_acc_cash() < m) { return false; }        // 계좌의 잔여금액이 m보다 작다면 false를 반환
	else { return true; }                              // 아니라면 true를 반환
}
bool bank::cn_enough_money_in_acc_bank(int card_num, int m) {   // 매개변수로 받은 카드번호에 돈이 충분히 있는지 확인하기
	account* acc = account_get_account(card_num);  // 카드번호로 계좌 인스턴스 찾기
	if (acc->get_acc_cash() < m) { return false; }        // 계좌의 잔여금액이 m보다 작다면 false를 반환
	else { return true; }                              // 아니라면 true를 반환
}
void bank::account_transfer_bank(long long int src_acc_num, bank* dest_bank, long long int dest_acc_num, int m, int fee) {// 계좌에서 계좌로 계좌이체하기기
	account* src_account = account_get_accountnum(src_acc_num);  // 돈을 보낼 계좌를 src_account에 저장함
	src_account->change_acc_cash(src_account->get_acc_cash() - m - fee);          // src_account 계좌에서 m원을 뺌
	dest_account = dest_bank->account_get_accountnum(dest_acc_num); // 돈을 받는 계좌를 dest_account에 저장함
	dest_account->change_acc_cash(dest_account->get_acc_cash() + m);        // dest_account 계좌에서 m원을 더함
}
void bank::cash_transfer_bank(bank* dest_bank, long long int dest_acc_num, int m) {  // 현금으로 계좌에 계좌이체하기
	dest_account = dest_bank->account_get_accountnum(dest_acc_num); // 돈을 받는 계좌를 dest_account에 저장함
	dest_account->change_acc_cash(dest_account->get_acc_cash() + m);        // dest_account 계좌에서 m원을 더함
}
void bank::withdrawl_bank(int cardnum, int m) { //출금함수 뱅크용
	account* acc = account_get_account(cardnum); //카드넘으로 어카운트
	acc->change_acc_cash(acc->get_acc_cash() - m);
}
void bank::deposit_bank(int cardnum, int m) {   //카드번호로 접근해서 돈 넣기
	account* acc = account_get_account(cardnum);
	acc->change_acc_cash(acc->get_acc_cash() + m);
}
bool bank::bool_accountnumber_in_bank(long long int accountnumber) { // 계좌번호가 이 은행에 존재하는지 bool로 확인
	int number_of_account = user_acc.size();
	for (int i = 0; i < number_of_account; i++) {
		if (accountnumber == user_acc[i]->get_acc_number()) { return true; }
	}
	return false;
}
bank* bank::bank_accountnumber_in_bank(int accountnumber) { // 계좌번호가 이 은행에 존재한다면 은행 주소 반환
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
	std::cout << "거래 ID: " << transaction_ID << endl;
	std::cout << "거래 유형: " << tr_type << endl;
	std::cout << "카드 번호: " << card_number << endl;
	std::cout << tr_type << " 금액: " << amount << "원" << endl;
	std::cout << tr_type << " 계좌 -> " << tr_bank->get_bankname() << "은행: " << tr_account->get_acc_number() << ", 소유주: " << tr_account->get_user_name() << endl;
	std::cout << "거래 후 금액: " << curr_acc_money << "원" << endl << endl;
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
	string t = "거래 ID: " + to_string(transaction_ID) + '\n'
		+ "거래 유형: " + tr_type + '\n'
		+ "카드 번호: " + to_string(card_number) + '\n'
		+ tr_type + " 금액: " + to_string(amount) + "원" + '\n'
		+ tr_type + " 계좌 -> " + tr_bank->get_bankname() + "은행: " + to_string(tr_account->get_acc_number()) + ", 소유주: " + tr_account->get_user_name() + '\n'
		+ "거래 후 금액: " + to_string(curr_acc_money) + "원" + '\n' + '\n';
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
	std::cout << "거래 ID: " << transaction_ID << endl;
	std::cout << "거래 유형: " << tr_type << endl;
	std::cout << "카드 번호: " << card_number << endl;
	std::cout << tr_type << "금액: " << amount << "원" << endl;
	std::cout << "이체 계좌 -> " << tr_bank->get_bankname() << "은행: " << tr_account->get_acc_number() << ", 소유주: " << tr_account->get_user_name() << endl;
	std::cout << "입금 계좌 -> " << end_bank->get_bankname() << "은행: " << end_account->get_acc_number() << ", 소유주: " << end_account->get_user_name() << endl;
	std::cout << "거래 후 금액: " << curr_acc_money << "원" << endl << endl;
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
	string t = "거래 ID: " + to_string(transaction_ID) + '\n'
		+ "거래 유형: " + tr_type + '\n'
		+ "카드 번호: " + to_string(card_number) + '\n'
		+ tr_type + " 금액: " + to_string(amount) + "원" + '\n'
		+ "이체 계좌 -> " + tr_bank->get_bankname() + "은행: " + to_string(tr_account->get_acc_number()) + ", 소유주: " + tr_account->get_user_name() + '\n'
		+ "입금 계좌 -> " + end_bank->get_bankname() + "은행: " + to_string(end_account->get_acc_number()) + ", 소유주: " + end_account->get_user_name() + '\n'
		+ "거래 후 금액: " + to_string(curr_acc_money) + "원" + '\n' + '\n';
	return t;
}



//계좌부분
account::account(long long int acc_n, bank* b_adress, string u_name, int pd, int a_cash) {
	bank_adress = b_adress; //bank 주소
	user_name = u_name; // 유저 이름
	acc_number = acc_n; // 계좌 번호
	acc_cash = a_cash; // 계좌 돈
	password = pd; // 비밀번호
	account_card = new card(this->get_bank_adress());
	card_num = account_card->get_cardnum();
	accountlist.push_back(this);
	std::cout << "계좌가 개설되었습니다. 계좌의 은행은 " << bank_adress->get_bankname() << "이며 " << "유저 이름은 " << user_name << "입니다. 계좌번호는 " << acc_number << "입니다." << endl;
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

	std::cout << "atm 이 생성되었습니다. atm의 serial number은 " << serial_number << "입니다." << "현재 돈은 " << get_atm_cash() << "보유하고 있습니다." << endl;
}
bool atm::valid_cardnumber_account(int cardnumber) { //카드 번호에 해당하는 계좌가 있는 지 없는지
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
bank* atm::find_cardnumber_bank(int cardnumber) { // 카드 번호에 해당하는 은행을 찾기
	int banknum = banklist.size();
	for (int i = 0; i < banknum; i++) {
		if ((banklist[i]->bool_get_account(cardnumber)) == true)
		{
			return banklist[i];
		}
	}
}
bool atm::valid_find_bank_by_account(long long int accountnumber) { //계좌번호가 유효한지를 true false return
	int banknum = banklist.size();
	for (int i = 0; i < banknum; i++) {
		if ((banklist[i]->bool_accountnumber_in_bank(accountnumber)) == true)
		{
			return true;
		}
	}
	return false;
}

bank* atm::find_bank_by_account(long long int accountnumber) { //계좌번호로 은행 찾기
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
	temp_tr_his.clear();//정세훈 찾아보고 벡터 초기화
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
		if (c1 == 1) {       //현금 계좌이체
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
			transaction_history* th = new transaction_history(card_insert_slot, "현금_계좌이체", money, dest_bank, dest_bank->account_get_accountnum(dest_account));
			temp_tr_his.push_back(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);

			return;
		}

		else if (c1 == 2) {    // 계좌끼리 계좌이체
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
			transfer_acc_transaction_history* th = new transfer_acc_transaction_history(card_insert_slot, "계좌_계좌이체", money, src_bank, dest_bank, src_bank->account_get_accountnum(src_account), dest_bank->account_get_accountnum(dest_account));
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
		std::cout << "타 은행 입금에는 수수료 1000원이 발생합니다. 수수료를 포함해서 입금하세요" << endl;
	}
	while (true) {
		std::cout << "천원을 몇 장 넣으시겠습니니까? (종료하려면 -1을 입력하세요)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "오천원을 몇 장 넣으시겠습니니까? ";
		std::cin >> five_thou;
		std::cout << "만원을 몇 장 넣으시겠습니니까? ";
		std::cin >> ten_thou;
		std::cout << "오만원을 몇 장 넣으시겠습니니까? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "가능한 현금 장 수를 초과하셨습니다." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "수표를 몇 장 넣으시겠습니까? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "가능한 수표 장 수를 초과하셨습니다." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 일때 정상
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "번째 수표의 금액은 얼마입니까? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100억 이상은 불가
				flag = 1; //flag = 1이면 에러 발생
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "잘못된 수표 금액 입니다." << endl;
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
	std::cout << m << "원이 입금되었습니다." << endl;
	std::cout << "잔액은 " << b->account_get_account(card_insert_slot)->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(card_insert_slot, "입금", m, b, b->account_get_account(card_insert_slot));
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
	if (number_of_withdrawl >= limit_of_number_of_withdral) { //withdrawl 초기화 필요. atm에 카드를 넣으면 withdrawl을 초기화 해줘야함.
		std::cout << "더 출금하시려면 세션을 초기화 하셔야 합니다." << endl;
		return;
	}
	if (b != primary_bank) {
		std::cout << "타 은행 출금에는 수수료 2000원이 발생합니다." << endl;
		flag = 1;
	}
	else {
		std::cout << "자 은행 출금에는 수수료 1000원이 발생합니다." << endl;
	}
	while (1) {
		std::cout << "얼마를 출금하시겠습니까?(수수료 제외) (종료하려면 -1을 입력하세요)";
		std::cin >> check_num;
		if (check_num == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//출금 가능한 금액을 초과하였는지 조사
			std::cout << "출금 가능하신 금액을 초과하셨습니다" << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "화폐 최소 단위는 1000원 입니다" << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "ATM에 충분한 현금이 존재하지 않습니다" << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(card_insert_slot, (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "계좌에 돈이 충분하지 않습니다" << endl;
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
				std::cout << "ATM에 지폐의 개수가 부족합니다" << endl;
			}
			else {
				//atm_cash 조정
				atm_cash[0] -= one_thou;
				atm_cash[1] -= five_thou;
				atm_cash[2] -= ten_thou;
				atm_cash[3] -= fifty_thou;
				b->withdrawl_bank(card_insert_slot, (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "은행에서 " << (flag ? (check_num + 2000) : (check_num + 1000)) << "원이 출금되었고 가져가실 현금은 총 " << check_num << "원 입니다." << endl;
				std::cout << "천원 " << one_thou << "장 / " << "오천원 " << five_thou << "장 / " << "만원 " << ten_thou << "장 / " << "오만원 " << fifty_thou << "장" << endl;
				std::cout << "잔액은 " << b->account_get_account(card_insert_slot)->get_acc_cash() << endl;
				number_of_withdrawl += 1;
				return;
				transaction_history* th;
				th = new transaction_history(card_insert_slot, "출금", check_num, b, b->account_get_account(card_insert_slot));
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
	int number;//추가
main1:
	std::cout << "Please insert the card.(Please enter the card number). If you want to exit, enter -1.";
	std::cin >> number; //변경
	set_card_insert_slot(number);//추가
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
		goto end;//셰션 end
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
	std::cout << "카드를 넣어주세요.(카드번호를 입력해주세요). 종료하고 싶으시면 -1을 입력해주세요.";
	std::cin >> card_insert_slot;
	if (card_insert_slot == -1) {
		return;
	}
	if (card_insert_slot == 7777777) {
		admin_interface_korean();
		goto end;
	}
	if (valid_cardnumber_account(card_insert_slot) == 0) {
		cout << "유효하지 않은 카드 번호입니다. " << endl;
		goto main1;
	}
	while (1) {
		if (password_count == 3) {
			break;
		}
		std::cout << "비밀번호를 입력해주세요." << endl;
		std::cin >> password;
		if (check_password(card_insert_slot, password)) {
			break;
		}
		cout << "잘못된 비밀번호입니다." << endl;
		password_count++;
	}
	if (password_count == 3) {
		cout << "비밀번호를 3번 틀리셨습니다. " << endl;
		goto end;//셰션 end
	}
	while (1) {
		std::cout << "어떤 업무를 하시겠습니까? 번호를 기입하여 주세요." << endl;
		std::cout << "1. 입금 " << "2. 출금 " << "3. 이체 " << "4. 세션 종료 :  ";
		std::cin >> work_type;
		if (work_type == 1) { deposit(); }
		else if (work_type == 2) { withdrawl(); }
		else if (work_type == 3) { transfer(); }
		else if (work_type == 4) { break; }
		else { std::cout << "잘못된 메뉴를 선택하셨습니다." << endl; }
	}
end:
	cout << "세션을 종료합니다." << endl;
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
		// transaction_history 출력해줄수 있 나요?
		//정세훈 여기다가 fileout
	}
	return;
}

void atm::admin_interface_korean() {
	int number;
	cout << "선택하실 메뉴를 골라주세요." << endl;
	cout << "메뉴" << endl;
	cout << "1. 거래 기록" << endl;
menu1:
	cin >> number;
	if (number != 1) {
		cout << "잘못된 번호를 입력하셨습니다. 다시 입력해주세요." << endl;
		goto menu1;
	}
	else {
		korean_print_atm_history();
		// transaction_history 출력해줄수 있 나요?
		//정세훈 여기다가 fileout
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
			cout << "1000*" << atmlist[i]->get_1000() << "장 ";
		}
		if (atmlist[i]->get_5000() >= 1) {
			cout << "5000*" << atmlist[i]->get_5000() << "장 ";
		}
		if (atmlist[i]->get_10000() >= 1) {
			cout << "10000*" << atmlist[i]->get_10000() << "장 ";
		}
		if (atmlist[i]->get_50000() >= 1) {
			cout << "50000*" << atmlist[i]->get_50000() << "장 ";
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
		cout << "어떤 언어를 고르시겠습니까? Which language do you prefer?" << endl;
		cout << "0. 영어 \t 1. 한국어" << endl;
		cin >> number;
		if (number == 0) {
			interface();
		}
		else if (number == 1) {
			koreaninterface();
		}
		else {
			cout << "잘못된 번호를 입력하셨습니다" << endl;
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
		std::cout << "어떤 업무를 하시겠습니까? 번호를 기입해주세요. 1. 현금 이체 2. 계좌 이체 : ";
		std::cin >> c1;
		if (c1 == 1) {       //현금 계좌이체
			std::cout << "누구에게 계좌 이체하시겠습니까? 송금 받을 계좌를 입력해주세요. : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "해당하는 은행계좌는 존재하지 않습니다." << endl;
				return;
			}
			std::cout << "얼마를 송금하시겠습니까?: ";
			std::cin >> money;
			std::cout << "5000원의 수수료가 드니, 수수료를 포함해서 현금을 넣어주세요" << endl;
			std::cout << "1000원 짜리 지폐를 몇 장 넣겠습니까? ";
			std::cin >> one_thou;
			std::cout << "5000원 짜리 지폐를 몇 장 넣겠습니까? ";
			std::cin >> five_thou;
			std::cout << "10000원 짜리 지폐를 몇 장 넣겠습니까? ";
			std::cin >> ten_thou;
			std::cout << "50000원 짜리 지폐를 몇 장 넣겠습니까? ";
			std::cin >> fifty_thou;
			money_sum = one_thou * 1000 + five_thou * 5000 + ten_thou * 10000 + fifty_thou * 50000;
			if (money_sum != money + 5000) {
				std::cout << "당신이 입력한 돈과 넣은 돈이 다릅니다." << endl;
				return;
			}
			atm_cash[0] += one_thou;
			atm_cash[1] += five_thou;
			atm_cash[2] += ten_thou;
			atm_cash[3] += fifty_thou;
			curr_bank->cash_transfer_bank(dest_bank, dest_account, money);
			std::cout << "현금 이체가 완료되었습니다." << endl << endl;
			return;
		}

		else if (c1 == 2) {    // 계좌끼리 계좌이체
			std::cout << "돈을 받을 계좌의 계좌번호를 입력하세요 : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "해당 계좌는 유효하지 않습니다." << endl; //이게 맞아?
				return;
			}
			std::cout << "당신의 계좌번호를 입력해주세요 : ";
			std::cin >> src_account;
			src_bank = find_bank_by_account(src_account);
			if (!valid_find_bank_by_account(src_account)) {
				std::cout << "해당하는 계좌가 없습니다." << endl;
				return;
			}

			if (src_bank == primary_bank && dest_bank == primary_bank) { fee = 2000; }
			else if ((src_bank != primary_bank && dest_bank == primary_bank) or (src_bank == primary_bank && dest_bank != primary_bank)) { fee = 3000; }
			else if (src_bank != primary_bank && dest_bank != primary_bank) { fee = 4000; }

			if (u_name != src_bank->account_get_accountnum(src_account)->get_user_name() or src_bank != curr_bank) {
				std::cout << "해당 계좌에서 돈을 이체할 수 없습니다." << endl;
				return;
			}

			std::cout << "얼마나 보낼 지 입력하세요. : ";
			std::cin >> money;
			if (!curr_bank->cn_enough_money_in_acc_bank(card_insert_slot, money + fee)) {
				std::cout << "계좌에 충분한 돈이 있지 않습니다.";
				return;
			}
			curr_bank->account_transfer_bank(src_account, dest_bank, dest_account, money, fee);
			std::cout << money << "+" << fee << "원이 계좌에서 빠집니다." << endl;
			cout << "계좌 이체가 완료되었습니다." << endl << endl;
			return;
		}
		else {
			std::cout << "1번과 2번 사이에 고르시오." << endl;
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
		std::cout << "타 은행 입금에는 수수료 1000원이 발생합니다. 수수료를 포함해서 입금하세요" << endl;
	}
	while (true) {
		std::cout << "천원을 몇 장 넣으시겠습니니까? (종료하려면 -1을 입력하세요)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "오천원을 몇 장 넣으시겠습니니까? ";
		std::cin >> five_thou;
		std::cout << "만원을 몇 장 넣으시겠습니니까? ";
		std::cin >> ten_thou;
		std::cout << "오만원을 몇 장 넣으시겠습니니까? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "가능한 현금 장 수를 초과하셨습니다." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "수표를 몇 장 넣으시겠습니까? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "가능한 수표 장 수를 초과하셨습니다." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 일때 정상
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "번째 수표의 금액은 얼마입니까? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) {
				flag = 1; //flag = 1이면 에러 발생
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "잘못된 수표 금액 입니다." << endl;
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
	std::cout << m << "원이 입금되었습니다." << endl;
	std::cout << "잔액은 " << b->account_get_account(card_insert_slot)->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(card_insert_slot, "입금", m, b, b->account_get_account(card_insert_slot));
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
	if (number_of_withdrawl > limit_of_number_of_withdral) { //withdrawl 초기화 필요. atm에 카드를 넣으면 withdrawl을 초기화 해줘야함.
		std::cout << "더 출금하시려면 세션을 초기화 하셔야 합니다." << endl;
		return;
	}
	if (b != primary_bank) {
		std::cout << "타 은행 출금에는 수수료 2000원이 발생합니다." << endl;
		flag = 1;
	}
	else {
		std::cout << "자 은행 출금에는 수수료 1000원이 발생합니다." << endl;
	}
	while (1) {
		std::cout << "천원을 몇 장 넣으시겠습니니까? (종료하려면 -1을 입력하세요)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//출금 가능한 금액을 초과하였는지 조사
			std::cout << "출금 가능하신 금액을 초과하셨습니다" << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "화폐 최소 단위는 1000원 입니다" << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "ATM에 충분한 현금이 존재하지 않습니다" << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(card_insert_slot, (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "계좌에 돈이 충분하지 않습니다" << endl;
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
				std::cout << "ATM에 지폐의 개수가 부족합니다" << endl;
			}
			else {
				//atm_cash 조정
				atm_cash[0] -= one_thou;
				atm_cash[1] -= five_thou;
				atm_cash[2] -= ten_thou;
				atm_cash[3] -= fifty_thou;
				b->withdrawl_bank(card_insert_slot, (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "은행에서 " << (flag ? (check_num + 2000) : (check_num + 1000)) << "원이 출금되었고 가져가실 현금은 총 " << check_num << "원 입니다." << endl;
				std::cout << "천원 " << one_thou << "장 / " << "오천원 " << five_thou << "장 / " << "만원 " << ten_thou << "장 / " << "오만원 " << fifty_thou << "장" << endl;
				std::cout << "잔액은 " << b->account_get_account(card_insert_slot)->get_acc_cash() << endl;
				number_of_withdrawl += 1;
				transaction_history* th;
				th = new transaction_history(card_insert_slot, "출금", check_num, b, b->account_get_account(card_insert_slot));
				temp_tr_his.push_back(th);
				b->account_get_account(card_insert_slot)->change_account_tr(th);
				return;
			}
		}
	}
}
//윤현서 시작

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
		if (c1 == 1) {       //현금 계좌이체
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

		else if (c1 == 2) {    // 계좌끼리 계좌이체
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
		int flag = 0; //flag = 0 일때 정상
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "th check, how much? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100억 이상은 불가
				flag = 1; //flag = 1이면 에러 발생
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
		if (check_num > 500000) {//출금 가능한 금액을 초과하였는지 조사
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
				//atm_cash 조정
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
	int number;//추가
main1:
	std::cout << "Please insert the card.(Please enter the card number). If you want to exit, enter -1.";
	std::cin >> number; //변경
	set_card_insert_slot(number);//추가
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
		goto end;//셰션 end
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
		if (c1 == 1) {       //현금 계좌이체
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

		else if (c1 == 2) {    // 계좌끼리 계좌이체
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
		int flag = 0; //flag = 0 일때 정상
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "th check, how much? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100억 이상은 불가
				flag = 1; //flag = 1이면 에러 발생
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
		if (check_num > 500000) {//출금 가능한 금액을 초과하였는지 조사
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
				//atm_cash 조정
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
	int number;//추가
main1:
	std::cout << "Please insert the card.(Please enter the card number). If you want to exit, enter -1.";
	std::cin >> number; //변경
	set_card_insert_slot(number);//추가
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
		goto end;//셰션 end
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





//강승수 시작

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
		if (c1 == 1) {       //현금 계좌이체
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

		else if (c1 == 2) {    // 계좌끼리 계좌이체
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
		int flag = 0; //flag = 0 일때 정상
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "th check, how much? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100억 이상은 불가
				flag = 1; //flag = 1이면 에러 발생
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
		if (check_num > 500000) {//출금 가능한 금액을 초과하였는지 조사
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
				//atm_cash 조정
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
	int number;//추가
main1:
	std::cout << "Please insert the card.(Please enter the card number). If you want to exit, enter -1.";
	std::cin >> number; //변경
	set_card_insert_slot(number);//추가
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
		goto end;//셰션 end
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
		std::cout << "번호를 고르시오. " << "1. 현금 이체 2. 계좌 이체: ";
		std::cin >> c1;
		if (c1 == 1) {       //현금 계좌이체
			std::cout << "돈을 받고자 하는 계좌의 계좌번호를 입력하시오. : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "당신이 입력한 계좌번호가 존재하지 않습니다." << endl;
				return;
			}
			std::cout << "돈을 얼마나 보내겠습니까? ";
			std::cin >> money;
			std::cout << "5000원의 수수료가 드니, 수수료를 포함해서 현금을 넣어주세요" << endl;
			std::cout << "1000원 몇 장 넣겠습니까? ";
			std::cin >> one_thou;
			std::cout << "5000원 몇 장 넣겠습니까? ";
			std::cin >> five_thou;
			std::cout << "10000원 몇 장 넣겠습니까? ";
			std::cin >> ten_thou;
			std::cout << "50000원 몇 장 넣겠습니까? ";
			std::cin >> fifty_thou;
			money_sum = one_thou * 1000 + five_thou * 5000 + ten_thou * 10000 + fifty_thou * 50000;
			if (money_sum != money + 5000) {
				std::cout << "요청하신 금액과 지불하신 현금 액수가 다릅니다." << endl;
				return;
			}
			change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
			curr_bank->cash_transfer_bank(dest_bank, dest_account, money);
			std::cout << "이체가 완료되었습니다." << endl;
			transaction_history* th = new transaction_history(get_card_insert_slot(), "현금_계좌이체", money, dest_bank, dest_bank->account_get_accountnum(dest_account));
			change_temp(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}

		else if (c1 == 2) {    // 계좌끼리 계좌이체
			std::cout << "돈을 받을 계좌의 계좌번호를 입력하세요: ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "입력하신 계좌번호의 계좌가 존재하지 않습니다." << endl;
				return;
			}
			std::cout << "본인의 계좌번호를 입력하세요: ";
			std::cin >> src_account;
			src_bank = find_bank_by_account(src_account);
			if (!valid_find_bank_by_account(src_account)) {
				std::cout << "입력하신 계좌번호의 계좌가 존재하지 않습니다." << endl;
				return;
			}
			if (u_name != src_bank->account_get_accountnum(src_account)->get_user_name() or src_bank != curr_bank) {
				std::cout << "해당 계좌에서 돈을 이체할 수 없습니다." << endl;
				return;
			}

			if (src_bank == get_primary_bank() && dest_bank == get_primary_bank()) { fee = 2000; }
			else if ((src_bank != get_primary_bank() && dest_bank == get_primary_bank()) or (src_bank == get_primary_bank() && dest_bank != get_primary_bank())) { fee = 3000; }
			else if (src_bank != get_primary_bank() && dest_bank != get_primary_bank()) { fee = 4000; }

			std::cout << "송금하실 금액을 알려주세요: ";
			std::cin >> money;
			if (!curr_bank->cn_enough_money_in_acc_bank(get_card_insert_slot(), money + fee)) {
				std::cout << "계좌에 돈이 충분하지 않습니다.";
				return;
			}
			curr_bank->account_transfer_bank(src_account, dest_bank, dest_account, money, fee);
			std::cout << money << "+" << fee << " 원이 계좌에서 출금되었습니다." << endl;
			cout << "계좌이체가 완료되었습니다." << endl << endl;
			transfer_acc_transaction_history* th = new transfer_acc_transaction_history(get_card_insert_slot(), "계좌_계좌이체", money, src_bank, dest_bank, src_bank->account_get_accountnum(src_account), dest_bank->account_get_accountnum(dest_account));
			change_transfer_temp(th);
			src_bank->account_get_accountnum(src_account)->change_account_tr(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}
		else {
			std::cout << "1과 2중에 고르세요." << endl;
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
		std::cout << "더 출금하시려면 세션을 초기화 하셔야 합니다." << endl;
		return;
	}
	if (b != get_primary_bank()) {
		std::cout << "타 은행 출금에는 수수료 2000원이 발생합니다." << endl;
		flag = 1;
	}
	else {
		std::cout << "자 은행 출금에는 수수료 1000원이 발생합니다." << endl;
	}
	while (1) {
		std::cout << "얼마를 출금하시겠습니까?(수수료 제외) (종료하려면 -1을 입력하세요)";
		std::cin >> check_num;
		if (check_num == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//출금 가능한 금액을 초과하였는지 조사
			std::cout << "출금 가능하신 금액을 초과하셨습니다" << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "화폐 최소 단위는 1000원 입니다" << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "ATM에 충분한 현금이 존재하지 않습니다" << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "계좌에 돈이 충분하지 않습니다" << endl;
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
				std::cout << "ATM에 지폐의 개수가 부족합니다" << endl;
			}
			else {
				//atm_cash 조정
				change_atm_cash(-1 * (one_thou), -1 * (five_thou), -1 * (ten_thou), -1 * (fifty_thou));
				b->withdrawl_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "은행에서 " << (flag ? (check_num + 2000) : (check_num + 1000)) << "원이 출금되었고 가져가실 현금은 총 " << check_num << "원 입니다." << endl;
				std::cout << "천원 " << one_thou << "장 / " << "오천원 " << five_thou << "장 / " << "만원 " << ten_thou << "장 / " << "오만원 " << fifty_thou << "장" << endl;
				std::cout << "잔액은 " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
				set_number_of_withdrawl(get_number_of_withdrawl() + 1);
				transaction_history* th = new transaction_history(get_card_insert_slot(), "출금", check_num, b, b->account_get_account(get_card_insert_slot()));
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
		std::cout << "타 은행 입금에는 수수료 1000원이 발생합니다. 수수료를 포함해서 입금하세요" << endl;
	}
	while (true) {
		std::cout << "천원을 몇 장 넣으시겠습니니까? (종료하려면 -1을 입력하세요)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "오천원을 몇 장 넣으시겠습니니까? ";
		std::cin >> five_thou;
		std::cout << "만원을 몇 장 넣으시겠습니니까? ";
		std::cin >> ten_thou;
		std::cout << "오만원을 몇 장 넣으시겠습니니까? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "가능한 현금 장 수를 초과하셨습니다." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "수표를 몇 장 넣으시겠습니까? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "가능한 수표 장 수를 초과하셨습니다." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 일때 정상
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "번째 수표의 금액은 얼마입니까? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100억 이상은 불가
				flag = 1; //flag = 1이면 에러 발생
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "잘못된 수표 금액 입니다." << endl;
		}
		else { break; }
	}
	change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
	long long int m = 1000 * one_thou + 5000 * five_thou + 10000 * ten_thou + 50000 * fifty_thou + check_sum;
	if (b != get_primary_bank()) { m = m - 1000; }
	b->deposit_bank(get_card_insert_slot(), m);
	std::cout << m << "원이 입금되었습니다." << endl;
	std::cout << "잔액은 " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(get_card_insert_slot(), "입금", m, b, b->account_get_account(get_card_insert_slot()));
	change_temp(th);
	b->account_get_account(get_card_insert_slot())->change_account_tr(th);
	return;
}

void multi_bi_atm::korean_print_session_history() {
	for (int i = 0; i < get_temp().size(); i++) {
		get_temp()[i]->korean_print_history();
		change_atm_tr(get_temp()[i]);
	}
	clear_temp();//정세훈 찾아보고 벡터 초기화
}

void multi_bi_atm::koreaninterface() {
	increase_session();
	set_number_of_withdrawl(0);
	int password;
	int work_type;
	int password_count = 0;
	int number;//추가
main1:
	std::cout << "카드를 넣어주세요.(카드번호를 입력해주세요). 종료하고 싶으시면 -1을 입력해주세요.";
	std::cin >> number; //변경
	set_card_insert_slot(number);//추가
	if (get_card_insert_slot() == -1) {
		return;
	}
	if (get_card_insert_slot() == 7777777) {
		admin_interface_korean();
		goto end;
	}
	if (valid_cardnumber_account(get_card_insert_slot()) == false) {
		cout << "유효하지 않은 카드 번호입니다. " << endl;
		goto main1;
	}
	while (1) {
		if (password_count == 3) {
			break;
		}
		std::cout << "비밀번호를 입력해주세요." << endl;
		std::cin >> password;
		if (check_password(get_card_insert_slot(), password)) {
			break;
		}
		cout << "비밀번호가 틀립니다." << endl;
		password_count++;
	}
	if (password_count == 3) {
		cout << "비밀번호를 3번 틀리셨습니다. " << endl;
		goto end;//셰션 end
	}
	while (1) {
		std::cout << "어떤 업무를 하시겠습니까? 번호를 기입하여 주세요." << endl;
		std::cout << "1. 입금 " << "2. 출금 " << "3. 이체 " << "4. 세션 종료 :  ";
		std::cin >> work_type;
		if (work_type == 1) { koreandeposit(); }
		else if (work_type == 2) { koreanwithdrawl(); }
		else if (work_type == 3) { koreantransfer(); }
		else if (work_type == 4) { break; }
		else { std::cout << "잘못된 메뉴를 선택하셨습니다." << endl; }
	}
end:
	cout << "세션을 종료합니다." << endl;
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
		if (c1 == 1) {       //현금 계좌이체
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

		else if (c1 == 2) {    // 계좌끼리 계좌이체
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
		int flag = 0; //flag = 0 일때 정상
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "th check, how much? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100억 이상은 불가
				flag = 1; //flag = 1이면 에러 발생
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
		if (check_num > 500000) {//출금 가능한 금액을 초과하였는지 조사
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
				//atm_cash 조정
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
	clear_temp();//정세훈 찾아보고 벡터 초기화
}





void single_bi_atm::interface() {
	increase_session();
	set_number_of_withdrawl(0);
	int password;
	int work_type;
	int password_count = 0;
	int number;//추가
main1:
	std::cout << "Please insert the card.(Please enter the card number). If you want to exit, enter -1.";
	std::cin >> number; //변경
	set_card_insert_slot(number);//추가
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
		goto end;//셰션 end
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
		std::cout << "번호를 고르시오. " << "1. 현금 이체 2. 계좌 이체: ";
		std::cin >> c1;
		if (c1 == 1) {       //현금 계좌이체
			std::cout << "돈을 받고자 하는 계좌의 계좌번호를 입력하시오. : ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (dest_bank != get_primary_bank()) {
				cout << "이 은행은 주 은행 계좌가 아닙니다." << endl;
				return;
			}
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "당신이 입력한 계좌번호가 존재하지 않습니다." << endl;
				return;
			}
			std::cout << "돈을 얼마나 보내시겠습니까? " << endl;
			std::cin >> money;
			std::cout << "5000원의 수수료가 드니, 수수료를 포함해서 현금을 넣어주세요" << endl;
			std::cout << "1000원 몇 장 넣겠습니까? ";
			std::cin >> one_thou;
			std::cout << "5000원 몇 장 넣겠습니까? ";
			std::cin >> five_thou;
			std::cout << "10000원 몇 장 넣겠습니까? ";
			std::cin >> ten_thou;
			std::cout << "50000원 몇 장 넣겠습니까? ";
			std::cin >> fifty_thou;
			money_sum = one_thou * 1000 + five_thou * 5000 + ten_thou * 10000 + fifty_thou * 50000;
			if (money_sum != money + 5000) {
				std::cout << "요청하신 금액과 지불하신 현금 액수가 다릅니다." << endl;
				return;
			}
			change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
			curr_bank->cash_transfer_bank(dest_bank, dest_account, money);
			std::cout << "이체가 완료되었습니다." << endl;
			transaction_history* th = new transaction_history(get_card_insert_slot(), "현금_계좌이체", money, dest_bank, dest_bank->account_get_accountnum(dest_account));
			change_temp(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}

		else if (c1 == 2) {    // 계좌끼리 계좌이체
			std::cout << "돈을 받을 계좌의 계좌번호를 입력하세요: ";
			std::cin >> dest_account;
			dest_bank = find_bank_by_account(dest_account);
			if (!valid_find_bank_by_account(dest_account)) {
				std::cout << "입력하신 계좌번호의 계좌가 존재하지 않습니다." << endl;
				return;
			}
			if (dest_bank != get_primary_bank()) {
				cout << "주 은행 계좌가 아닙니다." << endl;
				return;
			}
			std::cout << "본인의 계좌번호를 입력하세요: ";
			std::cin >> src_account;
			src_bank = find_bank_by_account(src_account);
			if (!valid_find_bank_by_account(src_account)) {
				std::cout << "입력하신 계좌번호의 계좌가 존재하지 않습니다." << endl;
				return;
			}
			if (src_bank != get_primary_bank()) {
				cout << "주계좌 은행이 아닙니다." << endl;
				return;
			}
			if (u_name != src_bank->account_get_accountnum(src_account)->get_user_name() or src_bank != curr_bank) {
				std::cout << "해당 계좌에서 돈을 이체할 수 없습니다." << endl;
				return;
			}

			if (src_bank == get_primary_bank() && dest_bank == get_primary_bank()) { fee = 2000; }
			else if ((src_bank != get_primary_bank() && dest_bank == get_primary_bank()) or (src_bank == get_primary_bank() && dest_bank != get_primary_bank())) { fee = 3000; }
			else if (src_bank != get_primary_bank() && dest_bank != get_primary_bank()) { fee = 4000; }

			std::cout << "송금하실 금액을 알려주세요: ";
			std::cin >> money;
			if (!curr_bank->cn_enough_money_in_acc_bank(get_card_insert_slot(), money + fee)) {
				std::cout << "계좌에 돈이 충분하지 않습니다.";
				return;
			}
			curr_bank->account_transfer_bank(src_account, dest_bank, dest_account, money, fee);
			std::cout << money << "+" << fee << " 원이 계좌에서 출금되었습니다." << endl;
			cout << "계좌이체가 완료되었습니다." << endl << endl;
			transfer_acc_transaction_history* th = new transfer_acc_transaction_history(get_card_insert_slot(), "계좌_계좌이체", money, src_bank, dest_bank, src_bank->account_get_accountnum(src_account), dest_bank->account_get_accountnum(dest_account));
			change_transfer_temp(th);
			src_bank->account_get_accountnum(src_account)->change_account_tr(th);
			dest_bank->account_get_accountnum(dest_account)->change_account_tr(th);
			return;
		}
		else {
			std::cout << "1과 2중에 고르세요." << endl;
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
		std::cout << "더 출금하시려면 세션을 초기화 하셔야 합니다." << endl;
		return;
	}
	if (b != get_primary_bank()) {
		std::cout << "타 은행 출금에는 수수료 2000원이 발생합니다." << endl;
		flag = 1;
	}
	else {
		std::cout << "자 은행 출금에는 수수료 1000원이 발생합니다." << endl;
	}
	while (1) {
		std::cout << "얼마를 출금하시겠습니까?(수수료 제외) (종료하려면 -1을 입력하세요)";
		std::cin >> check_num;
		if (check_num == -1) {
			return;
		}
		std::cout << endl;
		if (check_num > 500000) {//출금 가능한 금액을 초과하였는지 조사
			std::cout << "출금 가능하신 금액을 초과하셨습니다" << endl;
		}
		else if (check_num % 1000 != 0) {
			std::cout << "화폐 최소 단위는 1000원 입니다" << endl;
		}
		else if (get_atm_cash() < check_num) {
			std::cout << "ATM에 충분한 현금이 존재하지 않습니다" << endl;
		}
		else if (!(b->cn_enough_money_in_acc_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000))))) {
			std::cout << "계좌에 돈이 충분하지 않습니다" << endl;
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
				std::cout << "ATM에 지폐의 개수가 부족합니다" << endl;
			}
			else {
				//atm_cash 조정
				change_atm_cash(-1 * (one_thou), -1 * (five_thou), -1 * (ten_thou), -1 * (fifty_thou));
				b->withdrawl_bank(get_card_insert_slot(), (flag ? (check_num + 2000) : (check_num + 1000)));
				std::cout << "은행에서 " << (flag ? (check_num + 2000) : (check_num + 1000)) << "원이 출금되었고 가져가실 현금은 총 " << check_num << "원 입니다." << endl;
				std::cout << "천원 " << one_thou << "장 / " << "오천원 " << five_thou << "장 / " << "만원 " << ten_thou << "장 / " << "오만원 " << fifty_thou << "장" << endl;
				std::cout << "잔액은 " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
				set_number_of_withdrawl(get_number_of_withdrawl() + 1);
				transaction_history* th = new transaction_history(get_card_insert_slot(), "출금", check_num, b, b->account_get_account(get_card_insert_slot()));
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
		std::cout << "타 은행 입금에는 수수료 1000원이 발생합니다. 수수료를 포함해서 입금하세요" << endl;
	}
	while (true) {
		std::cout << "천원을 몇 장 넣으시겠습니니까? (종료하려면 -1을 입력하세요)";
		std::cin >> one_thou;
		if (one_thou == -1) {
			return;
		}
		std::cout << "오천원을 몇 장 넣으시겠습니니까? ";
		std::cin >> five_thou;
		std::cout << "만원을 몇 장 넣으시겠습니니까? ";
		std::cin >> ten_thou;
		std::cout << "오만원을 몇 장 넣으시겠습니니까? ";
		std::cin >> fifty_thou;
		if (one_thou + five_thou + ten_thou + fifty_thou > 50) {
			std::cout << "가능한 현금 장 수를 초과하셨습니다." << endl;
		}
		else { break; }
	}
	while (1) {
		std::cout << "수표를 몇 장 넣으시겠습니까? ";
		std::cin >> check_num;
		std::cout << endl;
		if (check_num > 30) {
			std::cout << "가능한 수표 장 수를 초과하셨습니다." << endl;
		}
		else { break; }
	}
	while (1) {
		int flag = 0; //flag = 0 일때 정상
		check_sum = 0;
		for (int i = 0; i < check_num; i++) {
			long long int temp = 0;
			std::cout << i + 1 << "번째 수표의 금액은 얼마입니까? ";
			std::cin >> temp;
			if (temp < 100000 || temp > 9999999999) { // 100억 이상은 불가
				flag = 1; //flag = 1이면 에러 발생
				break;
			}
			else {
				check_sum += temp;
			}
		}
		if (flag == 1) {
			std::cout << "잘못된 수표 금액 입니다." << endl;
		}
		else { break; }
	}
	change_atm_cash(one_thou, five_thou, ten_thou, fifty_thou);
	long long int m = 1000 * one_thou + 5000 * five_thou + 10000 * ten_thou + 50000 * fifty_thou + check_sum;
	if (b != get_primary_bank()) { m = m - 1000; }
	b->deposit_bank(get_card_insert_slot(), m);
	std::cout << m << "원이 입금되었습니다." << endl;
	std::cout << "잔액은 " << b->account_get_account(get_card_insert_slot())->get_acc_cash() << endl;
	transaction_history* th;
	th = new transaction_history(get_card_insert_slot(), "입금", m, b, b->account_get_account(get_card_insert_slot()));
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
	int number;//추가
main1:
	std::cout << "카드를 넣어주세요.(카드번호를 입력해주세요). 종료하고 싶으시면 -1을 입력해주세요.";
	std::cin >> number; //변경
	set_card_insert_slot(number);//추가
	if (get_card_insert_slot() == -1) {
		return;
	}
	if (get_card_insert_slot() == 7777777) {
		admin_interface_korean();
		goto end;
	}
	if (find_cardnumber_bank(get_card_insert_slot()) != get_primary_bank()) {                                       // 윤현서가 추가한 부분(정세훈 확인필요)
		cout << " 주은행 계좌가 아닙니다." << endl;
		return;
	}
	if (valid_cardnumber_account(get_card_insert_slot()) == 0) {
		cout << "유효하지 않은 카드 번호입니다. " << endl;
		goto main1;
	}
	while (1) {
		if (password_count == 3) {
			break;
		}
		std::cout << "비밀번호를 입력해주세요." << endl;
		std::cin >> password;
		if (check_password(get_card_insert_slot(), password)) {
			break;
		}
		cout << "비밀번호가 틀립니다." << endl;
		password_count++;
	}
	if (password_count == 3) {
		cout << "비밀번호를 3번 틀리셨습니다. " << endl;
		goto end;//셰션 end
	}
	while (1) {
		std::cout << "어떤 업무를 하시겠습니까? 번호를 기입하여 주세요." << endl;
		std::cout << "1. 입금 " << "2. 출금 " << "3. 이체 " << "4. 세션 종료 :  ";
		std::cin >> work_type;
		if (work_type == 1) { koreandeposit(); }
		else if (work_type == 2) { koreanwithdrawl(); }
		else if (work_type == 3) { koreantransfer(); }
		else if (work_type == 4) { break; }
		else { std::cout << "잘못된 메뉴를 선택하셨습니다." << endl; }
	}
end:
	cout << "세션을 종료합니다." << endl;
	korean_print_session_history();
	decrease_session();
}


bool check_atm_serial_number(int number) { // atm serial number 사용할 수 있는지 확인하는 함수
	int size = atmlist.size();
	for (int i = 0; i < size; i++) {
		if (number == atmlist[i]->get_serial_number()) {
			return false; //이미 존재하면 false
		}
	}
	return true; //존재하지않으면 사용할수 있으면 true
}

int main() {
	int number; // 변수
	int size; // 변수
	int banknumber; // 은행 어디 선택했는지.
	string bank_name; // 은행 이름
	string user_name; // 유저 이름
	int initial_money = 0; // 돈 초기화
	int password; //비밀번호
	int type; //타입 번호
	int language_type;// 언어 타입 번호
	int cardnumber; //카드 번호
	int password_count = 0;//비밀번호를 얼마나 틀렸는지.
	long long int accountnumber;
	int count = 0;
	int number_of_atm;
	int test_num;
	int atm_serial_number;
main1:	//메인 1 은행만들기 or 은행 선택하기
	std::cout << "어떤 업무를 하시겠습니까? 번호를 기입하여 주세요." << endl;
	std::cout << "1. 은행 만들기" << " " << "2. 은행 선택하기" << " " << "3. 정보 출력하기 :  ";
	std::cin >> number;
	if (number == 1) {
		std::cout << "<은행 만들기>" << endl;
		std::cout << "은행 이름을 기입하여 주세요. 이전 화면으로 돌아가고자 하면 back을 입력해주세요 :  ";
		std::cin >> bank_name;
		if (bank_name == "back") { goto main1; } // 이전 화면 돌아가기
		else {
			new bank(bank_name);
			goto main1;
		}
	}
	else if (number == 2) {
		std::cout << "<은행 선택하기>" << endl;
		std::cout << "가고자 하는 은행 번호를 입력해주세요. 이전 화면으로 돌아가고자 하면 -1을 입력해주세요." << endl;
		size = banklist.size();
		for (int i = 0; i < size; i++) {
			std::cout << i << "." << banklist[i]->get_bankname() << " "; // ex) 0. 대구은행
		}
		std::cout << endl;
	sub_main_1_2:
		std::cin >> banknumber; // banklist의 index를 입력 받은 것임.
		if (banknumber == -1) { goto main1; } // 이전 화면 돌아가기
		else if (0 <= banknumber and banknumber < size) {
			goto main2;
		}
		else {
			cout << "잘못된 번호를 입력하셨습니다. 다시 입력해주세요. " << endl;
			goto sub_main_1_2;
		} //
	}
	else if (number == 3) {
		show_information(); goto main1;
	}
	else {
		std::cout << "번호를 잘못 입력하셨습니다." << endl;
		goto main1;
	}
main2: // 은행 계좌 만들기 or  ATM 만들기 or ATM 선택하기
	std::cout << "어떤 업무를 하시겠습니까? 번호를 기입하여 주세요." << endl;
	std::cout << "1. 은행 계좌 만들기  " << "2. ATM 만들기  " << "3. ATM 이용하기  " << "4. 정보 출력하기  " << "5. 계좌 정보 접근" << "6. 이전 화면으로 돌아가기  :  ";
	std::cin >> number;
	if (number == 1) {
	sub_main2_0://은행 계좌 만들기
		std::cout << "<은행 계좌 만들기>" << endl;
		std::cout << "당신의 이름을 입력해주세요 :  ";
		std::cin >> user_name;
		std::cout << "계좌번호를 입력해주세요 : ";
		std::cin >> accountnumber;
		if ((accountnumber < 99999999999) or (accountnumber > 1000000000000)) {
			cout << "계좌번호 양식이 잘못되었습니다. 12자리로 입력해주세요." << endl;
			goto sub_main2_0;
		}
		for (int i = 0; i < banklist.size(); i++) {
			if (banklist[i]->bool_accountnumber_in_bank(accountnumber) == true) {
				cout << "이미 해당 계좌번호가 존재합니다! " << endl;
				goto sub_main2_0;
			}
		}
		//은행은 banklist[banknumber] 임.
		std::cout << "계좌에 가능한 돈이 얼마인지를 입력해주세요  :  ";
		std::cin >> initial_money;
		std::cout << "계좌의 비밀번호는 무엇으로 할지 입력해주세요  :  ";
		std::cin >> password;
		banklist[banknumber]->make_account(accountnumber, user_name, password, initial_money);
		goto main2;
	}
	else if (number == 2) { //ATM 만들기
		std::cout << "<ATM 만들기>" << endl;
	sub_main_2_1:
		std::cout << "ATM의 SERIAL NUMBER을 입력해주세요  ";
		cin >> atm_serial_number;
		if (atm_serial_number < 100000 or atm_serial_number>999999) {
			cout << "잘못된 ATM SERIAL NUMBER의 유형입니다. 6자리로 입력해주세요." << endl;
			goto sub_main_2_1;
		}
		if (!check_atm_serial_number(atm_serial_number)) {
			cout << "이미 존재하는 ATM SERIAL NUMBER 입니다. 다시 입력해주세요." << endl;
			goto sub_main_2_1;
		}
	sub_main_2_2:
		std::cout << "ATM type에 해당하는 번호를 입력해주세요" << endl;
		std::cout << "0. Single Bank ATM ";
		std::cout << "1. Multi-Bank ATM   ";
		std::cin >> type;
		if (type != 1 and type != 0) {
			cout << "잘못된 번호입니다." << endl;
			goto sub_main_2_2;
		}
	sub_main_2_3:
		std::cout << "ATM의 언어 지원 여부에 해당하는 번호를 입력해주세요" << endl;
		std::cout << "0. Unilingual ";
		std::cout << "1. Bilingual  ";
		std::cin >> language_type;
		if (language_type != 1 and language_type != 0) {
			cout << "잘못된 번호입니다." << endl;
			goto sub_main_2_3;
		}
		std::cout << "돈은 얼마나 들어있게 할까요?" << endl;
		int one_thou = 0;
		int five_thou = 0;
		int ten_thou = 0;
		int fifty_thou = 0;
		int check_num = 0;
		std::cout << "천원을 몇 장 넣으시겠습니니까? ";
		std::cin >> one_thou;
		std::cout << "오천원을 몇 장 넣으시겠습니니까? ";
		std::cin >> five_thou;
		std::cout << "만원을 몇 장 넣으시겠습니니까? ";
		std::cin >> ten_thou;
		std::cout << "오만원을 몇 장 넣으시겠습니니까? ";
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
	else if (number == 3) {// ATM 선택하기
		// atm 선택하기. 어떤 atm을 선택하시겠습니까? 과정 필요.
	sub_main2:
		std::cout << "<ATM 선택하기>" << endl;
		std::cout << "선택하고 싶은 atm의 번호를 기입해주세요. 이전으로 가고 싶으시면 -1을 입력해주세요." << endl;
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
			cout << "잘못된 ATM 번호입니다. 다시 입력해주세요." << endl;
			goto sub_main2;
		}

	}
	else if (number == 4) {
		show_information(); goto main2;
	}
	else if (number == 5) {
		std::cout << "계좌번호를 입력해주세요 : ";
		std::cin >> accountnumber;
		if (banklist[banknumber]->bool_accountnumber_in_bank(accountnumber)) {
			banklist[banknumber]->account_get_accountnum(accountnumber)->korean_print_account_history();
		}
		else {
			cout << "계좌번호가 은행에 존재하지 않습니다." << endl;
		}
		goto main2;
	}
	else if (number == 6) {
		goto main1;
	}
	else {
		cout << "번호를 잘못 입력하셨습니다." << endl;
		goto main2;
	}
main3://ATM 이용하기
	(banklist[banknumber]->get_atm_list())[number_of_atm]->mini_interface();
	goto main1;
}