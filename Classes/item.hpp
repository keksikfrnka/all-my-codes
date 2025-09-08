#ifndef ITEM
#define ITEM

#include <iostream>
#include <string>

class Item {
public:
	Item() = default;
	virtual bool operator<(const Item& item) = 0;
	virtual bool operator == (const Item& item) = 0;
	virtual ~Item() { }

};

class Real : public Item {

protected:
	int int_part_;
	int fract_part_;
	bool is_negative_;

public:
	friend std::ostream& operator<<(std::ostream& out, const Real& value);
	friend std::istream& operator>>(std::istream& in, Real& value);

	Real();
	Real(double x);
	Real(int aint_part, int afract_part, bool ais_negative);
	Real(const Real& another);
	Real operator+(const Real& second);
	Real operator+(int x);
	Real operator*(const Real& second);
	Real& operator+=(const Real& another);
	Real& operator+=(int another);
	Real& operator*=(const Real& another);
	bool operator<(const Item& item) override;
	bool operator==(const Item& item) override;
	Real& operator=(const Real& another);
	~Real();
};

class Pair : public Item {
private:
	Real a;
	Real b;
public:
	friend std::ostream& operator<<(std::ostream& out, const Pair& value);
	friend std::istream& operator>>(std::istream& in, Pair& value);

	Pair();
	Pair(double x, double y);
	Pair(const Pair& another);
	bool operator<(const Item& item) override;
	bool operator==(const Item& item) override;
	Pair& operator=(const Pair& another);
	~Pair();
};



#endif // ITEM