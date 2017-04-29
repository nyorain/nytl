CallbackConnectionID concept:

 - default constructor
 - constructor only with std::int64_t (or sth compatible) for the id
 - noexcept desctructor
 - copyable/copy-assignable
 - reset(int64_t): Will be called to reset the connection id.
 	Might be called multiple times. This will be called with a negative value and
 	signals that new callback calls will not trigger the function anymore.
	- remove(): Will be called before the connection is completely removed, i.e.
		the called will not touch it anymore in any way..
 - id() const noexcept: Returns the underlaying id.

 Implementations of the concept must care if they access the callback that triggered
 their callback to not get infinite recursion.
