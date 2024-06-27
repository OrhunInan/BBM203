#include "Network.h"
#include <stack>

Network::Network() = default;

//returns a timestamp in "YYYY-MM-DD hh:mm:ss" format
string get_timestamp() {

    time_t now = time(nullptr);
    tm * ptm = std::localtime(&now);
    char timestamp[32];
    strftime(timestamp, 32, "%Y-%m-%d %H:%M:%S", ptm);

    return timestamp;
}

//checks if the last character of the string is an end character
bool contains_message_end(ApplicationLayerPacket* &packet) {


    return (packet->message_data[packet->message_data.length() - 1] == '.' or
                    packet->message_data[packet->message_data.length() - 1] == '!' or
                    packet->message_data[packet->message_data.length() - 1] == '?');
}

void message(vector<Client> &clients, stringstream &args, int &message_limit, string const &sender_port,
             string const &receiver_port) {

    //getting arguments
    string from, to, message;
    args >> from >> to;
    getline(args, message);
    message = message.substr(2,message.length() - 3);

    cout << "Message to be sent: \"" << message << "\"\n\n";

    Client * sender = nullptr;
    Client * receiver = nullptr;
    Client * next_hop = nullptr;
    for (Client &client : clients) {

        if (client.client_id == from) sender = &client;
        if (client.client_id == to) receiver = &client;
    }

    string next_hop_id = sender->routing_table[to];
    for (Client &client : clients) {

        if (client.client_id == next_hop_id) next_hop = &client;
    }

    //splits message into frames and pushes them to outgoing queue
    int i(0);
    for (; i < message.length(); i += message_limit) {

        string message_portion;
        stack<Packet*> frame;
        message_portion = message.substr(i);
        if(message_portion.length() > message_limit) message_portion = message_portion.substr(0,message_limit);


        frame.push(new ApplicationLayerPacket(0, sender->client_id, receiver->client_id,
                                              message_portion));
        frame.push(new TransportLayerPacket(1, sender_port, receiver_port));
        frame.push(new NetworkLayerPacket(2, sender->client_ip, receiver->client_ip));
        frame.push(new PhysicalLayerPacket(3, sender->client_mac, next_hop->client_mac));

        sender->outgoing_queue.push(frame);


        cout << "Frame #" << (i / message_limit) + 1 << endl
        << "Sender MAC address: " << sender->client_mac << ", Receiver MAC address: " << next_hop->client_mac << endl
        << "Sender IP address: " << sender->client_ip << ", Receiver IP address: " << receiver->client_ip << endl
        << "Sender port number: " << sender_port << ", Receiver port number: " << receiver_port << endl
        << "Sender ID: " << sender->client_id << ", Receiver ID: " << receiver->client_id << endl
        << "Message chunk carried: \""<< message_portion <<"\"\n"
        << "Number of hops so far: 0\n"
        << "--------\n";
    }

    Log log(get_timestamp(), message, (i/message_limit), 0, sender->client_id,
            receiver->client_id, (receiver != nullptr), ActivityType::MESSAGE_SENT);
    sender->log_entries.push_back(log);

}

void show_frame_info(vector<Client> &clients, stringstream &args) {

    string id, queue_direction;
    int index;
    args >> id >> queue_direction >> index;

    Client* client;
    queue<stack<Packet*>> selected_queue;
    for (Client &c : clients) {

        if (c.client_id == id) client = &c;
    }
    if(queue_direction == "in") selected_queue = client->incoming_queue;
    if(queue_direction == "out") selected_queue = client->outgoing_queue;

    if (selected_queue.size() <= index ) {

        cout << "No such frame.\n";
    }
    else{

        for (int i = 0; i < index - 1; ++i) {
            selected_queue.pop();
        }
        stack<Packet*>* selected_stack = &selected_queue.front();

        auto* physical = dynamic_cast<PhysicalLayerPacket*>(selected_stack->top());
        selected_stack->pop();
        auto* network = dynamic_cast<NetworkLayerPacket*>(selected_stack->top());
        selected_stack->pop();
        auto* transport = dynamic_cast<TransportLayerPacket*>(selected_stack->top());
        selected_stack->pop();
        auto* application = dynamic_cast<ApplicationLayerPacket*>(selected_stack->top());


        cout << "Current Frame #" << index << " on the ";
        if(queue_direction == "out") cout << "outgoing";
        else cout << "incoming";

        cout << " queue of client " << id << endl
        << "Carried Message: \"" << application->message_data << "\"\n"
        << "Layer 0 info: Sender ID: " << application->sender_ID
        << ", Receiver ID: " << application->receiver_ID << endl
        << "Layer 1 info: Sender port number: " << transport->sender_port_number
        << ", Receiver port number: " << transport->receiver_port_number << endl
        << "Layer 2 info: Sender IP address: " << network->sender_IP_address
        << ", Receiver IP address: " << network->receiver_IP_address << endl
        << "Layer 3 info: Sender MAC address: " << physical->sender_MAC_address
        << ", Receiver MAC address: " << physical->receiver_MAC_address << endl
        << "Number of hops so far: " << physical->num_of_hops << endl;

    }

}

void show_q_info(vector<Client> &clients, stringstream &args) {

    string id, queue_direction;
    args >> id >> queue_direction;

    Client* client;
    for (Client &c : clients) {

        if (c.client_id == id) client = &c;
    }

    if(queue_direction == "in") {

        cout << "Client " << id << " Incoming Queue Status\n"
        << "Current total number of frames: " << client->incoming_queue.size() << endl;
    }
    else {
        cout << "Client " << id << " Outgoing Queue Status\n"
        << "Current total number of frames: " << client->outgoing_queue.size() << endl;
    }


}

void send(vector<Client> &clients){

    for (Client &sender : clients) {

        int i(0);
        stack<Packet*> frame;
        PhysicalLayerPacket* physical = nullptr;
        NetworkLayerPacket* network = nullptr;
        TransportLayerPacket* transport = nullptr;
        ApplicationLayerPacket* application = nullptr;
        while (!sender.outgoing_queue.empty()){

            frame = sender.outgoing_queue.front();
            sender.outgoing_queue.pop();
            physical = dynamic_cast<PhysicalLayerPacket *>(frame.top());
            i++;

            for (Client &receiver: clients) {

                if (physical->receiver_MAC_address == receiver.client_mac) {

                    physical->num_of_hops++;
                    receiver.incoming_queue.push(frame);
                    frame.pop();
                    network = dynamic_cast<NetworkLayerPacket*>(frame.top());
                    frame.pop();
                    transport = dynamic_cast<TransportLayerPacket*>(frame.top());
                    frame.pop();
                    application = dynamic_cast<ApplicationLayerPacket*>(frame.top());

                    cout << "Client " << sender.client_id << " sending frame #" << i << " to client "
                    << receiver.client_id << endl << "Sender MAC address: " << sender.client_mac
                    << ", Receiver MAC address: " << receiver.client_mac << endl << "Sender IP address: "
                    << network->sender_IP_address << ", Receiver IP address: " << network->receiver_IP_address
                    << endl << "Sender port number: " << transport->sender_port_number << ", Receiver port number: "
                    << transport->receiver_port_number << endl << "Sender ID: " << application->sender_ID
                    << ", Receiver ID: " << application->receiver_ID << endl << "Message chunk carried: \""
                    << application->message_data << "\"\n" << "Number of hops so far: " << physical->num_of_hops
                    << endl << "--------\n";

                    if (contains_message_end(application)) i = 0;
                    break;
                }
            }
        }
    }
}

void receive(vector<Client> &clients) {

    string sender_id, message;
    stack<Packet*> frame;
    PhysicalLayerPacket* physical = nullptr;
    NetworkLayerPacket* network = nullptr;
    TransportLayerPacket* transport = nullptr;
    ApplicationLayerPacket* application = nullptr;
    Client* next_hop = nullptr;
    for (Client &receiver : clients) {

        int i(0);
        while (!receiver.incoming_queue.empty()) {

            i++;
            frame = receiver.incoming_queue.front();

            physical = dynamic_cast<PhysicalLayerPacket*>(frame.top());
            frame.pop();
            network = dynamic_cast<NetworkLayerPacket*>(frame.top());
            frame.pop();
            transport = dynamic_cast<TransportLayerPacket*>(frame.top());
            frame.pop();
            application = dynamic_cast<ApplicationLayerPacket*>(frame.top());

            message += application->message_data;


            if (network->receiver_IP_address == receiver.client_ip) {//if the message arrived at its destination

                sender_id = "";
                for (Client &c : clients) {

                    if (c.client_mac == physical->sender_MAC_address) sender_id = c.client_id;
                }

                cout << "Client " << receiver.client_id << " receiving frame #" << i << " from client " << sender_id
                << ", originating from client " << application->sender_ID << endl << "Sender MAC address: "
                << physical->sender_MAC_address << ", Receiver MAC address: " << physical->receiver_MAC_address << endl
                << "Sender IP address: " << network->sender_IP_address << ", Receiver IP address: "
                << network->receiver_IP_address << endl << "Sender port number: " << transport->sender_port_number
                << ", Receiver port number: " << transport->receiver_port_number << endl << "Sender ID: "
                << application->sender_ID << ", Receiver ID: " << application->receiver_ID << endl
                << "Message chunk carried: \"" << application->message_data << "\"\n" << "Number of hops so far: "
                << physical->num_of_hops << "\n--------\n";

                if (contains_message_end(application)) {

                    Log log(get_timestamp(), message, i, physical->num_of_hops, application->sender_ID,
                            application->receiver_ID, true, ActivityType::MESSAGE_RECEIVED);
                    receiver.log_entries.push_back(log);

                    cout << "Client " << receiver.client_id << " received the message \"" << message
                    << "\" from client " << application->sender_ID << ".\n--------\n";


                    i = 0;
                    message = "";
                }

                delete physical;
                delete network;
                delete transport;
                delete application;
                receiver.incoming_queue.pop();
            }
            else {

                sender_id = "";
                next_hop = nullptr;
                for (Client &c : clients) {

                    if (c.client_mac == physical->sender_MAC_address) sender_id = c.client_id;
                    if (receiver.routing_table[application->receiver_ID] == c.client_id) next_hop = &c;
                }


                if (next_hop == nullptr) {//if destination can't be reached

                    cout << "Client " << receiver.client_id << " receiving frame #" << i << " from client " << sender_id
                    << ", but intended for client " << application->receiver_ID
                    << ". Forwarding... \nError: Unreachable destination. Packets are dropped after "
                    << physical->num_of_hops << " hops!\n";

                    if (contains_message_end(application)) {

                        Log log(get_timestamp(), message, i, physical->num_of_hops,
                                application->sender_ID, application->receiver_ID, false,
                                ActivityType::MESSAGE_DROPPED);
                        receiver.log_entries.push_back(log);

                        i = 0;
                        message = "";
                        cout << "--------\n";
                    }

                    delete physical;
                    delete network;
                    delete transport;
                    delete application;
                    receiver.incoming_queue.pop();
                }
                else {
                    if (i == 1)
                        cout << "Client " << receiver.client_id << " receiving a message from client "
                             << sender_id << ", but intended for client " << application->receiver_ID
                             << ". Forwarding... \n";

                    cout << "Frame #" << i << " MAC address change: New sender MAC " << receiver.client_mac
                         << ", new receiver MAC " << next_hop->client_mac << endl;

                    frame = receiver.incoming_queue.front();
                    receiver.incoming_queue.pop();
                    dynamic_cast<PhysicalLayerPacket *>(frame.top())->sender_MAC_address = receiver.client_mac;
                    dynamic_cast<PhysicalLayerPacket *>(frame.top())->receiver_MAC_address = next_hop->client_mac;
                    receiver.outgoing_queue.push(frame);


                    if (contains_message_end(application)) {

                        Log log(get_timestamp(), message, i, physical->num_of_hops, application->sender_ID,
                                application->receiver_ID, true, ActivityType::MESSAGE_FORWARDED);
                        receiver.log_entries.push_back(log);

                        i = 0;
                        message = "";
                        cout << "--------\n";
                    }
                }
            }
        }
    }
}

void print_log(vector<Client> &clients, stringstream &args) {

    string client_id;
    int i(0);

    args >> client_id;

    for (Client &client : clients){

        if(client.client_id == client_id and !client.log_entries.empty()) {

            cout << "Client " << client_id << " Logs:\n";


            for (Log &log : client.log_entries) {

                cout << "--------------\nLog Entry #" << ++i << ":\nActivity: ";
                if (log.activity_type == ActivityType::MESSAGE_RECEIVED) cout << "Message Received\n";
                else if (log.activity_type == ActivityType::MESSAGE_FORWARDED) cout << "Message Forwarded\n";
                else if (log.activity_type == ActivityType::MESSAGE_SENT) cout << "Message Sent\n";
                else cout << "Message Dropped\n";
                cout << "Timestamp: " << log.timestamp << "\nNumber of frames: " <<  log.number_of_frames
                << "\nNumber of hops: " << log.number_of_hops << "\nSender ID: " << log.sender_id << "\nReceiver ID: "
                << log.receiver_id << "\nSuccess: ";
                if (log.success_status) cout << "Yes\n";
                else cout << "No\n";
                if (log.activity_type == ActivityType::MESSAGE_SENT or
                        log.activity_type == ActivityType::MESSAGE_RECEIVED) cout << "Message: \"" << log.message_content
                        << "\"\n";
            }
        }
    }
}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {

    vector<string> args;

    for (string &command : commands) {

        string command_divider = string(command.length() + 9, '-') + "\n";
        cout << command_divider << "Command: " << command << endl << command_divider;

        stringstream split_command(command);
        string call;
        split_command >> call;

        if (call == "MESSAGE") message(clients, split_command, message_limit, sender_port, receiver_port);
        else if (call == "SHOW_FRAME_INFO") show_frame_info(clients, split_command);
        else if (call == "SHOW_Q_INFO") show_q_info(clients, split_command);
        else if (call == "SEND") send(clients);
        else if (call == "RECEIVE") receive(clients);
        else if (call == "PRINT_LOG") print_log(clients, split_command);
        else cout << "Invalid command.\n";
    }
}

vector<Client> Network::read_clients(const string &filename) {

    vector<Client> clients;
    int num_of_clients;

    //opens file and gets the number of clients from file
    ifstream file(filename);
    file >> num_of_clients;

    //adds clients to vector;
    string id;
    string ip;
    string mac_address;
    for (int i = 0; i < num_of_clients; ++i) {

        file >> id >> ip >> mac_address;
        clients.emplace_back(id, ip, mac_address);
    }

    file.close();
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {

    ifstream file(filename);
    string target, to;

    for (Client &client : clients) {

        while (file >> target ) {

            if (target == "-") break;
            file >> to;
            client.routing_table.emplace(target, to);
        }
    }

    file.close();
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {

    vector<string> commands;
    ifstream file(filename);
    string line;
    int num_of_commands;

    file >> num_of_commands;
    getline(file, line); // removes first line since its empty;

    for (int i = 0; i < num_of_commands; ++i) {

        getline(file,line);
        commands.push_back(line);
    }

    file.close();
    return commands;
}

Network::~Network() = default;
