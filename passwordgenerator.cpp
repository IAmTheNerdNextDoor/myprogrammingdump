#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <cctype>
#include <limits>
#include <random>

bool containsLetters(const std::string& password)
{
	for (std::string::size_type i = 0; i < password.length(); ++i)
	{
		if (std::isalpha(password[i]))
		{
			return true;
		}
	}
	return false;
}

int getPasswordLength()
{
	int length;
	std::string input;
	
	while (true)
	{
		std::cout << "Password length: ";
		std::cin >> input;
		
		bool valid = true;
		for (std::string::size_type i = 0; i < input.length(); ++i)
		{
			if (!std::isdigit(input[i]))
			{
				valid = false;
				break;
			}
		}
		if (valid)
		{
			length = std::atoi(input.c_str());
			if (length <= 0 || containsLetters(input))
			{
				valid = false;
			}
		}
		
		if (valid)
		{
			break;
		}
		else
		{
			std::cout << "Length is empty or contains letter or symbols. Please enter numbers only.\n";
		}
		
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	
	return length;
}

std::string generatePassword(int length)
{
	const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&**()";
	const int charactersLength = characters.length();
	
	std::random_device rd;
	std::random_device::result_type seed = rd.entropy() ? rd() : static_cast<unsigned>(std::time(nullptr));
	
	std::mt19937_64 generator(rd());
	std::uniform_int_distribution<int> distribution(0, charactersLength -1);
	
	std::string password;
	srand(time(NULL));
	
	for (int i = 0; i < length; ++i)
	{
		int index = rand() % charactersLength;
		password += characters[index];
	}
	
	return password;
}

int main()
{
	int passwordLength = getPasswordLength();
	
	std::string password = generatePassword(passwordLength);
	std::cout << "Here is your generated password: " << password << std::endl;
	std::cout << "Make sure to copy your password down and then press any key to exit.";
	_getch();
	
	return 0;
}
