#include "Client.h"

void Client::update(string winner) {
    if (winner == this->choice) {
        cout << "Ура, страна за которую мы болели: " << this->choice << " выиграла!!!"<< endl;
    } else {
        cout << "Страна за которую мы болели: " << this->choice << " проиграла! Выиграла страна " << winner << endl;
    }
}

const string &Client::getChoice() const {
    return choice;
}

void Client::setChoice(const string &choice) {
    Client::choice = choice;
}

Client::Client() {}

Client::Client(const string &choice) : choice(choice) {}
