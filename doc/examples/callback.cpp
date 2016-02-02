#include <nytl/callback.hpp>

int main()
{
  nytl::Callback<void(void* data, int value)> myCallback {};
  myCallback = [](int value){ std::cout << value << "\n"; };
  
  myCallback.call(nullptr, 7); //should output 7 in callback listener
}
