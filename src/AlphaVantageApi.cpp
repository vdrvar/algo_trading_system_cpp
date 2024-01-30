#include <iostream>
#include <string>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

// ...

std::string MakeApiRequest(const std::string& url) {
    CURL* curl;
    CURLcode res;

    std::cout << "Making API request to: " << url << std::endl;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    std::string response;

    if (curl) {
        // Specify the path to CA certificates
        curl_easy_setopt(curl, CURLOPT_CAINFO, "C:/libs/cacert.pem");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        std::cout << "Executing API request..." << std::endl;

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "Error during API request: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return response;
}

// ...


int main() {
    // Fetch the API key from the environment variable
    const char* apiKeyEnvVar = "ALPHA_VANTAGE_API_KEY";
    char* apiKey = getenv(apiKeyEnvVar);

    if (!apiKey) {
        std::cerr << "Error: API key not found in environment variable " << apiKeyEnvVar << std::endl;
        return 1;
    }

    // Construct the API request URL
    std::string symbol = "AAPL"; // Example stock symbol
    std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + "&apikey=" + apiKey;

    std::cout << "API Key: " << apiKey << std::endl;

    // Make the API request and get the response
    std::string apiResponse = MakeApiRequest(url);

    // Print the API response
    std::cout << "API Response:\n" << apiResponse << std::endl;

    return 0;
}
