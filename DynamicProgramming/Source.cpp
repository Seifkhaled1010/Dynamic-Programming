#include <iostream>
#include <map>
#include <string>
using namespace std;

struct CommonKey
{
	int n;
	int sum;

	const bool operator=(const CommonKey& o) {
		return n == o.n && sum == o.sum;
	}

	const bool operator<(const CommonKey& o) const {
		return n < o.n || (n == o.n && sum < o.sum);
	}
};

int fibDynamicSteps = 0;
int fibNormalSteps = 0;
int stepsSubsetNormal = 0;
int stepsSubsetDynamic = 0;
int stepsLCANormal = 0;
int stepsLCADynamic = 0;
map<int, int> storage;
map<CommonKey, bool> storage1;
map<string, int> storage3;


int lcs(char* X, char* Y, int m, int n)
{
	stepsLCANormal++;
	if (m == 0 || n == 0)
		return 0;
	if (X[m - 1] == Y[n - 1])
		return 1 + lcs(X, Y, m - 1, n - 1);
	else
		return max(lcs(X, Y, m, n - 1),
			lcs(X, Y, m - 1, n));
}

int lcsDynamic(char* X, char* Y, int m, int n)
{
	stepsLCADynamic++;
	if (m == 0 || n == 0)
		return 0;
	if (X[m - 1] == Y[n - 1])
	{
		string key = std::to_string(n-1);
		key += "|";
		key += std::to_string(m-1);
		if (storage3.find(key) != storage3.end())
			return 1 + storage3[key];
		else
		{
			int length = lcsDynamic(X, Y, m - 1, n - 1);
			storage3[key] = length;
			return 1 + length;
		}
	}
	else
	{
		string key1 = std::to_string(m);
		key1 += "|";
		key1 += std::to_string(n-1);


		string key2 = std::to_string(m-1);
		key2 += "|";
		key2 += std::to_string(n);


		int l1, l2;

		if (storage3.find(key1) != storage3.end())
			l1 = storage3[key1];
		else
		{
			l1 = lcsDynamic(X, Y, m, n - 1);
			storage3[key1] = l1;
		}


		if (storage3.find(key2) != storage3.end())
			l2 = storage3[key2];
		else
		{
			l2 = lcsDynamic(X, Y, m - 1, n);
			storage3[key2] = l2;
		}


		return max(l1, l2);
	}
}

bool isSubsetSum(int set[], int n, int sum)
{
	stepsSubsetNormal++;
	// Base Cases
	if (sum == 0)
		return true;
	if (n == 0)
		return false;

	// If last element is greater than sum,
	// then ignore it
	if (set[n - 1] > sum)
		return isSubsetSum(set, n - 1, sum);

	// Else, check if sum can be obtained by any
	// of the following:
	// (a) including the last element
	// (b) excluding the last element
	return isSubsetSum(set, n - 1, sum)
		|| isSubsetSum(set, n - 1, sum - set[n - 1]);
}

bool isSubsetSumDynamic(int set[], int n, int sum)
{
	stepsSubsetDynamic++;
	// Base Cases
	if (sum == 0)
		return true;
	if (n == 0)
		return false;

	CommonKey key{ n - 1, sum };
	// If last element is greater than sum,
	// then ignore it
	if (set[n - 1] > sum)
	{
		
		if (storage1.find(key) != storage1.end())
		{
			return storage1[key];
		}
		else
		{
			bool value = isSubsetSumDynamic(set, n - 1, sum);
			storage1[key] = value;
			return value;
		}
	}

	// Else, check if sum can be obtained by any
	// of the following:
	// (a) including the last element
	// (b) excluding the last element

	bool f1, f2;
	if (storage1.find(key) != storage1.end())
	{
		return storage1[key];
	}
	else
	{
		f1 = isSubsetSumDynamic(set, n - 1, sum);
		storage1[key] = f1;
	}


	CommonKey key2{ n - 1, sum - set[n - 1] };
	if (storage1.find(key2) != storage1.end())
	{
		return storage1[key2];
	}
	else
	{
		f2 = isSubsetSumDynamic(set, n - 1, sum - set[n - 1]);
		storage1[key2] = f2;
	}

	return f1 || f2;
}


int FibDynamic(int n)
{
	fibDynamicSteps++;
	if (n <= 2)
		return 1;

	if (storage.find(n) != storage.end())
	{
		return storage[n];
	}

	int left, right;

	if (storage.find(n - 1) != storage.end())
	{
		left = storage[n - 1];
	}
	else
	{
		left = FibDynamic(n - 1);
		storage[n - 1] = left;
	}

	if (storage.find(n - 2) != storage.end())
	{
		right = storage[n - 2];
	}
	else
	{
		right = FibDynamic(n - 2);
		storage[n - 2] = right;
	}

	return left + right;
}

int FibRegular(int n)
{
	fibNormalSteps++;
	if (n <= 2)
		return 1;
	return FibRegular(n - 1) + FibRegular(n - 2);
}

void main()
{
	cout << "Normal fib for 40 is: " << FibRegular(40) << ", took steps: " << fibNormalSteps << endl;
	cout << "Dynamic programming fib for 40 is: " << FibDynamic(40) << ", took steps: " << fibDynamicSteps << endl;

	int set[] = { 3, 34, 4, 12, 5, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	int sum = 199;
	int n = sizeof(set) / sizeof(set[0]);
	if (isSubsetSum(set, n, sum) == true)
		cout << "Subset found!" <<", total steps: "<<stepsSubsetNormal << endl;
	else
		cout << "Subset not found!" << ", total steps: " << stepsSubsetNormal << endl;

	if (isSubsetSumDynamic(set, n, sum) == true)
		cout << "Subset found!" << ", total steps: " << stepsSubsetDynamic << endl;
	else
		cout << "Subset not found!" << ", total steps: " << stepsSubsetDynamic << endl;


	char X[] = "AGGTABSDOJFHSDFDS";
	char Y[] = "GXTXAYBZKXFNFSKDDFD";

	int m = strlen(X);
	 n = strlen(Y);

	int length = lcs(X, Y, m, n);
	cout << "Length = " << length << ", steps: " << stepsLCANormal << endl;


	int length2 = lcsDynamic(X, Y, m, n);
	cout << "Length = " << length2 << ", steps: " << stepsLCADynamic << endl;
}