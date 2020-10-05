#include <iostream>

#include "OlympicGames.h"
#include "Client.h"

string readCountry();

int main() {

    string country;
    Client *client = new Client();
    OlympicGames *games = new OlympicGames();
    games->subscribe(client);

    while (country == "") {
        cout << "Выбирите страну, за которую болете:" << endl;
        cout << "1 - Russia, 2 - Canada, 3 - Finland, 4 - France, 5 - Japan; 0 - Exit" << endl;
        country = readCountry();
        if (country == "exit") {
            cout << "Exit from programm" << endl;
            return 0;
        }
    }

    client->setChoice(country);
    cout << "You choose: " << country << endl << endl;

    games->getResults();
    free(games);

    return 0;
}

string readCountry() {
    int num = 0;
    cin >> num;

    switch (num) {
        case 1:
            return "Russia";
            break;
        case 2:
            return "Canada";
            break;
        case 3:
            return "Finland";
            break;
        case 4:
            return "France";
            break;
        case 5:
            return "Japan";
            break;
        case 0:
            return "exit";
        default:
            cout << "Country error" << endl;
            return "";
    }
}