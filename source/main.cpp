#include "mbed.h"
#include "EthernetInterface.h"
#include "LWIPStack.h"
#include "UDPConnection.h"
#include "ExampleMessageHandler.h"

// Network interface
EthernetInterface net;

SocketAddress localAddress("192.168.4.210");
SocketAddress subnetmask("255.255.255.0");
SocketAddress gateway("0.0.0.0");

DigitalOut  ledRed(LED3);
DigitalOut  ledGreen(LED1);

int main() {
    printf("Hello from "  MBED_STRINGIFY(TARGET_NAME) "\n");
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    // Bring up the ethernet interface
    volatile nsapi_error_t err;
    SocketAddress localAddr;

    // uncomment for fixed IP address, otherwise dhcp is used
    //net.set_network(localAddress, subnetmask, gateway);

    err = net.connect();
    if (err != NSAPI_ERROR_OK) {
        printf("Error connecting: %d\n", err);
        while(1) {
            ledRed = !ledRed;
            ThisThread::sleep_for(200ms);
        }
    }

    // Show the network address
    err = net.get_ip_address(&localAddr);
    if (err == NSAPI_ERROR_OK) {
        printf("get_ip_address : %s\n", localAddr.get_ip_address() ? localAddr.get_ip_address() : "No IP");
    }

    // Show get gateway
    err = net.get_gateway(&localAddr);
    (err == NSAPI_ERROR_OK) ? printf("gateway address is: %s\n", localAddr.get_ip_address()) : printf("get_gateway err: %d\n", err);

    // Show get netmask
    err = net.get_netmask(&localAddr);
    (err == NSAPI_ERROR_OK) ? printf("netmask is: %s\n", localAddr.get_ip_address()) : printf("get_netmask err: %d\n", err);

    // create udp connection1
    UDPConnection *udpConnection1 = new UDPConnection(net, 9000);        // listen on port 9000, sendto sender
    udpConnection1->addMessageHandler(new ExampleMessageHandler(udpConnection1));
    err = udpConnection1->start();
    (err == NSAPI_ERROR_OK) ? printf("udpConnection1 started\n") : printf("udpConnection1 start() err: %d\n", err);

    // create udp connection2
    UDPConnection *udpConnection2 = new UDPConnection(net, 9001);        // listen on port 9000, sendto sender
    udpConnection2->addMessageHandler(new ExampleMessageHandler(udpConnection2));
    err = udpConnection2->start();
    (err == NSAPI_ERROR_OK) ? printf("udpConnection2 started\n") : printf("udpConnection2 start() err: %d\n", err);
    
    while(1) {
        ledGreen = !ledGreen;
        ThisThread::sleep_for(500ms);
    }

    return 0;
}
