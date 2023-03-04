#include <iostream>
#include <string>
#include <openssl/sha.h>
#include<sstream>
#include <zlib.h>
#include<Windows.h>
#include <iomanip>
#include "Struct/HackSettings.hpp"

using namespace std;

extern HackSettings hackSettings;

class HashVerificator {
public:
    static std::string get_hwid_password() {
        std::string machineCode = "";

        DWORD serialNumber;
        GetVolumeInformation("C:\\", NULL, 0, &serialNumber, NULL, NULL, NULL, 0);

        machineCode += to_string(serialNumber);
        machineCode += hackSettings.guiSettings.hackVersion;

        std::string toVerify = HashVerificator::get_password(machineCode);
        return toVerify;
    }

    static std::string get_password(std::string s) {
        int hash = 0;

        SYSTEMTIME  utcTime;
        GetSystemTime(&utcTime);
        int dateUTC = utcTime.wYear * 10000 + utcTime.wMonth * 100 + utcTime.wDay;
        std::string dateUTC_str = to_string(dateUTC);
        s += dateUTC_str;

        hash = calculate_hash(s);

        int checksum1 = calculate_checksum(hash);
        int numWithChecksum = hash * 10 + checksum1;
        int checksum2 = calculate_checksum(numWithChecksum);
        int sixDigitNumber = abs(numWithChecksum * 10 + checksum2);

        stringstream ss;
        ss << std::setw(6) << std::setfill('0') << sixDigitNumber;
        std::string formatted = ss.str();

        return formatted;
    }

    /// <summary>
    /// 校验验证码是否合法
    /// </summary>
    /// <returns></returns>
    static int validate(int number) {
        // 检查数字是否为正数
        if (number <= 0) {
            return -1;
        }

        // 检查数字是否为 6 位数字
        if (number < 100000 || number > 999999) {
            return -1;
        }

        // 获取校验和 1 和校验和 2
        int checksum2 = number % 10;
        int numWithChecksum2 = number / 10;
        int checksum1 = numWithChecksum2 % 10;
        int hash = numWithChecksum2 / 10;

        int correctCheckSum1 = calculate_checksum(hash);
        // 计算数字的哈希值
        //int calculatedHash = calculate_hash(to_string(hash));

        // 验证校验和 1 和校验和 2
        if (checksum1 != calculate_checksum(hash)) {
            return -1;
        }

        int numWithChecksum1 = hash * 10 + checksum1;
        if (checksum2 != calculate_checksum(numWithChecksum2)) {
            return -1;
        }

        return 1;
    }

private:

    static  int calculate_hash(const std::string& s) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((const unsigned char*)s.c_str(), s.length(), hash);

        // 将SHA-256哈希值的前4个字节转换为4位数字
        long long num = 0;
        for (int i = 0; i < 4; i++) {
            if (hash[i] < 0) {
                num = num * 256 + hash[i] + 256;
            }
            else {
                num = num * 256 + hash[i];
            }
        }

        int result = (int)(num % 10000);

        return result;
    }

    static int calculate_checksum(int num) {
        std::stringstream ss;
        ss << num;
        std::string numString = ss.str();
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, (const Bytef*)numString.c_str(), numString.length());
        long long checksum = crc;
        return std::abs(checksum % 10);
    }
};


int main() {

     return 0;
}