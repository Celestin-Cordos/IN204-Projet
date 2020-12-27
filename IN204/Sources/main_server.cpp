#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
//#include <conio.h>
#include <curses.h>
#include <vector>
#include "network.hpp"
using namespace std;

int main() {
    string password = " ";
    //ask for password to login as server
    while (password != "tetris") {
        cout << "Enter password : ";
        cin >> password;
        if (password != "tetris") {
            cout << "Wrong password! Please try again." << endl;
            cout << endl;
        }
    }

    cout << "Server Running" << endl;

    sf::TcpListener listener;
    sf::SocketSelector selector;
    bool done = false;
    vector<sf::TcpSocket*> clients;

    listener.listen(8080);
    selector.add(listener);

    while (!done) {
        if (selector.wait()) {
            if (selector.isReady(listener)) {
                sf::TcpSocket *socket = new sf::TcpSocket;
                listener.accept(*socket);
                sf::Packet packet;
                string id;
                if (socket->receive(packet) == sf::Socket::Done) {
                    packet >> id;
                }

                cout << id << " has connected to the game" << endl;
                clients.push_back(socket);
                selector.add(*socket);
            }
            else {
                for (int i=0; i< clients.size(); i++) {
                    //send order to play
                    //wait for player status
                    //send status to all other players
                    if (selector.isReady(*clients[i])) {
                        sf::Packet packet, sendPacket;
                        if (clients[i]->receive(packet) == sf::Socket::Done) {
                            string text;
                            packet >> text;
                            cout << text << endl;
                            sendPacket << text;
                            for (int j=0; j<clients.size(); j++) {
                                if (i != j) {
                                    clients[j]->send(sendPacket);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    //delete sockets
    for(vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++) {
        delete *it;
    }

    return 0;
}