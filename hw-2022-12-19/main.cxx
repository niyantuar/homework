#include <iostream>
#include <cstddef>
#include <cmath>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t* mutex{new pthread_mutex_t{}};
int counter = 0;

bool isPrime(int n) {
	if (n <= 1) {
		return false;
	}
	if (n == 2) {
		return true;
	}
	if (n % 2 == 0) {
		return false;
	}
	for (int i = 3; i < std::sqrt(n); i += 2) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

struct range_t {  // holds range [left; right]
	int left;
	int right;
};

void* thread(void* arg) {
	auto range = static_cast<range_t*>(arg);
	for (int i = range->left; i <= range->right; ++i) {
		if (isPrime(i)) {
			pthread_mutex_lock(mutex);
			counter++;
			pthread_mutex_unlock(mutex);
		}
	}
	return nullptr;
}

int main() {
	int n;
	std::cin >> n;
	if (n == 0) {
		std::cout << 0 << std::endl;
		return 0;
	}
	pthread_mutex_init(mutex, nullptr);
	range_t range_1{0, n/2};
	range_t range_2{n/2 + 1, n};
	pthread_t thread_1{};
	pthread_t thread_2{};
	pthread_create(
		&thread_1,
		nullptr,
		thread,
		static_cast<void*>(&range_1)
	);
	pthread_create(
		&thread_2,
		nullptr,
		thread,
		static_cast<void*>(&range_2)
	);
	pthread_join(thread_1, nullptr);
	pthread_join(thread_2, nullptr);

	std::cout << counter << std::endl;

	pthread_mutex_destroy(mutex);
	delete mutex;
}
