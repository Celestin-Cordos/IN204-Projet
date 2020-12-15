#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
using namespace std;

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
		//if connect to port 2000
		listener.listen(2000);
		//accept socket
		listener.accept(socket);
		text += "Server";
		
	}
	else if (connectionType == 'c') {
		socket.connect(ip, 2000);
		text += "Client";

	}
	socket.send(text.c_str(), text.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);
	cout << buffer << endl;

	system("pause");

	return 0;
}