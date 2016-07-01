#define  MEL_IMPLEMENTATION
#include "MEL.hpp"
#include "MEL_stream.hpp"

void runBcastTest() {
	
	MEL::Comm comm = MEL::Comm::WORLD;
	const int rank = MEL::CommRank(comm),
	  		  size = MEL::CommSize(comm);

	MEL::Bcast_stream bstream(0, comm, 16);

	if (rank == 0) {
		for (int i = 0; i < 10; ++i) {
			bstream << i;
		}
	}
	else {
		for (int i = 0; i < 10; ++i) {
			int j;
			bstream >> j;

			std::cout << "Rank " << rank << " Received j = " << j << std::endl;
		}
	}
};

void runSendRecvTest() {

	MEL::Comm comm = MEL::Comm::WORLD;
	const int rank = MEL::CommRank(comm),
			  size = MEL::CommSize(comm);

	if (rank == 0) {
		std::vector<MEL::Send_stream*> sstreams;
		for (int j = 1; j < size; ++j) {
			sstreams.push_back(new MEL::Send_stream(j, 0, comm));
		}
		
		for (int i = 0; i < 10; ++i) {
			for (int j = 1; j < size; ++j) {
				*(sstreams[j-1]) << (i);
			}
		}

		for (int j = 0; j < size-1; ++j) {
			delete sstreams[j];
		}
	}
	else {
		MEL::Recv_stream rstream(0, 0, comm);

		for (int i = 0; i < 10; ++i) {
			int j;
			rstream >> j;

			std::cout << "Rank " << rank << " Received j = " << j << std::endl;
		}
	}
};

int main(int argc, char *argv[]) {
	MEL::Init(argc, argv);

	runSendRecvTest();
	runBcastTest();

	MEL::Finalize();
	return 0;
};