
#ifndef HRESULT_HPP
#define HRESULT_HPP

#include "dependencies.hpp"

class Result {
public:
	virtual bool failed() const = 0;
	virtual bool success() const = 0;
	virtual void clear_result() = 0;
};

class HResult: public Result {
protected:
	HRESULT result = 0;
	
public:
	bool failed() const override;
	bool success() const override;
	void clear_result() override;
	HRESULT get_result() const;
};

#endif
