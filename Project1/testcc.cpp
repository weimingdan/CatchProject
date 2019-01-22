#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "trompeloeil.hpp"
#include <string>
using namespace std;


namespace trompeloeil
{
	template <>
	void reporter<specialized>::send(
		severity s,
		const char* file,
		unsigned long line,
		const char* msg)
	{
		std::ostringstream os;
		if (line) os << file << ':' << line << '\n';
		os << msg;
		auto failure = os.str();
		if (s == severity::fatal)
		{
			FAIL(failure);
		}
		else
		{
			CAPTURE(failure);
			CHECK(failure.empty());
		}
	}
}

class MyClass
{
public:
	int add(int a, int b)
	{
		return a + b;
	}
};

SCENARIO("DBB style test case", "[bdd]")
{
	MyClass cc;
	GIVEN("a string name")
	{
		WHEN("string is People")
		{
			THEN("the result is People")
			{
				REQUIRE(cc.add(1, 2) == 3);
			}
		}
		WHEN("the number is 3 and 4")
		{
			THEN("the result is 7")
			{
				REQUIRE(cc.add(3, 4) == 7);
			}
		}
	}
}



class Warehouse
{
public:
	virtual bool hasInventory(const std::string& name, size_t amount) const = 0;
	virtual void remove(const std::string& name, size_t amount) = 0;
};


class Order
{
public:
	Order(const std::string& name, size_t amount)
		:mName(name), mAmount(amount)
	{
		
	}
	void fill(Warehouse& w)
	{
		bool xx = w.hasInventory(mName, mAmount);
		w.remove(mName, mAmount);
	}
	bool is_filled() const
	{
		return true;
	}
private:
	string mName;
	size_t mAmount;
};

class WarehouseMock : public Warehouse
{
public:
	MAKE_CONST_MOCK2(hasInventory, bool(const std::string&, size_t));
	MAKE_MOCK2(remove, void(const std::string&, size_t));
};

TEST_CASE("test1","[cc]")
{
	Order order("Talisker", 50);

	WarehouseMock warehouse;
	SECTION("test house")
	{
		REQUIRE_CALL(warehouse, hasInventory("Talisker", 50))
			.RETURN(true);

		REQUIRE_CALL(warehouse, remove("Talisker", 50));

		order.fill(warehouse);
		REQUIRE(order.is_filled());
	}
}