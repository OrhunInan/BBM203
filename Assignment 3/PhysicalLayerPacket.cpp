#include "PhysicalLayerPacket.h"

PhysicalLayerPacket::PhysicalLayerPacket(int _layer_ID, const string& _sender_MAC, const string& _receiver_MAC)
        : Packet(_layer_ID) {
    sender_MAC_address = _sender_MAC;
    receiver_MAC_address = _receiver_MAC;
    num_of_hops = 0;
}

void PhysicalLayerPacket::print() {
    cout << "Sender MAC address: " << sender_MAC_address << ", Receiver MAC address: " << receiver_MAC_address << endl;
 }

PhysicalLayerPacket::~PhysicalLayerPacket() = default;