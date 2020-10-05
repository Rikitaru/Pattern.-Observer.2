#include "OlympicGames.h"

#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

void OlympicGames::notify() {

    string winner = this->getWinner();

    for (int i = 0; i < count; ++i) {
        clients[i]->update(winner);
    }
}

int OlympicGames::getCount() const {
    return count;
}

void OlympicGames::unsubscribe(int index) {
    if (index < 0 || index >= this->count) {
        cout << "Ошибка, неверный индекс массива" << endl;
    } else {
        int k = 0;
        Client **copy = new Client*[this->count-1];
        for (int i = 0; i < this->count; ++i) {
            if (i != index) {
                copy[k] = this->clients[i];
                k++;
            } else {
                Client *removed = copy[k];
                free(removed);
            }
        }
        this->count--;
        this->clients = new Client*[this->count];
        for (int j = 0; j < this->count; ++j) {
            this->clients[j] = copy[j];
        }
    }
}
void OlympicGames::addPeople(People &p) {
    this->peopleCount++;
    People **copy = new People*[this->peopleCount - 1];
    for (int i = 0; i < this->peopleCount - 1; ++i) {
        copy[i] = this->peoples[i];
    }
    this->peoples = new People*[this->peopleCount];
    for (int j = 0; j < this->peopleCount - 1; ++j) {
        this->peoples[j] = copy[j];
    }
    this->peoples[this->peopleCount - 1] = &p;
}

void OlympicGames::subscribe(Client *client) {
    this->count++;
    Client **copy = new Client*[this->count - 1];
    for (int i = 0; i < this->count - 1; ++i) {
        copy[i] = this->clients[i];
    }
    this->clients = new Client*[this->count];
    for (int j = 0; j < this->count - 1; ++j) {
        this->clients[j] = copy[j];
    }
    this->clients[this->count - 1] = client;
}

OlympicGames::OlympicGames() {
    for (int i = 0; i < 5; ++i) {
        results[i] = 0;
    }
    this->peopleCount = 0;
    this->count = 0;
    this->clients = new Client*[this->count];
    this->peoples = new People*[this->peopleCount];

    string textJson = this->readFromFile("main.json");
    json j = json::parse(textJson);
    int size = j["size"];
    j = j["peoples"];

    cout << "Прочитано: " << size << " спротсменов" << endl;
    for (int i = 0; i < size; ++i) {
        json current = j[i];
        People *p = new People();
        p->parseJson2People(current);
        this->addPeople(*p);
    }

    for (int k = 0; k < peopleCount; ++k) {
        cout << *peoples[k] << endl;
    }

}


string OlympicGames::readFromFile(string filename) {
    ifstream file;
    file.open("../" + filename);
    string text = "";
    string line;

    // check if file not opened
    if (file.fail()) {
        cerr << "Файл не удолась прочитать" << endl;
        return "";
    }

    while (getline(file, line))
    {
        text += line;
    }

    file.close();
    return text;
}

void OlympicGames::getResults() {
    string results = "";
    for (int j = 0; j < 4; ++j) {
        int len = 0;
        string sport = this->sports[j];
        string winnerCountry = "Никто";
        int max = -1;
        for (int i = 0; i < peopleCount; ++i) {
            People *current = this->peoples[i];
            int result = 0;

            if (current->getSport() != sport) {
                continue;
            } else {
                len++;
            }

            if (sport == "Skeleton") {
                result = current->getForce();
            } else if (sport == "Biatlon") {
                result = current->getAgility();
            } else if (sport == "Skating") {
                result = current->getLuck();
            } else if (sport == "IceSkating") {
                result = current->getStamina();
            }

            if (result > max) {
                max = result;
                winnerCountry = current->getCountry();
            }
        }

        int k = 0;
        People **copy = new People*[len];

        for (int i = 0; i < peopleCount; ++i) {
            People *current = this->peoples[i];
            if (current->getSport() != sport) {
                continue;
            }
            copy[k] = current;
            k++;
        }

        results += "В ввиде спорта: " + sport + " победила: " + winnerCountry + "\n";
        cout << "В ввиде спорта: " << sport << " победила: " << winnerCountry << endl;
        results += this->sort(copy, len, sport);

        addCountryResult(winnerCountry);
    }
    this->writeResult(results);
    this->notify();
}

void OlympicGames::writeResult(string &line) {
    ofstream file;
    file.open("../results.txt");
    file << line;
    file.close();
}

void OlympicGames::addCountryResult(string &country) {
    if (country == "Russia") {
        this->results[RUSSIA]++;
    } else if (country == "Canada") {
        this->results[CANADA]++;
    } else if (country == "Finland") {
        this->results[FINLAND]++;
    } else if (country == "France") {
        this->results[FRANCE]++;
    } else if (country == "Japan") {
        this->results[JAPAN]++;
    }
}

string OlympicGames::getWinner() {
    int max = 0;
    int index = 0;
    for (int i = 0; i < 4; ++i) {
        if (results[i] > max) {
            max = results[i];
            index = i;
        }
    }
    string winner;
    switch (index) {
        case RUSSIA:
            winner = "Russia";
            break;
        case CANADA:
            winner = "Canada";
            break;
        case FINLAND:
            winner = "Finlan";
            break;
        case FRANCE:
            winner = "France";
            break;
        case JAPAN:
            winner = "Japan";
            break;

    }

    return winner;
}

OlympicGames::~OlympicGames() {
    for (int i = 0; i < this->count; ++i) {
        free(clients[i]);
    }
    for (int i = 0; i < this->peopleCount; ++i) {
        free(peoples[i]);
    }
}

string OlympicGames::sort(People **peoples, int len, string &sport) {
    int result;
    string line;
    for (int i = 0; i < len; ++i) {
        People *current = peoples[i];
        result = this->filterSportParam(current, sport);
        for (int j = i; j < len; ++j) {
            People *second = peoples[j];
            int secParam = this->filterSportParam(second, sport);
            if (secParam > result) {
                People *tmp = peoples[i];
                peoples[i] = peoples[j];
                peoples[j] = tmp;
            }
        }
    }

    for (int k = 0; k < len; ++k) {
        cout << k + 1 << ") " << peoples[k]->getFio() << " " << peoples[k]->getCountry() << " " << this->filterSportParam(peoples[k], sport) << endl;
        line += to_string(k + 1) + ") " + peoples[k]->getFio() + " " + peoples[k]->getCountry() + " " + to_string(this->filterSportParam(peoples[k], sport)) + "\n";
    }

    return line;
}

int OlympicGames::filterSportParam(People *current, string sport) {
    int result = 0;
    if (sport == "Skeleton") {
        result = current->getForce();
    } else if (sport == "Biatlon") {
        result = current->getAgility();
    } else if (sport == "Skating") {
        result = current->getLuck();
    } else if (sport == "IceSkating") {
        result = current->getStamina();
    }
    return result;
}


