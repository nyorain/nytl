ConnectableID concept:

 - default constructor
 - constructor only with std::int64_t (or sth compatible) for the id (uniform init is enough)
 	- the constructor might throw
 - noexcept desctructor
 - copyable/copy-assignable
 - noexcept movable (constructor/assignment)
 - set(int64_t) noexcept: Will be called to reset the connection id.
 	Might be called multiple times. This will only be called with a negative value
	from the callback which signals that new callback calls will not trigger the
	function anymore. The function might still be called from pending callbacks.
	This is called when the represented connection is formally removed, e.g.
	by disconnect or clear but the callback cannot yet remove the connection
	because it is still iterating.
 - removed() noexcept: Will be called before the connection is completely removed, i.e.
	the connectable will not touch it anymore in any way. This might be called
	directly with a call to reset before or without a call to reset.
	This will not be called mutliple times.
	After this call returns the connection id will not be accessed anymore.
 - get() const noexcept: Returns the underlaying id.
 - Note how none of the above function are allowed to throw. Otherwise
   the behvaiour is undefined.
 - The connection id is still not allowed to recursively access the callback
   object accessing it. This means set/removed/get and the constructor/destructor
   must NOT access the callback object they are called from in any way.
   In a nutshell: Just don't abuse them. They are only there to allow
   different notification methods about the connection and callback lifetime.
   - Reasoning for this: It's really hard to argue about 'correct' semantics
     in this case. What should happen if 'clear() calls id.removed() calls
     callback.add(...)' on the same object? It's additionally really hard
     to get completly right and would probably involve runtime overhead
     which is totally not worth it.
