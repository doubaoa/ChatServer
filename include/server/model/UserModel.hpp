#ifndef USERMODEL_H
#define USERMODEL_H
#include"user.hpp"
class UserModel
{
private:
    /* data */
public:
    bool insert(User &user);

    User query(int id);

    bool updateState(User &user);

    void reset();
};

#endif