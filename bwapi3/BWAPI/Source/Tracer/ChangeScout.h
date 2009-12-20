#pragma once
/**
 *  Holds a variable. Returns true when assigning a changed value
 */

template <typename T> class ChangeScout
{

	// constructors
public:
	ChangeScout()
	{
		used = false;
	}
	~ChangeScout()
	{
	}

	// state
private:
	bool used;
	T value;

	// methods
public:
	T get()
	{
		return value;
	}
	bool change(T newvalue)
	{
		if(!used)
		{
			used = true;
			value = newvalue;
			return true;
		}
		bool changed = (newvalue != value);
		if(changed)
			value = newvalue;
		return changed;
	}
};
