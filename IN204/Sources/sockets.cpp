#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
using namespace std;

//THIS IS A PRIMITIVE VERSION :
//SERVER - CLIENT
//WITH ONLY ONE CLIENT

int main() {
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

	system("pause");

	return 0;
}