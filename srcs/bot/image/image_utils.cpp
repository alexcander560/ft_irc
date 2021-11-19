#include "image.hpp"

void    set_standart_headers(struct curl_slist **headers)
{
	//TODO: Добавить иные заголовки для более стабильной работы
    *headers = curl_slist_append(*headers, "User-Agent: Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:93.0) Gecko/20100101 Firefox/93.0");
}

/* Записывает полученный через CURL результат из ptr в нашу переменную - response */ 
size_t	write_response(char *ptr, size_t size, size_t nmemb, void *response)
{
	size_t	real_size;

	real_size = size * nmemb;
	(reinterpret_cast<std::string *>(response))->append(static_cast<const char *>(ptr), real_size);
	return (real_size);
}

/* Выполняет GET запрос по URL, сохраняя содержимое в переменную response */
void	get_request(std::string &response, const std::string &URL)
{
	CURL		*curl;
    CURLcode	curl_result;
    struct curl_slist   *headers = NULL;

	response.clear();
	curl = curl_easy_init();
	if (!curl)
		throw (std::string("Cannot initialize CURL")); // Ошибка CURL
	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)(&response));
    set_standart_headers(&headers);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_result = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
	if (curl_result != CURLE_OK)
		throw (std::string("CURL request takes error: ")); // Ошибка CURL
}
