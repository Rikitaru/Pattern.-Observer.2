#ifndef ISTCURS_PEOPLE_H
#define ISTCURS_PEOPLE_H

#include <string>
#include <iostream>

#include "json.hpp"

using json = nlohmann::json;

using namespace std;

class People {

    string fio;
    string country;
    string sport;
public:
    const string &getSport() const;

    void setSport(const string &sport);

private:
    int force;
    int agility;
    int luck;
    int stamina;

public:
    const string &getFio() const;

    void setFio(const string &fio);

    const string &getCountry() const;

    void setCountry(const string &country);

    int getForce() const;

    void setForce(int force);

    int getAgility() const;

    void setAgility(int agility);

    int getLuck() const;

    void setLuck(int luck);

    int getStamina() const;

    void setStamina(int stamina);

    void parseJson2People(json j);

    friend ostream &operator<<(ostream &os, const People &people);

};


#endif //ISTCURS_PEOPLE_H
