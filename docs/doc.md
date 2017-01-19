- If any function throws something else than std::exception, nytl will not handle it
	- e.g. Connection will not handle exceptions other than std::exception in
		its destructor, if C::disconnect throws
