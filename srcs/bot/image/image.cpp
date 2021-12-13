#include "image.hpp"

/* Получает ссылку на картинку по ключевому слову, обращаясь к Яндексу через GET запрос посредством CURL */
std::string	get_link(const char *name) {
	std::string response;
	const char	*begin, *end;
	std::string	URL, link;
	char		*temp = curl_easy_escape(NULL, name, std::strlen(name));

	URL = std::string("https://yandex.ru/images/search?text=") + std::string(temp);
	try {
		get_request(response, URL);
	}
	catch (std::string e) {
		throw (e);
	}
	begin = std::strstr(response.c_str(), ",\"img_href\":\"");
	if (begin == NULL)
		throw (std::string("Link to image was not found"));
	begin += std::strlen(",\"img_href\":\"");
	end = std::strstr(begin, "\"");
	if (end == NULL)
		throw (std::string("Link to image was not found"));
	link = std::string(begin, end - begin);
	free (temp);
	return (link);
}
