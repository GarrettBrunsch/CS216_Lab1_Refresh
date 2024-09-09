// This program reads an unknown quantity of characters from a
// pre-specified file and will encrypt/decrypt the user info, create passwords,
// and display the information in an organized format

#include <cstdlib> //rand, srand, exit
#include <ctime>   //time
#include <fstream> // opening and closing files
#include <iomanip> //setw, fill, etc for table formatting
#include <iostream>
#include <limits> //ignore numeric_limits
#include <string>
using namespace std;

struct UserData {
  string m_userID;
  string m_encryptedID;
  string m_userPassword;
  string m_DecryptedID;
};

const int MAX_USER_SIZE = 10;
const int ENCRYPTION_SHIFT = 7;
const int PASSWORD_LENGTH = 10;

void getData(UserData users[], int &userCount);
void encryptUserData(UserData users[], int userCount, int ENCRYPTION_SHIFT);
char shiftChar(char ch);
string reverseString(const string &input);
string generatePassword(int length);
void saveEncryptedData(UserData users[], int userCount);
void printData(UserData users[], int userCount, string encryptionStatus);
void decryptUserData(UserData users[], int userCount, int DECRYPTION_SHIFT);

int main() {
  int menuChoice;
  bool quit = false;
  UserData users[MAX_USER_SIZE];
  int userCount = 0;
  srand(time(0));
  string encryptionStatus = "unencrypted";

  do {
    cout << "\n\nMain menu:\n"
            "1. Encryption options\n"
            "2. Decrypt and display entries\n"
            "3. Quit\n"
            "Enter choice: ";
    cin >> menuChoice;

    switch (menuChoice) {
    case 1:
      // Submenu start
      do {
        cout << "\n\nEncryption menu:\n"
                "1. Encrypt data\n"
                "2. Generate random user passwords\n"
                "3. Display encrypted data\n"
                "4. Return to main menu\n"
                "Enter choice: ";
        cin >> menuChoice;
        switch (menuChoice) {
        case 1:
          getData(users, userCount);
          encryptUserData(users, userCount, ENCRYPTION_SHIFT);
          break;

        case 2:
          for (int i = 0; i < userCount; i++) {
            users[i].m_userPassword = generatePassword(PASSWORD_LENGTH);
          }
          cout << userCount << " passwords have been created\n";
          break;
        case 3:
          encryptionStatus = "encrypted";
          printData(users, userCount, encryptionStatus);
          saveEncryptedData(users, userCount);
          break;
        case 4:
          cout << "Returning to main menu\n";
          break;
        default:
          cout << "\nInvalid input detected. Please double check the input and "
                  "try "
                  "again\n";
          cin.clear();
          // cin.ignore();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
      } while (menuChoice != 4);
      break;
      // Submenu ends
    case 2:
      cout << "\nNow decrypting entries\n";
      decryptUserData(users, userCount, ENCRYPTION_SHIFT);
      printData(users, userCount, "unencrypted");
      break;
    case 3:
      cout << "\nNow exiting the program\n";
      quit = true;
      break;
    default:
      cout << "\nInvalid input detected. Please double check the input and try "
              "again\n";
      cin.clear();
      // cin.ignore();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  } while (!quit);
  return 0;
}

// Checks for file problems then reads and stores the data until the size limit
void getData(UserData users[], int &userCount) {
  string readString;
  char currentCh;
  bool maxUsersReached = false;
  ifstream inputFile("data.txt");

  if (!inputFile) {
    cout << "Error opening file\n";
    exit(EXIT_FAILURE);
  }

  cout << "\nReading data from file...\n";

  if (inputFile.peek() == EOF) {
    cout << "File is empty\n";
  }
  // Iterates through chars until end of the file. Adds the chars to a
  // string until a space or newline is detected and saves those strings up to
  // size limit
  while (inputFile.get(currentCh)) {
    if (maxUsersReached) {
      // inputFile.ignore();
      inputFile.ignore(numeric_limits<streamsize>::max(), '\n');
    } else if (currentCh == ' ' || currentCh == '\n') {
      if (!readString.empty()) {

        if (userCount < MAX_USER_SIZE) {
          // Adds the readString to the current UserData struct
          users[userCount].m_userID = readString;

          userCount++;
        } else {
          cout << "\nMaximum number of users reached. Cannot store more than "
               << MAX_USER_SIZE << " entries.\n";
          maxUsersReached = true;
        }
        readString.clear(); // Clears the readString for the next entry. Needed
                            // to avoid infinite appendage?
      }
    } else {
      readString += currentCh; // Appends the current character to readString
    }
  }

  inputFile.close();
  cout << "Data read successfully. " << userCount
       << " entries have been stored.\n";
}
// Takes a string and reverses the order
string reverseString(const string &input) {
  string reversedInput = input;
  int charLength = reversedInput.size();
  for (int i = 0; i < charLength / 2; i++) {
    char temp = reversedInput[i];
    reversedInput[i] = reversedInput[charLength - i - 1];
    reversedInput[charLength - i - 1] = temp;
  }
  return reversedInput;
}
// Takes a char and shifts it a pre-determined amount. Will wrap if
// necessary
char shiftChar(char ch, int ENCRYPTION_SHIFT) {
  char shiftedChar;
  if (ch >= 'a' && ch <= 'z') {
    shiftedChar = 'a' + (ch - 'a' + ENCRYPTION_SHIFT + 26) % 26;
  } else if (ch >= 'A' && ch <= 'Z') {
    shiftedChar = 'A' + (ch - 'A' + ENCRYPTION_SHIFT + 26) % 26;
  } else {
    shiftedChar = ch; // Leave in to handle special characters (Non-alpha)
  }
  return shiftedChar;
}

// Takes an entire string and shifts it by going through each letter
string shiftString(const string &input, int ENCRYPTION_SHIFT) {
  string shiftedString = input;
  for (char &ch : shiftedString) {
    ch = shiftChar(ch, ENCRYPTION_SHIFT);
  }
  return shiftedString;
}
// Finalizes the encryptedID value
void encryptUserData(UserData users[], int userCount, int ENCRYPTION_SHIFT) {
  for (int i = 0; i < userCount; i++) {
    string reversedID = reverseString(users[i].m_userID); // Reverses m_userID
    string encryptedID = shiftString(
        reversedID, ENCRYPTION_SHIFT); // Shifts and stores newly encrypted ID
    users[i].m_encryptedID = encryptedID;
  }
}
void decryptUserData(UserData users[], int userCount, int ENCRYPTION_SHIFT) {
  for (int i = 0; i < userCount; i++) {
    // Reverts the encrypted ID back to it's pre-shift value
    string reversedID = reverseString(users[i].m_encryptedID);
    // Now reverses that value to get the original ID
    string decryptedID = shiftString(reversedID, -ENCRYPTION_SHIFT);

    users[i].m_DecryptedID = decryptedID;
  }
}
// Saves the ecrypted data in a table format to encryptedData.txt
void saveEncryptedData(UserData users[], int userCount) {
  ofstream outFile("encryptedData.txt");
  if (!outFile) {
    cout << "Error opening file\n";
    exit(EXIT_FAILURE);
  }
  outFile << left << setw(10) << "User #" << setw(20) << "Encrypted ID"
          << setw(15) << "Password" << endl;
  outFile << setfill('-') << setw(45) << "" << setfill(' ') << "\n";

  for (int i = 0; i < userCount; i++) {
    outFile << left << setw(10) << i + 1 << setw(20) << users[i].m_encryptedID
            << setw(15) << users[i].m_userPassword << "\n";
  }
  outFile.close();
  cout << "\nEncrypted data saved to encryptedData.txt\n";
}

// Prints the data in a table format. Works encrypted or unencrypted
void printData(UserData users[], int userCount, string encryptionStatus) {

  // Checks to see if the data has been decrypted
  bool showDecrypted = (encryptionStatus == "unencrypted");

  // Adjusts the table headers accordingly
  cout << left << setw(10) << "User #" << setw(20)
       << (showDecrypted ? "Encrypted ID" : "User ID");

  if (showDecrypted) {
    cout << setw(20) << "Decrypted ID";
  }

  cout << setw(15) << "Password";

  if (showDecrypted) {
    cout << setw(10) << "Matches?";
  }

  cout << "\n"; // Prevents "overflow" of hyphens beyond Password

  // Dynamically adjusts the dash fill to current sum of setw values
  int dashLength = showDecrypted ? 75 : 45;
  cout << setfill('-') << setw(dashLength) << "" << setfill(' ') << "\n";

  // Outputs each row of the table
  for (int i = 0; i < userCount; i++) {
    cout << left << setw(10) << i + 1;

    // Shows both encrypted and decrypted IDs when displaying decrypted data
    if (showDecrypted) {
      cout << setw(20) << users[i].m_encryptedID << setw(20)
           << users[i].m_DecryptedID;
    } else {
      cout << setw(20) << users[i].m_encryptedID;
    }

    cout << setw(15) << users[i].m_userPassword;

    if (showDecrypted) {
      cout << setw(10)
           << (users[i].m_userID == users[i].m_DecryptedID ? "Yes" : "No");
    }

    cout << "\n"; // Do not remove. Very important for formatting
  }
}

// Not sure if this was the best way to include non-alpha keys, but it works
string generatePassword(int length) {
  const string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrs"
                       "tuvwxyz{}|~[]<>=?@";
  const int numChars = chars.length();
  string password = "";
  for (int i = 0; i < length; i++) {
    int index = rand() % numChars;
    password += chars[index];
  }
  return password;
}

/*      OUTPUT:
Main menu:
1. Encryption options
2. Decrypt and display entries
3. Quit
Enter choice: g

Invalid input detected. Please double check the input and try again


Main menu:
1. Encryption options
2. Decrypt and display entries
3. Quit
Enter choice: 1


Encryption menu:
1. Encrypt data
2. Generate random user passwords
3. Display encrypted data
4. Return to main menu
Enter choice: 7

Invalid input detected. Please double check the input and try again


Encryption menu:
1. Encrypt data
2. Generate random user passwords
3. Display encrypted data
4. Return to main menu
Enter choice: 1

Reading data from file...

Maximum number of users reached. Cannot store more than 10 entries.
Data read successfully. 10 entries have been stored.


Encryption menu:
1. Encrypt data
2. Generate random user passwords
3. Display encrypted data
4. Return to main menu
Enter choice: 2
10 passwords have been created


Encryption menu:
1. Encrypt data
2. Generate random user passwords
3. Display encrypted data
4. Return to main menu
Enter choice: 3
User #    User ID             Password
---------------------------------------------
1         hrzhsH              0=4F6d7q|n
2         hthihsh             J|b{=NU6xh
3         ZHZUHRYH            UC66tAQsKC
4         HuVgPyH             v49{4FCB]3
5         nuptvfD             y?]OimmwtI
6         hpuyvmpshJ          SCUJ2>TSoY
7         vkhyvsvJ            eJcY8gX4sS
8         abjpajluuvJ         7PA1osX9NF
9         lyhdhslK            RqSg?EMBhv
10        hkpyvsM             j]3BS<b{@I

Encrypted data saved to encryptedData.txt


Encryption menu:
1. Encrypt data
2. Generate random user passwords
3. Display encrypted data
4. Return to main menu
Enter choice: 4
Returning to main menu


Main menu:
1. Encryption options
2. Decrypt and display entries
3. Quit
Enter choice: 2

Now decrypting entries
User #    Encrypted ID        Decrypted ID        Password       Matches?
---------------------------------------------------------------------------
1         hrzhsH              Alaska              0=4F6d7q|n     Yes
2         hthihsh             alabama             J|b{=NU6xh     Yes
3         ZHZUHRYH            ARKANSAS            UC66tAQsKC     Yes
4         HuVgPyH             ArIzOnA             v49{4FCB]3     Yes
5         nuptvfD             Wyoming             y?]OimmwtI     Yes
6         hpuyvmpshJ          California          SCUJ2>TSoY     Yes
7         vkhyvsvJ            Colorado            eJcY8gX4sS     Yes
8         abjpajluuvJ         Connecticut         7PA1osX9NF     Yes
9         lyhdhslK            Delaware            RqSg?EMBhv     Yes
10        hkpyvsM             Florida             j]3BS<b{@I     Yes


Main menu:
1. Encryption options
2. Decrypt and display entries
3. Quit
Enter choice: 3

Now exiting the program
*/
