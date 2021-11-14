#include "../../general.hpp"
#include "user.hpp"

int	handle_user(const string str_message, string pass, User *user)
{
	int len = str_message.size();
	int res = 0;

	if (len >= 6)
	{
		if (str_message.find("PASS") == 0)
			res = user->setPass(str_message.substr(5, len), pass);
		else if (str_message.find("NICK") == 0)
			res = user->setNick(str_message.substr(5, len));
		else if (str_message.find("USER") == 0)
			res = user->setData(str_message.substr(5, len));
		else if (str_message.find("QUIT") == 0)
			cout << "QUIT запуск\n";
		else if (str_message.find("OPER") == 0)
			cout << "OPER запуск\n";
	}
	else
	{
		if (DEBUG)
			debug("[handle_start()] str_message < 6");
	}

	return (res);
}

int main()
{
	string pass = "123";
	User user;
	int res;

	res = handle_user("NICK Alex", pass, &user);
	res = handle_user("PASS 12345", pass, &user);
	res = handle_user("PASS 123", pass, &user);
	//res = handle_user("USER    500  600 700     :800 300", pass, &user);
	res = handle_user("USER 500    600    700 800  ", pass, &user);

	cout << "res= " << res << endl;
	user.printUser();

	return (0);
}
