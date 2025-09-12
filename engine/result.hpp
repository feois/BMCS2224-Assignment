
#ifndef RESULT_HPP
#define RESULT_HPP

class Result {
public:
	virtual bool failed() const = 0;
	virtual bool success() const = 0;
	virtual void clear_result() = 0;
};

#endif
