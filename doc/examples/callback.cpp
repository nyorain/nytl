#include <nytl/callback.hpp>
#include <nytl/vec.hpp>
#include <iostream>

//Some dummy listener function
void someListenerFunction(nytl::Vec2f vec)
{
	std::cout << vec << "\n";
}

int main()
{
	//Create a callback object with void(int, Vec2f) signature.
	//Note that this does NOT mean that all listeners (functions that are registered to be called
	//when the callback is triggered by Callback::call or by the call operator) must have the same
	//signature.
	//It does simply means they must have a compatible signature (see CompatibleFunction for
	//deeper information on that).
	//The return type of an callback listener must be convertible to void (so it can be any return
	//type) and the listener can take any of the 2 parameters, optionally.
	//Therefore listeners with signatures like int(int), char*(), void(nytl::Vec2f) or
	//std::string(int, nytl::Vec2f) are totally fine. The return type is not used anyways and the
	//not-needed parameters will simpley be ommitted.
	//On the other hand signatures like void(int, int) or char*(nytl::Vec2f, int) are NOT allowed.
	//The order of parameters can not be switched and there cannot be additional parameters.
	nytl::Callback<void(int, nytl::Vec2f)> onEvent;

	//Adds a callback listener in form of a lambda function
	//The returned object is a nytl::Connection, capable of observing the state of the callback and
	//listener as well as unregistering the listener (destroying the connection).
	//Note that Connection is just a proxy class around the real connection so it can be copied
	//and its destruction (i.e. going out of scope) has no effect.
	//If you want an RAII connection that disconnects itself automatically on destruction, see
	//nytl::RaiiConnection.
	auto connection = onEvent.add([]{ std::cout << "called\n"; });

	//Output whether connection is connected, i.e the listener is still registered.
	//Will output true since the callback object still exists and the connection have not been
	//disconnected yet.
	std::cout << connection.connected() << "\n";

	//Destroys the connection and therefore unregisters the lambda listener we added to the callback
	//above. Further calls to destroy() have no effect.
	connection.destroy();

	//Will now output false
	std::cout << connection.connected() << "\n";

	//Callback does also overload the "=" and the "+=" operators.
	//+= adds a new callback listener. The operator does also return a connection object but since
	//the syntax for this would be a little weird (connection = callback += listener) one should
	//prefer to call Callback::add() if the returned connection is needed.
	//Does furhtermore exactly the same as an call to Callback::add().
	onEvent += [](int a) -> int { return a * 420; };

	//= sets the callback listener. This does simply mean that it clears all existing ones and
	//sets the new listener as the ONLY one. In this case the above registered lambda listener
	//would be unregistered.
	//This effect can also be achieved with a call to Callback::clear which clears all existing
	//listeners and a following call to Callback::add which then sets one new listener.
	onEvent = &someListenerFunction;

	//The syntax is therefore a lot like html/javascript event callbacks (nytl::Callback has some
	//serious benefits, though) where you would also write something
	//like "object.onEvent = function" which would then set the given function as only listener.

	//Finally calls/triggers the callback and all registered listenered with the given arguments.
	//In this case only the dummy someListenerFunction would be called.
	onEvent.call(7, {23.f, 0.3f});

	//Calling can also be achieved directly with the call operator
	onEvent(4, {12.f, 42.f});

	//There are many other (mostly more complex) things one can with a callback, such as retrieving
	//the returned values from all listeners when triggering the callback; disconnecting a listener
	//from inside its called function without the need to store a connection object or using
	//nytl::CompatibleFunction to e.g. add a callback to a member function of an object.
	//See the documentation of nytl::Callback, nytl::Connection, nytl::RaiiConnection,
	//nytl::ConnectionRef and nytl::CompatibleFunction for more information.
}
