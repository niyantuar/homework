all: init destroy client
	true

init: init.o mutex.o customer.o
	g++ init.o mutex.o customer.o -o init

destroy: destroy.o mutex.o
	g++ destroy.o mutex.o -o destroy

client: client.o bank.o customer.o mutex.o
	g++ client.o bank.o customer.o mutex.o -o client

bank.o: bank.cxx bank.hxx bank_storage.hxx customer.hxx mutex.hxx bank_storage.hxx
	g++ -c bank.cxx

customer.o: customer.cxx customer.hxx
	g++ -c customer.cxx

init.o: init.cxx bank_storage.hxx mutex.hxx customer.hxx
	g++ -c init.cxx

destroy.o: destroy.cxx mutex.hxx
	g++ -c destroy.cxx

client.o: client.cxx bank.hxx customer.hxx mutex.hxx bank_storage.hxx
	g++ -c client.cxx

mutex.o: mutex.cxx mutex.hxx
	g++ -c mutex.cxx

clean:
	rm *.o init destroy client | true
