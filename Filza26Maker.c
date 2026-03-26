#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <curl/curl.h>
#include <direct.h>

#define OUTPUT_IPA "CopyEditFilza-Jailed-iOS26-GeoSn0w.ipa"

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

int download_deb() {
    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    FILE *fp = fopen("filza.deb", "wb");
    if (!fp) return 0;

    curl_easy_setopt(curl, CURLOPT_URL,
        "https://tigisoftware.com/cydia/com.tigisoftware.filza_4.0.1-2_iphoneos-arm.deb");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    int success = (curl_easy_perform(curl) == CURLE_OK);

    fclose(fp);
    curl_easy_cleanup(curl);
    return success;
}

void clean() {
    system("rmdir /s /q work 2>nul");
    system("rmdir /s /q Payload 2>nul");
    system("del /f /q filza.deb 2>nul");
}

int main() {
    printf("========================================\n");
    printf("   Filza26 Production Builder\n");
    printf("========================================\n");

    clean();

    printf("[*] Downloading Filza...\n");
    if (!download_deb()) {
        printf("[-] Download failed\n");
        return 1;
    }

    printf("[*] Creating workspace...\n");
    _mkdir("work");
    _mkdir("Payload");

    printf("[*] Extracting DEB (ar)...\n");
if (system("cd work && ar x ..\\filza.deb") != 0) {
    printf("[-] Failed to unpack deb\n");
    return 1;
}

printf("[*] Extracting data.tar...\n");
if (system("cd work && tar -xf data.tar.*") != 0) {
    printf("[-] Failed to extract data\n");
    return 1;
}

    printf("[*] Building Payload...\n");
    if (system("xcopy /E /I /Y work\\Applications Payload\\Filza.app") != 0) {
        printf("[-] Failed to copy app\n");
        return 1;
    }

    printf("[*] Creating IPA...\n");
    char zipCmd[512];
    printf("[*] Creating IPA...\n");

// Step 1: Create ZIP
if (system("powershell Compress-Archive -Force Payload temp.zip") != 0) {
    printf("[-] Failed to create zip\n");
    return 1;
}

// Step 2: Rename to IPA
if (system("rename temp.zip CopyEditFilza-Jailed-iOS26-GeoSn0w.ipa") != 0) {
    printf("[-] Failed to rename IPA\n");
    return 1;
}

    if (system(zipCmd) != 0) {
        printf("[-] Failed to create IPA\n");
        return 1;
    }

    printf("\n[SUCCESS] IPA CREATED:\n%s\n", OUTPUT_IPA);

    printf("\n[*] Ready for sideloading.\n");

    return 0;
}