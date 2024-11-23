## Project Quran

This project fetches a random Ayah (verse) from the Quran using the Al-Quran Cloud API and displays it in a message box. The program runs continuously, fetching and displaying a new Ayah every minute.

### Prerequisites

- [cURL](https://curl.se/download.html)
- [cJSON](https://github.com/DaveGamble/cJSON)
- Windows OS

### Installation

1. Install cURL and cJSON libraries.
2. Clone the repository.
3. Compile the code using a C compiler.

### Usage

Run the compiled executable. The program will fetch a random Ayah from the Quran every minute and display it in a message box.

### Code Overview

- **MemoryStruct**: A structure to handle dynamic memory allocation for the cURL response.
- **Format**: An enumeration for different data formats.
- **utf8_to_wchar**: Converts a UTF-8 string to a wide character string.
- **ayah_url**: Generates a random Ayah URL.
- **WriteMemoryCallback**: A callback function for handling cURL data.
- **main**: The main function that initializes cURL, fetches the Ayah, parses the JSON response, and displays the Ayah in a message box.

### License

This project is licensed under the MIT License.

### Acknowledgements

- [Al-Quran Cloud API](https://alquran.cloud/api)
- [cURL](https://curl.se/)
- [cJSON](https://github.com/DaveGamble/cJSON)