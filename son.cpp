#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include <conio.h>  // getch için (Windows)

using namespace std;

// Hash simulasyonu
string fakeHash(string input) {
	int toplam = 0;
	for (char c : input) {
		
		toplam += (int)c;
	}
	stringstream ss;
	ss << "HASH" << toplam * 17;
	return ss.str();
}

// Gizli girdi alma (şifreyi * ile göster)
string getHiddenInput() {
	string input;
	char ch;
	while ((ch = _getch()) != '\r') { // Enter tusuna kadar
		if (ch == '\b') { // Backspace
			if (!input.empty()) {
				cout << "\b \b";
				input.pop_back();
			}
		} else {
			input += ch;
			cout << '*';
		}
	}
	cout << endl;
	return input;
}

// Biyometrik dogrulama
bool verifyBiometric(string input, string stored_hash) {
	return fakeHash(input) == stored_hash;
}

// Basit sifreleme
string encryptVote(string vote) {
	reverse(vote.begin(), vote.end());
	return vote;
}

// Basit sifre cozme
string decryptVote(string encrypted) {
	reverse(encrypted.begin(), encrypted.end());
	return encrypted;
}

// Oy dosyasina yaz (şifreli oy ve il birlikte)
void saveVoteToFile(const string& encrypted_vote, const string& city) {
	ofstream file("votes.txt", ios::app);
	if (file.is_open()) {
		file << city << "|" << encrypted_vote << endl;
		file.close();
	} else {
		cout << "Oy dosyasina yazilamadi!" << endl;
	}
}

// Sonuclari illere göre göster
void showResultsByCity() {
	ifstream file("votes.txt");
	if (!file.is_open()) {
		cout << "Oy dosyasi bulunamadi!" << endl;
		return;
	}
	
	string line;
	map<string, map<string, int>> cityVotes;
	
	while (getline(file, line)) {
		size_t sep = line.find('|');
		if (sep != string::npos) {
			string city = line.substr(0, sep);
			string enc_vote = line.substr(sep + 1);
			string vote = decryptVote(enc_vote);
			cityVotes[city][vote]++;
		}
	}
	
	if (cityVotes.empty()) {
		cout << "Henuz hic oy verilmemis." << endl;
		return;
	}
	
	cout << "\n--- Oylama Sonuclari (Illere Gore) ---" << endl;
	for (const auto& cityPair : cityVotes) {
		cout << "\nIl: " << cityPair.first << endl;
		for (const auto& votePair : cityPair.second) {
			cout << "  Aday: " << votePair.first << " - Oy: " << votePair.second << endl;
		}
	}
}

// Ana program
int main() {
	string correctUsername = "admin";
	string correctPassword = "123";
	
	cout << "Kullanici Adi: ";
	string username;
	cin >> username;
	
	cout << "Sifre: ";
	string password = getHiddenInput();
	
	if (username != correctUsername || password != correctPassword) {
		cout << "X Kullanici adi veya sifre hatali! Programdan cikiliyor..." << endl;
		return 0;
	}
	
	cout << "Giris basarili!\n";
	
	string stored_hash = fakeHash("123");
	
	while (true) {
		cout << "\n--- Menu ---" << endl;
		cout << "1. Oy Ver" << endl;
		cout << "2. Sonuclari Gor" << endl;
		cout << "3. Cikis" << endl;
		int choice;
		cout << "Seciminiz: ";
		cin >> choice;
		
		if (choice == 1) {
			cout << "Biyometrik veri giriniz (ornegin: 123): ";
			string bio = getHiddenInput();
			
			if (!verifyBiometric(bio, stored_hash)) {
				cout << "X Biyometrik dogrulama basarisiz!" << endl;
				continue;
			}
			
			string city;
			cout << "Secmen ili: ";
			cin >> city;
			
			string vote;
			cout << "Aday ismi giriniz: ";
			cin >> vote;
			
			string encrypted_vote = encryptVote(vote);
			saveVoteToFile(encrypted_vote, city);
			
			cout << "Oy basariyla kaydedildi!" << endl;
			
		} else if (choice == 2) {
			showResultsByCity();
		} else if (choice == 3) {
			cout << "Programdan cikiliyor..." << endl;
			break;
		} else {
			cout << "Gecersiz secim!" << endl;
		}
	}
	return 0;
}

