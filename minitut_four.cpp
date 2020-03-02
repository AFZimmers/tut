/*
 * minitut4.cpp
 *
 *  Created on: 16 Mar 2012
 *      Author: simon
 */

#include <iostream>
#include <string>
#include <set>

#include "cheapsocket.h"
#include "noisy.h"

// g++ minitut_four.cpp -o minitut_four -std=c++11

// Take a look at PART ONE, where a cheapsocket library is used.
// * A socket is acquired using the get_socket() method
// * The socket is written to, and read from by the write_to_socket and read_from_socket methods
// * Once finished. the socket is released. by the release_socket method.
// The number of sockets is limited and our programmer hasn't release the sockets!
// Create a class that manages sockets automatically.
// * Implement the six special member functions, except for the
//   copy constructor and copy assigment operator. They can be deleted.
//   It doesn't make sense to create a copy of a socket handle in this case.
// * Implement write/read methods that delegate to the cheapsocket methods.
// You can define all the methods within the raii_socket 
// class declaration above the main method.

// Take a look at PART TWO
// Examine the output to see the extent of object creation, copying and destruction.
// Make the arguments of addrats and addtorat references and note
// the creation + destruction changes.
// \textbf{addtorat} could return a reference to the lhs argument.
// Change this and note the creation + destruction changes.

 // Review http://ideone.com/N5iJkl for further insight into
 // the special member functions and where they occur

// Adds two noisy rational numbers together, creating
// a new object
noisyrat addrats(noisyrat lhs, noisyrat rhs)
{
	std::cout << "addrats()" << std::endl;
	return noisyrat(
		lhs.get_n()*rhs.get_d() + rhs.get_n()*lhs.get_d(),
		lhs.get_d()*rhs.get_d());
}

// Adds the value of rhs to that of lhs and returns lhs
noisyrat addtorat(noisyrat lhs, noisyrat rhs)
{
	std::cout << "addtorats()" << std::endl;
	lhs.set_n(lhs.get_n()*rhs.get_d() + rhs.get_n()*lhs.get_d()) ;
	lhs.set_d(lhs.get_d()*rhs.get_d());
	return lhs;
}

// Implement the six special member functions here.
class raii_socket
{
	private:
		int socket;
	public:
		raii_socket(void) : socket(cheapsocket::get_socket()) {} // default constructor
		raii_socket(const raii_socket & rhs) = delete;// deleted copy constructor
		// move constructor
		raii_socket(raii_socket && rhs) : socket(rhs.socket){
			rhs.socket = -1;
		}

		raii_socket & operator=(const raii_socket & rhs) = delete;// deleted copy assignment operator
		//  move assignment operator
		raii_socket & operator=(raii_socket && rhs){
			if(this != &rhs){
				socket = rhs.socket;
				rhs.socket = -1;
			}
			return *this;
		}

		// destructor
		~raii_socket(void){
			if(socket != -1){
				cheapsocket::release_socket(socket);
			}
		}

		// Write and read methods that passthrough
		void write(const std::string & s){
			cheapsocket::write_to_socket(socket, s);
		}
		std::string read(void){
			return cheapsocket::read_from_socket(socket);
		}
};

int main(void)
{
	std::cout << "PART ONE:" << std::endl;
	for(int i=0; i<10;++i)
	{
		int socket = cheapsocket::get_socket();
		cheapsocket::write_to_socket(socket, "Hello World!");
		std::string s = cheapsocket::read_from_socket(socket);
		cheapsocket::release_socket(socket);
	}

	// UNCOMMENT
	{

		raii_socket socket;
		socket.write("Hello World");
		std::string s1 = socket.read();
		
		//raii_socket copy = socket; // This should fail
		raii_socket new_socket = std::move(socket); // This should succeed.
		new_socket.write("Hello World");
		std::string s2 = new_socket.read();

	}	

	std::cout << "PART TWO:" << std::endl;
	{
		noisyrat n1(2,3);
		noisyrat n2(4,5);
		noisyrat n3(5,6);
		n3 = addrats(n1,n2);
		std::cout << "n3=" << n3 << std::endl;
		std::cout.flush();
	}

	{
		noisyrat n1(2,3);
		noisyrat n2(4,5);
		addtorat(n1,n2);
		std::cout << "n1=" << n1 << std::endl;
		std::cout.flush();
	}
}
