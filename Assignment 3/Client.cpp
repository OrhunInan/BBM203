//
// Created by alperen on 27.09.2023.
//

#include "Client.h"

Client::Client(string const& _id, string const& _ip, string const& _mac) {
    client_id = _id;
    client_ip = _ip;
    client_mac = _mac;
}

ostream &operator<<(ostream &os, const Client &client) {
    os << "client_id: " << client.client_id << " client_ip: " << client.client_ip << " client_mac: "
       << client.client_mac << endl;
    return os;
}

Client::~Client() {

    stack<Packet*> packet_stack;

    while (!outgoing_queue.empty()) {

        packet_stack = outgoing_queue.front();
        outgoing_queue.pop();
        while (!packet_stack.empty()) {

            Packet* packet = packet_stack.top();
            packet_stack.pop();
            delete packet;
        }
    }

    while (!incoming_queue.empty()) {

        packet_stack = incoming_queue.front();
        incoming_queue.pop();
        while (!packet_stack.empty()) {

            Packet* packet = packet_stack.top();
            packet_stack.pop();
            delete packet;
        }
    }
}