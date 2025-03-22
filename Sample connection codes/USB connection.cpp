#include <iostream>
#include <windows.h>

const char* serialPort = "COM3";  // Change this to your actual COM port

int main() {
    HANDLE hSerial = CreateFile(serialPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port\n";
        return -1;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state\n";
        return -1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.StopBits = ONESTOPBIT;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port parameters\n";
        return -1;
    }

    char buffer[256];
    DWORD bytesRead;

    while (true) {
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            buffer[bytesRead] = '\0';
            std::cout << "Received: " << buffer << std::endl;
        }
    }

    CloseHandle(hSerial);
    return 0;
}
