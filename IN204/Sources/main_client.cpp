#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <vector>
//#include <conio.h>
#include <curses.h>
#include "network.hpp"
using namespace std;

int main() {
	/*
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	//tcp connexion : needs a connexion to work
	sf::TcpSocket socket;
	char connectionType, mode;
	char buffer[2000];
	size_t received;
	string text = "Connected to: ";

	cout<< "Enter (s) for server, Enter (c) for client: " << endl;
	cin >> connectionType;

	if (connectionType == 's') {
		sf::TcpListener listener;
		//if connect to port 8080
		listener.listen(8080);
		//accept socket
		listener.accept(socket);
		text += "Server";
		//mode s for send
		mode = 's';
		
	}
	else if (connectionType == 'c') {
		socket.connect(ip, 8080);
		text += "Client";
		mode = 'r';

	}
	socket.send(text.c_str(), text.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);
	cout << buffer << endl;

	bool done = false;

	while (!done) {
		if (mode == 's') {
			getline(cin, text);
			socket.send(text.c_str(), text.length() + 1);
			mode = 'r';
		}
		else if (mode == 'r') {
			socket.receive(buffer, sizeof(buffer), received);
			if (received > 0) {
				cout << "Received: " << buffer << endl;
			mode = 's';
			}	
		}
	}

	system("pause");*/

	/*Client player;

    bool done = false;
	string text;
	while (!done) {
		getline(cin, text);
		player.send(text);
	}*/

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	bool done = false;
	string id;
	string text = "";

	cout << "Enter online id: ";
	std::cin >> id;

	socket.connect(ip, 8080);

	sf::Packet packet;
	packet << id;
	socket.send(packet);
	socket.setBlocking(false);

	while (!done) {
		//if rece
		//receive order to play
		//send status
		getline(cin, text);
		sf::Packet packet;
		packet << id + ": " + text;
		socket.send(packet);
		text = "";

		sf::Packet receivePacket;
		socket.receive(receivePacket);
		
		string temptext;
		if (receivePacket >> temptext) {
			cout << temptext;
		}
	}

	

	return 0;
}