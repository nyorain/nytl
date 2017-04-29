CallbackConnectionID concept:

 - default constructor
 - constructor only with std::int64_t (or sth compatible) for the id
 - noexcept desctructor
 - copyable/copy-assignable
 - reset(int64_t): Will be called to reset the connection id.
 	Might be called multiple times. This will only be called with a negative value
	from the callback which signals that new callback calls will not trigger the
	function anymore. The function might still be called from pending callbacks.
	This is called when the represented connection is formally removed, e.g.
	by disconnect or clear but the callback cannot yet remove the connection
	because it is still iterating.
 - remove(): Will be called before the connection is completely removed, i.e.
	the called will not touch it anymore in any way. This might be called
	directly with a call to reset before.
	This will not be called mutliple times.
	After this call returns the connection id will not be accessed anymore.
 - id() const noexcept: Returns the underlaying id.

 Implementations of the concept must care if they access the callback that triggered
 their callback to not get infinite recursion.
 Except from the destructor, or when already propagating an exception, the Callback class always
 propagates all exceptions from the ID class to its user/caller.
