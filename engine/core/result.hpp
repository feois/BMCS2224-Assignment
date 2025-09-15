
#ifndef RESULT_HPP
#define RESULT_HPP

// record down last operation's status
class Result {
public:
	// return true if last operation failed
	virtual bool failed() const = 0;
	// return true if last operation succeeded
	virtual bool success() const = 0;
	// reset result
	virtual void clear_result() = 0;
};

#endif
