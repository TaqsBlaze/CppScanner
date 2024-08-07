#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cerrno>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

using namespace std;

//Function prototypes
void wizard();
int tcp_scanner(const char* target_ip, int start_port, int end_port);


int main(int argc, char* argv[]){
	//Main function
	cout << "\t-----------------------------------" << endl;
	cout << "\t\tDeveloped By R3$3t" << endl;
	cout << "\t\t C-Port Scanner" << endl;
	cout << "\t-----------------------------------" << endl;

	if ( argc != 4){
		char option[3];
		cerr << "Usage:\n\t" << argv[0] << "<target-ip> <start-port> <end-port>\n\n" << endl;
		cout << "Would you like to provide target IP and port range?\n" << endl;
		cout << "y/n:" << endl;

		cin >> option;

		if (strcmp(option, "y") == 0){
			//call wizard function
			wizard();
		} else {
			//exit application
			cout << "Exiting...." << endl;
			exit(1);
		}

		return 0;
	}


	//grabing variables form arguments
	const char* target_ip = argv[1];
	int start_port = std::atoi(argv[2]);
	int end_port = std::atoi(argv[3]);

	int scan = tcp_scanner(target_ip, start_port, end_port);

	if (scan == 0){
		return 0;
	} else {
		return 1;
	}
}


void wizard(){
	// wizard function for captuirng target details
	char ip[16];
	int sport;
	int eport;
	int scan_type;

	cout << "Target IP" << endl;
	cin >> ip;

	cout << "Start port:" << endl;
	cin >> sport;

	cout << "End port:" << endl;
	cin >> eport;

	cout << "Scan Type:" << endl;
	cout << "1. TCP\n2. UDP" << endl;
	cin >> scan_type;

	if ( scan_type == 1){
		//call scanner
		tcp_scanner(ip, sport, eport);
	} else {
		cout << "UDP scan coming soon.." << endl;
		exit(1);
	}


}

int tcp_scanner(const char* tartget_ip, int start_port, int end_port){
	//Now we are doing port scan
	cout << "Scanning target..." << endl;
	cout << "Target: " << tartget_ip << endl;

	const char* target_ip = tartget_ip;
	int controller = 0;
	int total_ports = 0;
	std::vector<int> open_ports;

	for (int port = start_port; port <= end_port; ++port){
		int sock = socket(AF_INET, SOCK_STREAM,0);
		if (sock == -1) {
			cerr << "Socket could not be created:" << strerror(errno) << endl;
			return 1;

		}
		total_ports ++;

		sockaddr_in server_address;
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(port);

		//checking IP validity
		if (inet_pton(AF_INET, target_ip, &server_address.sin_addr) <= 0){
			cerr << "Invalid IP address" << endl;
			return 1;
		}
		
		//connecting to tartget port
		if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == 0) {
			//cout << "Port " << port << " is open" << endl;
			open_ports.push_back(port);
			controller ++;

			close(sock);
		}
		close(sock);
	}


	// displaying scan results	
	cout << "------------------------------------" << endl;
	cout << "[+]Dicovered Ports:" << endl;

	for (size_t i = 0; i < open_ports.size(); i++) {
		cout << "Port: " << open_ports[i] << endl;
	}

	cout << "[+]Total ports scanned:" << endl;
	cout << total_ports << endl;
	cout << "------------------------------------" << endl;

	return 0;
}